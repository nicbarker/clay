// TODO: including additional structures required for Clay_Context led to bloat

// module clay;

// // import std::core::cinterop ;
// import clay::carray;

// // =======================
// // ===== USER MACROS =====
// // =======================
// macro @clay(...; @body()) @builtin
// {
//     clay::openElement(); 
//     $for (var $i = 0; $i < $vacount; $i++)
//         $vaexpr[$i]; // If you get an error here consisder the @body[...]() macros
//     $endfor
//     clay::elementPostConfiguration();
//     @body();
//     clay::closeElement();
// }

// macro text(String text, TextElementConfig *config) { clay::openTextElement({text.len, text}, config); }

// <*Provides you conditional calls (eg #booleanCondition ? #doA : 0) within the condifuration of @clay(...)*>
// macro @bodyIf(#condition, #ifRes) { if (#condition) { #ifRes; } }

// <*Provides you conditional calls (eg #booleanCondition ? #doA : #doB) within the condifuration of @clay(...)*>
// macro @bodyIfElse(#condition, #ifRes, #elseRes) { if (#condition) { #ifRes; } else { #elseRes; } }

// // <*Facilitates non-method calls (eg { #doWhatever }, { #booleanExpression ? #doA : #doB}, etc.) within the parameters of @clay(...)*>
// // macro @inline(; @body()) { @body(); }

// <*attaches a RectangleElementConfig to the clay element when called within @clay(...)*>
// macro rectangle(RectangleElementConfig config) { clay::attachElementConfig({ .rectangleElementConfig = clay::storeRectangleElementConfig(config) }, clay::ELEMENT_CONFIG_TYPE_RECTANGLE ); }

// <*attaches a LayoutConfig to the clay element when called within @clay(...)*>
// macro layout(LayoutConfig config) { clay::attachLayoutConfig( clay::storeLayoutConfig(config) ); }

// <*attaches a LayoutConfig to the clay element when called within @clay(...)*>
// macro scroll(ScrollElementConfig config) { clay::attachElementConfig({ .scrollElementConfig = clay::storeScrollElementConfig(config) }, clay::ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER ); }

// <*attaches a FloatingElementConfig to the clay element when called within @clay(...)*>
// macro floating(FloatingElementConfig config) { clay::attachElementConfig({ .floatingElementConfig = clay::storeFloatingElementConfig(config) }, clay::ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER ); }

// <*attaches a BorderElementConfig to the clay element when called within @clay(...)*>
// macro @borderRadiusUni(uint #width, ClayColor #color, float #cornerRadius = 0) { clay::attachElementConfig({ .borderElementConfig = clay::storeBorderElementConfig({ .left = { #width, #color }, .right = { #width, #color }, .top = { #width, #color }, .bottom = { #width, #color }, .#cornerRadius = {#cornerRadius, #cornerRadius, #cornerRadius, #cornerRadius}})}, clay::ELEMENT_CONFIG_TYPE_BORDER_CONTAINER); }

// macro id(String idString) { clay::attachId(clay::hashString({idString.len, idString}, 0, 0)); }

// macro TextElementConfig* textConfig(TextElementConfig config) { return clay::storeTextElementConfig(config); }

// macro SizingAxis sizingFit(float min = 0, float max = float.max) { return { .size.minMax = {min, max}, .type = SizingType.FIT }; }

// macro SizingAxis sizingGrow() { return { .size.minMax = {0, 0}, .type = SizingType.GROW }; }

// macro SizingAxis sizingFixed(float pixels) { return { .size.minMax = {pixels, pixels}, .type = SizingType.FIXED }; }

// macro SizingAxis sizingPercent(float percent) { return { .size.percent = percent, .type = SizingType.PERCENT }; }

// macro Padding paddingUni(ushort uniform) { return {uniform, uniform, uniform, uniform}; }

// macro Padding padding(ushort horizontal, ushort vertical) { return {horizontal, horizontal, vertical, vertical}; }

