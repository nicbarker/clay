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
    length: c.int32_t,
    chars:  [^]c.char,
}

StringSlice :: struct {
    length: c.int32_t,
    chars:  [^]c.char,
    baseChars:  [^]c.char,
}

Vector2 :: [2]c.float

Dimensions :: struct {
    width:  c.float,
    height: c.float,
}

Arena :: struct {
    nextAllocation: uintptr,
    capacity:       uintptr,
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
}

TextWrapMode :: enum EnumBackingType {
    Words,
    Newlines,
    None,
}

TextElementConfig :: struct {
    textColor:          Color,
    fontId:             u16,
    fontSize:           u16,
    letterSpacing:      u16,
    lineHeight:         u16,
    wrapMode:           TextWrapMode,
    hashStringContents: bool,
}

ImageElementConfig :: struct {
    imageData:        rawptr,
    sourceDimensions: Dimensions,
}

CustomElementConfig :: struct {
    customData: rawptr,
}

BorderWidth :: struct {
    left: u16,
    right: u16,
    top: u16,
    bottom: u16,
    betweenChildren: u16,
}

BorderElementConfig :: struct {
    color: Color,
    width: BorderWidth,
}

ScrollElementConfig :: struct {
    horizontal: bool,
    vertical:   bool,
}

FloatingAttachPointType :: enum EnumBackingType {
    LeftTop,
    LeftCenter,
    LeftBottom,
    CenterTop,
    CenterCenter,
    CenterBottom,
    RightTop,
    RightCenter,
    RightBottom,
}

FloatingAttachPoints :: struct {
    element: FloatingAttachPointType,
    parent:  FloatingAttachPointType,
}

PointerCaptureMode :: enum EnumBackingType {
    Capture,
    Passthrough,
}

FloatingAttachToElement :: enum EnumBackingType {
    None,
    Parent,
    ElementWithId,
    Root,
}

FloatingElementConfig :: struct {
    offset:             Vector2,
    expand:             Dimensions,
    parentId:           u32,
    zIndex:             i32,
    attachment:         FloatingAttachPoints,
    pointerCaptureMode: PointerCaptureMode,
    attachTo:           FloatingAttachToElement
}

TextRenderData :: struct {
    stringContents: StringSlice,
    textColor: Color,
    fontId: u16,
    fontSize: u16,
    letterSpacing: u16,
    lineHeight: u16,
}

RectangleRenderData :: struct {
    backgroundColor: Color,
    cornerRadius: CornerRadius,
}

ImageRenderData :: struct {
    backgroundColor: Color,
    cornerRadius: CornerRadius,
    sourceDimensions: Dimensions,
    imageData: rawptr,
}

CustomRenderData :: struct {
    backgroundColor: Color,
    cornerRadius: CornerRadius,
    customData: rawptr,
}

BorderRenderData :: struct {
    color: Color,
    cornerRadius: CornerRadius,
    width: BorderWidth,
}

RenderCommandData :: struct #raw_union {
    rectangle: RectangleRenderData,
    text: TextRenderData,
    image: ImageRenderData,
    custom: CustomRenderData,
    border: BorderRenderData,
}

RenderCommand :: struct {
    boundingBox:        BoundingBox,
    renderData:         RenderCommandData,
    userData:           rawptr,
    id:                 u32,
    zIndex:             i16,
    commandType:        RenderCommandType,
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
    Fit,
    Grow,
    Percent,
    Fixed,
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
    left: u16,
    right: u16,
    top: u16,
    bottom: u16,
}

LayoutDirection :: enum EnumBackingType {
    LeftToRight,
    TopToBottom,
}

LayoutAlignmentX :: enum EnumBackingType {
    Left,
    Right,
    Center,
}

LayoutAlignmentY :: enum EnumBackingType {
    Top,
    Bottom,
    Center,
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
    capacity:      i32,
    length:        i32,
    internalArray: [^]type,
}

ElementDeclaration :: struct {
    id: ElementId,
    layout: LayoutConfig,
    backgroundColor: Color,
    cornerRadius: CornerRadius,
    image: ImageElementConfig,
    floating: FloatingElementConfig,
    custom: CustomElementConfig,
    scroll: ScrollElementConfig,
    border: BorderElementConfig,
    userData: rawptr
}

ErrorType :: enum {
    TextMeasurementFunctionNotProvided,
    ArenaCapacityExceeded,
    ElementsCapacityExceeded,
    TextMeasurementCapacityExceeded,
    DuplicateId,
    FloatingContainerParentNotFound,
    PercentageOver1,
    InternalError,
}

