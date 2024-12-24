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
StoreLayoutConfig :: (config: LayoutConfig) -> *LayoutConfig #foreign clay "Clay__StoreLayoutConfig";
ElementPostConfiguration :: (__args: ..Any) -> void #foreign clay "Clay__ElementPostConfiguration";
AttachId :: (id: ElementId) -> void #foreign clay "Clay__AttachId";
AttachLayoutConfig :: (config: *LayoutConfig) -> void #foreign clay "Clay__AttachLayoutConfig";
AttachElementConfig :: (config: ElementConfigUnion, type: ElementConfigType) -> void #foreign clay "Clay__AttachElementConfig";
StoreRectangleElementConfig :: (config: RectangleElementConfig) -> *RectangleElementConfig #foreign clay "Clay__StoreRectangleElementConfig";
StoreTextElementConfig :: (config: TextElementConfig) -> *TextElementConfig #foreign clay "Clay__StoreTextElementConfig";
StoreImageElementConfig :: (config: ImageElementConfig) -> *ImageElementConfig #foreign clay "Clay__StoreImageElementConfig";
StoreFloatingElementConfig :: (config: FloatingElementConfig) -> *FloatingElementConfig #foreign clay "Clay__StoreFloatingElementConfig";
StoreCustomElementConfig :: (config: CustomElementConfig) -> *CustomElementConfig #foreign clay "Clay__StoreCustomElementConfig";
StoreScrollElementConfig :: (config: ScrollElementConfig) -> *ScrollElementConfig #foreign clay "Clay__StoreScrollElementConfig";
StoreBorderElementConfig :: (config: BorderElementConfig) -> *BorderElementConfig #foreign clay "Clay__StoreBorderElementConfig";
HashString :: (key: String, offset: u32, seed: u32) -> ElementId #foreign clay "Clay__HashString";
Noop :: (__args: ..Any) -> void #foreign clay "Clay__Noop";
OpenTextElement :: (text: String, textConfig: *TextElementConfig) -> void #foreign clay "Clay__OpenTextElement";

Clay__debugViewHighlightColor: Color #elsewhere clay;
Clay__debugViewWidth: u32 #elsewhere clay;
Clay__debugMaxElementsLatch: bool #elsewhere clay;

#scope_file

#import "Basic"; // For assert

clay :: #library,no_dll "clay-jai/windows/clay";

