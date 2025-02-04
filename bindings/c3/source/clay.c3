module clay;

import clay::carray;

// =======================
// ===== USER MACROS =====
// =======================
macro @clay(...; @body()) @builtin
{
    clay::openElement(); 
    $for (var $i = 0; $i < $vacount; $i++)
        $vaexpr[$i]; // If you get an error here consider the @body[...]() macros
    $endfor
    clay::elementPostConfiguration();
    @body();
    clay::closeElement();
}

macro text(String text, TextElementConfig *config) { clay::openTextElement({text.len, text}, config); }

macro @bodyIf(#condition, #ifRes) { if (#condition) { #ifRes; } }

macro @bodyIfElse(#condition, #ifRes, #elseRes) { if (#condition) { #ifRes; } else { #elseRes; } }

macro rectangle(RectangleElementConfig config) { clay::attachElementConfig({ .rectangleElementConfig = clay::storeRectangleElementConfig(config) }, clay::ELEMENT_CONFIG_TYPE_RECTANGLE ); }

macro layout(LayoutConfig config) { clay::attachLayoutConfig( clay::storeLayoutConfig(config) ); }

macro scroll(ScrollElementConfig config) { clay::attachElementConfig({ .scrollElementConfig = clay::storeScrollElementConfig(config) }, clay::ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER ); }

macro floating(FloatingElementConfig config) { clay::attachElementConfig({ .floatingElementConfig = clay::storeFloatingElementConfig(config) }, clay::ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER ); }

macro borderRadiusUni(uint width, ClayColor color, float cornerRadius = 0) { clay::attachElementConfig({ .borderElementConfig = clay::storeBorderElementConfig({ .left = { width, color }, .right = { width, color }, .top = { width, color }, .bottom = { width, color }, .cornerRadius = {cornerRadius, cornerRadius, cornerRadius, cornerRadius}})}, clay::ELEMENT_CONFIG_TYPE_BORDER_CONTAINER); }

macro id(String idString) { clay::attachId(clay::hashString({idString.len, idString}, 0, 0)); }

macro idi(String idString, uint seed) { clay::attachId(clay::hashString({idString.len, idString}, 0, seed)); }

macro idLocal(String idString, uint offset) { clay::attachId(clay::hashString({idString.len, idString}, offset, 0)); }

macro idiLocal(String idString, uint offset, uint seed) { clay::attachId(clay::hashString({idString.len, idString}, offset, seed)); }

macro TextElementConfig* textConfig(TextElementConfig config) { return clay::storeTextElementConfig(config); }

macro SizingAxis sizingFit(float min = 0) { return { .size.minMax = {min, float.max}, .type = SizingType.FIT }; }

macro SizingAxis sizingGrow(float max = 0) { return { .size.minMax = {0, max}, .type = SizingType.GROW }; }

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

// TODO: figure out why this isn't working
// macro ClayColor @colorHex(#hex) { return {(float)(((#hex >> 16) & 0xFF) / 255.0), (float)(((#hex >> 8) & 0xFF) / 255.0), (float)(((#hex) & 0xFF) / 255.0), 255}; }

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

distinct ElementConfigType                                      = char;
const ElementConfigType ELEMENT_CONFIG_TYPE_NONE                = 0;
const ElementConfigType ELEMENT_CONFIG_TYPE_RECTANGLE           = 1;
const ElementConfigType ELEMENT_CONFIG_TYPE_BORDER_CONTAINER    = 2;
const ElementConfigType ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER  = 4;
const ElementConfigType ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER    = 8;
const ElementConfigType ELEMENT_CONFIG_TYPE_IMAGE               = 16;
const ElementConfigType ELEMENT_CONFIG_TYPE_TEXT                = 32;
const ElementConfigType ELEMENT_CONFIG_TYPE_CUSTOM              = 64;

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
    // No C-like struct extension macros (eg. CLAY_EXTEND_CONFIG_RECTANGLE)
    // TODO: C3 doesn't support conditional compilation outside of functions/methods/macros
}

enum WrapMode
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
    // No C-like struct extension macros (eg. CLAY_EXTEND_CONFIG_TEXT)
    // TODO: C3 doesn't support conditional compilation outside of functions/methods/macros
}

