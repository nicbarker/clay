from dataclasses import dataclass
from typing import Optional, TypedDict, NotRequired, Union
from pycparser import c_ast, parse_file, preprocess_file
from pathlib import Path
import os
import json
import shutil
import logging

logger = logging.getLogger(__name__)

ExtractedSymbolType = Union[str, "ExtractedFunction"]

class ExtractedStructAttributeUnion(TypedDict):
    type: Optional[ExtractedSymbolType]

class ExtractedStructAttribute(TypedDict):
    type: NotRequired[ExtractedSymbolType]
    union: NotRequired[dict[str, Optional[ExtractedSymbolType]]]

class ExtractedStruct(TypedDict):
    attrs: dict[str, ExtractedStructAttribute]
    is_union: NotRequired[bool]

ExtractedEnum = dict[str, Optional[str]]
ExtractedFunctionParam = tuple[str, Optional[ExtractedSymbolType]]

class ExtractedFunction(TypedDict):
    return_type: Optional["ExtractedSymbolType"]
    params: list[ExtractedFunctionParam]

@dataclass
class ExtractedSymbols:
    structs: dict[str, ExtractedStruct]
    enums: dict[str, ExtractedEnum]
    functions: dict[str, ExtractedFunction]

def get_type_names(node: c_ast.Node, prefix: str="") -> Optional[ExtractedSymbolType]:
    if isinstance(node, c_ast.TypeDecl) and hasattr(node, 'quals') and node.quals:
        prefix = " ".join(node.quals) + " " + prefix
    if isinstance(node, c_ast.PtrDecl):
        prefix = "*" + prefix
    if isinstance(node, c_ast.FuncDecl):
        func: ExtractedFunction = {
            'return_type': get_type_names(node.type),
            'params': [],
        }
        for param in node.args.params:
            if param.name is None:
                continue
            func['params'].append((param.name, get_type_names(param)))
        return func

    if hasattr(node, 'names'):
        return prefix + node.names[0] # type: ignore
    elif hasattr(node, 'type'):
        return get_type_names(node.type, prefix) # type: ignore
    return None

class Visitor(c_ast.NodeVisitor):
    def __init__(self):
        self.structs: dict[str, ExtractedStruct] = {}
        self.enums: dict[str, ExtractedEnum] = {}
        self.functions: dict[str, ExtractedFunction] = {}

    def visit_FuncDecl(self, node: c_ast.FuncDecl):
        # node.show()
        # logger.debug(node)
        node_type = node.type
        is_pointer = False
        if isinstance(node.type, c_ast.PtrDecl):
            node_type = node.type.type
            is_pointer = True
        
        if hasattr(node_type, "declname"):
            return_type = get_type_names(node_type.type)
            if return_type is not None and isinstance(return_type, str) and is_pointer:
                return_type = "*" + return_type
            func: ExtractedFunction = {
                'return_type': return_type,
                'params': [],
            }
            for param in node.args.params:
                if param.name is None:
                    continue
                func['params'].append((param.name, get_type_names(param)))
            self.functions[node_type.declname] = func
        self.generic_visit(node)

    def visit_Struct(self, node: c_ast.Struct):
        # node.show()
        if node.name and node.decls:
            struct = {}
            for decl in node.decls:
                struct[decl.name] = {
                    "type": get_type_names(decl),
                }
            self.structs[node.name] = {
                'attrs': struct,
            }
        self.generic_visit(node)

    def visit_Typedef(self, node: c_ast.Typedef):
        # node.show()
        if hasattr(node.type, 'type') and hasattr(node.type.type, 'decls') and node.type.type.decls:
            struct = {}
            for decl in node.type.type.decls:
                if hasattr(decl, 'type') and hasattr(decl.type, 'type') and isinstance(decl.type.type, c_ast.Union):
                    union = {}
                    for field in decl.type.type.decls:
                        union[field.name] = get_type_names(field)
                    struct[decl.name] = {
                        'union': union
                    }
                else:
                    struct[decl.name] = {
                        "type": get_type_names(decl),
                    }

            self.structs[node.name] = {
                'attrs': struct,
                'is_union': isinstance(node.type.type, c_ast.Union),
            }
        if hasattr(node.type, 'type') and isinstance(node.type.type, c_ast.Enum):
            enum = {}
            for enumerator in node.type.type.values.enumerators:
                if enumerator.value is None:
                    enum[enumerator.name] = None
                else:
                    enum[enumerator.name] = enumerator.value.value
            self.enums[node.name] = enum
        self.generic_visit(node)


def parse_headers(input_files: list[Path], tmp_dir: Path) -> ExtractedSymbols:
    cpp_args = ["-nostdinc", "-D__attribute__(x)=", "-E"]

    # Make a new clay.h that combines the provided input files, so that we can add bindings for customized structs
    with open(tmp_dir / 'merged_clay.h', 'w') as f:
        for input_file in input_files:
            with open(input_file, 'r') as f2:
                for line in f2:
                    # Ignore includes, as they should be manually included in input_files.
                    if line.startswith("#include"):
                        continue

                    # Ignore the CLAY_IMPLEMENTATION define, because we only want to parse the public api code.
                    # This is helpful so that the user can provide their implementation code, which will contain any custom extensions
                    if "#define CLAY_IMPLEMENTATION" in line:
                        continue

                    f.write(line)

    # Preprocess the file
    logger.info("Preprocessing file")
    preprocessed = preprocess_file(tmp_dir / 'merged_clay.h', cpp_path="cpp", cpp_args=cpp_args) # type: ignore
    with open(tmp_dir / 'clay.preprocessed.h', 'w') as f:
        f.write(preprocessed)

    # Parse the file
    logger.info("Parsing file")
    ast = parse_file(tmp_dir / 'clay.preprocessed.h', use_cpp=False) # type: ignore

    # Extract symbols
    visitor = Visitor()
    visitor.visit(ast)
    
    result = ExtractedSymbols(
        structs=visitor.structs,
        enums=visitor.enums,
        functions=visitor.functions
    )
    return result