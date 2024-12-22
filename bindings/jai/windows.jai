//
// This file was auto-generated using the following command:
//
// jai ./generate.jai - -compile
//



// Utility Structs -------------------------
// Note: Clay_String is not guaranteed to be null terminated. It may be if created from a literal C string,
// but it is also used to represent slices.
String :: struct {
    length: s32;
    chars:  *u8;
}

StringArray :: struct {
    capacity:      u32;
    length:        u32;
    internalArray: *String;
}

Arena :: struct {
    label:          String;
    nextAllocation: u64;
    capacity:       u64;
    memory:         *u8;
}

Dimensions :: struct {
    width:  float;
    height: float;
}

Vector2 :: struct {
    x: float;
    y: float;
}

Color :: struct {
    r: float;
    g: float;
    b: float;
    a: float;
}

BoundingBox :: struct {
    x:      float;
    y:      float;
    width:  float;
    height: float;
}

// baseId + offset = id
ElementId :: struct {
    id:       u32;
    offset:   u32;
    baseId:   u32;
    stringId: String;
}

CornerRadius :: struct {
    topLeft:     float;
    topRight:    float;
    bottomLeft:  float;
    bottomRight: float;
}

ElementConfigType :: enum s32 {
    RECTANGLE          :: 1;
    BORDER_CONTAINER   :: 2;
    FLOATING_CONTAINER :: 4;
    SCROLL_CONTAINER   :: 8;
    IMAGE              :: 16;
    TEXT               :: 32;
    CUSTOM             :: 64;

    CLAY__ELEMENT_CONFIG_TYPE_RECTANGLE          :: RECTANGLE;
    CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER   :: BORDER_CONTAINER;
    CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER :: FLOATING_CONTAINER;
    CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER   :: SCROLL_CONTAINER;
    CLAY__ELEMENT_CONFIG_TYPE_IMAGE              :: IMAGE;
    CLAY__ELEMENT_CONFIG_TYPE_TEXT               :: TEXT;
    CLAY__ELEMENT_CONFIG_TYPE_CUSTOM             :: CUSTOM;
}

// Element Configs ---------------------------
// Layout
LayoutDirection :: enum s32 {
    LEFT_TO_RIGHT :: 0;
    TOP_TO_BOTTOM :: 1;

    CLAY_LEFT_TO_RIGHT :: LEFT_TO_RIGHT;
    CLAY_TOP_TO_BOTTOM :: TOP_TO_BOTTOM;
}

LayoutAlignmentX :: enum s32 {
    LEFT   :: 0;
    RIGHT  :: 1;
    CENTER :: 2;

    CLAY_ALIGN_X_LEFT   :: LEFT;
    CLAY_ALIGN_X_RIGHT  :: RIGHT;
    CLAY_ALIGN_X_CENTER :: CENTER;
}

LayoutAlignmentY :: enum s32 {
    TOP    :: 0;
    BOTTOM :: 1;
    CENTER :: 2;

    CLAY_ALIGN_Y_TOP    :: TOP;
    CLAY_ALIGN_Y_BOTTOM :: BOTTOM;
    CLAY_ALIGN_Y_CENTER :: CENTER;
}

SizingType :: enum s32 {
    FIT     :: 0;
    GROW    :: 1;
    PERCENT :: 2;
    FIXED   :: 3;

    CLAY__SIZING_TYPE_FIT     :: FIT;
    CLAY__SIZING_TYPE_GROW    :: GROW;
    CLAY__SIZING_TYPE_PERCENT :: PERCENT;
    CLAY__SIZING_TYPE_FIXED   :: FIXED;
}

ChildAlignment :: struct {
    x: LayoutAlignmentX;
    y: LayoutAlignmentY;
}

SizingMinMax :: struct {
    min: float;
    max: float;
}

SizingAxis :: struct {
    union {
        sizeMinMax:  SizingMinMax;
        sizePercent: float;
    }

    type: SizingType;
}

Sizing :: struct {
    width:  SizingAxis;
    height: SizingAxis;
}

Padding :: struct {
    x: u16;
    y: u16;
}

LayoutConfig :: struct {
    sizing:          Sizing;
    padding:         Padding;
    childGap:        u16;
    childAlignment:  ChildAlignment;
    layoutDirection: LayoutDirection;
}

CLAY_LAYOUT_DEFAULT: LayoutConfig #elsewhere clay;

// Rectangle
RectangleElementConfig :: struct {
    color:        Color;
    cornerRadius: CornerRadius;
}

// Text
TextElementConfigWrapMode :: enum s32 {
    WORDS    :: 0;
    NEWLINES :: 1;
    NONE     :: 2;

    CLAY_TEXT_WRAP_WORDS    :: WORDS;
    CLAY_TEXT_WRAP_NEWLINES :: NEWLINES;
    CLAY_TEXT_WRAP_NONE     :: NONE;
}