// macro CornerRadius cornerRadiusUni(float uniform) { return {uniform, uniform, uniform, uniform}; }

// macro SizingAxis sizingFitCT(float $min = 0, float $max = float.max) { return { .size.minMax = {$min, $max}, .type = SizingType.FIT }; }

// macro SizingAxis sizingFixedCT(float $pixels) { return { .size.minMax = {$pixels, $pixels}, .type = SizingType.FIXED }; }

// macro SizingAxis sizingPercentCT(float $percent) { return { .size.percent = $percent, .type = SizingType.PERCENT }; }

// macro Padding paddingCT(ushort $a, ushort $b, ushort $c, ushort $d) { return { $a, $b, $c, $d }; }

// macro CornerRadius @cornerRadiusUniCT(float #uniform) { return {#uniform, #uniform, #uniform, #uniform}; }

// // ===================
// // ===== STRUCTS =====
// // ===================
// struct ClayString
// {
//     int length;
//     char *chars;
// }
// def ClayStringArray = carray::Array(<ClayString>) @private;

// struct Arena
// {
//     uint128 nextAllocation;
//     uint128 capacity;
//     char *memory;
// }

// struct Dimensions
// {
//     float width, height;
// }

// struct ClayVector2 
// {
//     float x, y;
// }

// struct ClayColor
// {
//     float r, g, b, a;
// }

// struct ClayBoundingBox
// {
//     float x, y, width, height;
// }

// struct ElementId
// {
//     uint id;
//     uint offset;
//     uint baseId;
//     ClayString stringId;
// }

// struct CornerRadius
// {
//     float topLeft;
//     float topRight;
//     float bottomLeft;
//     float bottomRight;
// }

// // ===== Element Configs =====
// distinct ElementConfigType                                      @private = char;
// const ElementConfigType ELEMENT_CONFIG_TYPE_NONE                @private = 0;
// const ElementConfigType ELEMENT_CONFIG_TYPE_RECTANGLE           @private = 1;
// const ElementConfigType ELEMENT_CONFIG_TYPE_BORDER_CONTAINER    @private = 2;
// const ElementConfigType ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER  @private = 4;
// const ElementConfigType ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER    @private = 8;
// const ElementConfigType ELEMENT_CONFIG_TYPE_IMAGE               @private = 16;
// const ElementConfigType ELEMENT_CONFIG_TYPE_TEXT                @private = 32;
// const ElementConfigType ELEMENT_CONFIG_TYPE_CUSTOM              @private = 64;

// enum LayoutDirection : char @export 
// {
//     LEFT_TO_RIGHT,
//     TOP_TO_BOTTOM,
// }

// enum AlignX : char @export 
// {
//     LEFT,
//     RIGHT,
//     CENTER,
// }

// enum AlignY : char @export 
// {
//     TOP,
//     BOTTOM,
//     CENTER,
// }

// enum SizingType : char @export 
// {
//     FIT,
//     GROW,
//     PERCENT,
//     FIXED,
// }

// struct ChildAlignment
// {
//     AlignX x;
//     AlignY y;
// }

// struct SizingMinMax
// {
//     float min;
//     float max;
// }

// struct SizingAxis
// {
//     union size
//     {
//         SizingMinMax minMax;
//         float percent;
//     }
//     SizingType type;
// }

// struct Sizing
// {
//     SizingAxis width;
//     SizingAxis height;
// }

// struct Padding
// {
//     ushort left;
//     ushort right;
//     ushort top;
//     ushort bottom;
// }

// struct LayoutConfig
// {
//     Sizing sizing;
//     Padding padding;
//     ushort childGap;
//     ChildAlignment childAlignment;
//     LayoutDirection layoutDirection;
// }

// struct RectangleElementConfig
// {
//     ClayColor color;
//     CornerRadius cornerRadius;
//     // #ifdef CLAY_EXTEND_CONFIG_RECTANGLE
//     // CLAY_EXTEND_CONFIG_RECTANGLE
//     // #endif
// }