struct ImageElementConfig
{
    void *imageData;
    Dimensions sourceDimensions;
    // No C-like struct extension macros (eg. CLAY_EXTEND_CONFIG_IMAGE)
    // TODO: C3 doesn't support conditional compilation outside of functions/methods/macros
}

enum AttachPoint : char
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

enum PointerCaptureMode
{
    CAPTURE,
    // MODE_PASSTHROUGH,    // not fully implemented in C as of bindings dev-time
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
    // No C-like struct extension macros (eg. CLAY_EXTEND_CONFIG_FLOATING)
    // TODO: C3 doesn't support conditional compilation outside of functions/methods/macros
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
    // No C-like struct extension macros (eg. CLAY_EXTEND_CONFIG_BORDER)
    // TODO: C3 doesn't support conditional compilation outside of functions/methods/macros
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
    ClayVector2 *scrollPosition;
    Dimensions scrollContainerDimensions;
    Dimensions contentDimensions;
    ScrollElementConfig config;
    bool found;
}

struct ElementData
{
    ClayBoundingBox boundingBox;
    bool found;
}

enum PointerState
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

enum ErrorType
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

struct BooleanWarnings
{
    bool maxElementsExceeded;
    bool maxRenderCommandsExceeded;
    bool maxTextMeasureCacheExceeded;
}

struct Warning
{
    ClayString baseMessage;
    ClayString dynamicMessage;
}

def WarningArray = carray::Array(<Warning>);

struct LayoutElementChildren
{ 
    int *elements;
    ushort length;
}

struct LayoutElement
{
    union childrenOrTextContent {
        LayoutElementChildren children;
        TextElementData *textElementData;
    }
    Dimensions dimensions;
    Dimensions minDimensions;
    LayoutConfig *layoutConfig;
    ElementConfigArraySlice elementConfigs;
    uint configsEnabled;
    uint id;
}

struct WrappedTextLine
{
    Dimensions dimensions;
    ClayString line;
}

struct WrappedTextLineArraySlice
{
	int length;
	WrappedTextLine *internalArray;
}

struct TextElementData
{
    ClayString text;
    Dimensions preferredDimensions;
    int elementIndex;
    WrappedTextLineArraySlice wrappedLines;
}

struct ElementConfigArraySlice
{
	int length;
	ElementConfig *internalArray;
}

def DebugElementData = bool[<2>];

struct LayoutElementHashMapItem
{ 
    ClayBoundingBox boundingBox;
    ElementId elementId;
    LayoutElement* layoutElement;
    OnHoverEvent onHoverFunction;
    int128 hoverFunctionUserData;
    int nextIndex;
    uint128 generation;
    DebugElementData *debugData;
}
 
struct LayoutElementTreeRoot
{
    int layoutElementIndex;
    uint parentId; // This can be zero in the case of the root layout tree
    uint clipElementId; // This can be zero if there is no clip element
    int zIndex;
    ClayVector2 pointerOffset; // Only used when scroll containers are managed externally
}

struct LayoutElementTreeNode
{
    LayoutElement *layoutElement;
    ClayVector2 position;
    ClayVector2 nextChildOffset;
}

struct MeasuredWord
{
    int startOffset;
    int length;
    float width;
    int next;
}

struct MeasureTextCacheItem
{
    Dimensions unwrappedDimensions;
    int measuredWordsStartIndex;
    bool containsNewlines;
    // Hash map data
    uint id;
    int nextIndex;
    uint generation;
}

struct ScrollContainerDataInternal
{
    LayoutElement *layoutElement;
    ClayBoundingBox boundingBox;
    Dimensions contentSize;
    ClayVector2 scrollOrigin;
    ClayVector2 pointerOrigin;
    ClayVector2 scrollMomentum;
    ClayVector2 scrollPosition;
    ClayVector2 previousDelta;
    float momentumTime;
    uint elementId;
    bool openThisFrame;
    bool pointerScrollActive;
}

