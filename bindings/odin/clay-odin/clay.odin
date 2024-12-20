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

String :: struct {
    length: c.int,
    chars:  [^]c.char,
}

Vector2 :: [2]c.float

Dimensions :: struct {
    width:  c.float,
    height: c.float,
}

Arena :: struct {
    label:          String,
    nextAllocation: u64,
    capacity:       u64,
    memory:         [^]c.char,
}

BoundingBox :: struct {
    x:      c.float,
    y:      c.float,
    width:  c.float,
    height: c.float,
}

Color :: [4]c.float

CornerRadius :: struct {
    topLeft:     c.float,
    topRight:    c.float,
    bottomLeft:  c.float,
    bottomRight: c.float,
}

BorderData :: struct {
    width: u32,
    color: Color,
}

ElementId :: struct {
    id:       u32,
    offset:   u32,
    baseId:   u32,
    stringId: String,
}

when ODIN_OS == .Windows {
    EnumBackingType :: u32
} else {
    EnumBackingType :: u8
}

ElementConfigType :: enum EnumBackingType {
    Rectangle = 1,
    Border    = 2,
    Floating  = 4,
    Scroll    = 8,
    Image     = 16,
    Text      = 32,
    Custom    = 64,
    // Odin specific enum types
    Id        = 65,
    Layout    = 66,
}

RenderCommandType :: enum EnumBackingType {
    None,
    Rectangle,
    Border,
    Text,
    Image,
    ScissorStart,
    ScissorEnd,
    Custom,
}

RectangleElementConfig :: struct {
    color:        Color,
    cornerRadius: CornerRadius,
}

TextWrapMode :: enum EnumBackingType {
    Words,
    Newlines,
    None,
}

TextElementConfig :: struct {
    textColor:     Color,
    fontId:        u16,
    fontSize:      u16,
    letterSpacing: u16,
    lineHeight:    u16,
    wrapMode:      TextWrapMode,
}

ImageElementConfig :: struct {
    imageData:        rawptr,
    sourceDimensions: Dimensions,
}

CustomElementConfig :: struct {
    customData: rawptr,
}

BorderElementConfig :: struct {
    left:            BorderData,
    right:           BorderData,
    top:             BorderData,
    bottom:          BorderData,
    betweenChildren: BorderData,
    cornerRadius:    CornerRadius,
}

ScrollElementConfig :: struct {
    horizontal: bool,
    vertical:   bool,
}

FloatingAttachPointType :: enum EnumBackingType {
    LEFT_TOP,
    LEFT_CENTER,
    LEFT_BOTTOM,
    CENTER_TOP,
    CENTER_CENTER,
    CENTER_BOTTOM,
    RIGHT_TOP,
    RIGHT_CENTER,
    RIGHT_BOTTOM,
}

FloatingAttachPoints :: struct {
    element: FloatingAttachPointType,
    parent:  FloatingAttachPointType,
}

PointerCaptureMode :: enum EnumBackingType {
    CAPTURE,
    PASSTHROUGH,
}

FloatingElementConfig :: struct {
    offset:             Vector2,
    expand:             Dimensions,
    zIndex:             u16,
    parentId:           u32,
    attachment:         FloatingAttachPoints,
    pointerCaptureMode: PointerCaptureMode,
}

ElementConfigUnion :: struct #raw_union {
    rectangleElementConfig: ^RectangleElementConfig,
    textElementConfig:      ^TextElementConfig,
    imageElementConfig:     ^ImageElementConfig,
    customElementConfig:    ^CustomElementConfig,
    borderElementConfig:    ^BorderElementConfig,
}

RenderCommand :: struct {
    boundingBox: BoundingBox,
    config:      ElementConfigUnion,
    text:        String,
    id:          u32,
    commandType: RenderCommandType,
}

ScrollContainerData :: struct {
    // Note: This is a pointer to the real internal scroll position, mutating it may cause a change in final layout.
    // Intended for use with external functionality that modifies scroll position, such as scroll bars or auto scrolling.
    scrollPosition:            ^Vector2,
    scrollContainerDimensions: Dimensions,
    contentDimensions:         Dimensions,
    config:                    ScrollElementConfig,
    // Indicates whether an actual scroll container matched the provided ID or if the default struct was returned.
    found:                     bool,
}

SizingType :: enum EnumBackingType {
    FIT,
    GROW,
    PERCENT,
    FIXED,
}

