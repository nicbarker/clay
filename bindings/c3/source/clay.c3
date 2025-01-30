module clay;

import clay::carray;

macro @clay(...; @body()) @builtin
{
    clay::openElement(); 
    $for (var $i = 0; $i < $vacount; $i++)
        $vaexpr[$i]; // If you get an error here consisder the @body[...]() macros
    $endfor
    clay::elementPostConfiguration();
    @body();
    clay::closeElement();
}

macro text(String text, TextElementConfig *config) { clay::openTextElement({text.len, text}, config); }

<*Provides you conditional calls (eg #booleanCondition ? #doA : 0) within the condifuration of @clay(...)*>
macro @bodyIf(#condition, #ifRes) { if (#condition) { #ifRes; } }

<*Provides you conditional calls (eg #booleanCondition ? #doA : #doB) within the condifuration of @clay(...)*>
macro @bodyIfElse(#condition, #ifRes, #elseRes) { if (#condition) { #ifRes; } else { #elseRes; } }

<*attaches a RectangleElementConfig to the clay element when called within @clay(...)*>
macro rectangle(RectangleElementConfig config) { clay::attachElementConfig({ .rectangleElementConfig = clay::storeRectangleElementConfig(config) }, clay::ELEMENT_CONFIG_TYPE_RECTANGLE ); }

<*attaches a LayoutConfig to the clay element when called within @clay(...)*>
macro layout(LayoutConfig config) { clay::attachLayoutConfig( clay::storeLayoutConfig(config) ); }

<*attaches a LayoutConfig to the clay element when called within @clay(...)*>
macro scroll(ScrollElementConfig config) { clay::attachElementConfig({ .scrollElementConfig = clay::storeScrollElementConfig(config) }, clay::ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER ); }

<*attaches a FloatingElementConfig to the clay element when called within @clay(...)*>
macro floating(FloatingElementConfig config) { clay::attachElementConfig({ .floatingElementConfig = clay::storeFloatingElementConfig(config) }, clay::ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER ); }

<*attaches a BorderElementConfig to the clay element when called within @clay(...)*>
macro @borderRadiusUni(uint #width, ClayColor #color, float #cornerRadius = 0) { clay::attachElementConfig({ .borderElementConfig = clay::storeBorderElementConfig({ .left = { #width, #color }, .right = { #width, #color }, .top = { #width, #color }, .bottom = { #width, #color }, .#cornerRadius = {#cornerRadius, #cornerRadius, #cornerRadius, #cornerRadius}})}, clay::ELEMENT_CONFIG_TYPE_BORDER_CONTAINER); }

macro id(String idString) { clay::attachId(clay::hashString({idString.len, idString}, 0, 0)); }

macro TextElementConfig* textConfig(TextElementConfig config) { return clay::storeTextElementConfig(config); }

macro SizingAxis sizingFit(float min = 0, float max = float.max) { return { .size.minMax = {min, max}, .type = SizingType.FIT }; }

macro SizingAxis sizingGrow() { return { .size.minMax = {0, 0}, .type = SizingType.GROW }; }

macro SizingAxis sizingFixed(float pixels) { return { .size.minMax = {pixels, pixels}, .type = SizingType.FIXED }; }

macro SizingAxis sizingPercent(float percent) { return { .size.percent = percent, .type = SizingType.PERCENT }; }

macro Padding paddingUni(ushort uniform) { return {uniform, uniform, uniform, uniform}; }

macro Padding padding(ushort horizontal, ushort vertical) { return {horizontal, horizontal, vertical, vertical}; }

macro CornerRadius cornerRadiusUni(float uniform) { return {uniform, uniform, uniform, uniform}; }

macro SizingAxis sizingFitCT(float $min = 0, float $max = float.max) { return { .size.minMax = {$min, $max}, .type = SizingType.FIT }; }

macro SizingAxis sizingFixedCT(float $pixels) { return { .size.minMax = {$pixels, $pixels}, .type = SizingType.FIXED }; }

macro SizingAxis sizingPercentCT(float $percent) { return { .size.percent = $percent, .type = SizingType.PERCENT }; }

macro Padding paddingCT(ushort $a, ushort $b, ushort $c, ushort $d) { return { $a, $b, $c, $d }; }

macro CornerRadius @cornerRadiusUniCT(float #uniform) { return {#uniform, #uniform, #uniform, #uniform}; }

struct ClayString
{
    int length;
    char *chars;
}
def ClayStringArray = carray::Array(<ClayString>) @private;

struct Arena
{
    uint128 nextAllocation;
    uint128 capacity;
    char *memory;
}

struct Dimensions
{
    float width, height;
}

struct ClayVector2 
{
    float x, y;
}

struct ClayColor
{
    float r, g, b, a;
}


struct ElementId
{
    uint id;
    uint offset;
    uint baseId;
    ClayString stringId;
}

struct CornerRadius
{
    float topLeft;
    float topRight;
    float bottomLeft;
    float bottomRight;
}
distinct ElementConfigType                                      @private = char;
const ElementConfigType ELEMENT_CONFIG_TYPE_NONE                @private = 0;
const ElementConfigType ELEMENT_CONFIG_TYPE_RECTANGLE           @private = 1;
const ElementConfigType ELEMENT_CONFIG_TYPE_BORDER_CONTAINER    @private = 2;
const ElementConfigType ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER  @private = 4;
const ElementConfigType ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER    @private = 8;
const ElementConfigType ELEMENT_CONFIG_TYPE_IMAGE               @private = 16;
const ElementConfigType ELEMENT_CONFIG_TYPE_TEXT                @private = 32;
const ElementConfigType ELEMENT_CONFIG_TYPE_CUSTOM              @private = 64;

enum LayoutDirection : char @export 
{
    LEFT_TO_RIGHT,
    TOP_TO_BOTTOM,
}

enum AlignX : char @export 
{
    LEFT,
    RIGHT,
    CENTER,
}

enum AlignY : char @export 
{
    TOP,
    BOTTOM,
    CENTER,
}

enum SizingType : char @export 
{
    FIT,
    GROW,
    PERCENT,
    FIXED,
}

struct ChildAlignment
{
    AlignX x;
    AlignY y;
}

struct SizingMinMax
{
    float min;
    float max;
}

struct SizingAxis
{
    union size
    {
        SizingMinMax minMax;
        float percent;
    }
    SizingType type;
}

struct Sizing
{
    SizingAxis width;
    SizingAxis height;
}

struct Padding
{
    ushort left;
    ushort right;
    ushort top;
    ushort bottom;
}

struct LayoutConfig
{
    Sizing sizing;
    Padding padding;
    ushort childGap;
    ChildAlignment childAlignment;
    LayoutDirection layoutDirection;
}

struct RectangleElementConfig
{
    ClayColor color;
    CornerRadius cornerRadius;
    // #ifdef CLAY_EXTEND_CONFIG_RECTANGLE
    // CLAY_EXTEND_CONFIG_RECTANGLE
    // #endif
}

enum WrapMode @export
{
    WORDS,
    NEWLINES,
    NONE,
}

struct TextElementConfig
{
    ClayColor textColor;
    ushort fontId;
    ushort fontSize;
    ushort letterSpacing;
    ushort lineHeight;
    WrapMode wrapMode;
    // #ifdef CLAY_EXTEND_CONFIG_TEXT
    // CLAY_EXTEND_CONFIG_TEXT
    // #endif
}

struct ImageElementConfig
{
    void *imageData;
    Dimensions sourceDimensions;
    // #ifdef CLAY_EXTEND_CONFIG_IMAGE
    // CLAY_EXTEND_CONFIG_IMAGE
    // #endif
}

enum AttachPoint : char @export
{
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

struct FloatingAttachPoints
{
    AttachPoint element;
    AttachPoint parent;
}

enum PointerCaptureMode @export
{
    CAPTURE,
    // MODE_PASSTHROUGH,
    PARENT, 
}

struct FloatingElementConfig
{
    ClayVector2 offset;
    Dimensions expand;
    ushort zIndex;
    uint parentId;
    FloatingAttachPoints attachment;
    PointerCaptureMode pointerCaptureMode;
}


struct CustomElementConfig
{        
    void *customData;
}

struct ScrollElementConfig
{
    bool horizontal;
    bool vertical;
}

// Border
struct Border
{
    uint width;
    ClayColor color;
}

struct BorderElementConfig
{
    Border left;
    Border right;
    Border top;
    Border bottom;
    Border betweenChildren;
    CornerRadius cornerRadius;
}

union ElementConfigUnion
{
    RectangleElementConfig *rectangleElementConfig;
    TextElementConfig *textElementConfig;
    ImageElementConfig *imageElementConfig;
    FloatingElementConfig *floatingElementConfig;
    CustomElementConfig *customElementConfig;
    ScrollElementConfig *scrollElementConfig;
    BorderElementConfig *borderElementConfig;
}

struct ElementConfig
{
    ElementConfigType type;
    ElementConfigUnion config;
}

struct ClayBoundingBox
{
    float x, y, width, height;
}

enum RenderCommandType : char @export
{
    NONE,
    RECTANGLE,
    BORDER,
    TEXT,
    IMAGE,
    SCISSOR_START,
    SCISSOR_END,
    CUSTOM,
}

struct RenderCommand
{
    ClayBoundingBox boundingBox;
    ElementConfigUnion config;
    ClayString text;
    uint id;
    RenderCommandType commandType;
}
def RenderCommandArray = carray::Array(<RenderCommand>);

struct ScrollContainerData
{
    // Note: This is a pointer to the real internal scroll position, mutating it may cause a change in final layout.
    // Intended for use with external functionality that modifies scroll position, such as scroll bars or auto scrolling.
    ClayVector2 *scrollPosition;
    Dimensions scrollContainerDimensions;
    Dimensions contentDimensions;
    ScrollElementConfig config;
    // Indicates whether an actual scroll container matched the provided ID or if the default struct was returned.
    bool found;
}

struct ElementData
{
    ClayBoundingBox boundingBox;
    // Indicates whether an actual Element matched the provided ID or if the default struct was returned.
    bool found;
}

enum PointerState @export
{
    PRESSED_THIS_FRAME,
    PRESSED,
    RELEASED_THIS_FRAME,
    RELEASED,
}

struct PointerData
{
    ClayVector2 position;
    PointerState state;
}

def OnHoverEvent = fn void(ElementId elementId, PointerData pointerData, iptr userData);
def MeasureTextFunc = fn Dimensions(ClayString *text, TextElementConfig *config);
def QueryScrollOffsetFunc = fn ClayVector2(uint elementId);

enum ErrorType @export
{
    TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED,
    ARENA_CAPACITY_EXCEEDED,
    ELEMENTS_CAPACITY_EXCEEDED,
    TEXT_MEASUREMENT_CAPACITY_EXCEEDED,
    DUPLICATE_ID,
    FLOATING_CONTAINER_PARENT_NOT_FOUND,
    INTERNAL_ERROR,
}

struct ErrorData
{
    ErrorType errorType;
    ClayString errorText;
    uint128 userData;
}

def ErrorHandleFunc = fn void(ErrorData errorText);

struct ErrorHandler
{
    ErrorHandleFunc errorHandler;
    uint128 userData;
}

distinct Context = void; // I'm not doing all that again

// =====================
// ===== FUNCTIONS =====
// =====================

// ===== Public Clay API C3 Functions (String replacement) =====                                                                                                // TODO @export and @wasm can be ignored rn
                                                                                                                                // I haven't been able to get c3c and c compilations to work together for wasm32 or a c3 library
fn ElementId getElementIdWithIndex(String idString, uint index)                                                                                                 @export @inline
{ return __getElementIdWithIndex({idString.len, idString}, (uint)index); }
fn ElementId getElementId(String idString)                                                                                                                      @export @inline
{ return __getElementId({idString.len, idString}); }

// ===== Public Clay API Functions =====
extern fn uint minMemorySize()                                                                              @extern("Clay_MinMemorySize")                       @wasm @export;
extern fn Arena createArena(uint capacity, void* offset)                                                    @extern("Clay_CreateArenaWithCapacityAndMemory")    @wasm @export;
extern fn void setPointerState(ClayVector2 position, bool pointerDown)                                      @extern("Clay_SetPointerState")                     @export;
extern fn Context* initialize(Arena arena, Dimensions layoutDimensions, ErrorHandler errorHandler)          @extern("Clay_Initialize")                          @wasm @export;
extern fn Context* getCurrentContext()                                                                      @extern("Clay_GetCurrentContext")                   @export;
extern fn void setCurrentContext(Context* context)                                                          @extern("Clay_SetCurrentContext")                   @export;
extern fn void updateScrollContainer(bool enableDragScrolling, ClayVector2 scrollDelta, float deltaTime)    @extern("Clay_UpdateScrollContainers")              @export;             
extern fn void setLayoutDimensions (Dimensions dimensions)                                                  @extern("Clay_SetLayoutDimensions")                 @export;
extern fn ElementData getElementData(ElementId id)                                                          @extern("Clay_GetElementData")                      @export;
extern fn bool hovered()                                                                                    @extern("Clay_Hovered")                             @export;
extern fn void onHover(OnHoverEvent onHover, iptr userData)                                                 @extern("Clay_OnHover")                             @export;                                        
extern fn bool pointerOver(ElementId elementId)                                                             @extern("Clay_PointerOver")                         @wasm @export;
extern fn ScrollContainerData getScrollContainerData(ElementId id)                                          @extern("Clay_GetScrollContainerData")              @export;
extern fn void setMeasureTextFunction(MeasureTextFunc measureText)                                          @extern("Clay_SetMeasureTextFunction")              @export;
extern fn void setQueryScrollOffsetFunction(QueryScrollOffsetFunc queryScrollOffset)                        @extern("Clay_SetQueryScrollOffsetFunction")        @export;
extern fn RenderCommand * RenderCommandArray.get(RenderCommandArray* array, int index)                      @extern("Clay_RenderCommandArray_Get")              @export;
extern fn void setDebugModeEnabled(bool enabled)                                                            @extern("Clay_SetDebugModeEnabled")                 @export;
extern fn bool isDebugModeEnabled()                                                                         @extern("Clay_IsDebugModeEnabled")                  @export;
extern fn void setCullingEnabled(bool enabled)                                                              @extern("Clay_SetCullingEnabled")                   @export;
extern fn int getMaxMeasuredTextCachedWordCount()                                                           @extern("Clay_GetMaxElementCount")                  @export;
extern fn void setMaxElementCount(int maxElementCount)                                                      @extern("Clay_SetMaxElementCount")                  @export;
extern fn int getMaxElementCount()                                                                          @extern("Clay_GetMaxMeasureTextCacheWordCount")     @export;
extern fn void setMaxMeasureTextCacheWordCount(int maxMeasureTextCacheWordCount)                            @extern("Clay_SetMaxMeasureTextCacheWordCount")     @export;
extern fn void resetMeasureTextCache()                                                                      @extern("Clay_ResetMeasureTextCache")               @export;
extern fn void beginLayout()                                                                                @extern("Clay_BeginLayout")                         @export;
extern fn RenderCommandArray endLayout()                                                                    @extern("Clay_EndLayout")                           @export;

// ===== (NEW) Internal Clay API Functions (String replacement) =====
extern fn ElementId __getElementIdWithIndex(ClayString idString, uint index)                                @extern("Clay_GetElementIdWithIndex")               @export @private;
extern fn ElementId __getElementId(ClayString idString)                                                     @extern("Clay_GetElementId")                        @export @private;

// ===== Internal Clay API Functions =====
extern fn void openElement()                                                                                @extern ("Clay__OpenElement")                       @export @private;
extern fn void closeElement()                                                                               @extern("Clay__CloseElement")                       @export @private;
extern fn void openTextElement(ClayString text, TextElementConfig *textConfig)                              @extern("Clay__OpenTextElement")                    @export @private;
extern fn void elementPostConfiguration()                                                                   @extern("Clay__ElementPostConfiguration")           @export @private;
extern fn LayoutConfig * storeLayoutConfig(LayoutConfig config)                                             @extern("Clay__StoreLayoutConfig")                  @export @private;
extern fn void attachId(ElementId id)                                                                       @extern("Clay__AttachId")                           @export @private;
extern fn void attachLayoutConfig(LayoutConfig *config)                                                     @extern("Clay__AttachLayoutConfig")                 @export @private;
extern fn void attachElementConfig(ElementConfigUnion config, ElementConfigType type)                       @extern("Clay__AttachElementConfig")                @export @private;
extern fn RectangleElementConfig * storeRectangleElementConfig(RectangleElementConfig config)               @extern("Clay__StoreRectangleElementConfig")        @export @private;
extern fn TextElementConfig * storeTextElementConfig(TextElementConfig config)                              @extern("Clay__StoreTextElementConfig")             @export @private;
extern fn ImageElementConfig * storeImageElementConfig(ImageElementConfig config)                           @extern("Clay__StoreImageElementConfig")            @export @private;
extern fn FloatingElementConfig * storeFloatingElementConfig(FloatingElementConfig config)                  @extern("Clay__StoreFloatingElementConfig")         @export @private;
extern fn CustomElementConfig * storeCustomElementConfig(CustomElementConfig config)                        @extern("Clay__StoreCustomElementConfig")           @export @private;
extern fn ScrollElementConfig * storeScrollElementConfig(ScrollElementConfig config)                        @extern("Clay__StoreScrollElementConfig")           @export @private;
extern fn BorderElementConfig * storeBorderElementConfig(BorderElementConfig config)                        @extern("Clay__StoreBorderElementConfig")           @export @private;
extern fn ElementId hashString(ClayString key, uint offset, uint seed) 						                @extern("Clay__HashString")                         @export @private;
extern fn uint getParentElementId()                                                                         @extern("Clay__GetParentElementId")                 @export @private;

// ==========================================================================
// ===== An internal module for wrapping Struct Array's defined in Clay =====
// ==========================================================================
module clay::carray(<ElementType>);

struct Array {
    int capacity;
    int length;
    ElementType *data;
}