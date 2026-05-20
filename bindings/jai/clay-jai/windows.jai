//
// This file was auto-generated using the following command:
//
// jai generate.jai - -compile
//



// NOTE: If you need to get the offset for other standard types in the future, add them here.
_Alignpointer :: struct {
    c: u8;
    x: *void;
}
_Alignbool :: struct {
    c: u8;
    x: bool;
}
_Alignuint8_t :: struct {
    c: u8;
    x: u8;
}
_Alignint32_t :: struct {
    c: u8;
    x: s32;
}

// Utility Structs -------------------------
// Note: Clay_String is not guaranteed to be null terminated. It may be if created from a literal C string,
// but it is also used to represent slices.
String :: struct {
    length: s32;
    chars:  *u8;
}
_AlignClay_String :: struct {
    c: u8;
    // Utility Structs -------------------------
    // Note: Clay_String is not guaranteed to be null terminated. It may be if created from a literal C string,
    // but it is also used to represent slices.
    x: String;
}

_StringArray :: struct {
    capacity:      s32;
    length:        s32;
    internalArray: *String;
}
_AlignClay__StringArray :: struct {
    c: u8;
    x: _StringArray;
}

Arena :: struct {
    nextAllocation: u64;
    capacity:       u64;
    memory:         *u8;
}
_AlignClay_Arena :: struct {
    c: u8;
    x: Arena;
}

Dimensions :: struct {
    width:  float;
    height: float;
}
_AlignClay_Dimensions :: struct {
    c: u8;
    x: Dimensions;
}

_AlignClay_Vector2 :: struct {
    c: u8;
    x: Vector2;
}

Color :: struct {
    r: float;
    g: float;
    b: float;
    a: float;
}
_AlignClay_Color :: struct {
    c: u8;
    x: Color;
}

BoundingBox :: struct {
    x:      float;
    y:      float;
    width:  float;
    height: float;
}
_AlignClay_BoundingBox :: struct {
    c: u8;
    x: BoundingBox;
}

// baseId + offset = id
ElementId :: struct {
    id:       u32;
    offset:   u32;
    baseId:   u32;
    stringId: String;
}
_AlignClay_ElementId :: struct {
    c: u8;
    // baseId + offset = id
    x: ElementId;
}

CornerRadius :: struct {
    topLeft:     float;
    topRight:    float;
    bottomLeft:  float;
    bottomRight: float;
}
_AlignClay_CornerRadius :: struct {
    c: u8;
    x: CornerRadius;
}

LayoutDirection :: enum s32 {
    LEFT_TO_RIGHT :: 0;
    TOP_TO_BOTTOM :: 1;
    CLAY_LEFT_TO_RIGHT :: LEFT_TO_RIGHT;
    CLAY_TOP_TO_BOTTOM :: TOP_TO_BOTTOM;
}
_AlignClay_LayoutDirection :: struct {
    c: u8;
    // Element Configs ---------------------------
    // Layout
    x: LayoutDirection;
}

LayoutAlignmentX :: enum s32 {
    LEFT   :: 0;
    RIGHT  :: 1;
    CENTER :: 2;
    CLAY_ALIGN_X_LEFT   :: LEFT;
    CLAY_ALIGN_X_RIGHT  :: RIGHT;
    CLAY_ALIGN_X_CENTER :: CENTER;
}
_AlignClay_LayoutAlignmentX :: struct {
    c: u8;
    x: LayoutAlignmentX;
}

LayoutAlignmentY :: enum s32 {
    TOP    :: 0;
    BOTTOM :: 1;
    CENTER :: 2;
    CLAY_ALIGN_Y_TOP    :: TOP;
    CLAY_ALIGN_Y_BOTTOM :: BOTTOM;
    CLAY_ALIGN_Y_CENTER :: CENTER;
}
_AlignClay_LayoutAlignmentY :: struct {
    c: u8;
    x: LayoutAlignmentY;
}

_SizingType :: enum s32 {
    FIT     :: 0;
    GROW    :: 1;
    PERCENT :: 2;
    FIXED   :: 3;
    CLAY__SIZING_TYPE_FIT     :: FIT;
    CLAY__SIZING_TYPE_GROW    :: GROW;
    CLAY__SIZING_TYPE_PERCENT :: PERCENT;
    CLAY__SIZING_TYPE_FIXED   :: FIXED;
}
_AlignClay__SizingType :: struct {
    c: u8;
    x: _SizingType;
}

ChildAlignment :: struct {
    x: LayoutAlignmentX;
    y: LayoutAlignmentY;
}
_AlignClay_ChildAlignment :: struct {
    c: u8;
    x: ChildAlignment;
}

SizingMinMax :: struct {
    min: float;
    max: float;
}
_AlignClay_SizingMinMax :: struct {
    c: u8;
    x: SizingMinMax;
}

SizingAxis :: struct {
    size: union {
        minMax:  SizingMinMax;
        percent: float;
    };
    type: _SizingType;
}
_AlignClay_SizingAxis :: struct {
    c: u8;
    x: SizingAxis;
}

