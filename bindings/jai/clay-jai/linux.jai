//
// This file was auto-generated using the following command:
//
// jai generate.jai - -compile
//



// Utility Structs -------------------------
// Note: Clay_String is not guaranteed to be null terminated. It may be if created from a literal C string,
// but it is also used to represent slices.
String :: struct {
    length: s32;
    chars:  *u8;
}

_StringArray :: struct {
    capacity:      s32;
    length:        s32;
    internalArray: *String;
}

Context :: struct {}

Arena :: struct {
    nextAllocation: u64;
    capacity:       u64;
    memory:         *u8;
}

Dimensions :: struct {
    width:  float;
    height: float;
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

LayoutDirection :: enum u8 {
    LEFT_TO_RIGHT :: 0;
    TOP_TO_BOTTOM :: 1;
    CLAY_LEFT_TO_RIGHT :: LEFT_TO_RIGHT;
    CLAY_TOP_TO_BOTTOM :: TOP_TO_BOTTOM;
}

LayoutAlignmentX :: enum u8 {
    LEFT   :: 0;
    RIGHT  :: 1;
    CENTER :: 2;
    CLAY_ALIGN_X_LEFT   :: LEFT;
    CLAY_ALIGN_X_RIGHT  :: RIGHT;
    CLAY_ALIGN_X_CENTER :: CENTER;
}

LayoutAlignmentY :: enum u8 {
    TOP    :: 0;
    BOTTOM :: 1;
    CENTER :: 2;
    CLAY_ALIGN_Y_TOP    :: TOP;
    CLAY_ALIGN_Y_BOTTOM :: BOTTOM;
    CLAY_ALIGN_Y_CENTER :: CENTER;
}

_SizingType :: enum u8 {
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
    size: union {
        minMax:  SizingMinMax;
        percent: float;
    };
    type: _SizingType;
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
// NOTE: Not declared in the typedef as an ifdef inside macro arguments is UB
RectangleElementConfig :: struct {
    color:        Color;
    cornerRadius: CornerRadius;
}

// Text
TextElementConfigWrapMode :: enum u32 {
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

FloatingAttachPointType :: enum u8 {
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

PointerCaptureMode :: enum u32 {
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
    found:                     bool;
}

RenderCommandType :: enum u8 {
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
    capacity:      s32;
    length:        s32;
    internalArray: *RenderCommand;
}

PointerDataInteractionState :: enum u32 {
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

ErrorType :: enum u32 {
    TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED :: 0;
    ARENA_CAPACITY_EXCEEDED                :: 1;
    ELEMENTS_CAPACITY_EXCEEDED             :: 2;
    TEXT_MEASUREMENT_CAPACITY_EXCEEDED     :: 3;
    DUPLICATE_ID                           :: 4;
    FLOATING_CONTAINER_PARENT_NOT_FOUND    :: 5;
    INTERNAL_ERROR                         :: 6;
    CLAY_ERROR_TYPE_TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED :: TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED;
    CLAY_ERROR_TYPE_ARENA_CAPACITY_EXCEEDED                :: ARENA_CAPACITY_EXCEEDED;
    CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED             :: ELEMENTS_CAPACITY_EXCEEDED;
    CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED     :: TEXT_MEASUREMENT_CAPACITY_EXCEEDED;
    CLAY_ERROR_TYPE_DUPLICATE_ID                           :: DUPLICATE_ID;
    CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND    :: FLOATING_CONTAINER_PARENT_NOT_FOUND;
    CLAY_ERROR_TYPE_INTERNAL_ERROR                         :: INTERNAL_ERROR;
}

ErrorData :: struct {
    errorType: ErrorType;
    errorText: String;
    userData:  u64;
}

ErrorHandler :: struct {
    errorHandlerFunction: #type (errorText: ErrorData) -> void #c_call;
    userData:             u64;
}

// Function Forward Declarations ---------------------------------
// Public API functions ---
MinMemorySize :: () -> u32 #foreign clay "Clay_MinMemorySize";
CreateArenaWithCapacityAndMemory :: (capacity: u32, offset: *void) -> Arena #foreign clay "Clay_CreateArenaWithCapacityAndMemory";
SetPointerState :: (position: Vector2, pointerDown: bool) -> void #foreign clay "Clay_SetPointerState";
Initialize :: (arena: Arena, layoutDimensions: Dimensions, errorHandler: ErrorHandler) -> *Context #foreign clay "Clay_Initialize";
GetCurrentContext :: () -> *Context #foreign clay "Clay_GetCurrentContext";
SetCurrentContext :: (_context: *Context) -> void #foreign clay "Clay_SetCurrentContext";
UpdateScrollContainers :: (enableDragScrolling: bool, scrollDelta: Vector2, deltaTime: float) -> void #foreign clay "Clay_UpdateScrollContainers";
SetLayoutDimensions :: (dimensions: Dimensions) -> void #foreign clay "Clay_SetLayoutDimensions";
BeginLayout :: () -> void #foreign clay "Clay_BeginLayout";
EndLayout :: () -> RenderCommandArray #foreign clay "Clay_EndLayout";
GetElementId :: (idString: String) -> ElementId #foreign clay "Clay_GetElementId";
GetElementIdWithIndex :: (idString: String, index: u32) -> ElementId #foreign clay "Clay_GetElementIdWithIndex";

PointerOver :: (elementId: ElementId) -> bool #foreign clay "Clay_PointerOver";
GetScrollContainerData :: (id: ElementId) -> ScrollContainerData #foreign clay "Clay_GetScrollContainerData";
SetMeasureTextFunction :: (measureTextFunction: #type (text: *String, config: *TextElementConfig) -> Dimensions #c_call) -> void #foreign clay "Clay_SetMeasureTextFunction";
SetQueryScrollOffsetFunction :: (queryScrollOffsetFunction: #type (elementId: u32) -> Vector2 #c_call) -> void #foreign clay "Clay_SetQueryScrollOffsetFunction";
RenderCommandArray_Get :: (array: *RenderCommandArray, index: s32) -> *RenderCommand #foreign clay "Clay_RenderCommandArray_Get";
SetDebugModeEnabled :: (enabled: bool) -> void #foreign clay "Clay_SetDebugModeEnabled";
IsDebugModeEnabled :: () -> bool #foreign clay "Clay_IsDebugModeEnabled";
SetCullingEnabled :: (enabled: bool) -> void #foreign clay "Clay_SetCullingEnabled";
GetMaxElementCount :: () -> s32 #foreign clay "Clay_GetMaxElementCount";
SetMaxElementCount :: (maxElementCount: s32) -> void #foreign clay "Clay_SetMaxElementCount";
GetMaxMeasureTextCacheWordCount :: () -> s32 #foreign clay "Clay_GetMaxMeasureTextCacheWordCount";
SetMaxMeasureTextCacheWordCount :: (maxMeasureTextCacheWordCount: s32) -> void #foreign clay "Clay_SetMaxMeasureTextCacheWordCount";

// Internal API functions required by macros
_OpenElement :: () -> void #foreign clay "Clay__OpenElement";
_CloseElement :: () -> void #foreign clay "Clay__CloseElement";
_StoreLayoutConfig :: (config: LayoutConfig) -> *LayoutConfig #foreign clay "Clay__StoreLayoutConfig";
_ElementPostConfiguration :: () -> void #foreign clay "Clay__ElementPostConfiguration";
_AttachId :: (id: ElementId) -> void #foreign clay "Clay__AttachId";
_AttachLayoutConfig :: (config: *LayoutConfig) -> void #foreign clay "Clay__AttachLayoutConfig";
_AttachElementConfig :: (config: ElementConfigUnion, type: ElementConfigType) -> void #foreign clay "Clay__AttachElementConfig";
_StoreRectangleElementConfig :: (config: RectangleElementConfig) -> *RectangleElementConfig #foreign clay "Clay__StoreRectangleElementConfig";
_StoreTextElementConfig :: (config: TextElementConfig) -> *TextElementConfig #foreign clay "Clay__StoreTextElementConfig";
_StoreImageElementConfig :: (config: ImageElementConfig) -> *ImageElementConfig #foreign clay "Clay__StoreImageElementConfig";
_StoreFloatingElementConfig :: (config: FloatingElementConfig) -> *FloatingElementConfig #foreign clay "Clay__StoreFloatingElementConfig";
_StoreCustomElementConfig :: (config: CustomElementConfig) -> *CustomElementConfig #foreign clay "Clay__StoreCustomElementConfig";
_StoreScrollElementConfig :: (config: ScrollElementConfig) -> *ScrollElementConfig #foreign clay "Clay__StoreScrollElementConfig";
_StoreBorderElementConfig :: (config: BorderElementConfig) -> *BorderElementConfig #foreign clay "Clay__StoreBorderElementConfig";
_HashString :: (key: String, offset: u32, seed: u32) -> ElementId #foreign clay "Clay__HashString";
_OpenTextElement :: (text: String, textConfig: *TextElementConfig) -> void #foreign clay "Clay__OpenTextElement";
_GetParentElementId :: () -> u32 #foreign clay "Clay__GetParentElementId";

_debugViewHighlightColor: Color #elsewhere clay "Clay__debugViewHighlightColor";
_debugViewWidth: u32 #elsewhere clay "Clay__debugViewWidth";

#scope_file

#import "Basic"; // For assert

clay :: #library,no_dll "linux/clay";

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
        instance: _StringArray;
        assert(((cast(*void)(*instance.capacity)) - cast(*void)(*instance)) == 0, "_StringArray.capacity has unexpected offset % instead of 0", ((cast(*void)(*instance.capacity)) - cast(*void)(*instance)));
        assert(size_of(type_of(_StringArray.capacity)) == 4, "_StringArray.capacity has unexpected size % instead of 4", size_of(type_of(_StringArray.capacity)));
        assert(((cast(*void)(*instance.length)) - cast(*void)(*instance)) == 4, "_StringArray.length has unexpected offset % instead of 4", ((cast(*void)(*instance.length)) - cast(*void)(*instance)));
        assert(size_of(type_of(_StringArray.length)) == 4, "_StringArray.length has unexpected size % instead of 4", size_of(type_of(_StringArray.length)));
        assert(((cast(*void)(*instance.internalArray)) - cast(*void)(*instance)) == 8, "_StringArray.internalArray has unexpected offset % instead of 8", ((cast(*void)(*instance.internalArray)) - cast(*void)(*instance)));
        assert(size_of(type_of(_StringArray.internalArray)) == 8, "_StringArray.internalArray has unexpected size % instead of 8", size_of(type_of(_StringArray.internalArray)));
        assert(size_of(_StringArray) == 16, "_StringArray has size % instead of 16", size_of(_StringArray));
    }

    {
        instance: Arena;
        assert(((cast(*void)(*instance.nextAllocation)) - cast(*void)(*instance)) == 0, "Arena.nextAllocation has unexpected offset % instead of 0", ((cast(*void)(*instance.nextAllocation)) - cast(*void)(*instance)));
        assert(size_of(type_of(Arena.nextAllocation)) == 8, "Arena.nextAllocation has unexpected size % instead of 8", size_of(type_of(Arena.nextAllocation)));
        assert(((cast(*void)(*instance.capacity)) - cast(*void)(*instance)) == 8, "Arena.capacity has unexpected offset % instead of 8", ((cast(*void)(*instance.capacity)) - cast(*void)(*instance)));
        assert(size_of(type_of(Arena.capacity)) == 8, "Arena.capacity has unexpected size % instead of 8", size_of(type_of(Arena.capacity)));
        assert(((cast(*void)(*instance.memory)) - cast(*void)(*instance)) == 16, "Arena.memory has unexpected offset % instead of 16", ((cast(*void)(*instance.memory)) - cast(*void)(*instance)));
        assert(size_of(type_of(Arena.memory)) == 8, "Arena.memory has unexpected size % instead of 8", size_of(type_of(Arena.memory)));
        assert(size_of(Arena) == 24, "Arena has size % instead of 24", size_of(Arena));
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
        instance: Color;
        assert(((cast(*void)(*instance.r)) - cast(*void)(*instance)) == 0, "Color.r has unexpected offset % instead of 0", ((cast(*void)(*instance.r)) - cast(*void)(*instance)));
        assert(size_of(type_of(Color.r)) == 4, "Color.r has unexpected size % instead of 4", size_of(type_of(Color.r)));
        assert(((cast(*void)(*instance.g)) - cast(*void)(*instance)) == 4, "Color.g has unexpected offset % instead of 4", ((cast(*void)(*instance.g)) - cast(*void)(*instance)));
        assert(size_of(type_of(Color.g)) == 4, "Color.g has unexpected size % instead of 4", size_of(type_of(Color.g)));
        assert(((cast(*void)(*instance.b)) - cast(*void)(*instance)) == 8, "Color.b has unexpected offset % instead of 8", ((cast(*void)(*instance.b)) - cast(*void)(*instance)));
        assert(size_of(type_of(Color.b)) == 4, "Color.b has unexpected size % instead of 4", size_of(type_of(Color.b)));
        assert(((cast(*void)(*instance.a)) - cast(*void)(*instance)) == 12, "Color.a has unexpected offset % instead of 12", ((cast(*void)(*instance.a)) - cast(*void)(*instance)));
        assert(size_of(type_of(Color.a)) == 4, "Color.a has unexpected size % instead of 4", size_of(type_of(Color.a)));
        assert(size_of(Color) == 16, "Color has size % instead of 16", size_of(Color));
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
        assert(size_of(type_of(ChildAlignment.x)) == 1, "ChildAlignment.x has unexpected size % instead of 1", size_of(type_of(ChildAlignment.x)));
        assert(((cast(*void)(*instance.y)) - cast(*void)(*instance)) == 1, "ChildAlignment.y has unexpected offset % instead of 1", ((cast(*void)(*instance.y)) - cast(*void)(*instance)));
        assert(size_of(type_of(ChildAlignment.y)) == 1, "ChildAlignment.y has unexpected size % instead of 1", size_of(type_of(ChildAlignment.y)));
        assert(size_of(ChildAlignment) == 2, "ChildAlignment has size % instead of 2", size_of(ChildAlignment));
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
        assert(((cast(*void)(*instance.size)) - cast(*void)(*instance)) == 0, "SizingAxis.size has unexpected offset % instead of 0", ((cast(*void)(*instance.size)) - cast(*void)(*instance)));
        assert(size_of(type_of(SizingAxis.size)) == 8, "SizingAxis.size has unexpected size % instead of 8", size_of(type_of(SizingAxis.size)));
        assert(((cast(*void)(*instance.type)) - cast(*void)(*instance)) == 8, "SizingAxis.type has unexpected offset % instead of 8", ((cast(*void)(*instance.type)) - cast(*void)(*instance)));
        assert(size_of(type_of(SizingAxis.type)) == 1, "SizingAxis.type has unexpected size % instead of 1", size_of(type_of(SizingAxis.type)));
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
        assert(((cast(*void)(*instance.childAlignment)) - cast(*void)(*instance)) == 30, "LayoutConfig.childAlignment has unexpected offset % instead of 30", ((cast(*void)(*instance.childAlignment)) - cast(*void)(*instance)));
        assert(size_of(type_of(LayoutConfig.childAlignment)) == 2, "LayoutConfig.childAlignment has unexpected size % instead of 2", size_of(type_of(LayoutConfig.childAlignment)));
        assert(((cast(*void)(*instance.layoutDirection)) - cast(*void)(*instance)) == 32, "LayoutConfig.layoutDirection has unexpected offset % instead of 32", ((cast(*void)(*instance.layoutDirection)) - cast(*void)(*instance)));
        assert(size_of(type_of(LayoutConfig.layoutDirection)) == 1, "LayoutConfig.layoutDirection has unexpected size % instead of 1", size_of(type_of(LayoutConfig.layoutDirection)));
        assert(size_of(LayoutConfig) == 36, "LayoutConfig has size % instead of 36", size_of(LayoutConfig));
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
        assert(size_of(type_of(FloatingAttachPoints.element)) == 1, "FloatingAttachPoints.element has unexpected size % instead of 1", size_of(type_of(FloatingAttachPoints.element)));
        assert(((cast(*void)(*instance.parent)) - cast(*void)(*instance)) == 1, "FloatingAttachPoints.parent has unexpected offset % instead of 1", ((cast(*void)(*instance.parent)) - cast(*void)(*instance)));
        assert(size_of(type_of(FloatingAttachPoints.parent)) == 1, "FloatingAttachPoints.parent has unexpected size % instead of 1", size_of(type_of(FloatingAttachPoints.parent)));
        assert(size_of(FloatingAttachPoints) == 2, "FloatingAttachPoints has size % instead of 2", size_of(FloatingAttachPoints));
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
        assert(size_of(type_of(FloatingElementConfig.attachment)) == 2, "FloatingElementConfig.attachment has unexpected size % instead of 2", size_of(type_of(FloatingElementConfig.attachment)));
        assert(((cast(*void)(*instance.pointerCaptureMode)) - cast(*void)(*instance)) == 28, "FloatingElementConfig.pointerCaptureMode has unexpected offset % instead of 28", ((cast(*void)(*instance.pointerCaptureMode)) - cast(*void)(*instance)));
        assert(size_of(type_of(FloatingElementConfig.pointerCaptureMode)) == 4, "FloatingElementConfig.pointerCaptureMode has unexpected size % instead of 4", size_of(type_of(FloatingElementConfig.pointerCaptureMode)));
        assert(size_of(FloatingElementConfig) == 32, "FloatingElementConfig has size % instead of 32", size_of(FloatingElementConfig));
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
        assert(size_of(type_of(ElementConfig.type)) == 1, "ElementConfig.type has unexpected size % instead of 1", size_of(type_of(ElementConfig.type)));
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
        assert(size_of(type_of(RenderCommand.commandType)) == 1, "RenderCommand.commandType has unexpected size % instead of 1", size_of(type_of(RenderCommand.commandType)));
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

    {
        instance: ErrorData;
        assert(((cast(*void)(*instance.errorType)) - cast(*void)(*instance)) == 0, "ErrorData.errorType has unexpected offset % instead of 0", ((cast(*void)(*instance.errorType)) - cast(*void)(*instance)));
        assert(size_of(type_of(ErrorData.errorType)) == 4, "ErrorData.errorType has unexpected size % instead of 4", size_of(type_of(ErrorData.errorType)));
        assert(((cast(*void)(*instance.errorText)) - cast(*void)(*instance)) == 8, "ErrorData.errorText has unexpected offset % instead of 8", ((cast(*void)(*instance.errorText)) - cast(*void)(*instance)));
        assert(size_of(type_of(ErrorData.errorText)) == 16, "ErrorData.errorText has unexpected size % instead of 16", size_of(type_of(ErrorData.errorText)));
        assert(((cast(*void)(*instance.userData)) - cast(*void)(*instance)) == 24, "ErrorData.userData has unexpected offset % instead of 24", ((cast(*void)(*instance.userData)) - cast(*void)(*instance)));
        assert(size_of(type_of(ErrorData.userData)) == 8, "ErrorData.userData has unexpected size % instead of 8", size_of(type_of(ErrorData.userData)));
        assert(size_of(ErrorData) == 32, "ErrorData has size % instead of 32", size_of(ErrorData));
    }

    {
        instance: ErrorHandler;
        assert(((cast(*void)(*instance.errorHandlerFunction)) - cast(*void)(*instance)) == 0, "ErrorHandler.errorHandlerFunction has unexpected offset % instead of 0", ((cast(*void)(*instance.errorHandlerFunction)) - cast(*void)(*instance)));
        assert(size_of(type_of(ErrorHandler.errorHandlerFunction)) == 8, "ErrorHandler.errorHandlerFunction has unexpected size % instead of 8", size_of(type_of(ErrorHandler.errorHandlerFunction)));
        assert(((cast(*void)(*instance.userData)) - cast(*void)(*instance)) == 8, "ErrorHandler.userData has unexpected offset % instead of 8", ((cast(*void)(*instance.userData)) - cast(*void)(*instance)));
        assert(size_of(type_of(ErrorHandler.userData)) == 8, "ErrorHandler.userData has unexpected size % instead of 8", size_of(type_of(ErrorHandler.userData)));
        assert(size_of(ErrorHandler) == 16, "ErrorHandler has size % instead of 16", size_of(ErrorHandler));
    }
}