TextElementConfig :: struct {
    textColor:     Color;
    fontId:        u16;
    fontSize:      u16;
    letterSpacing: u16;
    lineHeight:    u16;
    wrapMode:      TextElementConfigWrapMode;
}

// Image
ImageElementConfig :: struct {
    imageData:        *void;
    sourceDimensions: Dimensions;
}

// Floating
FloatingAttachPointType :: enum s32 {
    LEFT_TOP      :: 0;
    LEFT_CENTER   :: 1;
    LEFT_BOTTOM   :: 2;
    CENTER_TOP    :: 3;
    CENTER_CENTER :: 4;
    CENTER_BOTTOM :: 5;
    RIGHT_TOP     :: 6;
    RIGHT_CENTER  :: 7;
    RIGHT_BOTTOM  :: 8;

    CLAY_ATTACH_POINT_LEFT_TOP      :: LEFT_TOP;
    CLAY_ATTACH_POINT_LEFT_CENTER   :: LEFT_CENTER;
    CLAY_ATTACH_POINT_LEFT_BOTTOM   :: LEFT_BOTTOM;
    CLAY_ATTACH_POINT_CENTER_TOP    :: CENTER_TOP;
    CLAY_ATTACH_POINT_CENTER_CENTER :: CENTER_CENTER;
    CLAY_ATTACH_POINT_CENTER_BOTTOM :: CENTER_BOTTOM;
    CLAY_ATTACH_POINT_RIGHT_TOP     :: RIGHT_TOP;
    CLAY_ATTACH_POINT_RIGHT_CENTER  :: RIGHT_CENTER;
    CLAY_ATTACH_POINT_RIGHT_BOTTOM  :: RIGHT_BOTTOM;
}

FloatingAttachPoints :: struct {
    element: FloatingAttachPointType;
    parent:  FloatingAttachPointType;
}

PointerCaptureMode :: enum s32 {
    CAPTURE     :: 0;

    PASSTHROUGH :: 1;

    CLAY_POINTER_CAPTURE_MODE_CAPTURE     :: CAPTURE;

    CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH :: PASSTHROUGH;
}

FloatingElementConfig :: struct {
    offset:             Vector2;
    expand:             Dimensions;
    zIndex:             u16;
    parentId:           u32;
    attachment:         FloatingAttachPoints;
    pointerCaptureMode: PointerCaptureMode;
}

// Custom
CustomElementConfig :: struct {
    customData: *void;
}

// Scroll
ScrollElementConfig :: struct {
    horizontal: bool;
    vertical:   bool;
}

// Border
Border :: struct {
    width: u32;
    color: Color;
}

BorderElementConfig :: struct {
    left:            Border;
    right:           Border;
    top:             Border;
    bottom:          Border;
    betweenChildren: Border;
    cornerRadius:    CornerRadius;
}

ElementConfigUnion :: union {
    rectangleElementConfig: *RectangleElementConfig;
    textElementConfig:      *TextElementConfig;
    imageElementConfig:     *ImageElementConfig;
    floatingElementConfig:  *FloatingElementConfig;
    customElementConfig:    *CustomElementConfig;
    scrollElementConfig:    *ScrollElementConfig;
    borderElementConfig:    *BorderElementConfig;
}

ElementConfig :: struct {
    type:   ElementConfigType;
    config: ElementConfigUnion;
}

// Miscellaneous Structs & Enums ---------------------------------
ScrollContainerData :: struct {
    // Note: This is a pointer to the real internal scroll position, mutating it may cause a change in final layout.
    // Intended for use with external functionality that modifies scroll position, such as scroll bars or auto scrolling.
    scrollPosition:            *Vector2;
    scrollContainerDimensions: Dimensions;
    contentDimensions:         Dimensions;
    config:                    ScrollElementConfig;

    // Indicates whether an actual scroll container matched the provided ID or if the default struct was returned.
    found:                     bool;
}

RenderCommandType :: enum s32 {
    NONE          :: 0;
    RECTANGLE     :: 1;
    BORDER        :: 2;
    TEXT          :: 3;
    IMAGE         :: 4;
    SCISSOR_START :: 5;
    SCISSOR_END   :: 6;
    CUSTOM        :: 7;

    CLAY_RENDER_COMMAND_TYPE_NONE          :: NONE;
    CLAY_RENDER_COMMAND_TYPE_RECTANGLE     :: RECTANGLE;
    CLAY_RENDER_COMMAND_TYPE_BORDER        :: BORDER;
    CLAY_RENDER_COMMAND_TYPE_TEXT          :: TEXT;
    CLAY_RENDER_COMMAND_TYPE_IMAGE         :: IMAGE;
    CLAY_RENDER_COMMAND_TYPE_SCISSOR_START :: SCISSOR_START;
    CLAY_RENDER_COMMAND_TYPE_SCISSOR_END   :: SCISSOR_END;
    CLAY_RENDER_COMMAND_TYPE_CUSTOM        :: CUSTOM;
}