// enum WrapMode @export
// {
//     WORDS,
//     NEWLINES,
//     NONE,
// }

// struct TextElementConfig
// {
//     ClayColor textColor;
//     ushort fontId;
//     ushort fontSize;
//     ushort letterSpacing;
//     ushort lineHeight;
//     WrapMode wrapMode;
//     // #ifdef CLAY_EXTEND_CONFIG_TEXT
//     // CLAY_EXTEND_CONFIG_TEXT
//     // #endif
// }

// struct ImageElementConfig
// {
//     void *imageData;
//     Dimensions sourceDimensions;
//     // #ifdef CLAY_EXTEND_CONFIG_IMAGE
//     // CLAY_EXTEND_CONFIG_IMAGE
//     // #endif
// }

// enum AttachPoint : char @export
// {
//     LEFT_TOP,
//     LEFT_CENTER,
//     LEFT_BOTTOM,
//     CENTER_TOP,
//     CENTER_CENTER,
//     CENTER_BOTTOM,
//     RIGHT_TOP,
//     RIGHT_CENTER,
//     RIGHT_BOTTOM,
// }

// struct FloatingAttachPoints
// {
//     AttachPoint element;
//     AttachPoint parent;
// }

// enum PointerCaptureMode @export
// {
//     CAPTURE,
//     // MODE_PASSTHROUGH,
//     PARENT, 
// }

// struct FloatingElementConfig
// {
//     ClayVector2 offset;
//     Dimensions expand;
//     ushort zIndex;
//     uint parentId;
//     FloatingAttachPoints attachment;
//     PointerCaptureMode pointerCaptureMode;
// }


// struct CustomElementConfig
// {        
//     // #ifndef CLAY_EXTEND_CONFIG_CUSTOM
//     void *customData;
//     // #else
//     // CLAY_EXTEND_CONFIG_CUSTOM 
//     // #endif
// }

// struct ScrollElementConfig
// {
//     bool horizontal;
//     bool vertical;
// }

// // Border
// struct Border
// {
//     uint width;
//     ClayColor color;
// }

// struct BorderElementConfig
// {
//     Border left;
//     Border right;
//     Border top;
//     Border bottom;
//     Border betweenChildren;
//     CornerRadius cornerRadius;
//     // #ifdef CLAY_EXTEND_CONFIG_BORDER
//     // CLAY_EXTEND_CONFIG_BORDER
//     // #endif
// }

// union ElementConfigUnion
// {
//     RectangleElementConfig *rectangleElementConfig;
//     TextElementConfig *textElementConfig;
//     ImageElementConfig *imageElementConfig;
//     FloatingElementConfig *floatingElementConfig;
//     CustomElementConfig *customElementConfig;
//     ScrollElementConfig *scrollElementConfig;
//     BorderElementConfig *borderElementConfig;
// }

// struct ElementConfig
// {
//     ElementConfigType type;
//     ElementConfigUnion config;
// }

// // Miscellaneous Structs & Enums ---------------------------------
// struct ScrollContainerData
// {
//     // Note: This is a pointer to the real internal scroll position, mutating it may cause a change in final layout.
//     // Intended for use with external functionality that modifies scroll position, such as scroll bars or auto scrolling.
//     ClayVector2 *scrollPosition;
//     Dimensions scrollContainerDimensions;
//     Dimensions contentDimensions;
//     ScrollElementConfig config;
//     // Indicates whether an actual scroll container matched the provided ID or if the default struct was returned.
//     bool found;
// }

// <*
//     @
// *>
// struct ElementData
// {
//     ClayBoundingBox boundingBox;
//     // Indicates whether an actual Element matched the provided ID or if the default struct was returned.
//     bool found;
// }

