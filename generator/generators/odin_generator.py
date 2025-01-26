from pathlib import Path
import logging

from generators.base_generator import BaseGenerator

logger = logging.getLogger(__name__)

def get_common_prefix(keys: list[str]) -> str:
    # find a prefix that's shared between all keys
    prefix = ""
    for i in range(min(map(len, keys))):
        if len(set(key[i] for key in keys)) > 1:
            break
        prefix += keys[0][i]
    return prefix

def snake_case_to_pascal_case(snake_case: str) -> str:
    return ''.join(word.lower().capitalize() for word in snake_case.split('_'))


SYMBOL_NAME_OVERRIDES = {
    'Clay_TextElementConfigWrapMode': 'TextWrapMode',
    'Clay_Border': 'BorderData',
    'Clay_SizingMinMax': 'SizingConstraintsMinMax',
}
SYMBOL_COMPLETE_OVERRIDES = {
    'Clay_RenderCommandArray': 'ClayArray(RenderCommand)',
    'Clay_Context': 'Context',
    'Clay_ElementConfig': None,
    # 'Clay_SetQueryScrollOffsetFunction': None,
}

# These enums should have output binding members that are PascalCase instead of UPPER_SNAKE_CASE.
ENUM_MEMBER_PASCAL = {
    'Clay_RenderCommandType',
    'Clay_TextElementConfigWrapMode',
    'Clay__ElementConfigType',
}
ENUM_MEMBER_OVERRIDES = {
    'Clay__ElementConfigType': {
        'CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER': 'Border',
        'CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER': 'Floating',
        'CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER': 'Scroll',
    }
}
ENUM_ADDITIONAL_MEMBERS = {
    'Clay__ElementConfigType': {
        'Id': 65,
        'Layout': 66,
    }
}

TYPE_MAPPING = {
    '*char': '[^]c.char',
    'const *char': '[^]c.char',
    '*void': 'rawptr',
    'bool': 'bool',
    'float': 'c.float',
    'uint16_t': 'u16',
    'uint32_t': 'u32',
    'int32_t': 'c.int32_t',
    'uintptr_t': 'rawptr',
    'void': 'void',

    '*Clay_RectangleElementConfig': '^RectangleElementConfig',
    '*Clay_TextElementConfig': '^TextElementConfig',
    '*Clay_ImageElementConfig': '^ImageElementConfig',
    '*Clay_FloatingElementConfig': '^FloatingElementConfig',
    '*Clay_CustomElementConfig': '^CustomElementConfig',
    '*Clay_ScrollElementConfig': '^ScrollElementConfig',
    '*Clay_BorderElementConfig': '^BorderElementConfig',
}
STRUCT_TYPE_OVERRIDES = {
    'Clay_Arena': {
        'nextAllocation': 'uintptr',
        'capacity': 'uintptr',
    },
    'Clay_ErrorHandler': {
        'errorHandlerFunction': 'proc "c" (errorData: ErrorData)',
    },
    'Clay_SizingAxis': {
        'size': 'SizingConstraints',
    },
    "Clay_RenderCommand": {
        "zIndex": 'i32',
    },
}
STRUCT_MEMBER_OVERRIDES = {
    'Clay_ErrorHandler': {
        'errorHandlerFunction': 'handler',
    },
    'Clay_SizingAxis': {
        'size': 'constraints',
    },
}
STRUCT_OVERRIDE_AS_FIXED_ARRAY = {
    'Clay_Color',
    'Clay_Vector2',
}

FUNCTION_PARAM_OVERRIDES = {
    'Clay_SetCurrentContext': {
        'context': 'ctx',
    },
}
FUNCTION_TYPE_OVERRIDES = {
    'Clay_CreateArenaWithCapacityAndMemory': {
        'offset': '[^]u8',
    },
    'Clay_SetMeasureTextFunction': {
        'measureTextFunction': 'proc "c" (text: ^StringSlice, config: ^TextElementConfig, userData: uintptr) -> Dimensions',
        'userData': 'uintptr',
    },
    'Clay_RenderCommandArray_Get': {
        'index': 'i32',
    },
    "Clay__AttachElementConfig": {
        "config": 'rawptr',
    },
}