ErrorData :: struct {
    errorType: ErrorType,
    errorText: String,
    userData: rawptr
}

ErrorHandler :: struct {
    handler: proc "c" (errorData: ErrorData),
    userData: rawptr
}

Context :: struct {} // opaque structure, only use as a pointer

@(link_prefix = "Clay_", default_calling_convention = "c")
foreign Clay {
    MinMemorySize :: proc() -> u32 ---
    CreateArenaWithCapacityAndMemory :: proc(capacity: u32, offset: [^]u8) -> Arena ---
    SetPointerState :: proc(position: Vector2, pointerDown: bool) ---
    Initialize :: proc(arena: Arena, layoutDimensions: Dimensions, errorHandler: ErrorHandler) ---
    UpdateScrollContainers :: proc(enableDragScrolling: bool, scrollDelta: Vector2, deltaTime: c.float) ---
    SetLayoutDimensions :: proc(dimensions: Dimensions) ---
    BeginLayout :: proc() ---
    EndLayout :: proc() -> ClayArray(RenderCommand) ---
    Hovered :: proc() -> bool ---
    PointerOver :: proc(id: ElementId) -> bool ---
    GetElementId :: proc(id: String) -> ElementId ---
    GetScrollContainerData :: proc(id: ElementId) -> ScrollContainerData ---
    SetMeasureTextFunction :: proc(measureTextFunction: proc "c" (text: StringSlice, config: ^TextElementConfig, userData: uintptr) -> Dimensions, userData: uintptr) ---
    RenderCommandArray_Get :: proc(array: ^ClayArray(RenderCommand), index: i32) -> ^RenderCommand ---
    SetDebugModeEnabled :: proc(enabled: bool) ---
    GetCurrentContext :: proc() -> ^Context ---
    SetCurrentContext :: proc(ctx: ^Context) ---
}

@(link_prefix = "Clay_", default_calling_convention = "c", private)
foreign Clay {
    _OpenElement :: proc() ---
    _ConfigureOpenElement :: proc(config: ElementDeclaration) ---
    _CloseElement :: proc() ---
    _OpenTextElement :: proc(text: String, textConfig: ^TextElementConfig) ---
    _StoreTextElementConfig :: proc(config: TextElementConfig) -> ^TextElementConfig ---
    _HashString :: proc(toHash: String, index: u32, seed: u32) -> ElementId ---
    _GetOpenLayoutElementId :: proc() -> u32 ---
}

ClayOpenElement :: struct {
    configure: proc (config: ElementDeclaration) -> bool
}

ConfigureOpenElement :: proc(config: ElementDeclaration) -> bool {
    _ConfigureOpenElement(config)
    return true;
}

@(deferred_none = _CloseElement)
UI :: proc() -> ClayOpenElement {
    _OpenElement()
    return { configure = ConfigureOpenElement }
}

Text :: proc(text: string, config: ^TextElementConfig) {
    _OpenTextElement(MakeString(text), config)
}

TextConfig :: proc(config: TextElementConfig) -> ^TextElementConfig {
    return _StoreTextElementConfig(config)
}

PaddingAll :: proc(allPadding: u16) -> Padding {
    return { left = allPadding, right = allPadding, top = allPadding, bottom = allPadding }
}

CornerRadiusAll :: proc(radius: f32) -> CornerRadius {
    return CornerRadius{radius, radius, radius, radius}
}

SizingFit :: proc(sizeMinMax: SizingConstraintsMinMax) -> SizingAxis {
    return SizingAxis{type = SizingType.Fit, constraints = {sizeMinMax = sizeMinMax}}
}

SizingGrow :: proc(sizeMinMax: SizingConstraintsMinMax) -> SizingAxis {
    return SizingAxis{type = SizingType.Grow, constraints = {sizeMinMax = sizeMinMax}}
}

SizingFixed :: proc(size: c.float) -> SizingAxis {
    return SizingAxis{type = SizingType.Fixed, constraints = {sizeMinMax = {size, size}}}
}

SizingPercent :: proc(sizePercent: c.float) -> SizingAxis {
    return SizingAxis{type = SizingType.Percent, constraints = {sizePercent = sizePercent}}
}

MakeString :: proc(label: string) -> String {
    return String{chars = raw_data(label), length = cast(c.int)len(label)}
}

ID :: proc(label: string, index: u32 = 0) -> ElementId {
    return _HashString(MakeString(label), index, 0)
}