def LayoutElementArray = carray::Array(<LayoutElement>)                             ;
def IntArray = carray::Array(<int>)                                                 ;
def TextElementDataArray = carray::Array(<TextElementData>)                         ;
def RectangleElementConfigArray = carray::Array(<RectangleElementConfig>)           ;
def TextElementConfigArray = carray::Array(<TextElementConfig>)                     ;
def ImageElementConfigArray = carray::Array(<ImageElementConfig>)                   ;
def FloatingElementConfigArray = carray::Array(<FloatingElementConfig>)             ;
def ScrollElementConfigArray = carray::Array(<ScrollElementConfig>)                 ;
def CustomElementConfigArray = carray::Array(<CustomElementConfig>)                 ;
def BorderElementConfigArray = carray::Array(<BorderElementConfig>)                 ;
def LayoutElementPointerArray = carray::Array(<LayoutElement*>)                     ;
def LayoutConfigArray = carray::Array(<LayoutConfig>)                               ;
def ElementConfigArray = carray::Array(<ElementConfig>)                             ;
def WrappedTextLineArray = carray::Array(<WrappedTextLine>)                         ;
def LayoutElementTreeNodeArray = carray::Array(<LayoutElementTreeNode>)             ;
def LayoutElementTreeRootArray = carray::Array(<LayoutElementTreeRoot>)             ;
def LayoutElementHashMapItemArray = carray::Array(<LayoutElementHashMapItem>)       ;
def MeasureTextCacheItemArray = carray::Array(<MeasureTextCacheItem>)               ;
def MeasuredWordArray = carray::Array(<MeasuredWord>)                               ;
def ElementIdArray = carray::Array(<ElementId>)                                     ;
def ScrollContainerDataInternalArray = carray::Array(<ScrollContainerDataInternal>) ;
def BoolArray = carray::Array(<bool>)                                               ;
def CharArray = carray::Array(<char>)                                               ;
def DebugElementDataArray = carray::Array(<DebugElementData>)                       ;

// TODO: find out why alignment is off, accessing from Context doesn't really work
struct Context
{
    int maxElementCount;
    int maxMeasureTextCacheWordCount;
    bool warningsEnabled;
    ErrorHandler errorHandler;
    BooleanWarnings booleanWarnings;
    WarningArray warnings;

    PointerData pointerInfo;
    Dimensions layoutDimensions;
    ElementId dynamicElementIndexBaseHash;
    uint dynamicElementIndex;
    bool debugModeEnabled;
    bool disableCulling;
    bool externalScrollHandlingEnabled;
    uint debugSelectedElementId;
    uint generation;
    uint128 arenaResetOffset;
    Arena internalArena;

    // Layout Elements / Render Commands
    LayoutElementArray layoutElements;
    RenderCommandArray renderCommands;
    IntArray openLayoutElementStack;
    IntArray layoutElementChildren;
    IntArray layoutElementChildrenBuffer;
    TextElementDataArray textElementData;
    LayoutElementPointerArray imageElementPointers;
    IntArray reusableElementIndexBuffer;
    IntArray layoutElementClipElementIds;

    // Configs
    LayoutConfigArray layoutConfigs;
    ElementConfigArray elementConfigBuffer;
    ElementConfigArray elementConfigs;
    RectangleElementConfigArray rectangleElementConfigs;
    TextElementConfigArray textElementConfigs;
    ImageElementConfigArray imageElementConfigs;
    FloatingElementConfigArray floatingElementConfigs;
    ScrollElementConfigArray scrollElementConfigs;
    CustomElementConfigArray customElementConfigs;
    BorderElementConfigArray borderElementConfigs;
    