// enum RenderCommandType : char @export
// {
//     NONE,
//     RECTANGLE,
//     BORDER,
//     TEXT,
//     IMAGE,
//     SCISSOR_START,
//     SCISSOR_END,
//     CUSTOM,
// }

// struct RenderCommand
// {
//     ClayBoundingBox boundingBox;
//     ElementConfigUnion config;
//     ClayString text;
//     uint id;
//     RenderCommandType commandType;
// }
// def RenderCommandArray = carray::Array(<RenderCommand>);

// enum PointerState @export
// {
//     PRESSED_THIS_FRAME,
//     PRESSED,
//     RELEASED_THIS_FRAME,
//     RELEASED,
// }

// struct PointerData
// {
//     ClayVector2 position;
//     PointerState state;
// }

// enum ErrorType @export
// {
//     TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED,
//     ARENA_CAPACITY_EXCEEDED,
//     ELEMENTS_CAPACITY_EXCEEDED,
//     TEXT_MEASUREMENT_CAPACITY_EXCEEDED,
//     DUPLICATE_ID,
//     FLOATING_CONTAINER_PARENT_NOT_FOUND,
//     INTERNAL_ERROR,
// }

// struct ErrorData
// {
//     ErrorType errorType;
//     ClayString errorText;
//     uint128 userData;
// }

// def ErrorHandleFunc = fn void(ErrorData errorText);

// struct ErrorHandler
// {
//     ErrorHandleFunc errorHandler;
//     uint128 userData;
// }

// struct BooleanWarnings
// {
//     bool maxElementsExceeded;
//     bool maxRenderCommandsExceeded;
//     bool maxTextMeasureCacheExceeded;
// }

// struct Warning
// {
//     ClayString baseMessage;
//     ClayString dynamicMessage;
// }

// def WarningArray = carray::Array(<Warning>);

// struct LayoutElementChildren
// { 
//     int *elements;
//     ushort length;
// }

// struct LayoutElement
// {
//     union childrenOrTextContent {
//         LayoutElementChildren children;
//         TextElementData *textElementData;
//     }
//     Dimensions dimensions;
//     Dimensions minDimensions;
//     LayoutConfig *layoutConfig;
//     ElementConfigArraySlice elementConfigs;
//     uint configsEnabled;
//     uint id;
// }

// struct WrappedTextLine
// {
//     Dimensions dimensions;
//     ClayString line;
// }

// struct WrappedTextLineArraySlice
// {
// 	int length;
// 	WrappedTextLine *internalArray;
// }

// struct TextElementData
// {
//     ClayString text;
//     Dimensions preferredDimensions;
//     int elementIndex;
//     WrappedTextLineArraySlice wrappedLines;
// }

// struct ElementConfigArraySlice
// {
// 	int length;
// 	ElementConfig *internalArray;
// }


// def DebugElementData = bool[<2>];

// struct LayoutElementHashMapItem
// { 
//     ClayBoundingBox boundingBox;
//     ElementId elementId;
//     LayoutElement* layoutElement;
//     OnHoverEvent onHoverFunction;
//     int128 hoverFunctionUserData;
//     int nextIndex;
//     uint128 generation;
//     DebugElementData *debugData;
// }
 
// struct LayoutElementTreeRoot @private
// {
//     int layoutElementIndex;
//     uint parentId; // This can be zero in the case of the root layout tree
//     uint clipElementId; // This can be zero if there is no clip element
//     int zIndex;
//     ClayVector2 pointerOffset; // Only used when scroll containers are managed externally
// }

// struct LayoutElementTreeNode @private
// {
//     LayoutElement *layoutElement;
//     ClayVector2 position;
//     ClayVector2 nextChildOffset;
// }

// struct MeasuredWord @private
// {
//     int startOffset;
//     int length;
//     float width;
//     int next;
// }

// struct MeasureTextCacheItem @private
// {
//     Dimensions unwrappedDimensions;
//     int measuredWordsStartIndex;
//     bool containsNewlines;
//     // Hash map data
//     uint id;
//     int nextIndex;
//     uint generation;
// }