RenderCommand :: struct {
    boundingBox: BoundingBox;
    config:      ElementConfigUnion;
    text:        String; // TODO I wish there was a way to avoid having to have this on every render command
    id:          u32;
    commandType: RenderCommandType;
}

RenderCommandArray :: struct {
    capacity:      u32;
    length:        u32;
    internalArray: *RenderCommand;
}

PointerDataInteractionState :: enum s32 {
    PRESSED_THIS_FRAME  :: 0;
    PRESSED             :: 1;
    RELEASED_THIS_FRAME :: 2;
    RELEASED            :: 3;

    CLAY_POINTER_DATA_PRESSED_THIS_FRAME  :: PRESSED_THIS_FRAME;
    CLAY_POINTER_DATA_PRESSED             :: PRESSED;
    CLAY_POINTER_DATA_RELEASED_THIS_FRAME :: RELEASED_THIS_FRAME;
    CLAY_POINTER_DATA_RELEASED            :: RELEASED;
}

PointerData :: struct {
    position: Vector2;
    state:    PointerDataInteractionState;
}

// Function Forward Declarations ---------------------------------
// Public API functions ---
MinMemorySize :: (__args: ..Any) -> u32 #foreign clay "Clay_MinMemorySize";
CreateArenaWithCapacityAndMemory :: (capacity: u32, offset: *void) -> Arena #foreign clay "Clay_CreateArenaWithCapacityAndMemory";
SetPointerState :: (position: Vector2, pointerDown: bool) -> void #foreign clay "Clay_SetPointerState";
Initialize :: (arena: Arena, layoutDimensions: Dimensions) -> void #foreign clay "Clay_Initialize";
UpdateScrollContainers :: (enableDragScrolling: bool, scrollDelta: Vector2, deltaTime: float) -> void #foreign clay "Clay_UpdateScrollContainers";
SetLayoutDimensions :: (dimensions: Dimensions) -> void #foreign clay "Clay_SetLayoutDimensions";
BeginLayout :: (__args: ..Any) -> void #foreign clay "Clay_BeginLayout";
EndLayout :: (__args: ..Any) -> RenderCommandArray #foreign clay "Clay_EndLayout";
GetElementId :: (idString: String) -> ElementId #foreign clay "Clay_GetElementId";
GetElementIdWithIndex :: (idString: String, index: u32) -> ElementId #foreign clay "Clay_GetElementIdWithIndex";
Hovered :: (__args: ..Any) -> bool #foreign clay "Clay_Hovered";
OnHover :: (onHoverFunction: #type (elementId: ElementId, pointerData: PointerData, userData: s64) -> void #c_call, userData: s64) -> void #foreign clay "Clay_OnHover";
GetScrollContainerData :: (id: ElementId) -> ScrollContainerData #foreign clay "Clay_GetScrollContainerData";
SetMeasureTextFunction :: (measureTextFunction: #type (text: *String, config: *TextElementConfig) -> Dimensions #c_call) -> void #foreign clay "Clay_SetMeasureTextFunction";
SetQueryScrollOffsetFunction :: (queryScrollOffsetFunction: #type (elementId: u32) -> Vector2 #c_call) -> void #foreign clay "Clay_SetQueryScrollOffsetFunction";
RenderCommandArray_Get :: (array: *RenderCommandArray, index: s32) -> *RenderCommand #foreign clay "Clay_RenderCommandArray_Get";
SetDebugModeEnabled :: (enabled: bool) -> void #foreign clay "Clay_SetDebugModeEnabled";
SetCullingEnabled :: (enabled: bool) -> void #foreign clay "Clay_SetCullingEnabled";

// Internal API functions required by macros
OpenElement :: (__args: ..Any) -> void #foreign clay "Clay__OpenElement";
CloseElement :: (__args: ..Any) -> void #foreign clay "Clay__CloseElement";

ElementPostConfiguration :: (__args: ..Any) -> void #foreign clay "Clay__ElementPostConfiguration";
AttachId :: (id: ElementId) -> void #foreign clay "Clay__AttachId";
AttachLayoutConfig :: (config: *LayoutConfig) -> void #foreign clay "Clay__AttachLayoutConfig";
AttachElementConfig :: (config: ElementConfigUnion, type: ElementConfigType) -> void #foreign clay "Clay__AttachElementConfig";

HashString :: (key: String, offset: u32, seed: u32) -> ElementId #foreign clay "Clay__HashString";
Noop :: (__args: ..Any) -> void #foreign clay "Clay__Noop";
OpenTextElement :: (text: String, textConfig: *TextElementConfig) -> void #foreign clay "Clay__OpenTextElement";

Clay__debugViewHighlightColor: Color #elsewhere clay;
Clay__debugViewWidth: u32 #elsewhere clay;
Clay__debugMaxElementsLatch: bool #elsewhere clay;

#scope_file

clay :: #library,no_dll "clay-jai/windows/clay";