    // Misc Data Structures
    ClayStringArray layoutElementIdStrings;
    WrappedTextLineArray wrappedTextLines;
    LayoutElementTreeNodeArray layoutElementTreeNodeArray1;
    LayoutElementTreeRootArray layoutElementTreeRoots;
    LayoutElementHashMapItemArray layoutElementsHashMapInternal;
    IntArray layoutElementsHashMap;
    MeasureTextCacheItemArray measureTextHashMapInternal;
    IntArray measureTextHashMapInternalFreeList;
    IntArray measureTextHashMap;
    MeasuredWordArray measuredWords;
    IntArray measuredWordsFreeList;
    IntArray openClipElementStack;
    ElementIdArray pointerOverIds;
    ScrollContainerDataInternalArray scrollContainerDatas;
    BoolArray treeNodeVisited;
    CharArray dynamicStringData;
    DebugElementDataArray debugElementData;
}

// =====================
// ===== FUNCTIONS =====
// =====================

// ===== Public Clay API C3 Functions (String replacement) =====
fn ElementId getElementIdWithIndex(String idString, uint index)                                                                                                 @inline
{ return __getElementIdWithIndex({idString.len, idString}, (uint)index); }
fn ElementId getElementId(String idString)                                                                                                                      @inline
{ return __getElementId({idString.len, idString}); }

// ===== Public Clay API Functions =====                                                                                                                        // TODO: worry about exports/ static library creation later
extern fn uint minMemorySize()                                                                              @extern("Clay_MinMemorySize")                       ; // @export;
extern fn Arena createArena(uint capacity, void* offset)                                                    @extern("Clay_CreateArenaWithCapacityAndMemory")    ; // @export;
extern fn void setPointerState(ClayVector2 position, bool pointerDown)                                      @extern("Clay_SetPointerState")                     ; // @export;
extern fn Context* initialize(Arena arena, Dimensions layoutDimensions, ErrorHandler errorHandler)          @extern("Clay_Initialize")                          ; // @export;
extern fn Context* getCurrentContext()                                                                      @extern("Clay_GetCurrentContext")                   ; // @export;
extern fn void setCurrentContext(Context* context)                                                          @extern("Clay_SetCurrentContext")                   ; // @export;
extern fn void updateScrollContainer(bool enableDragScrolling, ClayVector2 scrollDelta, float deltaTime)    @extern("Clay_UpdateScrollContainers")              ; // @export;             
extern fn void setLayoutDimensions (Dimensions dimensions)                                                  @extern("Clay_SetLayoutDimensions")                 ; // @export;
extern fn ElementData getElementData(ElementId id)                                                          @extern("Clay_GetElementData")                      ; // @export;
extern fn bool hovered()                                                                                    @extern("Clay_Hovered")                             ; // @export;
extern fn void onHover(OnHoverEvent onHover, iptr userData)                                                 @extern("Clay_OnHover")                             ; // @export;                                        
extern fn bool pointerOver(ElementId elementId)                                                             @extern("Clay_PointerOver")                         ; // @export;
extern fn ScrollContainerData getScrollContainerData(ElementId id)                                          @extern("Clay_GetScrollContainerData")              ; // @export;
extern fn void setMeasureTextFunction(MeasureTextFunc measureText)                                          @extern("Clay_SetMeasureTextFunction")              ; // @export;
extern fn void setQueryScrollOffsetFunction(QueryScrollOffsetFunc queryScrollOffset)                        @extern("Clay_SetQueryScrollOffsetFunction")        ; // @export;
extern fn RenderCommand * RenderCommandArray.get(RenderCommandArray* array, int index)                      @extern("Clay_RenderCommandArray_Get")              ; // @export;
extern fn void setDebugModeEnabled(bool enabled)                                                            @extern("Clay_SetDebugModeEnabled")                 ; // @export;
extern fn bool isDebugModeEnabled()                                                                         @extern("Clay_IsDebugModeEnabled")                  ; // @export;
extern fn void setCullingEnabled(bool enabled)                                                              @extern("Clay_SetCullingEnabled")                   ; // @export;
extern fn int getMaxMeasuredTextCachedWordCount()                                                           @extern("Clay_GetMaxElementCount")                  ; // @export;
extern fn void setMaxElementCount(int maxElementCount)                                                      @extern("Clay_SetMaxElementCount")                  ; // @export;
extern fn int getMaxElementCount()                                                                          @extern("Clay_GetMaxMeasureTextCacheWordCount")     ; // @export;
extern fn void setMaxMeasureTextCacheWordCount(int maxMeasureTextCacheWordCount)                            @extern("Clay_SetMaxMeasureTextCacheWordCount")     ; // @export;
extern fn void resetMeasureTextCache()                                                                      @extern("Clay_ResetMeasureTextCache")               ; // @export;
extern fn void beginLayout()                                                                                @extern("Clay_BeginLayout")                         ; // @export;
extern fn RenderCommandArray endLayout()                                                                    @extern("Clay_EndLayout")                           ; // @export;