class OdinGenerator(BaseGenerator):

    def generate(self) -> None:
        self.generate_structs()
        self.generate_enums()
        self.generate_functions()

        odin_template_path = Path(__file__).parent / 'odin' / 'clay.template.odin'
        with open(odin_template_path, 'r') as f:
            template = f.read()
        self.output_content['clay.odin'] = (
            template
            .replace('{{structs}}', '\n'.join(self.output_content['struct']))
            .replace('{{enums}}', '\n'.join(self.output_content['enum']))
            .replace('{{public_functions}}', '\n'.join(self.output_content['public_function']))
            .replace('{{private_functions}}', '\n'.join(self.output_content['private_function']))
            .splitlines()
        )
        del self.output_content['struct']
        del self.output_content['enum']
        del self.output_content['private_function']
        del self.output_content['public_function']

    def get_symbol_name(self, symbol: str) -> str:
        if symbol in SYMBOL_NAME_OVERRIDES:
            return SYMBOL_NAME_OVERRIDES[symbol]
        symbol_type = self.get_symbol_type(symbol)
        base_name = symbol.removeprefix('Clay_')
        if symbol_type == 'enum':
            return base_name.removeprefix('_') # Clay_ and Clay__ are exported as public types.
        elif symbol_type == 'struct':
            return base_name
        elif symbol_type == 'function':
            return base_name
        raise ValueError(f'Unknown symbol: {symbol}')
    
    def resolve_binding_type(self, symbol: str, member: str | None, member_type: str | None, type_overrides: dict[str, dict[str, str]]) -> str | None:
        if member_type in SYMBOL_COMPLETE_OVERRIDES:
            return SYMBOL_COMPLETE_OVERRIDES[member_type]
        if symbol in type_overrides and member in type_overrides[symbol]:
            return type_overrides[symbol][member]
        if member_type in TYPE_MAPPING:
            return TYPE_MAPPING[member_type]
        if member_type and self.has_symbol(member_type):
            return self.get_symbol_name(member_type)
        if member_type and member_type.startswith('*'):
            result = self.resolve_binding_type(symbol, member, member_type[1:], type_overrides)
            if result:
                return f"^{result}"
        return None

    def generate_structs(self) -> None:
        for struct, struct_data in sorted(self.extracted_symbols.structs.items(), key=lambda x: x[0]):
            members = struct_data['attrs']
            if not struct.startswith('Clay_'):
                continue
            if struct in SYMBOL_COMPLETE_OVERRIDES:
                continue

            binding_name = self.get_symbol_name(struct)
            if binding_name.startswith('_'):
                continue

            if struct in STRUCT_OVERRIDE_AS_FIXED_ARRAY:
                array_size = len(members)
                array_type = list(members.values())[0]['type']

                if array_type in TYPE_MAPPING:
                    array_binding_type = TYPE_MAPPING[array_type]
                elif array_type and self.has_symbol(array_type):
                    array_binding_type = self.get_symbol_name(array_type)
                else:
                    self._write('struct', f"// {struct} ({array_type}) - has no mapping")
                    continue
                
                self._write('struct', f"// {struct} (overridden as fixed array)")
                self._write('struct', f"{binding_name} :: [{array_size}]{array_binding_type}")
                self._write('struct', "")
                continue

            raw_union = ' #raw_union' if struct_data.get('is_union', False) else ''

            self._write('struct', f"// {struct}")
            self._write('struct', f"{binding_name} :: struct{raw_union} {{")

            for member, member_info in members.items():
                if struct in STRUCT_TYPE_OVERRIDES and member in STRUCT_TYPE_OVERRIDES[struct]:
                    member_type = 'unknown'
                elif not 'type' in member_info:
                    self._write('struct', f"    // {member} (unknown type)")
                    continue
                else:
                    member_type = member_info['type']
                    
                binding_member_name = member
                if struct in STRUCT_MEMBER_OVERRIDES and member in STRUCT_MEMBER_OVERRIDES[struct]:
                    binding_member_name = STRUCT_MEMBER_OVERRIDES[struct][member]

                member_binding_type = self.resolve_binding_type(struct, member, member_type, STRUCT_TYPE_OVERRIDES)
                if member_binding_type is None:
                    self._write('struct', f"    // {binding_member_name} ({member_type}) - has no mapping")
                    continue
                self._write('struct', f"    {binding_member_name}: {member_binding_type}, // {member} ({member_type})")
            self._write('struct', "}")
            self._write('struct', '')

    def generate_enums(self) -> None:
        for enum, members in sorted(self.extracted_symbols.enums.items(), key=lambda x: x[0]):
            if not enum.startswith('Clay_'):
                continue
            if enum in SYMBOL_COMPLETE_OVERRIDES:
                continue

            binding_name = self.get_symbol_name(enum)
            common_member_prefix = get_common_prefix(list(members.keys()))
            self._write('enum', f"// {enum}")
            self._write('enum', f"{binding_name} :: enum EnumBackingType {{")
            for member in members:
                if enum in ENUM_MEMBER_OVERRIDES and member in ENUM_MEMBER_OVERRIDES[enum]:
                    binding_member_name = ENUM_MEMBER_OVERRIDES[enum][member]
                else:
                    binding_member_name = member.removeprefix(common_member_prefix)
                    if enum in ENUM_MEMBER_PASCAL:
                        binding_member_name = snake_case_to_pascal_case(binding_member_name)

                if members[member] is not None:
                    self._write('enum', f"    {binding_member_name} = {members[member]}, // {member}")
                else:
                    self._write('enum', f"    {binding_member_name}, // {member}")
                
            if enum in ENUM_ADDITIONAL_MEMBERS:
                self._write('enum', '    // Odin specific enum types')
                for member, value in ENUM_ADDITIONAL_MEMBERS[enum].items():
                    self._write('enum', f"    {member} = {value},")
            self._write('enum', "}")
            self._write('enum', '')

    def generate_functions(self) -> None:
        for function, function_info in sorted(self.extracted_symbols.functions.items(), key=lambda x: x[0]):
            if not function.startswith('Clay_'):
                continue
            if function in SYMBOL_COMPLETE_OVERRIDES:
                continue
            is_private = function.startswith('Clay__')
            write_to = 'private_function' if is_private else 'public_function'

            binding_name = self.get_symbol_name(function)

            return_type = function_info['return_type']
            binding_return_type = self.resolve_binding_type(function, None, return_type, {})
            if binding_return_type is None:
                self._write(write_to, f"    // {function} ({return_type}) - has no mapping")
                continue

            skip = False
            binding_params = []
            for param_name, param_type in function_info['params']:
                binding_param_name = param_name
                if function in FUNCTION_PARAM_OVERRIDES and param_name in FUNCTION_PARAM_OVERRIDES[function]:
                    binding_param_name = FUNCTION_PARAM_OVERRIDES[function][param_name]
                binding_param_type = self.resolve_binding_type(function, param_name, param_type, FUNCTION_TYPE_OVERRIDES)
                if binding_param_type is None:
                    skip = True
                binding_params.append(f"{binding_param_name}: {binding_param_type}")
            if skip:
                self._write(write_to, f"    // {function} - has no mapping")
                continue

            binding_params_str = ', '.join(binding_params)
            return_str = f" -> {binding_return_type}" if binding_return_type != 'void' else ''
            self._write(write_to, f"    {binding_name} :: proc({binding_params_str}){return_str} --- // {function}")
            