SizingConstraintsMinMax :: struct {
    min: c.float,
    max: c.float,
}

SizingConstraints :: struct #raw_union {
    sizeMinMax:  SizingConstraintsMinMax,
    sizePercent: c.float,
}

SizingAxis :: struct {
    // Note: `min` is used for CLAY_SIZING_PERCENT, slightly different to clay.h due to lack of C anonymous unions
    constraints: SizingConstraints,
    type:        SizingType,
}

Sizing :: struct {
    width:  SizingAxis,
    height: SizingAxis,
}

Padding :: struct {
    x: u16,
    y: u16,
}

LayoutDirection :: enum EnumBackingType {
    LEFT_TO_RIGHT,
    TOP_TO_BOTTOM,
}

LayoutAlignmentX :: enum EnumBackingType {
    LEFT,
    RIGHT,
    CENTER,
}

LayoutAlignmentY :: enum EnumBackingType {
    TOP,
    BOTTOM,
    CENTER,
}

ChildAlignment :: struct {
    x: LayoutAlignmentX,
    y: LayoutAlignmentY,
}

LayoutConfig :: struct {
    sizing:          Sizing,
    padding:         Padding,
    childGap:        u16,
    childAlignment:  ChildAlignment,
    layoutDirection: LayoutDirection,
}

ClayArray :: struct($type: typeid) {
    capacity:      u32,
    length:        u32,
    internalArray: [^]type,
}

TypedConfig :: struct {
    type:   ElementConfigType,
    config: rawptr,
    id:     ElementId,
}

@(link_prefix = "Clay_", default_calling_convention = "c")
foreign Clay {
    MinMemorySize :: proc() -> u32 ---
    CreateArenaWithCapacityAndMemory :: proc(capacity: u32, offset: [^]u8) -> Arena ---
    SetPointerState :: proc(position: Vector2, pointerDown: bool) ---
    Initialize :: proc(arena: Arena, layoutDimensions: Dimensions) ---
    UpdateScrollContainers :: proc(enableDragScrolling: bool, scrollDelta: Vector2, deltaTime: c.float) ---
    SetLayoutDimensions :: proc(dimensions: Dimensions) ---
    BeginLayout :: proc() ---
    EndLayout :: proc() -> ClayArray(RenderCommand) ---
    PointerOver :: proc(id: ElementId) -> bool ---
    GetElementId :: proc(id: String) -> ElementId ---
    GetScrollContainerData :: proc(id: ElementId) -> ScrollContainerData ---
    SetMeasureTextFunction :: proc(measureTextFunction: proc "c" (text: ^String, config: ^TextElementConfig) -> Dimensions) ---
    RenderCommandArray_Get :: proc(array: ^ClayArray(RenderCommand), index: i32) -> ^RenderCommand ---
    SetDebugModeEnabled :: proc(enabled: bool) ---
}

@(link_prefix = "Clay_", default_calling_convention = "c", private)
foreign Clay {
    _OpenElement :: proc() ---
    _CloseElement :: proc() ---
    _ElementPostConfiguration :: proc() ---
    _OpenTextElement :: proc(text: String, textConfig: ^TextElementConfig) ---
    _AttachId :: proc(id: ElementId) ---
    _AttachLayoutConfig :: proc(layoutConfig: ^LayoutConfig) ---
    _AttachElementConfig :: proc(config: rawptr, type: ElementConfigType) ---
    _StoreLayoutConfig :: proc(config: LayoutConfig) -> ^LayoutConfig ---
    _StoreRectangleElementConfig :: proc(config: RectangleElementConfig) -> ^RectangleElementConfig ---
    _StoreTextElementConfig :: proc(config: TextElementConfig) -> ^TextElementConfig ---
    _StoreImageElementConfig :: proc(config: ImageElementConfig) -> ^ImageElementConfig ---
    _StoreFloatingElementConfig :: proc(config: FloatingElementConfig) -> ^FloatingElementConfig ---
    _StoreCustomElementConfig :: proc(config: CustomElementConfig) -> ^CustomElementConfig ---
    _StoreScrollElementConfig :: proc(config: ScrollElementConfig) -> ^ScrollElementConfig ---
    _StoreBorderElementConfig :: proc(config: BorderElementConfig) -> ^BorderElementConfig ---
    _HashString :: proc(toHash: String, index: u32, seed: u32) -> ElementId ---
    _GetOpenLayoutElementId :: proc() -> u32 ---
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