// def LayoutElementArray = carray::Array(<LayoutElement>);
// def CIntArray = carray::Array(<int>);
// def TextElementDataArray = carray::Array(<TextElementData>);
// def RectangleElementConfigArray = carray::Array(<RectangleElementConfig>);
// def TextElementConfigArray = carray::Array(<TextElementConfig>);
// def ImageElementConfigArray = carray::Array(<ImageElementConfig>);
// def FloatingElementConfigArray = carray::Array(<FloatingElementConfig>);
// def ScrollElementConfigArray = carray::Array(<ScrollElementConfig>);
// def CustomElementConfigArray = carray::Array(<CustomElementConfig>);
// def BorderElementConfigArray = carray::Array(<BorderElementConfig>);
// def LayoutElementPointerArray = carray::Array(<LayoutElement*>);
// def LayoutConfigArray = carray::Array(<LayoutConfig>);
// def ElementConfigArray = carray::Array(<ElementConfig>);
// def WrappedTextLineArray = carray::Array(<WrappedTextLine>);
// def LayoutElementTreeNodeArray = carray::Array(<LayoutElementTreeNode>);
// def LayoutElementTreeRootArray = carray::Array(<LayoutElementTreeRoot>);
// def LayoutElementHashMapItemArray = carray::Array(<LayoutElementHashMapItem>);
// def MeasureTextCacheItemArray = carray::Array(<MeasureTextCacheItem>);
// def MeasuredWordArray = carray::Array(<MeasuredWord>);
// def ElementIdArray = carray::Array(<ElementId>);
// def ScrollContainerDataInternalArray = carray::Array(<ScrollContainerData>);
// def BoolArray = carray::Array(<bool>);
// def CharArray = carray::Array(<char>);
// def DebugElementDataArray = carray::Array(<DebugElementData>);

// struct Context @extern ("Clay_Context") 
// {
//     int maxElementCount;
//     int maxMeasureTextCacheWordCount;
//     bool warningsEnabled;
//     ErrorHandler errorHandler;
//     BooleanWarnings booleanWarnings;
//     WarningArray warnings;

//     PointerData pointerInfo;
//     Dimensions layoutDimensions;
//     ElementId dynamicElementIndexBaseHash;
//     uint dynamicElementIndex;
//     bool debugModeEnabled;
//     bool disableCulling;
//     bool externalScrollHandlingEnabled;
//     uint debugSelectedElementId;
//     uint generation;
//     uint128 arenaResetOffset;
//     Arena internalArena;

//     // Layout Elements / Render Commands
//     LayoutElementArray layoutElements;
//     RenderCommandArray renderCommands;
//     CIntArray openLayoutElementStack;
//     CIntArray layoutElementChildren;
//     CIntArray layoutElementChildrenBuffer;
//     TextElementDataArray textElementData;
//     LayoutElementPointerArray imageElementPointers;
//     CIntArray reusableElementIndexBuffer;
//     CIntArray layoutElementClipElementIds;

//     // Configs
//     LayoutConfigArray layoutConfigs;
//     ElementConfigArray elementConfigBuffer;
//     ElementConfigArray elementConfigs;
//     RectangleElementConfigArray rectangleElementConfigs;
//     TextElementConfigArray textElementConfigs;
//     ImageElementConfigArray imageElementConfigs;
//     FloatingElementConfigArray floatingElementConfigs;
//     ScrollElementConfigArray scrollElementConfigs;
//     CustomElementConfigArray customElementConfigs;
//     BorderElementConfigArray borderElementConfigs;
    