#run {
    {
        instance: String;
        assert(((cast(*void)(*instance.length)) - cast(*void)(*instance)) == 0, "String.length has unexpected offset % instead of 0", ((cast(*void)(*instance.length)) - cast(*void)(*instance)));
        assert(size_of(type_of(String.length)) == 4, "String.length has unexpected size % instead of 4", size_of(type_of(String.length)));
        assert(((cast(*void)(*instance.chars)) - cast(*void)(*instance)) == 8, "String.chars has unexpected offset % instead of 8", ((cast(*void)(*instance.chars)) - cast(*void)(*instance)));
        assert(size_of(type_of(String.chars)) == 8, "String.chars has unexpected size % instead of 8", size_of(type_of(String.chars)));
        assert(size_of(String) == 16, "String has size % instead of 16", size_of(String));
    }

    {
        instance: StringArray;
        assert(((cast(*void)(*instance.capacity)) - cast(*void)(*instance)) == 0, "StringArray.capacity has unexpected offset % instead of 0", ((cast(*void)(*instance.capacity)) - cast(*void)(*instance)));
        assert(size_of(type_of(StringArray.capacity)) == 4, "StringArray.capacity has unexpected size % instead of 4", size_of(type_of(StringArray.capacity)));
        assert(((cast(*void)(*instance.length)) - cast(*void)(*instance)) == 4, "StringArray.length has unexpected offset % instead of 4", ((cast(*void)(*instance.length)) - cast(*void)(*instance)));
        assert(size_of(type_of(StringArray.length)) == 4, "StringArray.length has unexpected size % instead of 4", size_of(type_of(StringArray.length)));
        assert(((cast(*void)(*instance.internalArray)) - cast(*void)(*instance)) == 8, "StringArray.internalArray has unexpected offset % instead of 8", ((cast(*void)(*instance.internalArray)) - cast(*void)(*instance)));
        assert(size_of(type_of(StringArray.internalArray)) == 8, "StringArray.internalArray has unexpected size % instead of 8", size_of(type_of(StringArray.internalArray)));
        assert(size_of(StringArray) == 16, "StringArray has size % instead of 16", size_of(StringArray));
    }

    {
        instance: Arena;
        assert(((cast(*void)(*instance.label)) - cast(*void)(*instance)) == 0, "Arena.label has unexpected offset % instead of 0", ((cast(*void)(*instance.label)) - cast(*void)(*instance)));
        assert(size_of(type_of(Arena.label)) == 16, "Arena.label has unexpected size % instead of 16", size_of(type_of(Arena.label)));
        assert(((cast(*void)(*instance.nextAllocation)) - cast(*void)(*instance)) == 16, "Arena.nextAllocation has unexpected offset % instead of 16", ((cast(*void)(*instance.nextAllocation)) - cast(*void)(*instance)));
        assert(size_of(type_of(Arena.nextAllocation)) == 8, "Arena.nextAllocation has unexpected size % instead of 8", size_of(type_of(Arena.nextAllocation)));
        assert(((cast(*void)(*instance.capacity)) - cast(*void)(*instance)) == 24, "Arena.capacity has unexpected offset % instead of 24", ((cast(*void)(*instance.capacity)) - cast(*void)(*instance)));
        assert(size_of(type_of(Arena.capacity)) == 8, "Arena.capacity has unexpected size % instead of 8", size_of(type_of(Arena.capacity)));
        assert(((cast(*void)(*instance.memory)) - cast(*void)(*instance)) == 32, "Arena.memory has unexpected offset % instead of 32", ((cast(*void)(*instance.memory)) - cast(*void)(*instance)));
        assert(size_of(type_of(Arena.memory)) == 8, "Arena.memory has unexpected size % instead of 8", size_of(type_of(Arena.memory)));
        assert(size_of(Arena) == 40, "Arena has size % instead of 40", size_of(Arena));
    }

    {
        instance: Dimensions;
        assert(((cast(*void)(*instance.width)) - cast(*void)(*instance)) == 0, "Dimensions.width has unexpected offset % instead of 0", ((cast(*void)(*instance.width)) - cast(*void)(*instance)));
        assert(size_of(type_of(Dimensions.width)) == 4, "Dimensions.width has unexpected size % instead of 4", size_of(type_of(Dimensions.width)));
        assert(((cast(*void)(*instance.height)) - cast(*void)(*instance)) == 4, "Dimensions.height has unexpected offset % instead of 4", ((cast(*void)(*instance.height)) - cast(*void)(*instance)));
        assert(size_of(type_of(Dimensions.height)) == 4, "Dimensions.height has unexpected size % instead of 4", size_of(type_of(Dimensions.height)));
        assert(size_of(Dimensions) == 8, "Dimensions has size % instead of 8", size_of(Dimensions));
    }

    {
        instance: BoundingBox;
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 0, "BoundingBox.x has unexpected offset % instead of 0", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(BoundingBox.x)) == 4, "BoundingBox.x has unexpected size % instead of 4", size_of(type_of(BoundingBox.x)));
        assert(((cast(*void)(*instance.y)) - cast(*void)(*instance)) == 4, "BoundingBox.y has unexpected offset % instead of 4", ((cast(*void)(*instance.y)) - cast(*void)(*instance)));
        assert(size_of(type_of(BoundingBox.y)) == 4, "BoundingBox.y has unexpected size % instead of 4", size_of(type_of(BoundingBox.y)));
        assert(((cast(*void)(*instance.width)) - cast(*void)(*instance)) == 8, "BoundingBox.width has unexpected offset % instead of 8", ((cast(*void)(*instance.width)) - cast(*void)(*instance)));
        assert(size_of(type_of(BoundingBox.width)) == 4, "BoundingBox.width has unexpected size % instead of 4", size_of(type_of(BoundingBox.width)));
        assert(((cast(*void)(*instance.height)) - cast(*void)(*instance)) == 12, "BoundingBox.height has unexpected offset % instead of 12", ((cast(*void)(*instance.height)) - cast(*void)(*instance)));
        assert(size_of(type_of(BoundingBox.height)) == 4, "BoundingBox.height has unexpected size % instead of 4", size_of(type_of(BoundingBox.height)));
        assert(size_of(BoundingBox) == 16, "BoundingBox has size % instead of 16", size_of(BoundingBox));
    }

    {
        instance: ElementId;
        assert(((cast(*void)(*instance.id)) - cast(*void)(*instance)) == 0, "ElementId.id has unexpected offset % instead of 0", ((cast(*void)(*instance.id)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementId.id)) == 4, "ElementId.id has unexpected size % instead of 4", size_of(type_of(ElementId.id)));
        assert(((cast(*void)(*instance.offset)) - cast(*void)(*instance)) == 4, "ElementId.offset has unexpected offset % instead of 4", ((cast(*void)(*instance.offset)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementId.offset)) == 4, "ElementId.offset has unexpected size % instead of 4", size_of(type_of(ElementId.offset)));
        assert(((cast(*void)(*instance.baseId)) - cast(*void)(*instance)) == 8, "ElementId.baseId has unexpected offset % instead of 8", ((cast(*void)(*instance.baseId)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementId.baseId)) == 4, "ElementId.baseId has unexpected size % instead of 4", size_of(type_of(ElementId.baseId)));
        assert(((cast(*void)(*instance.stringId)) - cast(*void)(*instance)) == 16, "ElementId.stringId has unexpected offset % instead of 16", ((cast(*void)(*instance.stringId)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementId.stringId)) == 16, "ElementId.stringId has unexpected size % instead of 16", size_of(type_of(ElementId.stringId)));
        assert(size_of(ElementId) == 32, "ElementId has size % instead of 32", size_of(ElementId));
    }

    {
        instance: CornerRadius;
        assert(((cast(*void)(*instance.topLeft)) - cast(*void)(*instance)) == 0, "CornerRadius.topLeft has unexpected offset % instead of 0", ((cast(*void)(*instance.topLeft)) - cast(*void)(*instance)));
        assert(size_of(type_of(CornerRadius.topLeft)) == 4, "CornerRadius.topLeft has unexpected size % instead of 4", size_of(type_of(CornerRadius.topLeft)));
        assert(((cast(*void)(*instance.topRight)) - cast(*void)(*instance)) == 4, "CornerRadius.topRight has unexpected offset % instead of 4", ((cast(*void)(*instance.topRight)) - cast(*void)(*instance)));
        assert(size_of(type_of(CornerRadius.topRight)) == 4, "CornerRadius.topRight has unexpected size % instead of 4", size_of(type_of(CornerRadius.topRight)));
        assert(((cast(*void)(*instance.bottomLeft)) - cast(*void)(*instance)) == 8, "CornerRadius.bottomLeft has unexpected offset % instead of 8", ((cast(*void)(*instance.bottomLeft)) - cast(*void)(*instance)));
        assert(size_of(type_of(CornerRadius.bottomLeft)) == 4, "CornerRadius.bottomLeft has unexpected size % instead of 4", size_of(type_of(CornerRadius.bottomLeft)));
        assert(((cast(*void)(*instance.bottomRight)) - cast(*void)(*instance)) == 12, "CornerRadius.bottomRight has unexpected offset % instead of 12", ((cast(*void)(*instance.bottomRight)) - cast(*void)(*instance)));
        assert(size_of(type_of(CornerRadius.bottomRight)) == 4, "CornerRadius.bottomRight has unexpected size % instead of 4", size_of(type_of(CornerRadius.bottomRight)));
        assert(size_of(CornerRadius) == 16, "CornerRadius has size % instead of 16", size_of(CornerRadius));
    }

    {
        instance: ChildAlignment;
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 0, "ChildAlignment.x has unexpected offset % instead of 0", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(ChildAlignment.x)) == 4, "ChildAlignment.x has unexpected size % instead of 4", size_of(type_of(ChildAlignment.x)));
        assert(((cast(*void)(*instance.y)) - cast(*void)(*instance)) == 4, "ChildAlignment.y has unexpected offset % instead of 4", ((cast(*void)(*instance.y)) - cast(*void)(*instance)));
        assert(size_of(type_of(ChildAlignment.y)) == 4, "ChildAlignment.y has unexpected size % instead of 4", size_of(type_of(ChildAlignment.y)));
        assert(size_of(ChildAlignment) == 8, "ChildAlignment has size % instead of 8", size_of(ChildAlignment));
    }

    {
        instance: SizingMinMax;
        assert(((cast(*void)(*instance.min)) - cast(*void)(*instance)) == 0, "SizingMinMax.min has unexpected offset % instead of 0", ((cast(*void)(*instance.min)) - cast(*void)(*instance)));
        assert(size_of(type_of(SizingMinMax.min)) == 4, "SizingMinMax.min has unexpected size % instead of 4", size_of(type_of(SizingMinMax.min)));
        assert(((cast(*void)(*instance.max)) - cast(*void)(*instance)) == 4, "SizingMinMax.max has unexpected offset % instead of 4", ((cast(*void)(*instance.max)) - cast(*void)(*instance)));
        assert(size_of(type_of(SizingMinMax.max)) == 4, "SizingMinMax.max has unexpected size % instead of 4", size_of(type_of(SizingMinMax.max)));
        assert(size_of(SizingMinMax) == 8, "SizingMinMax has size % instead of 8", size_of(SizingMinMax));
    }

    {
        instance: SizingAxis;
        assert(((cast(*void)(*instance.type)) - cast(*void)(*instance)) == 8, "SizingAxis.type has unexpected offset % instead of 8", ((cast(*void)(*instance.type)) - cast(*void)(*instance)));
        assert(size_of(type_of(SizingAxis.type)) == 4, "SizingAxis.type has unexpected size % instead of 4", size_of(type_of(SizingAxis.type)));
        assert(size_of(SizingAxis) == 12, "SizingAxis has size % instead of 12", size_of(SizingAxis));
    }

    {
        instance: Sizing;
        assert(((cast(*void)(*instance.width)) - cast(*void)(*instance)) == 0, "Sizing.width has unexpected offset % instead of 0", ((cast(*void)(*instance.width)) - cast(*void)(*instance)));
        assert(size_of(type_of(Sizing.width)) == 12, "Sizing.width has unexpected size % instead of 12", size_of(type_of(Sizing.width)));
        assert(((cast(*void)(*instance.height)) - cast(*void)(*instance)) == 12, "Sizing.height has unexpected offset % instead of 12", ((cast(*void)(*instance.height)) - cast(*void)(*instance)));
        assert(size_of(type_of(Sizing.height)) == 12, "Sizing.height has unexpected size % instead of 12", size_of(type_of(Sizing.height)));
        assert(size_of(Sizing) == 24, "Sizing has size % instead of 24", size_of(Sizing));
    }

    {
        instance: Padding;
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 0, "Padding.x has unexpected offset % instead of 0", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(Padding.x)) == 2, "Padding.x has unexpected size % instead of 2", size_of(type_of(Padding.x)));
        assert(((cast(*void)(*instance.y)) - cast(*void)(*instance)) == 2, "Padding.y has unexpected offset % instead of 2", ((cast(*void)(*instance.y)) - cast(*void)(*instance)));
        assert(size_of(type_of(Padding.y)) == 2, "Padding.y has unexpected size % instead of 2", size_of(type_of(Padding.y)));
        assert(size_of(Padding) == 4, "Padding has size % instead of 4", size_of(Padding));
    }

    {
        instance: LayoutConfig;
        assert(((cast(*void)(*instance.sizing)) - cast(*void)(*instance)) == 0, "LayoutConfig.sizing has unexpected offset % instead of 0", ((cast(*void)(*instance.sizing)) - cast(*void)(*instance)));
        assert(size_of(type_of(LayoutConfig.sizing)) == 24, "LayoutConfig.sizing has unexpected size % instead of 24", size_of(type_of(LayoutConfig.sizing)));
        assert(((cast(*void)(*instance.padding)) - cast(*void)(*instance)) == 24, "LayoutConfig.padding has unexpected offset % instead of 24", ((cast(*void)(*instance.padding)) - cast(*void)(*instance)));
        assert(size_of(type_of(LayoutConfig.padding)) == 4, "LayoutConfig.padding has unexpected size % instead of 4", size_of(type_of(LayoutConfig.padding)));
        assert(((cast(*void)(*instance.childGap)) - cast(*void)(*instance)) == 28, "LayoutConfig.childGap has unexpected offset % instead of 28", ((cast(*void)(*instance.childGap)) - cast(*void)(*instance)));
        assert(size_of(type_of(LayoutConfig.childGap)) == 2, "LayoutConfig.childGap has unexpected size % instead of 2", size_of(type_of(LayoutConfig.childGap)));
        assert(((cast(*void)(*instance.childAlignment)) - cast(*void)(*instance)) == 32, "LayoutConfig.childAlignment has unexpected offset % instead of 32", ((cast(*void)(*instance.childAlignment)) - cast(*void)(*instance)));
        assert(size_of(type_of(LayoutConfig.childAlignment)) == 8, "LayoutConfig.childAlignment has unexpected size % instead of 8", size_of(type_of(LayoutConfig.childAlignment)));
        assert(((cast(*void)(*instance.layoutDirection)) - cast(*void)(*instance)) == 40, "LayoutConfig.layoutDirection has unexpected offset % instead of 40", ((cast(*void)(*instance.layoutDirection)) - cast(*void)(*instance)));
        assert(size_of(type_of(LayoutConfig.layoutDirection)) == 4, "LayoutConfig.layoutDirection has unexpected size % instead of 4", size_of(type_of(LayoutConfig.layoutDirection)));
        assert(size_of(LayoutConfig) == 44, "LayoutConfig has size % instead of 44", size_of(LayoutConfig));
    }

    {
        instance: RectangleElementConfig;
        assert(((cast(*void)(*instance.color)) - cast(*void)(*instance)) == 0, "RectangleElementConfig.color has unexpected offset % instead of 0", ((cast(*void)(*instance.color)) - cast(*void)(*instance)));
        assert(size_of(type_of(RectangleElementConfig.color)) == 16, "RectangleElementConfig.color has unexpected size % instead of 16", size_of(type_of(RectangleElementConfig.color)));
        assert(((cast(*void)(*instance.cornerRadius)) - cast(*void)(*instance)) == 16, "RectangleElementConfig.cornerRadius has unexpected offset % instead of 16", ((cast(*void)(*instance.cornerRadius)) - cast(*void)(*instance)));
        assert(size_of(type_of(RectangleElementConfig.cornerRadius)) == 16, "RectangleElementConfig.cornerRadius has unexpected size % instead of 16", size_of(type_of(RectangleElementConfig.cornerRadius)));
        assert(size_of(RectangleElementConfig) == 32, "RectangleElementConfig has size % instead of 32", size_of(RectangleElementConfig));
    }

    {
        instance: TextElementConfig;
        assert(((cast(*void)(*instance.textColor)) - cast(*void)(*instance)) == 0, "TextElementConfig.textColor has unexpected offset % instead of 0", ((cast(*void)(*instance.textColor)) - cast(*void)(*instance)));
        assert(size_of(type_of(TextElementConfig.textColor)) == 16, "TextElementConfig.textColor has unexpected size % instead of 16", size_of(type_of(TextElementConfig.textColor)));
        assert(((cast(*void)(*instance.fontId)) - cast(*void)(*instance)) == 16, "TextElementConfig.fontId has unexpected offset % instead of 16", ((cast(*void)(*instance.fontId)) - cast(*void)(*instance)));
        assert(size_of(type_of(TextElementConfig.fontId)) == 2, "TextElementConfig.fontId has unexpected size % instead of 2", size_of(type_of(TextElementConfig.fontId)));
        assert(((cast(*void)(*instance.fontSize)) - cast(*void)(*instance)) == 18, "TextElementConfig.fontSize has unexpected offset % instead of 18", ((cast(*void)(*instance.fontSize)) - cast(*void)(*instance)));
        assert(size_of(type_of(TextElementConfig.fontSize)) == 2, "TextElementConfig.fontSize has unexpected size % instead of 2", size_of(type_of(TextElementConfig.fontSize)));
        assert(((cast(*void)(*instance.letterSpacing)) - cast(*void)(*instance)) == 20, "TextElementConfig.letterSpacing has unexpected offset % instead of 20", ((cast(*void)(*instance.letterSpacing)) - cast(*void)(*instance)));
        assert(size_of(type_of(TextElementConfig.letterSpacing)) == 2, "TextElementConfig.letterSpacing has unexpected size % instead of 2", size_of(type_of(TextElementConfig.letterSpacing)));
        assert(((cast(*void)(*instance.lineHeight)) - cast(*void)(*instance)) == 22, "TextElementConfig.lineHeight has unexpected offset % instead of 22", ((cast(*void)(*instance.lineHeight)) - cast(*void)(*instance)));
        assert(size_of(type_of(TextElementConfig.lineHeight)) == 2, "TextElementConfig.lineHeight has unexpected size % instead of 2", size_of(type_of(TextElementConfig.lineHeight)));
        assert(((cast(*void)(*instance.wrapMode)) - cast(*void)(*instance)) == 24, "TextElementConfig.wrapMode has unexpected offset % instead of 24", ((cast(*void)(*instance.wrapMode)) - cast(*void)(*instance)));
        assert(size_of(type_of(TextElementConfig.wrapMode)) == 4, "TextElementConfig.wrapMode has unexpected size % instead of 4", size_of(type_of(TextElementConfig.wrapMode)));
        assert(size_of(TextElementConfig) == 28, "TextElementConfig has size % instead of 28", size_of(TextElementConfig));
    }

    {
        instance: ImageElementConfig;
        assert(((cast(*void)(*instance.imageData)) - cast(*void)(*instance)) == 0, "ImageElementConfig.imageData has unexpected offset % instead of 0", ((cast(*void)(*instance.imageData)) - cast(*void)(*instance)));
        assert(size_of(type_of(ImageElementConfig.imageData)) == 8, "ImageElementConfig.imageData has unexpected size % instead of 8", size_of(type_of(ImageElementConfig.imageData)));
        assert(((cast(*void)(*instance.sourceDimensions)) - cast(*void)(*instance)) == 8, "ImageElementConfig.sourceDimensions has unexpected offset % instead of 8", ((cast(*void)(*instance.sourceDimensions)) - cast(*void)(*instance)));
        assert(size_of(type_of(ImageElementConfig.sourceDimensions)) == 8, "ImageElementConfig.sourceDimensions has unexpected size % instead of 8", size_of(type_of(ImageElementConfig.sourceDimensions)));
        assert(size_of(ImageElementConfig) == 16, "ImageElementConfig has size % instead of 16", size_of(ImageElementConfig));
    }

    {
        instance: FloatingAttachPoints;
        assert(((cast(*void)(*instance.element)) - cast(*void)(*instance)) == 0, "FloatingAttachPoints.element has unexpected offset % instead of 0", ((cast(*void)(*instance.element)) - cast(*void)(*instance)));
        assert(size_of(type_of(FloatingAttachPoints.element)) == 4, "FloatingAttachPoints.element has unexpected size % instead of 4", size_of(type_of(FloatingAttachPoints.element)));
        assert(((cast(*void)(*instance.parent)) - cast(*void)(*instance)) == 4, "FloatingAttachPoints.parent has unexpected offset % instead of 4", ((cast(*void)(*instance.parent)) - cast(*void)(*instance)));
        assert(size_of(type_of(FloatingAttachPoints.parent)) == 4, "FloatingAttachPoints.parent has unexpected size % instead of 4", size_of(type_of(FloatingAttachPoints.parent)));
        assert(size_of(FloatingAttachPoints) == 8, "FloatingAttachPoints has size % instead of 8", size_of(FloatingAttachPoints));
    }

    {
        instance: FloatingElementConfig;
        assert(((cast(*void)(*instance.offset)) - cast(*void)(*instance)) == 0, "FloatingElementConfig.offset has unexpected offset % instead of 0", ((cast(*void)(*instance.offset)) - cast(*void)(*instance)));
        assert(size_of(type_of(FloatingElementConfig.offset)) == 8, "FloatingElementConfig.offset has unexpected size % instead of 8", size_of(type_of(FloatingElementConfig.offset)));
        assert(((cast(*void)(*instance.expand)) - cast(*void)(*instance)) == 8, "FloatingElementConfig.expand has unexpected offset % instead of 8", ((cast(*void)(*instance.expand)) - cast(*void)(*instance)));
        assert(size_of(type_of(FloatingElementConfig.expand)) == 8, "FloatingElementConfig.expand has unexpected size % instead of 8", size_of(type_of(FloatingElementConfig.expand)));
        assert(((cast(*void)(*instance.zIndex)) - cast(*void)(*instance)) == 16, "FloatingElementConfig.zIndex has unexpected offset % instead of 16", ((cast(*void)(*instance.zIndex)) - cast(*void)(*instance)));
        assert(size_of(type_of(FloatingElementConfig.zIndex)) == 2, "FloatingElementConfig.zIndex has unexpected size % instead of 2", size_of(type_of(FloatingElementConfig.zIndex)));
        assert(((cast(*void)(*instance.parentId)) - cast(*void)(*instance)) == 20, "FloatingElementConfig.parentId has unexpected offset % instead of 20", ((cast(*void)(*instance.parentId)) - cast(*void)(*instance)));
        assert(size_of(type_of(FloatingElementConfig.parentId)) == 4, "FloatingElementConfig.parentId has unexpected size % instead of 4", size_of(type_of(FloatingElementConfig.parentId)));
        assert(((cast(*void)(*instance.attachment)) - cast(*void)(*instance)) == 24, "FloatingElementConfig.attachment has unexpected offset % instead of 24", ((cast(*void)(*instance.attachment)) - cast(*void)(*instance)));
        assert(size_of(type_of(FloatingElementConfig.attachment)) == 8, "FloatingElementConfig.attachment has unexpected size % instead of 8", size_of(type_of(FloatingElementConfig.attachment)));
        assert(((cast(*void)(*instance.pointerCaptureMode)) - cast(*void)(*instance)) == 32, "FloatingElementConfig.pointerCaptureMode has unexpected offset % instead of 32", ((cast(*void)(*instance.pointerCaptureMode)) - cast(*void)(*instance)));
        assert(size_of(type_of(FloatingElementConfig.pointerCaptureMode)) == 4, "FloatingElementConfig.pointerCaptureMode has unexpected size % instead of 4", size_of(type_of(FloatingElementConfig.pointerCaptureMode)));
        assert(size_of(FloatingElementConfig) == 36, "FloatingElementConfig has size % instead of 36", size_of(FloatingElementConfig));
    }

    {
        instance: CustomElementConfig;
        assert(((cast(*void)(*instance.customData)) - cast(*void)(*instance)) == 0, "CustomElementConfig.customData has unexpected offset % instead of 0", ((cast(*void)(*instance.customData)) - cast(*void)(*instance)));
        assert(size_of(type_of(CustomElementConfig.customData)) == 8, "CustomElementConfig.customData has unexpected size % instead of 8", size_of(type_of(CustomElementConfig.customData)));
        assert(size_of(CustomElementConfig) == 8, "CustomElementConfig has size % instead of 8", size_of(CustomElementConfig));
    }

    {
        instance: ScrollElementConfig;
        assert(((cast(*void)(*instance.horizontal)) - cast(*void)(*instance)) == 0, "ScrollElementConfig.horizontal has unexpected offset % instead of 0", ((cast(*void)(*instance.horizontal)) - cast(*void)(*instance)));
        assert(size_of(type_of(ScrollElementConfig.horizontal)) == 1, "ScrollElementConfig.horizontal has unexpected size % instead of 1", size_of(type_of(ScrollElementConfig.horizontal)));
        assert(((cast(*void)(*instance.vertical)) - cast(*void)(*instance)) == 1, "ScrollElementConfig.vertical has unexpected offset % instead of 1", ((cast(*void)(*instance.vertical)) - cast(*void)(*instance)));
        assert(size_of(type_of(ScrollElementConfig.vertical)) == 1, "ScrollElementConfig.vertical has unexpected size % instead of 1", size_of(type_of(ScrollElementConfig.vertical)));
        assert(size_of(ScrollElementConfig) == 2, "ScrollElementConfig has size % instead of 2", size_of(ScrollElementConfig));
    }

    {
        instance: Border;
        assert(((cast(*void)(*instance.width)) - cast(*void)(*instance)) == 0, "Border.width has unexpected offset % instead of 0", ((cast(*void)(*instance.width)) - cast(*void)(*instance)));
        assert(size_of(type_of(Border.width)) == 4, "Border.width has unexpected size % instead of 4", size_of(type_of(Border.width)));
        assert(((cast(*void)(*instance.color)) - cast(*void)(*instance)) == 4, "Border.color has unexpected offset % instead of 4", ((cast(*void)(*instance.color)) - cast(*void)(*instance)));
        assert(size_of(type_of(Border.color)) == 16, "Border.color has unexpected size % instead of 16", size_of(type_of(Border.color)));
        assert(size_of(Border) == 20, "Border has size % instead of 20", size_of(Border));
    }

    {
        instance: BorderElementConfig;
        assert(((cast(*void)(*instance.left)) - cast(*void)(*instance)) == 0, "BorderElementConfig.left has unexpected offset % instead of 0", ((cast(*void)(*instance.left)) - cast(*void)(*instance)));
        assert(size_of(type_of(BorderElementConfig.left)) == 20, "BorderElementConfig.left has unexpected size % instead of 20", size_of(type_of(BorderElementConfig.left)));
        assert(((cast(*void)(*instance.right)) - cast(*void)(*instance)) == 20, "BorderElementConfig.right has unexpected offset % instead of 20", ((cast(*void)(*instance.right)) - cast(*void)(*instance)));
        assert(size_of(type_of(BorderElementConfig.right)) == 20, "BorderElementConfig.right has unexpected size % instead of 20", size_of(type_of(BorderElementConfig.right)));
        assert(((cast(*void)(*instance.top)) - cast(*void)(*instance)) == 40, "BorderElementConfig.top has unexpected offset % instead of 40", ((cast(*void)(*instance.top)) - cast(*void)(*instance)));
        assert(size_of(type_of(BorderElementConfig.top)) == 20, "BorderElementConfig.top has unexpected size % instead of 20", size_of(type_of(BorderElementConfig.top)));
        assert(((cast(*void)(*instance.bottom)) - cast(*void)(*instance)) == 60, "BorderElementConfig.bottom has unexpected offset % instead of 60", ((cast(*void)(*instance.bottom)) - cast(*void)(*instance)));
        assert(size_of(type_of(BorderElementConfig.bottom)) == 20, "BorderElementConfig.bottom has unexpected size % instead of 20", size_of(type_of(BorderElementConfig.bottom)));
        assert(((cast(*void)(*instance.betweenChildren)) - cast(*void)(*instance)) == 80, "BorderElementConfig.betweenChildren has unexpected offset % instead of 80", ((cast(*void)(*instance.betweenChildren)) - cast(*void)(*instance)));
        assert(size_of(type_of(BorderElementConfig.betweenChildren)) == 20, "BorderElementConfig.betweenChildren has unexpected size % instead of 20", size_of(type_of(BorderElementConfig.betweenChildren)));
        assert(((cast(*void)(*instance.cornerRadius)) - cast(*void)(*instance)) == 100, "BorderElementConfig.cornerRadius has unexpected offset % instead of 100", ((cast(*void)(*instance.cornerRadius)) - cast(*void)(*instance)));
        assert(size_of(type_of(BorderElementConfig.cornerRadius)) == 16, "BorderElementConfig.cornerRadius has unexpected size % instead of 16", size_of(type_of(BorderElementConfig.cornerRadius)));
        assert(size_of(BorderElementConfig) == 116, "BorderElementConfig has size % instead of 116", size_of(BorderElementConfig));
    }

    {
        instance: ElementConfigUnion;
        assert(((cast(*void)(*instance.rectangleElementConfig)) - cast(*void)(*instance)) == 0, "ElementConfigUnion.rectangleElementConfig has unexpected offset % instead of 0", ((cast(*void)(*instance.rectangleElementConfig)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementConfigUnion.rectangleElementConfig)) == 8, "ElementConfigUnion.rectangleElementConfig has unexpected size % instead of 8", size_of(type_of(ElementConfigUnion.rectangleElementConfig)));
        assert(((cast(*void)(*instance.textElementConfig)) - cast(*void)(*instance)) == 0, "ElementConfigUnion.textElementConfig has unexpected offset % instead of 0", ((cast(*void)(*instance.textElementConfig)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementConfigUnion.textElementConfig)) == 8, "ElementConfigUnion.textElementConfig has unexpected size % instead of 8", size_of(type_of(ElementConfigUnion.textElementConfig)));
        assert(((cast(*void)(*instance.imageElementConfig)) - cast(*void)(*instance)) == 0, "ElementConfigUnion.imageElementConfig has unexpected offset % instead of 0", ((cast(*void)(*instance.imageElementConfig)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementConfigUnion.imageElementConfig)) == 8, "ElementConfigUnion.imageElementConfig has unexpected size % instead of 8", size_of(type_of(ElementConfigUnion.imageElementConfig)));
        assert(((cast(*void)(*instance.floatingElementConfig)) - cast(*void)(*instance)) == 0, "ElementConfigUnion.floatingElementConfig has unexpected offset % instead of 0", ((cast(*void)(*instance.floatingElementConfig)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementConfigUnion.floatingElementConfig)) == 8, "ElementConfigUnion.floatingElementConfig has unexpected size % instead of 8", size_of(type_of(ElementConfigUnion.floatingElementConfig)));
        assert(((cast(*void)(*instance.customElementConfig)) - cast(*void)(*instance)) == 0, "ElementConfigUnion.customElementConfig has unexpected offset % instead of 0", ((cast(*void)(*instance.customElementConfig)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementConfigUnion.customElementConfig)) == 8, "ElementConfigUnion.customElementConfig has unexpected size % instead of 8", size_of(type_of(ElementConfigUnion.customElementConfig)));
        assert(((cast(*void)(*instance.scrollElementConfig)) - cast(*void)(*instance)) == 0, "ElementConfigUnion.scrollElementConfig has unexpected offset % instead of 0", ((cast(*void)(*instance.scrollElementConfig)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementConfigUnion.scrollElementConfig)) == 8, "ElementConfigUnion.scrollElementConfig has unexpected size % instead of 8", size_of(type_of(ElementConfigUnion.scrollElementConfig)));
        assert(((cast(*void)(*instance.borderElementConfig)) - cast(*void)(*instance)) == 0, "ElementConfigUnion.borderElementConfig has unexpected offset % instead of 0", ((cast(*void)(*instance.borderElementConfig)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementConfigUnion.borderElementConfig)) == 8, "ElementConfigUnion.borderElementConfig has unexpected size % instead of 8", size_of(type_of(ElementConfigUnion.borderElementConfig)));
        assert(size_of(ElementConfigUnion) == 8, "ElementConfigUnion has size % instead of 8", size_of(ElementConfigUnion));
    }

    {
        instance: ElementConfig;
        assert(((cast(*void)(*instance.type)) - cast(*void)(*instance)) == 0, "ElementConfig.type has unexpected offset % instead of 0", ((cast(*void)(*instance.type)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementConfig.type)) == 4, "ElementConfig.type has unexpected size % instead of 4", size_of(type_of(ElementConfig.type)));
        assert(((cast(*void)(*instance.config)) - cast(*void)(*instance)) == 8, "ElementConfig.config has unexpected offset % instead of 8", ((cast(*void)(*instance.config)) - cast(*void)(*instance)));
        assert(size_of(type_of(ElementConfig.config)) == 8, "ElementConfig.config has unexpected size % instead of 8", size_of(type_of(ElementConfig.config)));
        assert(size_of(ElementConfig) == 16, "ElementConfig has size % instead of 16", size_of(ElementConfig));
    }

    {
        instance: ScrollContainerData;
        assert(((cast(*void)(*instance.scrollPosition)) - cast(*void)(*instance)) == 0, "ScrollContainerData.scrollPosition has unexpected offset % instead of 0", ((cast(*void)(*instance.scrollPosition)) - cast(*void)(*instance)));
        assert(size_of(type_of(ScrollContainerData.scrollPosition)) == 8, "ScrollContainerData.scrollPosition has unexpected size % instead of 8", size_of(type_of(ScrollContainerData.scrollPosition)));
        assert(((cast(*void)(*instance.scrollContainerDimensions)) - cast(*void)(*instance)) == 8, "ScrollContainerData.scrollContainerDimensions has unexpected offset % instead of 8", ((cast(*void)(*instance.scrollContainerDimensions)) - cast(*void)(*instance)));
        assert(size_of(type_of(ScrollContainerData.scrollContainerDimensions)) == 8, "ScrollContainerData.scrollContainerDimensions has unexpected size % instead of 8", size_of(type_of(ScrollContainerData.scrollContainerDimensions)));
        assert(((cast(*void)(*instance.contentDimensions)) - cast(*void)(*instance)) == 16, "ScrollContainerData.contentDimensions has unexpected offset % instead of 16", ((cast(*void)(*instance.contentDimensions)) - cast(*void)(*instance)));
        assert(size_of(type_of(ScrollContainerData.contentDimensions)) == 8, "ScrollContainerData.contentDimensions has unexpected size % instead of 8", size_of(type_of(ScrollContainerData.contentDimensions)));
        assert(((cast(*void)(*instance.config)) - cast(*void)(*instance)) == 24, "ScrollContainerData.config has unexpected offset % instead of 24", ((cast(*void)(*instance.config)) - cast(*void)(*instance)));
        assert(size_of(type_of(ScrollContainerData.config)) == 2, "ScrollContainerData.config has unexpected size % instead of 2", size_of(type_of(ScrollContainerData.config)));
        assert(((cast(*void)(*instance.found)) - cast(*void)(*instance)) == 26, "ScrollContainerData.found has unexpected offset % instead of 26", ((cast(*void)(*instance.found)) - cast(*void)(*instance)));
        assert(size_of(type_of(ScrollContainerData.found)) == 1, "ScrollContainerData.found has unexpected size % instead of 1", size_of(type_of(ScrollContainerData.found)));
        assert(size_of(ScrollContainerData) == 32, "ScrollContainerData has size % instead of 32", size_of(ScrollContainerData));
    }

    {
        instance: RenderCommand;
        assert(((cast(*void)(*instance.boundingBox)) - cast(*void)(*instance)) == 0, "RenderCommand.boundingBox has unexpected offset % instead of 0", ((cast(*void)(*instance.boundingBox)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderCommand.boundingBox)) == 16, "RenderCommand.boundingBox has unexpected size % instead of 16", size_of(type_of(RenderCommand.boundingBox)));
        assert(((cast(*void)(*instance.config)) - cast(*void)(*instance)) == 16, "RenderCommand.config has unexpected offset % instead of 16", ((cast(*void)(*instance.config)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderCommand.config)) == 8, "RenderCommand.config has unexpected size % instead of 8", size_of(type_of(RenderCommand.config)));
        assert(((cast(*void)(*instance.text)) - cast(*void)(*instance)) == 24, "RenderCommand.text has unexpected offset % instead of 24", ((cast(*void)(*instance.text)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderCommand.text)) == 16, "RenderCommand.text has unexpected size % instead of 16", size_of(type_of(RenderCommand.text)));
        assert(((cast(*void)(*instance.id)) - cast(*void)(*instance)) == 40, "RenderCommand.id has unexpected offset % instead of 40", ((cast(*void)(*instance.id)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderCommand.id)) == 4, "RenderCommand.id has unexpected size % instead of 4", size_of(type_of(RenderCommand.id)));
        assert(((cast(*void)(*instance.commandType)) - cast(*void)(*instance)) == 44, "RenderCommand.commandType has unexpected offset % instead of 44", ((cast(*void)(*instance.commandType)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderCommand.commandType)) == 4, "RenderCommand.commandType has unexpected size % instead of 4", size_of(type_of(RenderCommand.commandType)));
        assert(size_of(RenderCommand) == 48, "RenderCommand has size % instead of 48", size_of(RenderCommand));
    }

    {
        instance: RenderCommandArray;
        assert(((cast(*void)(*instance.capacity)) - cast(*void)(*instance)) == 0, "RenderCommandArray.capacity has unexpected offset % instead of 0", ((cast(*void)(*instance.capacity)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderCommandArray.capacity)) == 4, "RenderCommandArray.capacity has unexpected size % instead of 4", size_of(type_of(RenderCommandArray.capacity)));
        assert(((cast(*void)(*instance.length)) - cast(*void)(*instance)) == 4, "RenderCommandArray.length has unexpected offset % instead of 4", ((cast(*void)(*instance.length)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderCommandArray.length)) == 4, "RenderCommandArray.length has unexpected size % instead of 4", size_of(type_of(RenderCommandArray.length)));
        assert(((cast(*void)(*instance.internalArray)) - cast(*void)(*instance)) == 8, "RenderCommandArray.internalArray has unexpected offset % instead of 8", ((cast(*void)(*instance.internalArray)) - cast(*void)(*instance)));
        assert(size_of(type_of(RenderCommandArray.internalArray)) == 8, "RenderCommandArray.internalArray has unexpected size % instead of 8", size_of(type_of(RenderCommandArray.internalArray)));
        assert(size_of(RenderCommandArray) == 16, "RenderCommandArray has size % instead of 16", size_of(RenderCommandArray));
    }

    {
        instance: PointerData;
        assert(((cast(*void)(*instance.position)) - cast(*void)(*instance)) == 0, "PointerData.position has unexpected offset % instead of 0", ((cast(*void)(*instance.position)) - cast(*void)(*instance)));
        assert(size_of(type_of(PointerData.position)) == 8, "PointerData.position has unexpected size % instead of 8", size_of(type_of(PointerData.position)));
        assert(((cast(*void)(*instance.state)) - cast(*void)(*instance)) == 8, "PointerData.state has unexpected offset % instead of 8", ((cast(*void)(*instance.state)) - cast(*void)(*instance)));
        assert(size_of(type_of(PointerData.state)) == 4, "PointerData.state has unexpected size % instead of 4", size_of(type_of(PointerData.state)));
        assert(size_of(PointerData) == 12, "PointerData has size % instead of 12", size_of(PointerData));
    }
}