// ===== (NEW) Internal Clay API Functions (String replacement) =====
extern fn ElementId __getElementIdWithIndex(ClayString idString, uint index)                                @extern("Clay_GetElementIdWithIndex")               ; // @export;
extern fn ElementId __getElementId(ClayString idString)                                                     @extern("Clay_GetElementId")                        ; // @export;

// ===== Internal Clay API Functions =====
extern fn void openElement()                                                                                @extern ("Clay__OpenElement")                       ; // @export;
extern fn void closeElement()                                                                               @extern("Clay__CloseElement")                       ; // @export;
extern fn void openTextElement(ClayString text, TextElementConfig *textConfig)                              @extern("Clay__OpenTextElement")                    ; // @export;
extern fn void elementPostConfiguration()                                                                   @extern("Clay__ElementPostConfiguration")           ; // @export;
extern fn LayoutConfig * storeLayoutConfig(LayoutConfig config)                                             @extern("Clay__StoreLayoutConfig")                  ; // @export;
extern fn void attachId(ElementId id)                                                                       @extern("Clay__AttachId")                           ; // @export;
extern fn void attachLayoutConfig(LayoutConfig *config)                                                     @extern("Clay__AttachLayoutConfig")                 ; // @export;
extern fn void attachElementConfig(ElementConfigUnion config, ElementConfigType type)                       @extern("Clay__AttachElementConfig")                ; // @export;
extern fn RectangleElementConfig * storeRectangleElementConfig(RectangleElementConfig config)               @extern("Clay__StoreRectangleElementConfig")        ; // @export;
extern fn TextElementConfig * storeTextElementConfig(TextElementConfig config)                              @extern("Clay__StoreTextElementConfig")             ; // @export;
extern fn ImageElementConfig * storeImageElementConfig(ImageElementConfig config)                           @extern("Clay__StoreImageElementConfig")            ; // @export;
extern fn FloatingElementConfig * storeFloatingElementConfig(FloatingElementConfig config)                  @extern("Clay__StoreFloatingElementConfig")         ; // @export;
extern fn CustomElementConfig * storeCustomElementConfig(CustomElementConfig config)                        @extern("Clay__StoreCustomElementConfig")           ; // @export;
extern fn ScrollElementConfig * storeScrollElementConfig(ScrollElementConfig config)                        @extern("Clay__StoreScrollElementConfig")           ; // @export;
extern fn BorderElementConfig * storeBorderElementConfig(BorderElementConfig config)                        @extern("Clay__StoreBorderElementConfig")           ; // @export;
extern fn ElementId hashString(ClayString key, uint offset, uint seed) 						                @extern("Clay__HashString")                         ; // @export;
extern fn uint getParentElementId()                                                                         @extern("Clay__GetParentElementId")                 ; // @export;

// ==========================================================================
// ===== An internal module for wrapping Struct Array's defined in Clay =====
// ==========================================================================
module clay::carray(<ElementType>);

struct Array {
    int capacity;
    int length;
    ElementType *data;
}