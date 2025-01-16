package clay

import "core:c"
import "core:strings"

when ODIN_OS == .Windows {
    foreign import Clay "windows/clay.lib"
} else when ODIN_OS == .Linux {
    foreign import Clay "linux/clay.a"
} else when ODIN_OS == .Darwin {
    when ODIN_ARCH == .arm64 {
        foreign import Clay "macos-arm64/clay.a"
    } else {
        foreign import Clay "macos/clay.a"
    }
} else when ODIN_ARCH == .wasm32 || ODIN_ARCH == .wasm64p32 {
    foreign import Clay "wasm/clay.o"
}

when ODIN_OS == .Windows {
    EnumBackingType :: u32
} else {
    EnumBackingType :: u8
}

{{enums}}

Context :: struct {
    
}

ClayArray :: struct($type: typeid) {
    capacity:      i32,
    length:        i32,
    internalArray: [^]type,
}

SizingConstraints :: struct #raw_union {
    sizeMinMax:  SizingConstraintsMinMax,
    sizePercent: c.float,
}

{{structs}}

@(link_prefix = "Clay_", default_calling_convention = "c")
foreign Clay {
{{public_functions}}
}

@(link_prefix = "Clay_", default_calling_convention = "c", private)
foreign Clay {
{{private_functions}}
}

@(require_results, deferred_none = _CloseElement)
UI :: proc(configs: ..TypedConfig) -> bool {
    _OpenElement()
    for config in configs {
        #partial switch (config.type) {
        case ElementConfigType.Id:
            _AttachId(config.id)
        case ElementConfigType.Layout:
            _AttachLayoutConfig(cast(^LayoutConfig)config.config)
        case:
            _AttachElementConfig(config.config, config.type)
        }
    }
    _ElementPostConfiguration()
    return true
}

Layout :: proc(config: LayoutConfig) -> TypedConfig {
    return {type = ElementConfigType.Layout, config = _StoreLayoutConfig(config) }
}

PaddingAll :: proc (padding: u16) -> Padding {
    return { padding, padding, padding, padding }
}

Rectangle :: proc(config: RectangleElementConfig) -> TypedConfig {
    return {type = ElementConfigType.Rectangle, config = _StoreRectangleElementConfig(config)}
}

Text :: proc(text: string, config: ^TextElementConfig) {
    _OpenTextElement(MakeString(text), config)
}

TextConfig :: proc(config: TextElementConfig) -> ^TextElementConfig {
    return _StoreTextElementConfig(config)
}

Image :: proc(config: ImageElementConfig) -> TypedConfig {
    return {type = ElementConfigType.Image, config = _StoreImageElementConfig(config)}
}

Floating :: proc(config: FloatingElementConfig) -> TypedConfig {
    return {type = ElementConfigType.Floating, config = _StoreFloatingElementConfig(config)}
}

Custom :: proc(config: CustomElementConfig) -> TypedConfig {
    return {type = ElementConfigType.Custom, config = _StoreCustomElementConfig(config)}
}

Scroll :: proc(config: ScrollElementConfig) -> TypedConfig {
    return {type = ElementConfigType.Scroll, config = _StoreScrollElementConfig(config)}
}

Border :: proc(config: BorderElementConfig) -> TypedConfig {
    return {type = ElementConfigType.Border, config = _StoreBorderElementConfig(config)}
}

BorderOutside :: proc(outsideBorders: BorderData) -> TypedConfig {
    return { type = ElementConfigType.Border, config = _StoreBorderElementConfig((BorderElementConfig){left = outsideBorders, right = outsideBorders, top = outsideBorders, bottom = outsideBorders}) }
}

BorderOutsideRadius :: proc(outsideBorders: BorderData, radius: f32) -> TypedConfig {
    return { type = ElementConfigType.Border, config = _StoreBorderElementConfig(
        (BorderElementConfig){left = outsideBorders, right = outsideBorders, top = outsideBorders, bottom = outsideBorders, cornerRadius = {radius, radius, radius, radius}},
    ) }
}

BorderAll :: proc(allBorders: BorderData) -> TypedConfig {
    return { type = ElementConfigType.Border, config = _StoreBorderElementConfig((BorderElementConfig){left = allBorders, right = allBorders, top = allBorders, bottom = allBorders, betweenChildren = allBorders}) }
}

BorderAllRadius :: proc(allBorders: BorderData, radius: f32) -> TypedConfig {
    return { type = ElementConfigType.Border, config = _StoreBorderElementConfig(
        (BorderElementConfig){left = allBorders, right = allBorders, top = allBorders, bottom = allBorders, cornerRadius = {radius, radius, radius, radius}},
    ) }
}

CornerRadiusAll :: proc(radius: f32) -> CornerRadius {
    return CornerRadius{radius, radius, radius, radius}
}

SizingFit :: proc(sizeMinMax: SizingConstraintsMinMax) -> SizingAxis {
    return SizingAxis{type = SizingType.FIT, constraints = {sizeMinMax = sizeMinMax}}
}

SizingGrow :: proc(sizeMinMax: SizingConstraintsMinMax) -> SizingAxis {
    return SizingAxis{type = SizingType.GROW, constraints = {sizeMinMax = sizeMinMax}}
}

SizingFixed :: proc(size: c.float) -> SizingAxis {
    return SizingAxis{type = SizingType.FIXED, constraints = {sizeMinMax = {size, size}}}
}

SizingPercent :: proc(sizePercent: c.float) -> SizingAxis {
    return SizingAxis{type = SizingType.PERCENT, constraints = {sizePercent = sizePercent}}
}

MakeString :: proc(label: string) -> String {
    return String{chars = raw_data(label), length = cast(c.int)len(label)}
}

ID :: proc(label: string, index: u32 = 0) -> TypedConfig {
    return { type = ElementConfigType.Id, id = _HashString(MakeString(label), index, 0) }
}