//     // Misc Data Structures
//     ClayStringArray layoutElementIdStrings;
//     WrappedTextLineArray wrappedTextLines;
//     LayoutElementTreeNodeArray layoutElementTreeNodeArray1;
//     LayoutElementTreeRootArray layoutElementTreeRoots;
//     LayoutElementHashMapItemArray layoutElementsHashMapInternal;
//     CIntArray layoutElementsHashMap;
//     MeasureTextCacheItemArray measureTextHashMapInternal;
//     CIntArray measureTextHashMapInternalFreeList;
//     CIntArray measureTextHashMap;
//     MeasuredWordArray measuredWords;
//     CIntArray measuredWordsFreeList;
//     CIntArray openClipElementStack;
//     ElementIdArray pointerOverIds;
//     ScrollContainerDataInternalArray scrollContainerDatas;
//     BoolArray treeNodeVisited;
//     CharArray dynamicStringData;
//     DebugElementDataArray debugElementData;
// }

// def OnHoverEvent = fn void(ElementId elementId, PointerData pointerData, iptr userData);
// def MeasureTextFunc = fn Dimensions(ClayString *text, TextElementConfig *config);
// def QueryScrollOffsetFunc = fn ClayVector2(uint elementId);

// // =====================
// // ===== FUNCTIONS =====
// // =====================

// // ===== Public Clay API C3 Functions (String replacement) =====
// fn ElementId getElementIdWithIndex(String idString, uint index)                                                                         @export @inline 
// { return __getElementIdWithIndex({idString.len, idString}, (uint)index); }
// fn ElementId getElementId(String idString)                                                                                              @export @inline 
// { return __getElementId({idString.len, idString}); }


// // ===== Public Clay API Functions =====
// extern fn uint minMemorySize() @extern("Clay_MinMemorySize")                                                                            @wasm @export;
// extern fn Arena createArena(uint capacity, void* offset) @extern("Clay_CreateArenaWithCapacityAndMemory")                               @wasm @export;
// extern fn void setPointerState(ClayVector2 position, bool pointerDown) @extern("Clay_SetPointerState")                                  @export;
// extern fn Context* initialize(Arena arena, Dimensions layoutDimensions, ErrorHandler errorHandler) @extern("Clay_Initialize")           @wasm @export;
// extern fn Context* getCurrentContext() @extern("Clay_GetCurrentContext")                                                                @export;
// extern fn void setCurrentContext(Context* context) @extern("Clay_SetCurrentContext")                                                    @export;
// extern fn void updateScrollContainer(bool enableDragScrolling, ClayVector2 scrollDelta, float deltaTime) @extern("Clay_UpdateScrollContainers") @export;
// extern fn void setLayoutDimensions (Dimensions dimensions) @extern("Clay_SetLayoutDimensions")                                          @export;
// extern fn ElementData getElementData(ElementId id) @extern("Clay_GetElementData")                                                       @export;
// extern fn bool hovered() @extern("Clay_Hovered")                                                                                        @export;
// extern fn void onHover(OnHoverEvent onHover, iptr userData) @extern("Clay_OnHover")                                                     @export;
// extern fn bool pointerOver(ElementId elementId) @extern("Clay_PointerOver")                                                             @wasm @export;
// extern fn ScrollContainerData getScrollContainerData(ElementId id) @extern("Clay_GetScrollContainerData")                               @export;
// extern fn void setMeasureTextFunction(MeasureTextFunc measureText) @extern("Clay_SetMeasureTextFunction")                               @export;
// extern fn void setQueryScrollOffsetFunction(QueryScrollOffsetFunc queryScrollOffset) @extern("Clay_SetQueryScrollOffsetFunction")       @export;
// extern fn RenderCommand * RenderCommandArray.get(RenderCommandArray* array, int index) @extern("Clay_RenderCommandArray_Get")           @export;
// extern fn void setDebugModeEnabled(bool enabled) @extern("Clay_SetDebugModeEnabled")                                                    @export;
// extern fn bool isDebugModeEnabled() @extern("Clay_IsDebugModeEnabled")                                                                  @export;
// extern fn void setCullingEnabled(bool enabled) @extern("Clay_SetCullingEnabled")                                                        @export;
// extern fn int getMaxMeasuredTextCachedWordCount() @extern("Clay_GetMaxElementCount")                                                    @export;
// extern fn void setMaxElementCount(int maxElementCount) @extern("Clay_SetMaxElementCount")                                               @export;
// extern fn int getMaxElementCount() @extern("Clay_GetMaxMeasureTextCacheWordCount")                                                      @export;
// extern fn void setMaxMeasureTextCacheWordCount(int maxMeasureTextCacheWordCount) @extern("Clay_SetMaxMeasureTextCacheWordCount")        @export;
// extern fn void resetMeasureTextCache() @extern("Clay_ResetMeasureTextCache")                                                            @export;
// extern fn void beginLayout() @extern("Clay_BeginLayout")                                                                                @export;
// extern fn RenderCommandArray endLayout() @extern("Clay_EndLayout")                                                                      @export;