Sizing :: struct {
    width:  SizingAxis;
    height: SizingAxis;
}
_AlignClay_Sizing :: struct {
    c: u8;
    x: Sizing;
}

Padding :: struct {
    x: u16;
    y: u16;
}
_AlignClay_Padding :: struct {
    c: u8;
    x: Padding;
}

LayoutConfig :: struct {
    sizing:          Sizing;
    padding:         Padding;
    childGap:        u16;
    childAlignment:  ChildAlignment;
    layoutDirection: LayoutDirection;
}
_AlignClay_LayoutConfig :: struct {
    c: u8;
    x: LayoutConfig;
}

CLAY_LAYOUT_DEFAULT: LayoutConfig #elsewhere clay;

// Rectangle
// NOTE: Not declared in the typedef as an ifdef inside macro arguments is UB
RectangleElementConfig :: struct {
    color:        Color;
    cornerRadius: CornerRadius;
}

_AlignClay_RectangleElementConfig :: struct {
    c: u8;
    x: RectangleElementConfig;
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
_AlignClay_TextElementConfigWrapMode :: struct {
    c: u8;
    // Text
    x: TextElementConfigWrapMode;
}

TextElementConfig :: struct {
    textColor:     Color;
    fontId:        u16;
    fontSize:      u16;
    letterSpacing: u16;
    lineHeight:    u16;
    wrapMode:      TextElementConfigWrapMode;
}

_AlignClay_TextElementConfig :: struct {
    c: u8;
    x: TextElementConfig;
}

// Image
ImageElementConfig :: struct {
    imageData:        *void;
    sourceDimensions: Dimensions;
}

_AlignClay_ImageElementConfig :: struct {
    c: u8;
    x: ImageElementConfig;
}

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
_AlignClay_FloatingAttachPointType :: struct {
    c: u8;
    // Floating
    x: FloatingAttachPointType;
}

FloatingAttachPoints :: struct {
    element: FloatingAttachPointType;
    parent:  FloatingAttachPointType;
}
_AlignClay_FloatingAttachPoints :: struct {
    c: u8;
    x: FloatingAttachPoints;
}

PointerCaptureMode :: enum s32 {
    CAPTURE     :: 0;
    PASSTHROUGH :: 1;
    CLAY_POINTER_CAPTURE_MODE_CAPTURE     :: CAPTURE;
    CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH :: PASSTHROUGH;
}
_AlignClay_PointerCaptureMode :: struct {
    c: u8;
    x: PointerCaptureMode;
}

FloatingElementConfig :: struct {
    offset:             Vector2;
    expand:             Dimensions;
    zIndex:             u16;
    parentId:           u32;
    attachment:         FloatingAttachPoints;
    pointerCaptureMode: PointerCaptureMode;
}
_AlignClay_FloatingElementConfig :: struct {
    c: u8;
    x: FloatingElementConfig;
}

// Custom
CustomElementConfig :: struct {
    customData: *void;
}

_AlignClay_CustomElementConfig :: struct {
    c: u8;
    x: CustomElementConfig;
}

// Scroll
ScrollElementConfig :: struct {
    horizontal: bool;
    vertical:   bool;
}
_AlignClay_ScrollElementConfig :: struct {
    c: u8;
    // Scroll
    x: ScrollElementConfig;
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
_AlignClay_ElementConfigUnion :: struct {
    c: u8;
    x: ElementConfigUnion;
}

ElementConfig :: struct {
    type:   ElementConfigType;
    config: ElementConfigUnion;
}
_AlignClay_ElementConfig :: struct {
    c: u8;
    x: ElementConfig;
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
_AlignClay_ScrollContainerData :: struct {
    c: u8;
    // Miscellaneous Structs & Enums ---------------------------------
    x: ScrollContainerData;
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
_AlignClay_RenderCommandType :: struct {
    c: u8;
    x: RenderCommandType;
}

RenderCommand :: struct {
    boundingBox: BoundingBox;
    config:      ElementConfigUnion;
    text:        String; // TODO I wish there was a way to avoid having to have this on every render command
    id:          u32;
    commandType: RenderCommandType;
}
_AlignClay_RenderCommand :: struct {
    c: u8;
    x: RenderCommand;
}

RenderCommandArray :: struct {
    capacity:      s32;
    length:        s32;
    internalArray: *RenderCommand;
}
_AlignClay_RenderCommandArray :: struct {
    c: u8;
    x: RenderCommandArray;
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
_AlignClay_PointerDataInteractionState :: struct {
    c: u8;
    x: PointerDataInteractionState;
}

PointerData :: struct {
    position: Vector2;
    state:    PointerDataInteractionState;
}
_AlignClay_PointerData :: struct {
    c: u8;
    x: PointerData;
}

ErrorType :: enum s32 {
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
_AlignClay_ErrorType :: struct {
    c: u8;
    x: ErrorType;
}

ErrorData :: struct {
    errorType: ErrorType;
    errorText: String;
    userData:  u64;
}
_AlignClay_ErrorData :: struct {
    c: u8;
    x: ErrorData;
}

ErrorHandler :: struct {
    errorHandlerFunction: #type (errorText: ErrorData) -> void #c_call;
    userData:             u64;
}
_AlignClay_ErrorHandler :: struct {
    c: u8;
    x: ErrorHandler;
}

// Function Forward Declarations ---------------------------------
// Public API functions ---
MinMemorySize :: () -> u32 #foreign clay "Clay_MinMemorySize";
CreateArenaWithCapacityAndMemory :: (capacity: u32, offset: *void) -> Arena #foreign clay "Clay_CreateArenaWithCapacityAndMemory";
SetPointerState :: (position: Vector2, pointerDown: bool) -> void #foreign clay "Clay_SetPointerState";
Initialize :: (arena: Arena, layoutDimensions: Dimensions, errorHandler: ErrorHandler) -> void #foreign clay "Clay_Initialize";
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
SetMaxElementCount :: (maxElementCount: s32) -> void #foreign clay "Clay_SetMaxElementCount";
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

_debugViewHighlightColor: Color #elsewhere clay "Clay__debugViewHighlightColor";
_debugViewWidth: u32 #elsewhere clay "Clay__debugViewWidth";

#scope_file

#import "Basic"; // For assert

clay :: #library,no_dll "windows/clay";

#run {
    {
        instance: _Alignpointer;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_Alignpointer.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_Alignpointer.c)) == 1, "_Alignpointer.c has unexpected size % instead of 1", size_of(type_of(_Alignpointer.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_Alignpointer.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_Alignpointer.x)) == 8, "_Alignpointer.x has unexpected size % instead of 8", size_of(type_of(_Alignpointer.x)));
        assert(size_of(_Alignpointer) == 16, "_Alignpointer has size % instead of 16", size_of(_Alignpointer));
    }

    {
        instance: _Alignbool;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_Alignbool.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_Alignbool.c)) == 1, "_Alignbool.c has unexpected size % instead of 1", size_of(type_of(_Alignbool.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 1, "_Alignbool.x has unexpected offset % instead of 1", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_Alignbool.x)) == 1, "_Alignbool.x has unexpected size % instead of 1", size_of(type_of(_Alignbool.x)));
        assert(size_of(_Alignbool) == 2, "_Alignbool has size % instead of 2", size_of(_Alignbool));
    }

    {
        instance: _Alignuint8_t;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_Alignuint8_t.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_Alignuint8_t.c)) == 1, "_Alignuint8_t.c has unexpected size % instead of 1", size_of(type_of(_Alignuint8_t.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 1, "_Alignuint8_t.x has unexpected offset % instead of 1", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_Alignuint8_t.x)) == 1, "_Alignuint8_t.x has unexpected size % instead of 1", size_of(type_of(_Alignuint8_t.x)));
        assert(size_of(_Alignuint8_t) == 2, "_Alignuint8_t has size % instead of 2", size_of(_Alignuint8_t));
    }

    {
        instance: _Alignint32_t;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_Alignint32_t.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_Alignint32_t.c)) == 1, "_Alignint32_t.c has unexpected size % instead of 1", size_of(type_of(_Alignint32_t.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_Alignint32_t.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_Alignint32_t.x)) == 4, "_Alignint32_t.x has unexpected size % instead of 4", size_of(type_of(_Alignint32_t.x)));
        assert(size_of(_Alignint32_t) == 8, "_Alignint32_t has size % instead of 8", size_of(_Alignint32_t));
    }

    {
        instance: String;
        assert(((cast(*void)(*instance.length)) - cast(*void)(*instance)) == 0, "String.length has unexpected offset % instead of 0", ((cast(*void)(*instance.length)) - cast(*void)(*instance)));
        assert(size_of(type_of(String.length)) == 4, "String.length has unexpected size % instead of 4", size_of(type_of(String.length)));
        assert(((cast(*void)(*instance.chars)) - cast(*void)(*instance)) == 8, "String.chars has unexpected offset % instead of 8", ((cast(*void)(*instance.chars)) - cast(*void)(*instance)));
        assert(size_of(type_of(String.chars)) == 8, "String.chars has unexpected size % instead of 8", size_of(type_of(String.chars)));
        assert(size_of(String) == 16, "String has size % instead of 16", size_of(String));
    }

    {
        instance: _AlignClay_String;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_String.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_String.c)) == 1, "_AlignClay_String.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_String.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_String.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_String.x)) == 16, "_AlignClay_String.x has unexpected size % instead of 16", size_of(type_of(_AlignClay_String.x)));
        assert(size_of(_AlignClay_String) == 24, "_AlignClay_String has size % instead of 24", size_of(_AlignClay_String));
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
        instance: _AlignClay__StringArray;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay__StringArray.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay__StringArray.c)) == 1, "_AlignClay__StringArray.c has unexpected size % instead of 1", size_of(type_of(_AlignClay__StringArray.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay__StringArray.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay__StringArray.x)) == 16, "_AlignClay__StringArray.x has unexpected size % instead of 16", size_of(type_of(_AlignClay__StringArray.x)));
        assert(size_of(_AlignClay__StringArray) == 24, "_AlignClay__StringArray has size % instead of 24", size_of(_AlignClay__StringArray));
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
        instance: _AlignClay_Arena;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_Arena.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Arena.c)) == 1, "_AlignClay_Arena.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_Arena.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_Arena.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Arena.x)) == 24, "_AlignClay_Arena.x has unexpected size % instead of 24", size_of(type_of(_AlignClay_Arena.x)));
        assert(size_of(_AlignClay_Arena) == 32, "_AlignClay_Arena has size % instead of 32", size_of(_AlignClay_Arena));
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
        instance: _AlignClay_Dimensions;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_Dimensions.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Dimensions.c)) == 1, "_AlignClay_Dimensions.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_Dimensions.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_Dimensions.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Dimensions.x)) == 8, "_AlignClay_Dimensions.x has unexpected size % instead of 8", size_of(type_of(_AlignClay_Dimensions.x)));
        assert(size_of(_AlignClay_Dimensions) == 12, "_AlignClay_Dimensions has size % instead of 12", size_of(_AlignClay_Dimensions));
    }

    {
        instance: _AlignClay_Vector2;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_Vector2.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Vector2.c)) == 1, "_AlignClay_Vector2.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_Vector2.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_Vector2.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Vector2.x)) == 8, "_AlignClay_Vector2.x has unexpected size % instead of 8", size_of(type_of(_AlignClay_Vector2.x)));
        assert(size_of(_AlignClay_Vector2) == 12, "_AlignClay_Vector2 has size % instead of 12", size_of(_AlignClay_Vector2));
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
        instance: _AlignClay_Color;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_Color.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Color.c)) == 1, "_AlignClay_Color.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_Color.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_Color.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Color.x)) == 16, "_AlignClay_Color.x has unexpected size % instead of 16", size_of(type_of(_AlignClay_Color.x)));
        assert(size_of(_AlignClay_Color) == 20, "_AlignClay_Color has size % instead of 20", size_of(_AlignClay_Color));
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
        instance: _AlignClay_BoundingBox;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_BoundingBox.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_BoundingBox.c)) == 1, "_AlignClay_BoundingBox.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_BoundingBox.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_BoundingBox.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_BoundingBox.x)) == 16, "_AlignClay_BoundingBox.x has unexpected size % instead of 16", size_of(type_of(_AlignClay_BoundingBox.x)));
        assert(size_of(_AlignClay_BoundingBox) == 20, "_AlignClay_BoundingBox has size % instead of 20", size_of(_AlignClay_BoundingBox));
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
        instance: _AlignClay_ElementId;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_ElementId.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ElementId.c)) == 1, "_AlignClay_ElementId.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_ElementId.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_ElementId.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ElementId.x)) == 32, "_AlignClay_ElementId.x has unexpected size % instead of 32", size_of(type_of(_AlignClay_ElementId.x)));
        assert(size_of(_AlignClay_ElementId) == 40, "_AlignClay_ElementId has size % instead of 40", size_of(_AlignClay_ElementId));
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
        instance: _AlignClay_CornerRadius;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_CornerRadius.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_CornerRadius.c)) == 1, "_AlignClay_CornerRadius.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_CornerRadius.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_CornerRadius.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_CornerRadius.x)) == 16, "_AlignClay_CornerRadius.x has unexpected size % instead of 16", size_of(type_of(_AlignClay_CornerRadius.x)));
        assert(size_of(_AlignClay_CornerRadius) == 20, "_AlignClay_CornerRadius has size % instead of 20", size_of(_AlignClay_CornerRadius));
    }

    {
        instance: _AlignClay_LayoutDirection;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_LayoutDirection.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_LayoutDirection.c)) == 1, "_AlignClay_LayoutDirection.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_LayoutDirection.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_LayoutDirection.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_LayoutDirection.x)) == 4, "_AlignClay_LayoutDirection.x has unexpected size % instead of 4", size_of(type_of(_AlignClay_LayoutDirection.x)));
        assert(size_of(_AlignClay_LayoutDirection) == 8, "_AlignClay_LayoutDirection has size % instead of 8", size_of(_AlignClay_LayoutDirection));
    }

    {
        instance: _AlignClay_LayoutAlignmentX;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_LayoutAlignmentX.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_LayoutAlignmentX.c)) == 1, "_AlignClay_LayoutAlignmentX.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_LayoutAlignmentX.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_LayoutAlignmentX.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_LayoutAlignmentX.x)) == 4, "_AlignClay_LayoutAlignmentX.x has unexpected size % instead of 4", size_of(type_of(_AlignClay_LayoutAlignmentX.x)));
        assert(size_of(_AlignClay_LayoutAlignmentX) == 8, "_AlignClay_LayoutAlignmentX has size % instead of 8", size_of(_AlignClay_LayoutAlignmentX));
    }

    {
        instance: _AlignClay_LayoutAlignmentY;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_LayoutAlignmentY.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_LayoutAlignmentY.c)) == 1, "_AlignClay_LayoutAlignmentY.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_LayoutAlignmentY.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_LayoutAlignmentY.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_LayoutAlignmentY.x)) == 4, "_AlignClay_LayoutAlignmentY.x has unexpected size % instead of 4", size_of(type_of(_AlignClay_LayoutAlignmentY.x)));
        assert(size_of(_AlignClay_LayoutAlignmentY) == 8, "_AlignClay_LayoutAlignmentY has size % instead of 8", size_of(_AlignClay_LayoutAlignmentY));
    }

    {
        instance: _AlignClay__SizingType;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay__SizingType.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay__SizingType.c)) == 1, "_AlignClay__SizingType.c has unexpected size % instead of 1", size_of(type_of(_AlignClay__SizingType.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay__SizingType.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay__SizingType.x)) == 4, "_AlignClay__SizingType.x has unexpected size % instead of 4", size_of(type_of(_AlignClay__SizingType.x)));
        assert(size_of(_AlignClay__SizingType) == 8, "_AlignClay__SizingType has size % instead of 8", size_of(_AlignClay__SizingType));
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
        instance: _AlignClay_ChildAlignment;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_ChildAlignment.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ChildAlignment.c)) == 1, "_AlignClay_ChildAlignment.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_ChildAlignment.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_ChildAlignment.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ChildAlignment.x)) == 8, "_AlignClay_ChildAlignment.x has unexpected size % instead of 8", size_of(type_of(_AlignClay_ChildAlignment.x)));
        assert(size_of(_AlignClay_ChildAlignment) == 12, "_AlignClay_ChildAlignment has size % instead of 12", size_of(_AlignClay_ChildAlignment));
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
        instance: _AlignClay_SizingMinMax;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_SizingMinMax.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_SizingMinMax.c)) == 1, "_AlignClay_SizingMinMax.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_SizingMinMax.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_SizingMinMax.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_SizingMinMax.x)) == 8, "_AlignClay_SizingMinMax.x has unexpected size % instead of 8", size_of(type_of(_AlignClay_SizingMinMax.x)));
        assert(size_of(_AlignClay_SizingMinMax) == 12, "_AlignClay_SizingMinMax has size % instead of 12", size_of(_AlignClay_SizingMinMax));
    }

    {
        instance: SizingAxis;
        assert(((cast(*void)(*instance.size)) - cast(*void)(*instance)) == 0, "SizingAxis.size has unexpected offset % instead of 0", ((cast(*void)(*instance.size)) - cast(*void)(*instance)));
        assert(size_of(type_of(SizingAxis.size)) == 8, "SizingAxis.size has unexpected size % instead of 8", size_of(type_of(SizingAxis.size)));
        assert(((cast(*void)(*instance.type)) - cast(*void)(*instance)) == 8, "SizingAxis.type has unexpected offset % instead of 8", ((cast(*void)(*instance.type)) - cast(*void)(*instance)));
        assert(size_of(type_of(SizingAxis.type)) == 4, "SizingAxis.type has unexpected size % instead of 4", size_of(type_of(SizingAxis.type)));
        assert(size_of(SizingAxis) == 12, "SizingAxis has size % instead of 12", size_of(SizingAxis));
    }

    {
        instance: _AlignClay_SizingAxis;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_SizingAxis.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_SizingAxis.c)) == 1, "_AlignClay_SizingAxis.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_SizingAxis.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_SizingAxis.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_SizingAxis.x)) == 12, "_AlignClay_SizingAxis.x has unexpected size % instead of 12", size_of(type_of(_AlignClay_SizingAxis.x)));
        assert(size_of(_AlignClay_SizingAxis) == 16, "_AlignClay_SizingAxis has size % instead of 16", size_of(_AlignClay_SizingAxis));
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
        instance: _AlignClay_Sizing;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_Sizing.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Sizing.c)) == 1, "_AlignClay_Sizing.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_Sizing.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_Sizing.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Sizing.x)) == 24, "_AlignClay_Sizing.x has unexpected size % instead of 24", size_of(type_of(_AlignClay_Sizing.x)));
        assert(size_of(_AlignClay_Sizing) == 28, "_AlignClay_Sizing has size % instead of 28", size_of(_AlignClay_Sizing));
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
        instance: _AlignClay_Padding;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_Padding.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Padding.c)) == 1, "_AlignClay_Padding.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_Padding.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 2, "_AlignClay_Padding.x has unexpected offset % instead of 2", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_Padding.x)) == 4, "_AlignClay_Padding.x has unexpected size % instead of 4", size_of(type_of(_AlignClay_Padding.x)));
        assert(size_of(_AlignClay_Padding) == 6, "_AlignClay_Padding has size % instead of 6", size_of(_AlignClay_Padding));
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
        instance: _AlignClay_LayoutConfig;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_LayoutConfig.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_LayoutConfig.c)) == 1, "_AlignClay_LayoutConfig.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_LayoutConfig.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_LayoutConfig.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_LayoutConfig.x)) == 44, "_AlignClay_LayoutConfig.x has unexpected size % instead of 44", size_of(type_of(_AlignClay_LayoutConfig.x)));
        assert(size_of(_AlignClay_LayoutConfig) == 48, "_AlignClay_LayoutConfig has size % instead of 48", size_of(_AlignClay_LayoutConfig));
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
        instance: _AlignClay_RectangleElementConfig;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_RectangleElementConfig.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_RectangleElementConfig.c)) == 1, "_AlignClay_RectangleElementConfig.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_RectangleElementConfig.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_RectangleElementConfig.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_RectangleElementConfig.x)) == 32, "_AlignClay_RectangleElementConfig.x has unexpected size % instead of 32", size_of(type_of(_AlignClay_RectangleElementConfig.x)));
        assert(size_of(_AlignClay_RectangleElementConfig) == 36, "_AlignClay_RectangleElementConfig has size % instead of 36", size_of(_AlignClay_RectangleElementConfig));
    }

    {
        instance: _AlignClay_TextElementConfigWrapMode;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_TextElementConfigWrapMode.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_TextElementConfigWrapMode.c)) == 1, "_AlignClay_TextElementConfigWrapMode.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_TextElementConfigWrapMode.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_TextElementConfigWrapMode.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_TextElementConfigWrapMode.x)) == 4, "_AlignClay_TextElementConfigWrapMode.x has unexpected size % instead of 4", size_of(type_of(_AlignClay_TextElementConfigWrapMode.x)));
        assert(size_of(_AlignClay_TextElementConfigWrapMode) == 8, "_AlignClay_TextElementConfigWrapMode has size % instead of 8", size_of(_AlignClay_TextElementConfigWrapMode));
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
        instance: _AlignClay_TextElementConfig;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_TextElementConfig.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_TextElementConfig.c)) == 1, "_AlignClay_TextElementConfig.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_TextElementConfig.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_TextElementConfig.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_TextElementConfig.x)) == 28, "_AlignClay_TextElementConfig.x has unexpected size % instead of 28", size_of(type_of(_AlignClay_TextElementConfig.x)));
        assert(size_of(_AlignClay_TextElementConfig) == 32, "_AlignClay_TextElementConfig has size % instead of 32", size_of(_AlignClay_TextElementConfig));
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
        instance: _AlignClay_ImageElementConfig;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_ImageElementConfig.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ImageElementConfig.c)) == 1, "_AlignClay_ImageElementConfig.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_ImageElementConfig.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_ImageElementConfig.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ImageElementConfig.x)) == 16, "_AlignClay_ImageElementConfig.x has unexpected size % instead of 16", size_of(type_of(_AlignClay_ImageElementConfig.x)));
        assert(size_of(_AlignClay_ImageElementConfig) == 24, "_AlignClay_ImageElementConfig has size % instead of 24", size_of(_AlignClay_ImageElementConfig));
    }

    {
        instance: _AlignClay_FloatingAttachPointType;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_FloatingAttachPointType.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_FloatingAttachPointType.c)) == 1, "_AlignClay_FloatingAttachPointType.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_FloatingAttachPointType.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_FloatingAttachPointType.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_FloatingAttachPointType.x)) == 4, "_AlignClay_FloatingAttachPointType.x has unexpected size % instead of 4", size_of(type_of(_AlignClay_FloatingAttachPointType.x)));
        assert(size_of(_AlignClay_FloatingAttachPointType) == 8, "_AlignClay_FloatingAttachPointType has size % instead of 8", size_of(_AlignClay_FloatingAttachPointType));
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
        instance: _AlignClay_FloatingAttachPoints;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_FloatingAttachPoints.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_FloatingAttachPoints.c)) == 1, "_AlignClay_FloatingAttachPoints.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_FloatingAttachPoints.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_FloatingAttachPoints.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_FloatingAttachPoints.x)) == 8, "_AlignClay_FloatingAttachPoints.x has unexpected size % instead of 8", size_of(type_of(_AlignClay_FloatingAttachPoints.x)));
        assert(size_of(_AlignClay_FloatingAttachPoints) == 12, "_AlignClay_FloatingAttachPoints has size % instead of 12", size_of(_AlignClay_FloatingAttachPoints));
    }

    {
        instance: _AlignClay_PointerCaptureMode;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_PointerCaptureMode.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_PointerCaptureMode.c)) == 1, "_AlignClay_PointerCaptureMode.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_PointerCaptureMode.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_PointerCaptureMode.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_PointerCaptureMode.x)) == 4, "_AlignClay_PointerCaptureMode.x has unexpected size % instead of 4", size_of(type_of(_AlignClay_PointerCaptureMode.x)));
        assert(size_of(_AlignClay_PointerCaptureMode) == 8, "_AlignClay_PointerCaptureMode has size % instead of 8", size_of(_AlignClay_PointerCaptureMode));
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
        instance: _AlignClay_FloatingElementConfig;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_FloatingElementConfig.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_FloatingElementConfig.c)) == 1, "_AlignClay_FloatingElementConfig.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_FloatingElementConfig.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_FloatingElementConfig.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_FloatingElementConfig.x)) == 36, "_AlignClay_FloatingElementConfig.x has unexpected size % instead of 36", size_of(type_of(_AlignClay_FloatingElementConfig.x)));
        assert(size_of(_AlignClay_FloatingElementConfig) == 40, "_AlignClay_FloatingElementConfig has size % instead of 40", size_of(_AlignClay_FloatingElementConfig));
    }

    {
        instance: CustomElementConfig;
        assert(((cast(*void)(*instance.customData)) - cast(*void)(*instance)) == 0, "CustomElementConfig.customData has unexpected offset % instead of 0", ((cast(*void)(*instance.customData)) - cast(*void)(*instance)));
        assert(size_of(type_of(CustomElementConfig.customData)) == 8, "CustomElementConfig.customData has unexpected size % instead of 8", size_of(type_of(CustomElementConfig.customData)));
        assert(size_of(CustomElementConfig) == 8, "CustomElementConfig has size % instead of 8", size_of(CustomElementConfig));
    }

    {
        instance: _AlignClay_CustomElementConfig;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_CustomElementConfig.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_CustomElementConfig.c)) == 1, "_AlignClay_CustomElementConfig.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_CustomElementConfig.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_CustomElementConfig.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_CustomElementConfig.x)) == 8, "_AlignClay_CustomElementConfig.x has unexpected size % instead of 8", size_of(type_of(_AlignClay_CustomElementConfig.x)));
        assert(size_of(_AlignClay_CustomElementConfig) == 16, "_AlignClay_CustomElementConfig has size % instead of 16", size_of(_AlignClay_CustomElementConfig));
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
        instance: _AlignClay_ScrollElementConfig;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_ScrollElementConfig.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ScrollElementConfig.c)) == 1, "_AlignClay_ScrollElementConfig.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_ScrollElementConfig.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 1, "_AlignClay_ScrollElementConfig.x has unexpected offset % instead of 1", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ScrollElementConfig.x)) == 2, "_AlignClay_ScrollElementConfig.x has unexpected size % instead of 2", size_of(type_of(_AlignClay_ScrollElementConfig.x)));
        assert(size_of(_AlignClay_ScrollElementConfig) == 3, "_AlignClay_ScrollElementConfig has size % instead of 3", size_of(_AlignClay_ScrollElementConfig));
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
        instance: _AlignClay_ElementConfigUnion;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_ElementConfigUnion.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ElementConfigUnion.c)) == 1, "_AlignClay_ElementConfigUnion.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_ElementConfigUnion.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_ElementConfigUnion.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ElementConfigUnion.x)) == 8, "_AlignClay_ElementConfigUnion.x has unexpected size % instead of 8", size_of(type_of(_AlignClay_ElementConfigUnion.x)));
        assert(size_of(_AlignClay_ElementConfigUnion) == 16, "_AlignClay_ElementConfigUnion has size % instead of 16", size_of(_AlignClay_ElementConfigUnion));
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
        instance: _AlignClay_ElementConfig;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_ElementConfig.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ElementConfig.c)) == 1, "_AlignClay_ElementConfig.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_ElementConfig.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_ElementConfig.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ElementConfig.x)) == 16, "_AlignClay_ElementConfig.x has unexpected size % instead of 16", size_of(type_of(_AlignClay_ElementConfig.x)));
        assert(size_of(_AlignClay_ElementConfig) == 24, "_AlignClay_ElementConfig has size % instead of 24", size_of(_AlignClay_ElementConfig));
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
        instance: _AlignClay_ScrollContainerData;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_ScrollContainerData.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ScrollContainerData.c)) == 1, "_AlignClay_ScrollContainerData.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_ScrollContainerData.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_ScrollContainerData.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ScrollContainerData.x)) == 32, "_AlignClay_ScrollContainerData.x has unexpected size % instead of 32", size_of(type_of(_AlignClay_ScrollContainerData.x)));
        assert(size_of(_AlignClay_ScrollContainerData) == 40, "_AlignClay_ScrollContainerData has size % instead of 40", size_of(_AlignClay_ScrollContainerData));
    }

    {
        instance: _AlignClay_RenderCommandType;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_RenderCommandType.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_RenderCommandType.c)) == 1, "_AlignClay_RenderCommandType.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_RenderCommandType.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_RenderCommandType.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_RenderCommandType.x)) == 4, "_AlignClay_RenderCommandType.x has unexpected size % instead of 4", size_of(type_of(_AlignClay_RenderCommandType.x)));
        assert(size_of(_AlignClay_RenderCommandType) == 8, "_AlignClay_RenderCommandType has size % instead of 8", size_of(_AlignClay_RenderCommandType));
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
        instance: _AlignClay_RenderCommand;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_RenderCommand.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_RenderCommand.c)) == 1, "_AlignClay_RenderCommand.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_RenderCommand.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_RenderCommand.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_RenderCommand.x)) == 48, "_AlignClay_RenderCommand.x has unexpected size % instead of 48", size_of(type_of(_AlignClay_RenderCommand.x)));
        assert(size_of(_AlignClay_RenderCommand) == 56, "_AlignClay_RenderCommand has size % instead of 56", size_of(_AlignClay_RenderCommand));
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
        instance: _AlignClay_RenderCommandArray;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_RenderCommandArray.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_RenderCommandArray.c)) == 1, "_AlignClay_RenderCommandArray.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_RenderCommandArray.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_RenderCommandArray.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_RenderCommandArray.x)) == 16, "_AlignClay_RenderCommandArray.x has unexpected size % instead of 16", size_of(type_of(_AlignClay_RenderCommandArray.x)));
        assert(size_of(_AlignClay_RenderCommandArray) == 24, "_AlignClay_RenderCommandArray has size % instead of 24", size_of(_AlignClay_RenderCommandArray));
    }

    {
        instance: _AlignClay_PointerDataInteractionState;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_PointerDataInteractionState.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_PointerDataInteractionState.c)) == 1, "_AlignClay_PointerDataInteractionState.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_PointerDataInteractionState.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_PointerDataInteractionState.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_PointerDataInteractionState.x)) == 4, "_AlignClay_PointerDataInteractionState.x has unexpected size % instead of 4", size_of(type_of(_AlignClay_PointerDataInteractionState.x)));
        assert(size_of(_AlignClay_PointerDataInteractionState) == 8, "_AlignClay_PointerDataInteractionState has size % instead of 8", size_of(_AlignClay_PointerDataInteractionState));
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
        instance: _AlignClay_PointerData;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_PointerData.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_PointerData.c)) == 1, "_AlignClay_PointerData.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_PointerData.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_PointerData.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_PointerData.x)) == 12, "_AlignClay_PointerData.x has unexpected size % instead of 12", size_of(type_of(_AlignClay_PointerData.x)));
        assert(size_of(_AlignClay_PointerData) == 16, "_AlignClay_PointerData has size % instead of 16", size_of(_AlignClay_PointerData));
    }

    {
        instance: _AlignClay_ErrorType;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_ErrorType.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ErrorType.c)) == 1, "_AlignClay_ErrorType.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_ErrorType.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 4, "_AlignClay_ErrorType.x has unexpected offset % instead of 4", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ErrorType.x)) == 4, "_AlignClay_ErrorType.x has unexpected size % instead of 4", size_of(type_of(_AlignClay_ErrorType.x)));
        assert(size_of(_AlignClay_ErrorType) == 8, "_AlignClay_ErrorType has size % instead of 8", size_of(_AlignClay_ErrorType));
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
        instance: _AlignClay_ErrorData;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_ErrorData.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ErrorData.c)) == 1, "_AlignClay_ErrorData.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_ErrorData.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_ErrorData.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ErrorData.x)) == 32, "_AlignClay_ErrorData.x has unexpected size % instead of 32", size_of(type_of(_AlignClay_ErrorData.x)));
        assert(size_of(_AlignClay_ErrorData) == 40, "_AlignClay_ErrorData has size % instead of 40", size_of(_AlignClay_ErrorData));
    }

    {
        instance: ErrorHandler;
        assert(((cast(*void)(*instance.errorHandlerFunction)) - cast(*void)(*instance)) == 0, "ErrorHandler.errorHandlerFunction has unexpected offset % instead of 0", ((cast(*void)(*instance.errorHandlerFunction)) - cast(*void)(*instance)));
        assert(size_of(type_of(ErrorHandler.errorHandlerFunction)) == 8, "ErrorHandler.errorHandlerFunction has unexpected size % instead of 8", size_of(type_of(ErrorHandler.errorHandlerFunction)));
        assert(((cast(*void)(*instance.userData)) - cast(*void)(*instance)) == 8, "ErrorHandler.userData has unexpected offset % instead of 8", ((cast(*void)(*instance.userData)) - cast(*void)(*instance)));
        assert(size_of(type_of(ErrorHandler.userData)) == 8, "ErrorHandler.userData has unexpected size % instead of 8", size_of(type_of(ErrorHandler.userData)));
        assert(size_of(ErrorHandler) == 16, "ErrorHandler has size % instead of 16", size_of(ErrorHandler));
    }

    {
        instance: _AlignClay_ErrorHandler;
        assert(((cast(*void)(*instance.c)) - cast(*void)(*instance)) == 0, "_AlignClay_ErrorHandler.c has unexpected offset % instead of 0", ((cast(*void)(*instance.c)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ErrorHandler.c)) == 1, "_AlignClay_ErrorHandler.c has unexpected size % instead of 1", size_of(type_of(_AlignClay_ErrorHandler.c)));
        assert(((cast(*void)(*instance.x)) - cast(*void)(*instance)) == 8, "_AlignClay_ErrorHandler.x has unexpected offset % instead of 8", ((cast(*void)(*instance.x)) - cast(*void)(*instance)));
        assert(size_of(type_of(_AlignClay_ErrorHandler.x)) == 16, "_AlignClay_ErrorHandler.x has unexpected size % instead of 16", size_of(type_of(_AlignClay_ErrorHandler.x)));
        assert(size_of(_AlignClay_ErrorHandler) == 24, "_AlignClay_ErrorHandler has size % instead of 24", size_of(_AlignClay_ErrorHandler));
    }
}