// // ===== (NEW) Internal Clay API Functions (String replacement) =====
// extern fn ElementId __getElementIdWithIndex(ClayString idString, uint index) @extern("Clay_GetElementIdWithIndex")                          @export @private;
// extern fn ElementId __getElementId(ClayString idString) @extern("Clay_GetElementId")                                                        @export @private;

// // ===== Internal Clay API Functions =====
// extern fn void openElement() @extern ("Clay__OpenElement")                                                                                  @export @private;
// extern fn void closeElement() @extern("Clay__CloseElement")                                                                                 @export @private;
// extern fn void openTextElement(ClayString text, TextElementConfig *textConfig) @extern("Clay__OpenTextElement")                             @export @private;
// extern fn void elementPostConfiguration() @extern("Clay__ElementPostConfiguration")                                                         @export @private;
// extern fn LayoutConfig * storeLayoutConfig(LayoutConfig config) @extern("Clay__StoreLayoutConfig")                                          @export @private;
// extern fn void attachId(ElementId id) @extern("Clay__AttachId")                                                                             @export @private;
// extern fn void attachLayoutConfig(LayoutConfig *config) @extern("Clay__AttachLayoutConfig")                                                 @export @private;
// extern fn void attachElementConfig(ElementConfigUnion config, ElementConfigType type) @extern("Clay__AttachElementConfig")                  @export @private;
// extern fn RectangleElementConfig * storeRectangleElementConfig(RectangleElementConfig config) @extern("Clay__StoreRectangleElementConfig")  @export @private;
// extern fn TextElementConfig * storeTextElementConfig(TextElementConfig config) @extern("Clay__StoreTextElementConfig")                      @export @private;
// extern fn ImageElementConfig * storeImageElementConfig(ImageElementConfig config) @extern("Clay__StoreImageElementConfig")                  @export @private;
// extern fn FloatingElementConfig * storeFloatingElementConfig(FloatingElementConfig config) @extern("Clay__StoreFloatingElementConfig")      @export @private;
// extern fn CustomElementConfig * storeCustomElementConfig(CustomElementConfig config) @extern("Clay__StoreCustomElementConfig")              @export @private;
// extern fn ScrollElementConfig * storeScrollElementConfig(ScrollElementConfig config) @extern("Clay__StoreScrollElementConfig")              @export @private;
// extern fn BorderElementConfig * storeBorderElementConfig(BorderElementConfig config) @extern("Clay__StoreBorderElementConfig")              @export @private;
// extern fn ElementId hashString(ClayString key, uint offset, uint seed) @extern("Clay__HashString")                                          @export @private;
// extern fn uint getParentElementId() @extern("Clay__GetParentElementId")                                                                     @export @private;

// // ==========================================================================
// // ===== An internal module for wrapping Struct Array's defined in Clay =====
// // ==========================================================================
// module clay::carray(<ElementType>);

// struct Array {
//     int capacity;
//     int length;
//     ElementType *data;
// }