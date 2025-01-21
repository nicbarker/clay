// VERSION: 0.12

/*
    NOTE: In order to use this library you must define
    the following macro in exactly one file, _before_ including clay.h:

    #define CLAY_IMPLEMENTATION
    #include "clay.h"

    See the examples folder for details.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// -----------------------------------------
// HEADER DECLARATIONS ---------------------
// -----------------------------------------

#ifndef CLAY_HEADER
#define CLAY_HEADER

#if !( \
    (defined(__cplusplus) && __cplusplus >= 202002L) || \
    (defined(__STDC__) && __STDC__ == 1 && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
    defined(_MSC_VER) \
)
#error "Clay requires C99, C++20, or MSVC"
#endif

#ifdef CLAY_WASM
#define CLAY_WASM_EXPORT(name) __attribute__((export_name(name)))
#else
#define CLAY_WASM_EXPORT(null)
#endif

// Public Macro API ------------------------

#define CLAY__WRAPPER_TYPE(type) Clay__##type##Wrapper
#define CLAY__WRAPPER_STRUCT(type) typedef struct { type wrapped; } CLAY__WRAPPER_TYPE(type)
#define CLAY__CONFIG_WRAPPER(type, ...) (CLAY__INIT(CLAY__WRAPPER_TYPE(type)) { __VA_ARGS__ }).wrapped

#define CLAY__MAX(x, y) (((x) > (y)) ? (x) : (y))
#define CLAY__MIN(x, y) (((x) < (y)) ? (x) : (y))

#define CLAY_LAYOUT(...) Clay__AttachLayoutConfig(Clay__StoreLayoutConfig(CLAY__CONFIG_WRAPPER(Clay_LayoutConfig, __VA_ARGS__)))

#define CLAY_RECTANGLE(...) Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .rectangleElementConfig = Clay__StoreRectangleElementConfig(CLAY__CONFIG_WRAPPER(Clay_RectangleElementConfig, __VA_ARGS__)) }, CLAY__ELEMENT_CONFIG_TYPE_RECTANGLE)

#define CLAY_TEXT_CONFIG(...) Clay__StoreTextElementConfig(CLAY__CONFIG_WRAPPER(Clay_TextElementConfig, __VA_ARGS__))

#define CLAY_IMAGE(...) Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .imageElementConfig = Clay__StoreImageElementConfig(CLAY__CONFIG_WRAPPER(Clay_ImageElementConfig, __VA_ARGS__)) }, CLAY__ELEMENT_CONFIG_TYPE_IMAGE)

#define CLAY_FLOATING(...) Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .floatingElementConfig = Clay__StoreFloatingElementConfig(CLAY__CONFIG_WRAPPER(Clay_FloatingElementConfig, __VA_ARGS__)) }, CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER)

#define CLAY_CUSTOM_ELEMENT(...) Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .customElementConfig = Clay__StoreCustomElementConfig(CLAY__CONFIG_WRAPPER(Clay_CustomElementConfig, __VA_ARGS__)) }, CLAY__ELEMENT_CONFIG_TYPE_CUSTOM)

#define CLAY_SCROLL(...) Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .scrollElementConfig = Clay__StoreScrollElementConfig(CLAY__CONFIG_WRAPPER(Clay_ScrollElementConfig, __VA_ARGS__)) }, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER)

#define CLAY_BORDER(...) Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .borderElementConfig = Clay__StoreBorderElementConfig(CLAY__CONFIG_WRAPPER(Clay_BorderElementConfig, __VA_ARGS__)) }, CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER)

#define CLAY_BORDER_OUTSIDE(...) Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .borderElementConfig = Clay__StoreBorderElementConfig(CLAY__INIT(Clay_BorderElementConfig) { .left = __VA_ARGS__, .right = __VA_ARGS__, .top = __VA_ARGS__, .bottom = __VA_ARGS__ }) }, CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER)

#define CLAY_BORDER_OUTSIDE_RADIUS(width, color, radius) Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .borderElementConfig = Clay__StoreBorderElementConfig(CLAY__INIT(Clay_BorderElementConfig) { .left = { width, color }, .right = { width, color }, .top = { width, color }, .bottom = { width, color }, .cornerRadius = CLAY_CORNER_RADIUS(radius) })}, CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER)

#define CLAY_BORDER_ALL(...) Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .borderElementConfig = Clay__StoreBorderElementConfig(CLAY__INIT(Clay_BorderElementConfig) { .left = __VA_ARGS__, .right = __VA_ARGS__, .top = __VA_ARGS__, .bottom = __VA_ARGS__, .betweenChildren = __VA_ARGS__ }) }, CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER)

#define CLAY_BORDER_ALL_RADIUS(width, color, radius) Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .borderElementConfig = Clay__StoreBorderElementConfig(CLAY__INIT(Clay_BorderElementConfig) { .left = { width, color }, .right = { width, color }, .top = { width, color }, .bottom = { width, color }, .betweenChildren = { width, color }, .cornerRadius = CLAY_CORNER_RADIUS(radius)}) }, CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER)

#define CLAY_CORNER_RADIUS(radius) (CLAY__INIT(Clay_CornerRadius) { radius, radius, radius, radius })

#define CLAY_PADDING_ALL(padding) CLAY__CONFIG_WRAPPER(Clay_Padding, { padding, padding, padding, padding })

#define CLAY_SIZING_FIT(...) (CLAY__INIT(Clay_SizingAxis) { .size = { .minMax = { __VA_ARGS__ } }, .type = CLAY__SIZING_TYPE_FIT })

#define CLAY_SIZING_GROW(...) (CLAY__INIT(Clay_SizingAxis) { .size = { .minMax = { __VA_ARGS__ } }, .type = CLAY__SIZING_TYPE_GROW })

#define CLAY_SIZING_FIXED(fixedSize) (CLAY__INIT(Clay_SizingAxis) { .size = { .minMax = { fixedSize, fixedSize } }, .type = CLAY__SIZING_TYPE_FIXED })

#define CLAY_SIZING_PERCENT(percentOfParent) (CLAY__INIT(Clay_SizingAxis) { .size = { .percent = (percentOfParent) }, .type = CLAY__SIZING_TYPE_PERCENT })

#define CLAY_ID(label) Clay__AttachId(Clay__HashString(CLAY_STRING(label), 0, 0))

#define CLAY_IDI(label, index) Clay__AttachId(Clay__HashString(CLAY_STRING(label), index, 0))

#define CLAY_ID_LOCAL(label) CLAY_IDI_LOCAL(label, 0)

#define CLAY_IDI_LOCAL(label, index) Clay__AttachId(Clay__HashString(CLAY_STRING(label), index, Clay__GetParentElementId()))

#define CLAY__STRING_LENGTH(s) ((sizeof(s) / sizeof((s)[0])) - sizeof((s)[0]))

#define CLAY__ENSURE_STRING_LITERAL(x) ("" x "")

// Note: If an error led you here, it's because CLAY_STRING can only be used with string literals, i.e. CLAY_STRING("SomeString") and not CLAY_STRING(yourString)
#define CLAY_STRING(string) (CLAY__INIT(Clay_String) { .length = CLAY__STRING_LENGTH(CLAY__ENSURE_STRING_LITERAL(string)), .chars = (string) })

#define CLAY_STRING_CONST(string) { .length = CLAY__STRING_LENGTH(CLAY__ENSURE_STRING_LITERAL(string)), .chars = (string) }

static uint8_t CLAY__ELEMENT_DEFINITION_LATCH;

// Publicly visible layout element macros -----------------------------------------------------

/* This macro looks scary on the surface, but is actually quite simple.
  It turns a macro call like this:

  CLAY(
    CLAY_RECTANGLE(),
    CLAY_ID()
  ) {
      ...children declared here
  }

  Into calls like this:

  Clay_OpenElement();
  CLAY_RECTANGLE();
  CLAY_ID();
  Clay_ElementPostConfiguration();
  ...children declared here
  Clay_CloseElement();

  The for loop will only ever run a single iteration, putting Clay__CloseElement() in the increment of the loop
  means that it will run after the body - where the children are declared. It just exists to make sure you don't forget
  to call Clay_CloseElement().
*/
#define CLAY(...) \
	for (\
		CLAY__ELEMENT_DEFINITION_LATCH = (Clay__OpenElement(), __VA_ARGS__, Clay__ElementPostConfiguration(), 0); \
		CLAY__ELEMENT_DEFINITION_LATCH < 1; \
		++CLAY__ELEMENT_DEFINITION_LATCH, Clay__CloseElement() \
	)

#define CLAY_TEXT(text, textConfig) Clay__OpenTextElement(text, textConfig)

#ifdef __cplusplus

#define CLAY__INIT(type) type
#define CLAY__TYPEDEF(name, ...) typedef __VA_ARGS__ name; CLAY__WRAPPER_STRUCT(name)
#define CLAY__ALIGNMENT(type) alignof(type)
#define CLAY__POINTER_ALIGNMENT alignof(void *)

#define CLAY_PACKED_ENUM enum : uint8_t

#define CLAY__DEFAULT_STRUCT {}

#else

#define CLAY__INIT(type) (type)

#define CLAY__ALIGNMENT_STRUCT(type) struct Clay__Align##type { char c; type x; }
#define CLAY__TYPEDEF(name, ...) typedef __VA_ARGS__ name; CLAY__ALIGNMENT_STRUCT(name); CLAY__WRAPPER_STRUCT(name)
#define CLAY__ALIGNMENT(type) (offsetof(struct Clay__Align##type, x))
#define CLAY__POINTER_ALIGNMENT CLAY__ALIGNMENT(pointer)

// NOTE: If you need to get the offset for other standard types in the future, add them here.
struct Clay__Alignpointer { char c; void *x; };
CLAY__ALIGNMENT_STRUCT(bool);
CLAY__ALIGNMENT_STRUCT(uint8_t);
CLAY__ALIGNMENT_STRUCT(int32_t);

#if defined(_MSC_VER) && !defined(__clang__)
#define CLAY_PACKED_ENUM __pragma(pack(push, 1)) enum __pragma(pack(pop))
#else
#define CLAY_PACKED_ENUM enum __attribute__((__packed__))
#endif

#if __STDC_VERSION__ >= 202311L
#define CLAY__DEFAULT_STRUCT {}
#else
#define CLAY__DEFAULT_STRUCT {0}
#endif

#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

// Utility Structs -------------------------
// Note: Clay_String is not guaranteed to be null terminated. It may be if created from a literal C string,
// but it is also used to represent slices.
CLAY__TYPEDEF(Clay_String, struct {
    int32_t length;
    const char *chars;
});

CLAY__TYPEDEF(Clay__StringArray, struct {
    int32_t capacity;
    int32_t length;
    Clay_String *internalArray;
});

CLAY__TYPEDEF(Clay_StringSlice, struct {
    int32_t length;
    const char *chars;
    // The source string / char* that this slice was derived from
    const char *baseChars;
});

typedef struct Clay_Context Clay_Context;

CLAY__TYPEDEF(Clay_Arena, struct {
    uintptr_t nextAllocation;
    size_t capacity;
    char *memory;
});

CLAY__TYPEDEF(Clay_Dimensions, struct {
    float width, height;
});

CLAY__TYPEDEF(Clay_Vector2, struct {
    float x, y;
});

CLAY__TYPEDEF(Clay_Color, struct {
    float r, g, b, a;
});

CLAY__TYPEDEF(Clay_BoundingBox, struct {
    float x, y, width, height;
});

// baseId + offset = id
CLAY__TYPEDEF(Clay_ElementId, struct {
    uint32_t id;
    uint32_t offset;
    uint32_t baseId;
    Clay_String stringId;
});

CLAY__TYPEDEF(Clay_CornerRadius, struct {
    float topLeft;
    float topRight;
    float bottomLeft;
    float bottomRight;
});

CLAY__TYPEDEF(Clay__ElementConfigType, CLAY_PACKED_ENUM {
    CLAY__ELEMENT_CONFIG_TYPE_NONE = 0,
    CLAY__ELEMENT_CONFIG_TYPE_RECTANGLE = 1,
    CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER = 2,
    CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER = 4,
    CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER = 8,
    CLAY__ELEMENT_CONFIG_TYPE_IMAGE = 16,
    CLAY__ELEMENT_CONFIG_TYPE_TEXT = 32,
    CLAY__ELEMENT_CONFIG_TYPE_CUSTOM = 64,
});

// Element Configs ---------------------------
// Layout
CLAY__TYPEDEF(Clay_LayoutDirection, CLAY_PACKED_ENUM {
    CLAY_LEFT_TO_RIGHT,
    CLAY_TOP_TO_BOTTOM,
});

CLAY__TYPEDEF(Clay_LayoutAlignmentX, CLAY_PACKED_ENUM {
    CLAY_ALIGN_X_LEFT,
    CLAY_ALIGN_X_RIGHT,
    CLAY_ALIGN_X_CENTER,
});

CLAY__TYPEDEF(Clay_LayoutAlignmentY, CLAY_PACKED_ENUM {
    CLAY_ALIGN_Y_TOP,
    CLAY_ALIGN_Y_BOTTOM,
    CLAY_ALIGN_Y_CENTER,
});

CLAY__TYPEDEF(Clay__SizingType, CLAY_PACKED_ENUM {
    CLAY__SIZING_TYPE_FIT,
    CLAY__SIZING_TYPE_GROW,
    CLAY__SIZING_TYPE_PERCENT,
    CLAY__SIZING_TYPE_FIXED,
});

CLAY__TYPEDEF(Clay_ChildAlignment, struct {
    Clay_LayoutAlignmentX x;
    Clay_LayoutAlignmentY y;
});

CLAY__TYPEDEF(Clay_SizingMinMax, struct {
    float min;
    float max;
});

CLAY__TYPEDEF(Clay_SizingAxis, struct {
    union {
        Clay_SizingMinMax minMax;
        float percent;
    } size;
    Clay__SizingType type;
});

CLAY__TYPEDEF(Clay_Sizing, struct {
    Clay_SizingAxis width;
    Clay_SizingAxis height;
});

CLAY__TYPEDEF(Clay_Padding, struct {
    uint16_t left;
    uint16_t right;
    uint16_t top;
    uint16_t bottom;
});

CLAY__TYPEDEF(Clay_LayoutConfig, struct {
    Clay_Sizing sizing;
    Clay_Padding padding;
    uint16_t childGap;
    Clay_ChildAlignment childAlignment;
    Clay_LayoutDirection layoutDirection;
});

extern Clay_LayoutConfig CLAY_LAYOUT_DEFAULT;

// Rectangle
// NOTE: Not declared in the typedef as an ifdef inside macro arguments is UB
struct Clay_RectangleElementConfig {
    Clay_Color color;
    Clay_CornerRadius cornerRadius;
    #ifdef CLAY_EXTEND_CONFIG_RECTANGLE
    CLAY_EXTEND_CONFIG_RECTANGLE
    #endif
};
CLAY__TYPEDEF(Clay_RectangleElementConfig, struct Clay_RectangleElementConfig);

// Text
CLAY__TYPEDEF(Clay_TextElementConfigWrapMode, enum {
    CLAY_TEXT_WRAP_WORDS,
    CLAY_TEXT_WRAP_NEWLINES,
    CLAY_TEXT_WRAP_NONE,
});

struct Clay_TextElementConfig {
    Clay_Color textColor;
    uint16_t fontId;
    uint16_t fontSize;
    uint16_t letterSpacing;
    uint16_t lineHeight;
    Clay_TextElementConfigWrapMode wrapMode;
    #ifdef CLAY_EXTEND_CONFIG_TEXT
    CLAY_EXTEND_CONFIG_TEXT
    #endif
};
CLAY__TYPEDEF(Clay_TextElementConfig, struct Clay_TextElementConfig);

// Image
struct Clay_ImageElementConfig {
    void *imageData;
    Clay_Dimensions sourceDimensions;
    #ifdef CLAY_EXTEND_CONFIG_IMAGE
    CLAY_EXTEND_CONFIG_IMAGE
    #endif
};
CLAY__TYPEDEF(Clay_ImageElementConfig, struct Clay_ImageElementConfig);

// Floating
CLAY__TYPEDEF(Clay_FloatingAttachPointType, CLAY_PACKED_ENUM {
    CLAY_ATTACH_POINT_LEFT_TOP,
    CLAY_ATTACH_POINT_LEFT_CENTER,
    CLAY_ATTACH_POINT_LEFT_BOTTOM,
    CLAY_ATTACH_POINT_CENTER_TOP,
    CLAY_ATTACH_POINT_CENTER_CENTER,
    CLAY_ATTACH_POINT_CENTER_BOTTOM,
    CLAY_ATTACH_POINT_RIGHT_TOP,
    CLAY_ATTACH_POINT_RIGHT_CENTER,
    CLAY_ATTACH_POINT_RIGHT_BOTTOM,
});

CLAY__TYPEDEF(Clay_FloatingAttachPoints, struct {
    Clay_FloatingAttachPointType element;
    Clay_FloatingAttachPointType parent;
});

CLAY__TYPEDEF(Clay_PointerCaptureMode, enum {
    CLAY_POINTER_CAPTURE_MODE_CAPTURE,
//    CLAY_POINTER_CAPTURE_MODE_PARENT, TODO pass pointer through to attached parent
    CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
});

CLAY__TYPEDEF(Clay_FloatingElementConfig, struct {
    Clay_Vector2 offset;
    Clay_Dimensions expand;
    uint16_t zIndex;
    uint32_t parentId;
    Clay_FloatingAttachPoints attachment;
    Clay_PointerCaptureMode pointerCaptureMode;
});

// Custom
struct Clay_CustomElementConfig {
    #ifndef CLAY_EXTEND_CONFIG_CUSTOM
    void *customData;
    #else
    CLAY_EXTEND_CONFIG_CUSTOM
    #endif
};
CLAY__TYPEDEF(Clay_CustomElementConfig, struct Clay_CustomElementConfig);

// Scroll
CLAY__TYPEDEF(Clay_ScrollElementConfig, struct {
    bool horizontal;
    bool vertical;
});

// Border
CLAY__TYPEDEF(Clay_Border, struct {
    uint32_t width;
    Clay_Color color;
});

struct Clay_BorderElementConfig {
    Clay_Border left;
    Clay_Border right;
    Clay_Border top;
    Clay_Border bottom;
    Clay_Border betweenChildren;
    Clay_CornerRadius cornerRadius;
    #ifdef CLAY_EXTEND_CONFIG_BORDER
    CLAY_EXTEND_CONFIG_BORDER
    #endif
};
CLAY__TYPEDEF(Clay_BorderElementConfig, struct Clay_BorderElementConfig);

CLAY__TYPEDEF(Clay_ElementConfigUnion, union {
    Clay_RectangleElementConfig *rectangleElementConfig;
    Clay_TextElementConfig *textElementConfig;
    Clay_ImageElementConfig *imageElementConfig;
    Clay_FloatingElementConfig *floatingElementConfig;
    Clay_CustomElementConfig *customElementConfig;
    Clay_ScrollElementConfig *scrollElementConfig;
    Clay_BorderElementConfig *borderElementConfig;
});

CLAY__TYPEDEF(Clay_ElementConfig, struct {
    Clay__ElementConfigType type;
    Clay_ElementConfigUnion config;
});

// Miscellaneous Structs & Enums ---------------------------------
CLAY__TYPEDEF(Clay_ScrollContainerData, struct {
    // Note: This is a pointer to the real internal scroll position, mutating it may cause a change in final layout.
    // Intended for use with external functionality that modifies scroll position, such as scroll bars or auto scrolling.
    Clay_Vector2 *scrollPosition;
    Clay_Dimensions scrollContainerDimensions;
    Clay_Dimensions contentDimensions;
    Clay_ScrollElementConfig config;
    // Indicates whether an actual scroll container matched the provided ID or if the default struct was returned.
    bool found;
});

CLAY__TYPEDEF(Clay_ElementData, struct
{
    Clay_BoundingBox boundingBox;
    // Indicates whether an actual Element matched the provided ID or if the default struct was returned.
    bool found;
});

CLAY__TYPEDEF(Clay_RenderCommandType, CLAY_PACKED_ENUM {
    CLAY_RENDER_COMMAND_TYPE_NONE,
    CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
    CLAY_RENDER_COMMAND_TYPE_BORDER,
    CLAY_RENDER_COMMAND_TYPE_TEXT,
    CLAY_RENDER_COMMAND_TYPE_IMAGE,
    CLAY_RENDER_COMMAND_TYPE_SCISSOR_START,
    CLAY_RENDER_COMMAND_TYPE_SCISSOR_END,
    CLAY_RENDER_COMMAND_TYPE_CUSTOM,
});

CLAY__TYPEDEF(Clay_RenderCommand, struct {
    Clay_BoundingBox boundingBox;
    Clay_ElementConfigUnion config;
    Clay_String text; // TODO I wish there was a way to avoid having to have this on every render command
    uint32_t id;
    Clay_RenderCommandType commandType;
});

CLAY__TYPEDEF(Clay_RenderCommandArray, struct {
    int32_t capacity;
    int32_t length;
    Clay_RenderCommand *internalArray;
});

CLAY__TYPEDEF(Clay_PointerDataInteractionState, enum {
    CLAY_POINTER_DATA_PRESSED_THIS_FRAME,
    CLAY_POINTER_DATA_PRESSED,
    CLAY_POINTER_DATA_RELEASED_THIS_FRAME,
    CLAY_POINTER_DATA_RELEASED,
});

CLAY__TYPEDEF(Clay_PointerData, struct {
    Clay_Vector2 position;
    Clay_PointerDataInteractionState state;
});

CLAY__TYPEDEF(Clay_ErrorType, enum {
    CLAY_ERROR_TYPE_TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED,
    CLAY_ERROR_TYPE_ARENA_CAPACITY_EXCEEDED,
    CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED,
    CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED,
    CLAY_ERROR_TYPE_DUPLICATE_ID,
    CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND,
    CLAY_ERROR_TYPE_INTERNAL_ERROR,
});

CLAY__TYPEDEF(Clay_ErrorData, struct {
    Clay_ErrorType errorType;
    Clay_String errorText;
    uintptr_t userData;
});

CLAY__TYPEDEF(Clay_ErrorHandler, struct {
    void (*errorHandlerFunction)(Clay_ErrorData errorText);
    uintptr_t userData;
});

// Function Forward Declarations ---------------------------------
// Public API functions ---
uint32_t Clay_MinMemorySize(void);
Clay_Arena Clay_CreateArenaWithCapacityAndMemory(uint32_t capacity, void *offset);
void Clay_SetPointerState(Clay_Vector2 position, bool pointerDown);
Clay_Context* Clay_Initialize(Clay_Arena arena, Clay_Dimensions layoutDimensions, Clay_ErrorHandler errorHandler);
Clay_Context* Clay_GetCurrentContext(void);
void Clay_SetCurrentContext(Clay_Context* context);
void Clay_UpdateScrollContainers(bool enableDragScrolling, Clay_Vector2 scrollDelta, float deltaTime);
void Clay_SetLayoutDimensions(Clay_Dimensions dimensions);
void Clay_BeginLayout(void);
Clay_RenderCommandArray Clay_EndLayout(void);
Clay_ElementId Clay_GetElementId(Clay_String idString);
Clay_ElementId Clay_GetElementIdWithIndex(Clay_String idString, uint32_t index);
Clay_ElementData Clay_GetElementData (Clay_ElementId id);
bool Clay_Hovered(void);
void Clay_OnHover(void (*onHoverFunction)(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData), intptr_t userData);
bool Clay_PointerOver(Clay_ElementId elementId);
Clay_ScrollContainerData Clay_GetScrollContainerData(Clay_ElementId id);
void Clay_SetMeasureTextFunction(Clay_Dimensions (*measureTextFunction)(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData), uintptr_t userData);
void Clay_SetQueryScrollOffsetFunction(Clay_Vector2 (*queryScrollOffsetFunction)(uint32_t elementId, uintptr_t userData), uintptr_t userData);
Clay_RenderCommand * Clay_RenderCommandArray_Get(Clay_RenderCommandArray* array, int32_t index);
void Clay_SetDebugModeEnabled(bool enabled);
bool Clay_IsDebugModeEnabled(void);
void Clay_SetCullingEnabled(bool enabled);
int32_t Clay_GetMaxElementCount(void);
void Clay_SetMaxElementCount(int32_t maxElementCount);
int32_t Clay_GetMaxMeasureTextCacheWordCount(void);
void Clay_SetMaxMeasureTextCacheWordCount(int32_t maxMeasureTextCacheWordCount);
void Clay_ResetMeasureTextCache(void);

// Internal API functions required by macros
void Clay__OpenElement(void);
void Clay__CloseElement(void);
Clay_LayoutConfig * Clay__StoreLayoutConfig(Clay_LayoutConfig config);
void Clay__ElementPostConfiguration(void);
void Clay__AttachId(Clay_ElementId id);
void Clay__AttachLayoutConfig(Clay_LayoutConfig *config);
void Clay__AttachElementConfig(Clay_ElementConfigUnion config, Clay__ElementConfigType type);
Clay_RectangleElementConfig * Clay__StoreRectangleElementConfig(Clay_RectangleElementConfig config);
Clay_TextElementConfig * Clay__StoreTextElementConfig(Clay_TextElementConfig config);
Clay_ImageElementConfig * Clay__StoreImageElementConfig(Clay_ImageElementConfig config);
Clay_FloatingElementConfig * Clay__StoreFloatingElementConfig(Clay_FloatingElementConfig config);
Clay_CustomElementConfig * Clay__StoreCustomElementConfig(Clay_CustomElementConfig config);
Clay_ScrollElementConfig * Clay__StoreScrollElementConfig(Clay_ScrollElementConfig config);
Clay_BorderElementConfig * Clay__StoreBorderElementConfig(Clay_BorderElementConfig config);
Clay_ElementId Clay__HashString(Clay_String key, uint32_t offset, uint32_t seed);
void Clay__OpenTextElement(Clay_String text, Clay_TextElementConfig *textConfig);
uint32_t Clay__GetParentElementId(void);

extern Clay_Color Clay__debugViewHighlightColor;
extern uint32_t Clay__debugViewWidth;

#ifdef __cplusplus
}
#endif

#endif // CLAY_HEADER

// -----------------------------------------
// IMPLEMENTATION --------------------------
// -----------------------------------------
#ifdef CLAY_IMPLEMENTATION
#undef CLAY_IMPLEMENTATION

#ifndef CLAY__NULL
#define CLAY__NULL 0
#endif

#ifndef CLAY__MAXFLOAT
#define CLAY__MAXFLOAT 3.40282346638528859812e+38F
#endif

Clay_Context *Clay__currentContext;
int32_t Clay__defaultMaxElementCount = 8192;
int32_t Clay__defaultMaxMeasureTextWordCacheCount = 16384;

void Clay__ErrorHandlerFunctionDefault(Clay_ErrorData errorText) {
    (void) errorText;
}

Clay_String CLAY__SPACECHAR = { .length = 1, .chars = " " };
Clay_String CLAY__STRING_DEFAULT = { .length = 0, .chars = NULL };

CLAY__TYPEDEF(Clay_BooleanWarnings, struct {
    bool maxElementsExceeded;
    bool maxRenderCommandsExceeded;
    bool maxTextMeasureCacheExceeded;
    bool textMeasurementFunctionNotSet;
});

CLAY__TYPEDEF(Clay__Warning, struct {
    Clay_String baseMessage;
    Clay_String dynamicMessage;
});

Clay__Warning CLAY__WARNING_DEFAULT = CLAY__DEFAULT_STRUCT;

CLAY__TYPEDEF(Clay__WarningArray, struct {
    int32_t capacity;
    int32_t length;
    Clay__Warning *internalArray;
});

Clay__WarningArray Clay__WarningArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena);
Clay__Warning *Clay__WarningArray_Add(Clay__WarningArray *array, Clay__Warning item);
void* Clay__Array_Allocate_Arena(int32_t capacity, uint32_t itemSize, uint32_t alignment, Clay_Arena *arena);
bool Clay__Array_RangeCheck(int32_t index, int32_t length);
bool Clay__Array_AddCapacityCheck(int32_t length, int32_t capacity);

// __GENERATED__ template array_define,array_allocate TYPE=bool NAME=Clay__BoolArray
#pragma region generated
CLAY__TYPEDEF(Clay__BoolArray, struct
{
	int32_t capacity;
	int32_t length;
	bool *internalArray;
});
Clay__BoolArray Clay__BoolArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__BoolArray){.capacity = capacity, .length = 0, .internalArray = (bool *)Clay__Array_Allocate_Arena(capacity, sizeof(bool), CLAY__ALIGNMENT(bool), arena)};
}
#pragma endregion
// __GENERATED__ template

Clay_ElementId CLAY__ELEMENT_ID_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_get,array_add TYPE=Clay_ElementId NAME=Clay__ElementIdArray DEFAULT_VALUE=&CLAY__ELEMENT_ID_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__ElementIdArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_ElementId *internalArray;
});
Clay__ElementIdArray Clay__ElementIdArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__ElementIdArray){.capacity = capacity, .length = 0, .internalArray = (Clay_ElementId *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_ElementId), CLAY__ALIGNMENT(Clay_ElementId), arena)};
}
Clay_ElementId *Clay__ElementIdArray_Get(Clay__ElementIdArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__ELEMENT_ID_DEFAULT;
}
Clay_ElementId *Clay__ElementIdArray_Add(Clay__ElementIdArray *array, Clay_ElementId item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__ELEMENT_ID_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

Clay_ElementConfig CLAY__ELEMENT_CONFIG_DEFAULT = {CLAY__ELEMENT_CONFIG_TYPE_NONE, CLAY__DEFAULT_STRUCT};

// __GENERATED__ template array_define,array_define_slice,array_allocate,array_get,array_add,array_get_slice TYPE=Clay_ElementConfig NAME=Clay__ElementConfigArray DEFAULT_VALUE=&CLAY__ELEMENT_CONFIG_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__ElementConfigArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_ElementConfig *internalArray;
});
CLAY__TYPEDEF(Clay__ElementConfigArraySlice, struct
{
	int32_t length;
	Clay_ElementConfig *internalArray;
});
Clay__ElementConfigArray Clay__ElementConfigArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__ElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_ElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_ElementConfig), CLAY__ALIGNMENT(Clay_ElementConfig), arena)};
}
Clay_ElementConfig *Clay__ElementConfigArray_Get(Clay__ElementConfigArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__ELEMENT_CONFIG_DEFAULT;
}
Clay_ElementConfig *Clay__ElementConfigArray_Add(Clay__ElementConfigArray *array, Clay_ElementConfig item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__ELEMENT_CONFIG_DEFAULT;
}
Clay_ElementConfig *Clay__ElementConfigArraySlice_Get(Clay__ElementConfigArraySlice *slice, int32_t index) {
    return Clay__Array_RangeCheck(index, slice->length) ? &slice->internalArray[index] : &CLAY__ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

Clay_LayoutConfig CLAY_LAYOUT_DEFAULT = { .sizing = { .width = { .size = { .minMax = {0, CLAY__MAXFLOAT } }, .type = CLAY__SIZING_TYPE_FIT }, .height = { .size = { .minMax = {0, CLAY__MAXFLOAT } }, .type = CLAY__SIZING_TYPE_FIT } } };

// __GENERATED__ template array_define,array_allocate,array_add TYPE=Clay_LayoutConfig NAME=Clay__LayoutConfigArray DEFAULT_VALUE=&CLAY_LAYOUT_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__LayoutConfigArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_LayoutConfig *internalArray;
});
Clay__LayoutConfigArray Clay__LayoutConfigArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__LayoutConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_LayoutConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_LayoutConfig), CLAY__ALIGNMENT(Clay_LayoutConfig), arena)};
}
Clay_LayoutConfig *Clay__LayoutConfigArray_Add(Clay__LayoutConfigArray *array, Clay_LayoutConfig item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY_LAYOUT_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

Clay_RectangleElementConfig CLAY__RECTANGLE_ELEMENT_CONFIG_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add TYPE=Clay_RectangleElementConfig NAME=Clay__RectangleElementConfigArray DEFAULT_VALUE=&CLAY__RECTANGLE_ELEMENT_CONFIG_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__RectangleElementConfigArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_RectangleElementConfig *internalArray;
});
Clay__RectangleElementConfigArray Clay__RectangleElementConfigArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__RectangleElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_RectangleElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_RectangleElementConfig), CLAY__ALIGNMENT(Clay_RectangleElementConfig), arena)};
}
Clay_RectangleElementConfig *Clay__RectangleElementConfigArray_Add(Clay__RectangleElementConfigArray *array, Clay_RectangleElementConfig item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__RECTANGLE_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

Clay_TextElementConfig CLAY__TEXT_ELEMENT_CONFIG_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add TYPE=Clay_TextElementConfig NAME=Clay__TextElementConfigArray DEFAULT_VALUE=&CLAY__TEXT_ELEMENT_CONFIG_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__TextElementConfigArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_TextElementConfig *internalArray;
});
Clay__TextElementConfigArray Clay__TextElementConfigArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__TextElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_TextElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_TextElementConfig), CLAY__ALIGNMENT(Clay_TextElementConfig), arena)};
}
Clay_TextElementConfig *Clay__TextElementConfigArray_Add(Clay__TextElementConfigArray *array, Clay_TextElementConfig item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__TEXT_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

Clay_ImageElementConfig CLAY__IMAGE_ELEMENT_CONFIG_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add TYPE=Clay_ImageElementConfig NAME=Clay__ImageElementConfigArray DEFAULT_VALUE=&CLAY__IMAGE_ELEMENT_CONFIG_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__ImageElementConfigArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_ImageElementConfig *internalArray;
});
Clay__ImageElementConfigArray Clay__ImageElementConfigArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__ImageElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_ImageElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_ImageElementConfig), CLAY__ALIGNMENT(Clay_ImageElementConfig), arena)};
}
Clay_ImageElementConfig *Clay__ImageElementConfigArray_Add(Clay__ImageElementConfigArray *array, Clay_ImageElementConfig item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__IMAGE_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

Clay_FloatingElementConfig CLAY__FLOATING_ELEMENT_CONFIG_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add TYPE=Clay_FloatingElementConfig NAME=Clay__FloatingElementConfigArray DEFAULT_VALUE=&CLAY__FLOATING_ELEMENT_CONFIG_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__FloatingElementConfigArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_FloatingElementConfig *internalArray;
});
Clay__FloatingElementConfigArray Clay__FloatingElementConfigArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__FloatingElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_FloatingElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_FloatingElementConfig), CLAY__ALIGNMENT(Clay_FloatingElementConfig), arena)};
}
Clay_FloatingElementConfig *Clay__FloatingElementConfigArray_Add(Clay__FloatingElementConfigArray *array, Clay_FloatingElementConfig item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__FLOATING_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

Clay_CustomElementConfig CLAY__CUSTOM_ELEMENT_CONFIG_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add TYPE=Clay_CustomElementConfig NAME=Clay__CustomElementConfigArray DEFAULT_VALUE=&CLAY__CUSTOM_ELEMENT_CONFIG_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__CustomElementConfigArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_CustomElementConfig *internalArray;
});
Clay__CustomElementConfigArray Clay__CustomElementConfigArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__CustomElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_CustomElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_CustomElementConfig), CLAY__ALIGNMENT(Clay_CustomElementConfig), arena)};
}
Clay_CustomElementConfig *Clay__CustomElementConfigArray_Add(Clay__CustomElementConfigArray *array, Clay_CustomElementConfig item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__CUSTOM_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

Clay_ScrollElementConfig CLAY__SCROLL_ELEMENT_CONFIG_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add TYPE=Clay_ScrollElementConfig NAME=Clay__ScrollElementConfigArray DEFAULT_VALUE=&CLAY__SCROLL_ELEMENT_CONFIG_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__ScrollElementConfigArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_ScrollElementConfig *internalArray;
});
Clay__ScrollElementConfigArray Clay__ScrollElementConfigArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__ScrollElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_ScrollElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_ScrollElementConfig), CLAY__ALIGNMENT(Clay_ScrollElementConfig), arena)};
}
Clay_ScrollElementConfig *Clay__ScrollElementConfigArray_Add(Clay__ScrollElementConfigArray *array, Clay_ScrollElementConfig item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__SCROLL_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

// __GENERATED__ template array_define_slice,array_allocate,array_add TYPE=Clay_String NAME=Clay__StringArray DEFAULT_VALUE=&CLAY__STRING_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__StringArraySlice, struct
{
	int32_t length;
	Clay_String *internalArray;
});
Clay__StringArray Clay__StringArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__StringArray){.capacity = capacity, .length = 0, .internalArray = (Clay_String *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_String), CLAY__ALIGNMENT(Clay_String), arena)};
}
Clay_String *Clay__StringArray_Add(Clay__StringArray *array, Clay_String item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__STRING_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

CLAY__TYPEDEF(Clay__WrappedTextLine, struct {
    Clay_Dimensions dimensions;
    Clay_String line;
});

Clay__WrappedTextLine CLAY__WRAPPED_TEXT_LINE_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_define_slice,array_allocate,array_add,array_get TYPE=Clay__WrappedTextLine NAME=Clay__WrappedTextLineArray DEFAULT_VALUE=&CLAY__WRAPPED_TEXT_LINE_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__WrappedTextLineArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay__WrappedTextLine *internalArray;
});
CLAY__TYPEDEF(Clay__WrappedTextLineArraySlice, struct
{
	int32_t length;
	Clay__WrappedTextLine *internalArray;
});
Clay__WrappedTextLineArray Clay__WrappedTextLineArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__WrappedTextLineArray){.capacity = capacity, .length = 0, .internalArray = (Clay__WrappedTextLine *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__WrappedTextLine), CLAY__ALIGNMENT(Clay__WrappedTextLine), arena)};
}
Clay__WrappedTextLine *Clay__WrappedTextLineArray_Add(Clay__WrappedTextLineArray *array, Clay__WrappedTextLine item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__WRAPPED_TEXT_LINE_DEFAULT;
}
Clay__WrappedTextLine *Clay__WrappedTextLineArray_Get(Clay__WrappedTextLineArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__WRAPPED_TEXT_LINE_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

CLAY__TYPEDEF(Clay__TextElementData, struct {
    Clay_String text;
    Clay_Dimensions preferredDimensions;
    int32_t elementIndex;
    Clay__WrappedTextLineArraySlice wrappedLines;
});

Clay__TextElementData CLAY__TEXT_ELEMENT_DATA_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_get,array_add TYPE=Clay__TextElementData NAME=Clay__TextElementDataArray DEFAULT_VALUE=&CLAY__TEXT_ELEMENT_DATA_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__TextElementDataArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay__TextElementData *internalArray;
});
Clay__TextElementDataArray Clay__TextElementDataArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__TextElementDataArray){.capacity = capacity, .length = 0, .internalArray = (Clay__TextElementData *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__TextElementData), CLAY__ALIGNMENT(Clay__TextElementData), arena)};
}
Clay__TextElementData *Clay__TextElementDataArray_Get(Clay__TextElementDataArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__TEXT_ELEMENT_DATA_DEFAULT;
}
Clay__TextElementData *Clay__TextElementDataArray_Add(Clay__TextElementDataArray *array, Clay__TextElementData item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__TEXT_ELEMENT_DATA_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

Clay_BorderElementConfig CLAY__BORDER_ELEMENT_CONFIG_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add TYPE=Clay_BorderElementConfig NAME=Clay__BorderElementConfigArray DEFAULT_VALUE=&CLAY__BORDER_ELEMENT_CONFIG_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__BorderElementConfigArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_BorderElementConfig *internalArray;
});
Clay__BorderElementConfigArray Clay__BorderElementConfigArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__BorderElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_BorderElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_BorderElementConfig), CLAY__ALIGNMENT(Clay_BorderElementConfig), arena)};
}
Clay_BorderElementConfig *Clay__BorderElementConfigArray_Add(Clay__BorderElementConfigArray *array, Clay_BorderElementConfig item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__BORDER_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

CLAY__TYPEDEF(Clay__LayoutElementChildren, struct {
    int32_t *elements;
    uint16_t length;
});

CLAY__TYPEDEF(Clay_LayoutElement, struct {
    union {
        Clay__LayoutElementChildren children;
        Clay__TextElementData *textElementData;
    } childrenOrTextContent;
    Clay_Dimensions dimensions;
    Clay_Dimensions minDimensions;
    Clay_LayoutConfig *layoutConfig;
    Clay__ElementConfigArraySlice elementConfigs;
    uint32_t configsEnabled;
    uint32_t id;
});

Clay_LayoutElement CLAY__LAYOUT_ELEMENT_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add,array_get TYPE=Clay_LayoutElement NAME=Clay_LayoutElementArray DEFAULT_VALUE=&CLAY__LAYOUT_ELEMENT_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay_LayoutElementArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_LayoutElement *internalArray;
});
Clay_LayoutElementArray Clay_LayoutElementArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay_LayoutElementArray){.capacity = capacity, .length = 0, .internalArray = (Clay_LayoutElement *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_LayoutElement), CLAY__ALIGNMENT(Clay_LayoutElement), arena)};
}
Clay_LayoutElement *Clay_LayoutElementArray_Add(Clay_LayoutElementArray *array, Clay_LayoutElement item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__LAYOUT_ELEMENT_DEFAULT;
}
Clay_LayoutElement *Clay_LayoutElementArray_Get(Clay_LayoutElementArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__LAYOUT_ELEMENT_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

// __GENERATED__ template array_define,array_allocate_pointer,array_add,array_get_value,array_remove_swapback TYPE=Clay_LayoutElement* NAME=Clay__LayoutElementPointerArray DEFAULT_VALUE=CLAY__NULL
#pragma region generated
CLAY__TYPEDEF(Clay__LayoutElementPointerArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_LayoutElement* *internalArray;
});
Clay__LayoutElementPointerArray Clay__LayoutElementPointerArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__LayoutElementPointerArray){.capacity = capacity, .length = 0, .internalArray = (Clay_LayoutElement* *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_LayoutElement*), CLAY__POINTER_ALIGNMENT, arena)};
}
Clay_LayoutElement* *Clay__LayoutElementPointerArray_Add(Clay__LayoutElementPointerArray *array, Clay_LayoutElement* item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return CLAY__NULL;
}
Clay_LayoutElement* Clay__LayoutElementPointerArray_Get(Clay__LayoutElementPointerArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? array->internalArray[index] : CLAY__NULL;
}
Clay_LayoutElement* Clay__LayoutElementPointerArray_RemoveSwapback(Clay__LayoutElementPointerArray *array, int32_t index) {
	if (Clay__Array_RangeCheck(index, array->length)) {
		array->length--;
		Clay_LayoutElement* removed = array->internalArray[index];
		array->internalArray[index] = array->internalArray[array->length];
		return removed;
	}
	return CLAY__NULL;
}
#pragma endregion
// __GENERATED__ template

Clay_RenderCommand CLAY__RENDER_COMMAND_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_allocate,array_add,array_get TYPE=Clay_RenderCommand NAME=Clay_RenderCommandArray DEFAULT_VALUE=&CLAY__RENDER_COMMAND_DEFAULT
#pragma region generated
Clay_RenderCommandArray Clay_RenderCommandArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay_RenderCommandArray){.capacity = capacity, .length = 0, .internalArray = (Clay_RenderCommand *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_RenderCommand), CLAY__ALIGNMENT(Clay_RenderCommand), arena)};
}
Clay_RenderCommand *Clay_RenderCommandArray_Add(Clay_RenderCommandArray *array, Clay_RenderCommand item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__RENDER_COMMAND_DEFAULT;
}
Clay_RenderCommand *Clay_RenderCommandArray_Get(Clay_RenderCommandArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__RENDER_COMMAND_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

CLAY__TYPEDEF(Clay__ScrollContainerDataInternal, struct {
    Clay_LayoutElement *layoutElement;
    Clay_BoundingBox boundingBox;
    Clay_Dimensions contentSize;
    Clay_Vector2 scrollOrigin;
    Clay_Vector2 pointerOrigin;
    Clay_Vector2 scrollMomentum;
    Clay_Vector2 scrollPosition;
    Clay_Vector2 previousDelta;
    float momentumTime;
    uint32_t elementId;
    bool openThisFrame;
    bool pointerScrollActive;
});

Clay__ScrollContainerDataInternal CLAY__SCROLL_CONTAINER_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add,array_get TYPE=Clay__ScrollContainerDataInternal NAME=Clay__ScrollContainerDataInternalArray DEFAULT_VALUE=&CLAY__SCROLL_CONTAINER_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__ScrollContainerDataInternalArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay__ScrollContainerDataInternal *internalArray;
});
Clay__ScrollContainerDataInternalArray Clay__ScrollContainerDataInternalArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__ScrollContainerDataInternalArray){.capacity = capacity, .length = 0, .internalArray = (Clay__ScrollContainerDataInternal *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__ScrollContainerDataInternal), CLAY__ALIGNMENT(Clay__ScrollContainerDataInternal), arena)};
}
Clay__ScrollContainerDataInternal *Clay__ScrollContainerDataInternalArray_Add(Clay__ScrollContainerDataInternalArray *array, Clay__ScrollContainerDataInternal item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__SCROLL_CONTAINER_DEFAULT;
}
Clay__ScrollContainerDataInternal *Clay__ScrollContainerDataInternalArray_Get(Clay__ScrollContainerDataInternalArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__SCROLL_CONTAINER_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

// __GENERATED__ template array_remove_swapback TYPE=Clay__ScrollContainerDataInternal NAME=Clay__ScrollContainerDataInternalArray DEFAULT_VALUE=CLAY__SCROLL_CONTAINER_DEFAULT
#pragma region generated
Clay__ScrollContainerDataInternal Clay__ScrollContainerDataInternalArray_RemoveSwapback(Clay__ScrollContainerDataInternalArray *array, int32_t index) {
	if (Clay__Array_RangeCheck(index, array->length)) {
		array->length--;
		Clay__ScrollContainerDataInternal removed = array->internalArray[index];
		array->internalArray[index] = array->internalArray[array->length];
		return removed;
	}
	return CLAY__SCROLL_CONTAINER_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

CLAY__TYPEDEF(Clay__DebugElementData, struct {
    bool collision;
    bool collapsed;
});

Clay__DebugElementData CLAY__DEBUG_ELEMENT_DATA_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add,array_get TYPE=Clay__DebugElementData NAME=Clay__DebugElementDataArray DEFAULT_VALUE=&CLAY__DEBUG_ELEMENT_DATA_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__DebugElementDataArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay__DebugElementData *internalArray;
});
Clay__DebugElementDataArray Clay__DebugElementDataArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__DebugElementDataArray){.capacity = capacity, .length = 0, .internalArray = (Clay__DebugElementData *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__DebugElementData), CLAY__ALIGNMENT(Clay__DebugElementData), arena)};
}
Clay__DebugElementData *Clay__DebugElementDataArray_Add(Clay__DebugElementDataArray *array, Clay__DebugElementData item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__DEBUG_ELEMENT_DATA_DEFAULT;
}
Clay__DebugElementData *Clay__DebugElementDataArray_Get(Clay__DebugElementDataArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__DEBUG_ELEMENT_DATA_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

CLAY__TYPEDEF(Clay_LayoutElementHashMapItem, struct { // todo get this struct into a single cache line
    Clay_BoundingBox boundingBox;
    Clay_ElementId elementId;
    Clay_LayoutElement* layoutElement;
    void (*onHoverFunction)(Clay_ElementId elementId, Clay_PointerData pointerInfo, intptr_t userData);
    intptr_t hoverFunctionUserData;
    int32_t nextIndex;
    uint32_t generation;
    Clay__DebugElementData *debugData;
});

Clay_LayoutElementHashMapItem CLAY__LAYOUT_ELEMENT_HASH_MAP_ITEM_DEFAULT = { .layoutElement = &CLAY__LAYOUT_ELEMENT_DEFAULT };

// __GENERATED__ template array_define,array_allocate,array_get,array_add TYPE=Clay_LayoutElementHashMapItem NAME=Clay__LayoutElementHashMapItemArray DEFAULT_VALUE=&CLAY__LAYOUT_ELEMENT_HASH_MAP_ITEM_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__LayoutElementHashMapItemArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay_LayoutElementHashMapItem *internalArray;
});
Clay__LayoutElementHashMapItemArray Clay__LayoutElementHashMapItemArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__LayoutElementHashMapItemArray){.capacity = capacity, .length = 0, .internalArray = (Clay_LayoutElementHashMapItem *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_LayoutElementHashMapItem), CLAY__ALIGNMENT(Clay_LayoutElementHashMapItem), arena)};
}
Clay_LayoutElementHashMapItem *Clay__LayoutElementHashMapItemArray_Get(Clay__LayoutElementHashMapItemArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__LAYOUT_ELEMENT_HASH_MAP_ITEM_DEFAULT;
}
Clay_LayoutElementHashMapItem *Clay__LayoutElementHashMapItemArray_Add(Clay__LayoutElementHashMapItemArray *array, Clay_LayoutElementHashMapItem item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__LAYOUT_ELEMENT_HASH_MAP_ITEM_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

CLAY__TYPEDEF(Clay__MeasuredWord, struct {
    int32_t startOffset;
    int32_t length;
    float width;
    int32_t next;
});

Clay__MeasuredWord CLAY__MEASURED_WORD_DEFAULT = { .next = -1 };

// __GENERATED__ template array_define,array_allocate,array_get,array_set,array_add TYPE=Clay__MeasuredWord NAME=Clay__MeasuredWordArray DEFAULT_VALUE=&CLAY__MEASURED_WORD_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__MeasuredWordArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay__MeasuredWord *internalArray;
});
Clay__MeasuredWordArray Clay__MeasuredWordArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__MeasuredWordArray){.capacity = capacity, .length = 0, .internalArray = (Clay__MeasuredWord *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__MeasuredWord), CLAY__ALIGNMENT(Clay__MeasuredWord), arena)};
}
Clay__MeasuredWord *Clay__MeasuredWordArray_Get(Clay__MeasuredWordArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__MEASURED_WORD_DEFAULT;
}
void Clay__MeasuredWordArray_Set(Clay__MeasuredWordArray *array, int32_t index, Clay__MeasuredWord value) {
	if (Clay__Array_RangeCheck(index, array->capacity)) {
		array->internalArray[index] = value;
		array->length = index < array->length ? array->length : index + 1;
	}
}
Clay__MeasuredWord *Clay__MeasuredWordArray_Add(Clay__MeasuredWordArray *array, Clay__MeasuredWord item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__MEASURED_WORD_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

CLAY__TYPEDEF(Clay__MeasureTextCacheItem, struct {
    Clay_Dimensions unwrappedDimensions;
    int32_t measuredWordsStartIndex;
    bool containsNewlines;
    // Hash map data
    uint32_t id;
    int32_t nextIndex;
    uint32_t generation;
});

Clay__MeasureTextCacheItem CLAY__MEASURE_TEXT_CACHE_ITEM_DEFAULT = { .measuredWordsStartIndex = -1 };

// __GENERATED__ template array_define,array_allocate,array_get,array_add,array_set TYPE=Clay__MeasureTextCacheItem NAME=Clay__MeasureTextCacheItemArray DEFAULT_VALUE=&CLAY__MEASURE_TEXT_CACHE_ITEM_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__MeasureTextCacheItemArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay__MeasureTextCacheItem *internalArray;
});
Clay__MeasureTextCacheItemArray Clay__MeasureTextCacheItemArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__MeasureTextCacheItemArray){.capacity = capacity, .length = 0, .internalArray = (Clay__MeasureTextCacheItem *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__MeasureTextCacheItem), CLAY__ALIGNMENT(Clay__MeasureTextCacheItem), arena)};
}
Clay__MeasureTextCacheItem *Clay__MeasureTextCacheItemArray_Get(Clay__MeasureTextCacheItemArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__MEASURE_TEXT_CACHE_ITEM_DEFAULT;
}
Clay__MeasureTextCacheItem *Clay__MeasureTextCacheItemArray_Add(Clay__MeasureTextCacheItemArray *array, Clay__MeasureTextCacheItem item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__MEASURE_TEXT_CACHE_ITEM_DEFAULT;
}
void Clay__MeasureTextCacheItemArray_Set(Clay__MeasureTextCacheItemArray *array, int32_t index, Clay__MeasureTextCacheItem value) {
	if (Clay__Array_RangeCheck(index, array->capacity)) {
		array->internalArray[index] = value;
		array->length = index < array->length ? array->length : index + 1;
	}
}
#pragma endregion
// __GENERATED__ template

// __GENERATED__ template array_define,array_allocate,array_get_value,array_add_value,array_set,array_remove_swapback TYPE=int32_t NAME=Clay__int32_tArray DEFAULT_VALUE=-1
#pragma region generated
CLAY__TYPEDEF(Clay__int32_tArray, struct
{
	int32_t capacity;
	int32_t length;
	int32_t *internalArray;
});
Clay__int32_tArray Clay__int32_tArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__int32_tArray){.capacity = capacity, .length = 0, .internalArray = (int32_t *)Clay__Array_Allocate_Arena(capacity, sizeof(int32_t), CLAY__ALIGNMENT(int32_t), arena)};
}
int32_t Clay__int32_tArray_Get(Clay__int32_tArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? array->internalArray[index] : -1;
}
void Clay__int32_tArray_Add(Clay__int32_tArray *array, int32_t item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
	}
}
void Clay__int32_tArray_Set(Clay__int32_tArray *array, int32_t index, int32_t value) {
	if (Clay__Array_RangeCheck(index, array->capacity)) {
		array->internalArray[index] = value;
		array->length = index < array->length ? array->length : index + 1;
	}
}
int32_t Clay__int32_tArray_RemoveSwapback(Clay__int32_tArray *array, int32_t index) {
	if (Clay__Array_RangeCheck(index, array->length)) {
		array->length--;
		int32_t removed = array->internalArray[index];
		array->internalArray[index] = array->internalArray[array->length];
		return removed;
	}
	return -1;
}
#pragma endregion
// __GENERATED__ template

CLAY__TYPEDEF(Clay__LayoutElementTreeNode, struct {
    Clay_LayoutElement *layoutElement;
    Clay_Vector2 position;
    Clay_Vector2 nextChildOffset;
});

Clay__LayoutElementTreeNode CLAY__LAYOUT_ELEMENT_TREE_NODE_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add,array_get TYPE=Clay__LayoutElementTreeNode NAME=Clay__LayoutElementTreeNodeArray DEFAULT_VALUE=&CLAY__LAYOUT_ELEMENT_TREE_NODE_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__LayoutElementTreeNodeArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay__LayoutElementTreeNode *internalArray;
});
Clay__LayoutElementTreeNodeArray Clay__LayoutElementTreeNodeArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__LayoutElementTreeNodeArray){.capacity = capacity, .length = 0, .internalArray = (Clay__LayoutElementTreeNode *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__LayoutElementTreeNode), CLAY__ALIGNMENT(Clay__LayoutElementTreeNode), arena)};
}
Clay__LayoutElementTreeNode *Clay__LayoutElementTreeNodeArray_Add(Clay__LayoutElementTreeNodeArray *array, Clay__LayoutElementTreeNode item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__LAYOUT_ELEMENT_TREE_NODE_DEFAULT;
}
Clay__LayoutElementTreeNode *Clay__LayoutElementTreeNodeArray_Get(Clay__LayoutElementTreeNodeArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__LAYOUT_ELEMENT_TREE_NODE_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

CLAY__TYPEDEF(Clay__LayoutElementTreeRoot, struct {
    int32_t layoutElementIndex;
    uint32_t parentId; // This can be zero in the case of the root layout tree
    uint32_t clipElementId; // This can be zero if there is no clip element
    int32_t zIndex;
    Clay_Vector2 pointerOffset; // Only used when scroll containers are managed externally
});

Clay__LayoutElementTreeRoot CLAY__LAYOUT_ELEMENT_TREE_ROOT_DEFAULT = CLAY__DEFAULT_STRUCT;

// __GENERATED__ template array_define,array_allocate,array_add,array_get,array_set TYPE=Clay__LayoutElementTreeRoot NAME=Clay__LayoutElementTreeRootArray DEFAULT_VALUE=&CLAY__LAYOUT_ELEMENT_TREE_ROOT_DEFAULT
#pragma region generated
CLAY__TYPEDEF(Clay__LayoutElementTreeRootArray, struct
{
	int32_t capacity;
	int32_t length;
	Clay__LayoutElementTreeRoot *internalArray;
});
Clay__LayoutElementTreeRootArray Clay__LayoutElementTreeRootArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__LayoutElementTreeRootArray){.capacity = capacity, .length = 0, .internalArray = (Clay__LayoutElementTreeRoot *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__LayoutElementTreeRoot), CLAY__ALIGNMENT(Clay__LayoutElementTreeRoot), arena)};
}
Clay__LayoutElementTreeRoot *Clay__LayoutElementTreeRootArray_Add(Clay__LayoutElementTreeRootArray *array, Clay__LayoutElementTreeRoot item) {
	if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__LAYOUT_ELEMENT_TREE_ROOT_DEFAULT;
}
Clay__LayoutElementTreeRoot *Clay__LayoutElementTreeRootArray_Get(Clay__LayoutElementTreeRootArray *array, int32_t index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__LAYOUT_ELEMENT_TREE_ROOT_DEFAULT;
}
void Clay__LayoutElementTreeRootArray_Set(Clay__LayoutElementTreeRootArray *array, int32_t index, Clay__LayoutElementTreeRoot value) {
	if (Clay__Array_RangeCheck(index, array->capacity)) {
		array->internalArray[index] = value;
		array->length = index < array->length ? array->length : index + 1;
	}
}
#pragma endregion
// __GENERATED__ template

// __GENERATED__ template array_define,array_allocate TYPE=uint8_t NAME=Clay__CharArray DEFAULT_VALUE=0
#pragma region generated
CLAY__TYPEDEF(Clay__CharArray, struct
{
	int32_t capacity;
	int32_t length;
	uint8_t *internalArray;
});
Clay__CharArray Clay__CharArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    return CLAY__INIT(Clay__CharArray){.capacity = capacity, .length = 0, .internalArray = (uint8_t *)Clay__Array_Allocate_Arena(capacity, sizeof(uint8_t), CLAY__ALIGNMENT(uint8_t), arena)};
}
#pragma endregion
// __GENERATED__ template

struct Clay_Context {
    int32_t maxElementCount;
    int32_t maxMeasureTextCacheWordCount;
    bool warningsEnabled;
    Clay_ErrorHandler errorHandler;
    Clay_BooleanWarnings booleanWarnings;
    Clay__WarningArray warnings;

    Clay_PointerData pointerInfo;
    Clay_Dimensions layoutDimensions;
    Clay_ElementId dynamicElementIndexBaseHash;
    uint32_t dynamicElementIndex;
    bool debugModeEnabled;
    bool disableCulling;
    bool externalScrollHandlingEnabled;
    uint32_t debugSelectedElementId;
    uint32_t generation;
    uintptr_t arenaResetOffset;
    uintptr_t mesureTextUserData;
    uintptr_t queryScrollOffsetUserData;
    Clay_Arena internalArena;
    // Layout Elements / Render Commands
    Clay_LayoutElementArray layoutElements;
    Clay_RenderCommandArray renderCommands;
    Clay__int32_tArray openLayoutElementStack;
    Clay__int32_tArray layoutElementChildren;
    Clay__int32_tArray layoutElementChildrenBuffer;
    Clay__TextElementDataArray textElementData;
    Clay__LayoutElementPointerArray imageElementPointers;
    Clay__int32_tArray reusableElementIndexBuffer;
    Clay__int32_tArray layoutElementClipElementIds;
    // Configs
    Clay__LayoutConfigArray layoutConfigs;
    Clay__ElementConfigArray elementConfigBuffer;
    Clay__ElementConfigArray elementConfigs;
    Clay__RectangleElementConfigArray rectangleElementConfigs;
    Clay__TextElementConfigArray textElementConfigs;
    Clay__ImageElementConfigArray imageElementConfigs;
    Clay__FloatingElementConfigArray floatingElementConfigs;
    Clay__ScrollElementConfigArray scrollElementConfigs;
    Clay__CustomElementConfigArray customElementConfigs;
    Clay__BorderElementConfigArray borderElementConfigs;
    // Misc Data Structures
    Clay__StringArray layoutElementIdStrings;
    Clay__WrappedTextLineArray wrappedTextLines;
    Clay__LayoutElementTreeNodeArray layoutElementTreeNodeArray1;
    Clay__LayoutElementTreeRootArray layoutElementTreeRoots;
    Clay__LayoutElementHashMapItemArray layoutElementsHashMapInternal;
    Clay__int32_tArray layoutElementsHashMap;
    Clay__MeasureTextCacheItemArray measureTextHashMapInternal;
    Clay__int32_tArray measureTextHashMapInternalFreeList;
    Clay__int32_tArray measureTextHashMap;
    Clay__MeasuredWordArray measuredWords;
    Clay__int32_tArray measuredWordsFreeList;
    Clay__int32_tArray openClipElementStack;
    Clay__ElementIdArray pointerOverIds;
    Clay__ScrollContainerDataInternalArray scrollContainerDatas;
    Clay__BoolArray treeNodeVisited;
    Clay__CharArray dynamicStringData;
    Clay__DebugElementDataArray debugElementData;
};

struct Clay__AlignClay_Context {
    char c;
    Clay_Context x;
};
typedef struct {
    Clay_Context wrapped;
} Clay__Clay_ContextWrapper;

Clay_Context* Clay__Context_Allocate_Arena(Clay_Arena *arena) {
    uint32_t alignment = CLAY__ALIGNMENT(Clay_Context);
    size_t totalSizeBytes = sizeof(Clay_Context);
    uintptr_t nextAllocAddress = arena->nextAllocation + (uintptr_t)arena->memory;
    uintptr_t arenaOffsetAligned = nextAllocAddress + (alignment - (nextAllocAddress & alignment));
    arenaOffsetAligned -= (uintptr_t)arena->memory;
    if (arenaOffsetAligned + totalSizeBytes > arena->capacity)
    {
        return NULL;
    }
    arena->nextAllocation = arenaOffsetAligned + totalSizeBytes;
    return (Clay_Context*)((uintptr_t)arena->memory + arenaOffsetAligned);
}

Clay_String Clay__WriteStringToCharBuffer(Clay__CharArray *buffer, Clay_String string) {
    for (int32_t i = 0; i < string.length; i++) {
        buffer->internalArray[buffer->length + i] = string.chars[i];
    }
    buffer->length += string.length;
    return CLAY__INIT(Clay_String) { .length = string.length, .chars = (const char *)(buffer->internalArray + buffer->length - string.length) };
}

#ifdef CLAY_WASM
    __attribute__((import_module("clay"), import_name("measureTextFunction"))) Clay_Dimensions Clay__MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData);
    __attribute__((import_module("clay"), import_name("queryScrollOffsetFunction"))) Clay_Vector2 Clay__QueryScrollOffset(uint32_t elementId, uintptr_t userData);
#else
    Clay_Dimensions (*Clay__MeasureText)(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData);
    Clay_Vector2 (*Clay__QueryScrollOffset)(uint32_t elementId, uintptr_t userData);
#endif

Clay_LayoutElement* Clay__GetOpenLayoutElement(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    return Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_Get(&context->openLayoutElementStack, context->openLayoutElementStack.length - 1));
}

uint32_t Clay__GetParentElementId(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    return Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_Get(&context->openLayoutElementStack, context->openLayoutElementStack.length - 2))->id;
}

bool Clay__ElementHasConfig(Clay_LayoutElement *element, Clay__ElementConfigType type) {
    return (element->configsEnabled & type);
}

Clay_ElementConfigUnion Clay__FindElementConfigWithType(Clay_LayoutElement *element, Clay__ElementConfigType type) {
    for (int32_t i = 0; i < element->elementConfigs.length; i++) {
        Clay_ElementConfig *config = Clay__ElementConfigArraySlice_Get(&element->elementConfigs, i);
        if (config->type == type) {
            return config->config;
        }
    }
    return CLAY__INIT(Clay_ElementConfigUnion) { NULL };
}

Clay_ElementId Clay__HashNumber(const uint32_t offset, const uint32_t seed) {
    uint32_t hash = seed;
    hash += (offset + 48);
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return CLAY__INIT(Clay_ElementId) { .id = hash + 1, .offset = offset, .baseId = seed, .stringId = CLAY__STRING_DEFAULT }; // Reserve the hash result of zero as "null id"
}

Clay_ElementId Clay__HashString(Clay_String key, const uint32_t offset, const uint32_t seed) {
    uint32_t hash = 0;
    uint32_t base = seed;

    for (int32_t i = 0; i < key.length; i++) {
        base += key.chars[i];
        base += (base << 10);
        base ^= (base >> 6);
    }
    hash = base;
    hash += offset;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (hash << 3);
    base += (base << 3);
    hash ^= (hash >> 11);
    base ^= (base >> 11);
    hash += (hash << 15);
    base += (base << 15);
    return CLAY__INIT(Clay_ElementId) { .id = hash + 1, .offset = offset, .baseId = base + 1, .stringId = key }; // Reserve the hash result of zero as "null id"
}

Clay_ElementId Clay__Rehash(Clay_ElementId elementId, uint32_t number) {
    uint32_t id = elementId.baseId;
    id += number;
    id += (id << 10);
    id ^= (id >> 6);

    id += (id << 3);
    id ^= (id >> 11);
    id += (id << 15);
    return CLAY__INIT(Clay_ElementId) { .id = id, .offset = number, .baseId = elementId.baseId, .stringId = elementId.stringId };
}

uint32_t Clay__RehashWithNumber(uint32_t id, uint32_t number) {
    id += number;
    id += (id << 10);
    id ^= (id >> 6);

    id += (id << 3);
    id ^= (id >> 11);
    id += (id << 15);
    return id;
}

uint32_t Clay__HashTextWithConfig(Clay_String *text, Clay_TextElementConfig *config) {
    uint32_t hash = 0;
    uintptr_t pointerAsNumber = (uintptr_t)text->chars;

    hash += pointerAsNumber;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += text->length;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += config->fontId;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += config->fontSize;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += config->lineHeight;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += config->letterSpacing;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += config->wrapMode;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash + 1; // Reserve the hash result of zero as "null id"
}

Clay__MeasuredWord *Clay__AddMeasuredWord(Clay__MeasuredWord word, Clay__MeasuredWord *previousWord) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->measuredWordsFreeList.length > 0) {
        uint32_t newItemIndex = Clay__int32_tArray_Get(&context->measuredWordsFreeList, (int)context->measuredWordsFreeList.length - 1);
        context->measuredWordsFreeList.length--;
        Clay__MeasuredWordArray_Set(&context->measuredWords, (int)newItemIndex, word);
        previousWord->next = (int32_t)newItemIndex;
        return Clay__MeasuredWordArray_Get(&context->measuredWords, (int)newItemIndex);
    } else {
        previousWord->next = (int32_t)context->measuredWords.length;
        return Clay__MeasuredWordArray_Add(&context->measuredWords, word);
    }
}

Clay__MeasureTextCacheItem *Clay__MeasureTextCached(Clay_String *text, Clay_TextElementConfig *config) {
    Clay_Context* context = Clay_GetCurrentContext();
    #ifndef CLAY_WASM
    if (!Clay__MeasureText) {
        if (!context->booleanWarnings.textMeasurementFunctionNotSet) {
            context->booleanWarnings.textMeasurementFunctionNotSet = true;
            context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                    .errorType = CLAY_ERROR_TYPE_TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED,
                    .errorText = CLAY_STRING("Clay's internal MeasureText function is null. You may have forgotten to call Clay_SetMeasureTextFunction(), or passed a NULL function pointer by mistake."),
                    .userData = context->errorHandler.userData });
        }
        return &CLAY__MEASURE_TEXT_CACHE_ITEM_DEFAULT;
    }
    #endif
    uint32_t id = Clay__HashTextWithConfig(text, config);
    uint32_t hashBucket = id % (context->maxMeasureTextCacheWordCount / 32);
    int32_t elementIndexPrevious = 0;
    int32_t elementIndex = context->measureTextHashMap.internalArray[hashBucket];
    while (elementIndex != 0) {
        Clay__MeasureTextCacheItem *hashEntry = Clay__MeasureTextCacheItemArray_Get(&context->measureTextHashMapInternal, elementIndex);
        if (hashEntry->id == id) {
            hashEntry->generation = context->generation;
            return hashEntry;
        }
        // This element hasn't been seen in a few frames, delete the hash map item
        if (context->generation - hashEntry->generation > 2) {
            // Add all the measured words that were included in this measurement to the freelist
            int32_t nextWordIndex = hashEntry->measuredWordsStartIndex;
            while (nextWordIndex != -1) {
                Clay__MeasuredWord *measuredWord = Clay__MeasuredWordArray_Get(&context->measuredWords, nextWordIndex);
                Clay__int32_tArray_Add(&context->measuredWordsFreeList, nextWordIndex);
                nextWordIndex = measuredWord->next;
            }

            int32_t nextIndex = hashEntry->nextIndex;
            Clay__MeasureTextCacheItemArray_Set(&context->measureTextHashMapInternal, elementIndex, CLAY__INIT(Clay__MeasureTextCacheItem) { .measuredWordsStartIndex = -1 });
            Clay__int32_tArray_Add(&context->measureTextHashMapInternalFreeList, elementIndex);
            if (elementIndexPrevious == 0) {
                context->measureTextHashMap.internalArray[hashBucket] = nextIndex;
            } else {
                Clay__MeasureTextCacheItem *previousHashEntry = Clay__MeasureTextCacheItemArray_Get(&context->measureTextHashMapInternal, elementIndexPrevious);
                previousHashEntry->nextIndex = nextIndex;
            }
            elementIndex = nextIndex;
        } else {
            elementIndexPrevious = elementIndex;
            elementIndex = hashEntry->nextIndex;
        }
    }

    int32_t newItemIndex = 0;
    Clay__MeasureTextCacheItem newCacheItem = { .measuredWordsStartIndex = -1, .id = id, .generation = context->generation };
    Clay__MeasureTextCacheItem *measured = NULL;
    if (context->measureTextHashMapInternalFreeList.length > 0) {
        newItemIndex = Clay__int32_tArray_Get(&context->measureTextHashMapInternalFreeList, context->measureTextHashMapInternalFreeList.length - 1);
        context->measureTextHashMapInternalFreeList.length--;
        Clay__MeasureTextCacheItemArray_Set(&context->measureTextHashMapInternal, newItemIndex, newCacheItem);
        measured = Clay__MeasureTextCacheItemArray_Get(&context->measureTextHashMapInternal, newItemIndex);
    } else {
        if (context->measureTextHashMapInternal.length == context->measureTextHashMapInternal.capacity - 1) {
            if (context->booleanWarnings.maxTextMeasureCacheExceeded) {
                context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                        .errorType = CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED,
                        .errorText = CLAY_STRING("Clay ran out of capacity while attempting to measure text elements. Try using Clay_SetMaxElementCount() with a higher value."),
                        .userData = context->errorHandler.userData });
                context->booleanWarnings.maxTextMeasureCacheExceeded = true;
            }
            return &CLAY__MEASURE_TEXT_CACHE_ITEM_DEFAULT;
        }
        measured = Clay__MeasureTextCacheItemArray_Add(&context->measureTextHashMapInternal, newCacheItem);
        newItemIndex = context->measureTextHashMapInternal.length - 1;
    }

    int32_t start = 0;
    int32_t end = 0;
    float lineWidth = 0;
    float measuredWidth = 0;
    float measuredHeight = 0;
    float spaceWidth = Clay__MeasureText(CLAY__INIT(Clay_StringSlice) { .length = 1, .chars = CLAY__SPACECHAR.chars, .baseChars = CLAY__SPACECHAR.chars }, config, context->mesureTextUserData).width;
    Clay__MeasuredWord tempWord = { .next = -1 };
    Clay__MeasuredWord *previousWord = &tempWord;
    while (end < text->length) {
        if (context->measuredWords.length == context->measuredWords.capacity - 1) {
            if (!context->booleanWarnings.maxTextMeasureCacheExceeded) {
                context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                    .errorType = CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED,
                    .errorText = CLAY_STRING("Clay has run out of space in it's internal text measurement cache. Try using Clay_SetMaxMeasureTextCacheWordCount() (default 16384, with 1 unit storing 1 measured word)."),
                    .userData = context->errorHandler.userData });
                context->booleanWarnings.maxTextMeasureCacheExceeded = true;
            }
            return &CLAY__MEASURE_TEXT_CACHE_ITEM_DEFAULT;
        }
        char current = text->chars[end];
        if (current == ' ' || current == '\n') {
            int32_t length = end - start;
            Clay_Dimensions dimensions = Clay__MeasureText(CLAY__INIT(Clay_StringSlice) { .length = length, .chars = &text->chars[start], .baseChars = text->chars }, config, context->mesureTextUserData);
            measuredHeight = CLAY__MAX(measuredHeight, dimensions.height);
            if (current == ' ') {
                dimensions.width += spaceWidth;
                previousWord = Clay__AddMeasuredWord(CLAY__INIT(Clay__MeasuredWord) { .startOffset = start, .length = length + 1, .width = dimensions.width, .next = -1 }, previousWord);
                lineWidth += dimensions.width;
            }
            if (current == '\n') {
                if (length > 0) {
                    previousWord = Clay__AddMeasuredWord(CLAY__INIT(Clay__MeasuredWord) { .startOffset = start, .length = length, .width = dimensions.width, .next = -1 }, previousWord);
                }
                previousWord = Clay__AddMeasuredWord(CLAY__INIT(Clay__MeasuredWord) { .startOffset = end + 1, .length = 0, .width = 0, .next = -1 }, previousWord);
                lineWidth += dimensions.width;
                measuredWidth = CLAY__MAX(lineWidth, measuredWidth);
                measured->containsNewlines = true;
                lineWidth = 0;
            }
            start = end + 1;
        }
        end++;
    }
    if (end - start > 0) {
        Clay_Dimensions dimensions = Clay__MeasureText(CLAY__INIT(Clay_StringSlice) { .length = end - start, .chars = &text->chars[start], .baseChars = text->chars }, config, context->mesureTextUserData);
        Clay__AddMeasuredWord(CLAY__INIT(Clay__MeasuredWord) { .startOffset = start, .length = end - start, .width = dimensions.width, .next = -1 }, previousWord);
        lineWidth += dimensions.width;
        measuredHeight = CLAY__MAX(measuredHeight, dimensions.height);
    }
    measuredWidth = CLAY__MAX(lineWidth, measuredWidth);

    measured->measuredWordsStartIndex = tempWord.next;
    measured->unwrappedDimensions.width = measuredWidth;
    measured->unwrappedDimensions.height = measuredHeight;

    if (elementIndexPrevious != 0) {
        Clay__MeasureTextCacheItemArray_Get(&context->measureTextHashMapInternal, elementIndexPrevious)->nextIndex = newItemIndex;
    } else {
        context->measureTextHashMap.internalArray[hashBucket] = newItemIndex;
    }
    return measured;
}

bool Clay__PointIsInsideRect(Clay_Vector2 point, Clay_BoundingBox rect) {
    return point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height;
}

Clay_LayoutElementHashMapItem* Clay__AddHashMapItem(Clay_ElementId elementId, Clay_LayoutElement* layoutElement) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->layoutElementsHashMapInternal.length == context->layoutElementsHashMapInternal.capacity - 1) {
        return NULL;
    }
    Clay_LayoutElementHashMapItem item = { .elementId = elementId, .layoutElement = layoutElement, .nextIndex = -1, .generation = context->generation + 1 };
    uint32_t hashBucket = elementId.id % context->layoutElementsHashMap.capacity;
    int32_t hashItemPrevious = -1;
    int32_t hashItemIndex = context->layoutElementsHashMap.internalArray[hashBucket];
    while (hashItemIndex != -1) { // Just replace collision, not a big deal - leave it up to the end user
        Clay_LayoutElementHashMapItem *hashItem = Clay__LayoutElementHashMapItemArray_Get(&context->layoutElementsHashMapInternal, hashItemIndex);
        if (hashItem->elementId.id == elementId.id) { // Collision - resolve based on generation
            item.nextIndex = hashItem->nextIndex;
            if (hashItem->generation <= context->generation) { // First collision - assume this is the "same" element
                hashItem->generation = context->generation + 1;
                hashItem->layoutElement = layoutElement;
                hashItem->debugData->collision = false;
            } else { // Multiple collisions this frame - two elements have the same ID
                context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                    .errorType = CLAY_ERROR_TYPE_DUPLICATE_ID,
                    .errorText = CLAY_STRING("An element with this ID was already previously declared during this layout."),
                    .userData = context->errorHandler.userData });
                if (context->debugModeEnabled) {
                    hashItem->debugData->collision = true;
                }
            }
            return hashItem;
        }
        hashItemPrevious = hashItemIndex;
        hashItemIndex = hashItem->nextIndex;
    }
    Clay_LayoutElementHashMapItem *hashItem = Clay__LayoutElementHashMapItemArray_Add(&context->layoutElementsHashMapInternal, item);
    hashItem->debugData = Clay__DebugElementDataArray_Add(&context->debugElementData, CLAY__INIT(Clay__DebugElementData) CLAY__DEFAULT_STRUCT);
    if (hashItemPrevious != -1) {
        Clay__LayoutElementHashMapItemArray_Get(&context->layoutElementsHashMapInternal, hashItemPrevious)->nextIndex = (int32_t)context->layoutElementsHashMapInternal.length - 1;
    } else {
        context->layoutElementsHashMap.internalArray[hashBucket] = (int32_t)context->layoutElementsHashMapInternal.length - 1;
    }
    return hashItem;
}

Clay_LayoutElementHashMapItem *Clay__GetHashMapItem(uint32_t id) {
    Clay_Context* context = Clay_GetCurrentContext();
    uint32_t hashBucket = id % context->layoutElementsHashMap.capacity;
    int32_t elementIndex = context->layoutElementsHashMap.internalArray[hashBucket];
    while (elementIndex != -1) {
        Clay_LayoutElementHashMapItem *hashEntry = Clay__LayoutElementHashMapItemArray_Get(&context->layoutElementsHashMapInternal, elementIndex);
        if (hashEntry->elementId.id == id) {
            return hashEntry;
        }
        elementIndex = hashEntry->nextIndex;
    }
    return &CLAY__LAYOUT_ELEMENT_HASH_MAP_ITEM_DEFAULT;
}

void Clay__GenerateIdForAnonymousElement(Clay_LayoutElement *openLayoutElement) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay_LayoutElement *parentElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_Get(&context->openLayoutElementStack, context->openLayoutElementStack.length - 2));
    Clay_ElementId elementId = Clay__HashNumber(parentElement->childrenOrTextContent.children.length, parentElement->id);
    openLayoutElement->id = elementId.id;
    Clay__AddHashMapItem(elementId, openLayoutElement);
    Clay__StringArray_Add(&context->layoutElementIdStrings, elementId.stringId);
}

void Clay__ElementPostConfiguration(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    // ID
    if (openLayoutElement->id == 0) {
        Clay__GenerateIdForAnonymousElement(openLayoutElement);
    }
    // Layout Config
    if (!openLayoutElement->layoutConfig) {
        openLayoutElement->layoutConfig = &CLAY_LAYOUT_DEFAULT;
    }

    // Loop through element configs and handle special cases
    openLayoutElement->elementConfigs.internalArray = &context->elementConfigs.internalArray[context->elementConfigs.length];
    for (int32_t elementConfigIndex = 0; elementConfigIndex < openLayoutElement->elementConfigs.length; elementConfigIndex++) {
        Clay_ElementConfig *config = Clay__ElementConfigArray_Add(&context->elementConfigs, *Clay__ElementConfigArray_Get(&context->elementConfigBuffer, context->elementConfigBuffer.length - openLayoutElement->elementConfigs.length + elementConfigIndex));
        openLayoutElement->configsEnabled |= config->type;
        switch (config->type) {
            case CLAY__ELEMENT_CONFIG_TYPE_RECTANGLE:
            case CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER: break;
            case CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER: {
                Clay_FloatingElementConfig *floatingConfig = config->config.floatingElementConfig;
                // This looks dodgy but because of the auto generated root element the depth of the tree will always be at least 2 here
                Clay_LayoutElement *hierarchicalParent = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_Get(&context->openLayoutElementStack, context->openLayoutElementStack.length - 2));
                if (!hierarchicalParent) {
                    break;
                }
                uint32_t clipElementId = 0;
                if (floatingConfig->parentId == 0) {
                    // If no parent id was specified, attach to the elements direct hierarchical parent
                    Clay_FloatingElementConfig newConfig = *floatingConfig;
                    newConfig.parentId = hierarchicalParent->id;
                    floatingConfig = Clay__FloatingElementConfigArray_Add(&context->floatingElementConfigs, newConfig);
                    config->config.floatingElementConfig = floatingConfig;
                    if (context->openClipElementStack.length > 0) {
                        clipElementId = Clay__int32_tArray_Get(&context->openClipElementStack, (int)context->openClipElementStack.length - 1);
                    }
                } else {
                    Clay_LayoutElementHashMapItem *parentItem = Clay__GetHashMapItem(floatingConfig->parentId);
                    if (!parentItem) {
                        context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                            .errorType = CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND,
                            .errorText = CLAY_STRING("A floating element was declared with a parentId, but no element with that ID was found."),
                            .userData = context->errorHandler.userData });
                    } else {
                        clipElementId = Clay__int32_tArray_Get(&context->layoutElementClipElementIds, parentItem->layoutElement - context->layoutElements.internalArray);
                    }
                }
                Clay__LayoutElementTreeRootArray_Add(&context->layoutElementTreeRoots, CLAY__INIT(Clay__LayoutElementTreeRoot) {
                    .layoutElementIndex = Clay__int32_tArray_Get(&context->openLayoutElementStack, context->openLayoutElementStack.length - 1),
                    .parentId = floatingConfig->parentId,
                    .clipElementId = clipElementId,
                    .zIndex = floatingConfig->zIndex,
                });
                break;
            }
            case CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER: {
                Clay__int32_tArray_Add(&context->openClipElementStack, (int)openLayoutElement->id);
                // Retrieve or create cached data to track scroll position across frames
                Clay__ScrollContainerDataInternal *scrollOffset = CLAY__NULL;
                for (int32_t i = 0; i < context->scrollContainerDatas.length; i++) {
                    Clay__ScrollContainerDataInternal *mapping = Clay__ScrollContainerDataInternalArray_Get(&context->scrollContainerDatas, i);
                    if (openLayoutElement->id == mapping->elementId) {
                        scrollOffset = mapping;
                        scrollOffset->layoutElement = openLayoutElement;
                        scrollOffset->openThisFrame = true;
                    }
                }
                if (!scrollOffset) {
                    scrollOffset = Clay__ScrollContainerDataInternalArray_Add(&context->scrollContainerDatas, CLAY__INIT(Clay__ScrollContainerDataInternal){.layoutElement = openLayoutElement, .scrollOrigin = {-1,-1}, .elementId = openLayoutElement->id, .openThisFrame = true});
                }
                if (context->externalScrollHandlingEnabled) {
                    scrollOffset->scrollPosition = Clay__QueryScrollOffset(scrollOffset->elementId, context->queryScrollOffsetUserData);
                }
                break;
            }
            case CLAY__ELEMENT_CONFIG_TYPE_CUSTOM: break;
            case CLAY__ELEMENT_CONFIG_TYPE_IMAGE: {
                Clay__LayoutElementPointerArray_Add(&context->imageElementPointers, openLayoutElement);
                break;
            }
            case CLAY__ELEMENT_CONFIG_TYPE_TEXT:
            default: break;
        }
    }
    context->elementConfigBuffer.length -= openLayoutElement->elementConfigs.length;
}

void Clay__CloseElement(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    Clay_LayoutConfig *layoutConfig = openLayoutElement->layoutConfig;
    bool elementHasScrollHorizontal = false;
    bool elementHasScrollVertical = false;
    if (Clay__ElementHasConfig(openLayoutElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER)) {
        Clay_ScrollElementConfig *scrollConfig = Clay__FindElementConfigWithType(openLayoutElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER).scrollElementConfig;
        elementHasScrollHorizontal = scrollConfig->horizontal;
        elementHasScrollVertical = scrollConfig->vertical;
        context->openClipElementStack.length--;
    }

    // Attach children to the current open element
    openLayoutElement->childrenOrTextContent.children.elements = &context->layoutElementChildren.internalArray[context->layoutElementChildren.length];
    if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
        openLayoutElement->dimensions.width = (float)(layoutConfig->padding.left + layoutConfig->padding.right);
        for (int32_t i = 0; i < openLayoutElement->childrenOrTextContent.children.length; i++) {
            int32_t childIndex = Clay__int32_tArray_Get(&context->layoutElementChildrenBuffer, (int)context->layoutElementChildrenBuffer.length - openLayoutElement->childrenOrTextContent.children.length + i);
            Clay_LayoutElement *child = Clay_LayoutElementArray_Get(&context->layoutElements, childIndex);
            openLayoutElement->dimensions.width += child->dimensions.width;
            openLayoutElement->dimensions.height = CLAY__MAX(openLayoutElement->dimensions.height, child->dimensions.height + layoutConfig->padding.top + layoutConfig->padding.bottom);
            // Minimum size of child elements doesn't matter to scroll containers as they can shrink and hide their contents
            if (!elementHasScrollHorizontal) {
                openLayoutElement->minDimensions.width += child->minDimensions.width;
            }
            if (!elementHasScrollVertical) {
                openLayoutElement->minDimensions.height = CLAY__MAX(openLayoutElement->minDimensions.height, child->minDimensions.height + layoutConfig->padding.top + layoutConfig->padding.bottom);
            }
            Clay__int32_tArray_Add(&context->layoutElementChildren, childIndex);
        }
        float childGap = (float)(CLAY__MAX(openLayoutElement->childrenOrTextContent.children.length - 1, 0) * layoutConfig->childGap);
        openLayoutElement->dimensions.width += childGap; // TODO this is technically a bug with childgap and scroll containers
        openLayoutElement->minDimensions.width += childGap;
    }
    else if (layoutConfig->layoutDirection == CLAY_TOP_TO_BOTTOM) {
        openLayoutElement->dimensions.height = (float)(layoutConfig->padding.top + layoutConfig->padding.bottom);
        for (int32_t i = 0; i < openLayoutElement->childrenOrTextContent.children.length; i++) {
            int32_t childIndex = Clay__int32_tArray_Get(&context->layoutElementChildrenBuffer, (int)context->layoutElementChildrenBuffer.length - openLayoutElement->childrenOrTextContent.children.length + i);
            Clay_LayoutElement *child = Clay_LayoutElementArray_Get(&context->layoutElements, childIndex);
            openLayoutElement->dimensions.height += child->dimensions.height;
            openLayoutElement->dimensions.width = CLAY__MAX(openLayoutElement->dimensions.width, child->dimensions.width + layoutConfig->padding.left + layoutConfig->padding.right);
            // Minimum size of child elements doesn't matter to scroll containers as they can shrink and hide their contents
            if (!elementHasScrollVertical) {
                openLayoutElement->minDimensions.height += child->minDimensions.height;
            }
            if (!elementHasScrollHorizontal) {
                openLayoutElement->minDimensions.width = CLAY__MAX(openLayoutElement->minDimensions.width, child->minDimensions.width + layoutConfig->padding.left + layoutConfig->padding.right);
            }
            Clay__int32_tArray_Add(&context->layoutElementChildren, childIndex);
        }
        float childGap = (float)(CLAY__MAX(openLayoutElement->childrenOrTextContent.children.length - 1, 0) * layoutConfig->childGap);
        openLayoutElement->dimensions.height += childGap; // TODO this is technically a bug with childgap and scroll containers
        openLayoutElement->minDimensions.height += childGap;
    }

    context->layoutElementChildrenBuffer.length -= openLayoutElement->childrenOrTextContent.children.length;

    // Clamp element min and max width to the values configured in the layout
    if (layoutConfig->sizing.width.type != CLAY__SIZING_TYPE_PERCENT) {
        if (layoutConfig->sizing.width.size.minMax.max <= 0) { // Set the max size if the user didn't specify, makes calculations easier
            layoutConfig->sizing.width.size.minMax.max = CLAY__MAXFLOAT;
        }
        openLayoutElement->dimensions.width = CLAY__MIN(CLAY__MAX(openLayoutElement->dimensions.width, layoutConfig->sizing.width.size.minMax.min), layoutConfig->sizing.width.size.minMax.max);
        openLayoutElement->minDimensions.width = CLAY__MIN(CLAY__MAX(openLayoutElement->minDimensions.width, layoutConfig->sizing.width.size.minMax.min), layoutConfig->sizing.width.size.minMax.max);
    } else {
        openLayoutElement->dimensions.width = 0;
    }

    // Clamp element min and max height to the values configured in the layout
    if (layoutConfig->sizing.height.type != CLAY__SIZING_TYPE_PERCENT) {
        if (layoutConfig->sizing.height.size.minMax.max <= 0) { // Set the max size if the user didn't specify, makes calculations easier
            layoutConfig->sizing.height.size.minMax.max = CLAY__MAXFLOAT;
        }
        openLayoutElement->dimensions.height = CLAY__MIN(CLAY__MAX(openLayoutElement->dimensions.height, layoutConfig->sizing.height.size.minMax.min), layoutConfig->sizing.height.size.minMax.max);
        openLayoutElement->minDimensions.height = CLAY__MIN(CLAY__MAX(openLayoutElement->minDimensions.height, layoutConfig->sizing.height.size.minMax.min), layoutConfig->sizing.height.size.minMax.max);
    } else {
        openLayoutElement->dimensions.height = 0;
    }

    bool elementIsFloating = Clay__ElementHasConfig(openLayoutElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER);

    // Close the currently open element
    int32_t closingElementIndex = Clay__int32_tArray_RemoveSwapback(&context->openLayoutElementStack, (int)context->openLayoutElementStack.length - 1);
    openLayoutElement = Clay__GetOpenLayoutElement();

    if (!elementIsFloating && context->openLayoutElementStack.length > 1) {
        openLayoutElement->childrenOrTextContent.children.length++;
        Clay__int32_tArray_Add(&context->layoutElementChildrenBuffer, closingElementIndex);
    }
}

void Clay__OpenElement(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->layoutElements.length == context->layoutElements.capacity - 1 || context->booleanWarnings.maxElementsExceeded) {
        context->booleanWarnings.maxElementsExceeded = true;
        return;
    }
    Clay_LayoutElement layoutElement = CLAY__DEFAULT_STRUCT;
    Clay_LayoutElementArray_Add(&context->layoutElements, layoutElement);
    Clay__int32_tArray_Add(&context->openLayoutElementStack, context->layoutElements.length - 1);
    if (context->openClipElementStack.length > 0) {
        Clay__int32_tArray_Set(&context->layoutElementClipElementIds, context->layoutElements.length - 1, Clay__int32_tArray_Get(&context->openClipElementStack, (int)context->openClipElementStack.length - 1));
    } else {
        Clay__int32_tArray_Set(&context->layoutElementClipElementIds, context->layoutElements.length - 1, 0);
    }
}

void Clay__OpenTextElement(Clay_String text, Clay_TextElementConfig *textConfig) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->layoutElements.length == context->layoutElements.capacity - 1 || context->booleanWarnings.maxElementsExceeded) {
        context->booleanWarnings.maxElementsExceeded = true;
        return;
    }
    Clay_LayoutElement *parentElement = Clay__GetOpenLayoutElement();
    parentElement->childrenOrTextContent.children.length++;

    Clay__OpenElement();
    Clay_LayoutElement * openLayoutElement = Clay__GetOpenLayoutElement();
    Clay__int32_tArray_Add(&context->layoutElementChildrenBuffer, context->layoutElements.length - 1);
    Clay__MeasureTextCacheItem *textMeasured = Clay__MeasureTextCached(&text, textConfig);
    Clay_ElementId elementId = Clay__HashString(CLAY_STRING("Text"), parentElement->childrenOrTextContent.children.length, parentElement->id);
    openLayoutElement->id = elementId.id;
    Clay__AddHashMapItem(elementId, openLayoutElement);
    Clay__StringArray_Add(&context->layoutElementIdStrings, elementId.stringId);
    Clay_Dimensions textDimensions = { .width = textMeasured->unwrappedDimensions.width, .height = textConfig->lineHeight > 0 ? (float)textConfig->lineHeight : textMeasured->unwrappedDimensions.height };
    openLayoutElement->dimensions = textDimensions;
    openLayoutElement->minDimensions = CLAY__INIT(Clay_Dimensions) { .width = textMeasured->unwrappedDimensions.height, .height = textDimensions.height }; // TODO not sure this is the best way to decide min width for text
    openLayoutElement->childrenOrTextContent.textElementData = Clay__TextElementDataArray_Add(&context->textElementData, CLAY__INIT(Clay__TextElementData) { .text = text, .preferredDimensions = textMeasured->unwrappedDimensions, .elementIndex = context->layoutElements.length - 1 });
    openLayoutElement->elementConfigs = CLAY__INIT(Clay__ElementConfigArraySlice) {
        .length = 1,
        .internalArray = Clay__ElementConfigArray_Add(&context->elementConfigs, CLAY__INIT(Clay_ElementConfig) { .type = CLAY__ELEMENT_CONFIG_TYPE_TEXT, .config = { .textElementConfig = textConfig }})
    };
    openLayoutElement->configsEnabled |= CLAY__ELEMENT_CONFIG_TYPE_TEXT;
    openLayoutElement->layoutConfig = &CLAY_LAYOUT_DEFAULT;
    // Close the currently open element
    Clay__int32_tArray_RemoveSwapback(&context->openLayoutElementStack, (int)context->openLayoutElementStack.length - 1);
}

void Clay__InitializeEphemeralMemory(Clay_Context* context) {
    int32_t maxElementCount = context->maxElementCount;
    // Ephemeral Memory - reset every frame
    Clay_Arena *arena = &context->internalArena;
    arena->nextAllocation = context->arenaResetOffset;

    context->layoutElementChildrenBuffer = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElements = Clay_LayoutElementArray_Allocate_Arena(maxElementCount, arena);
    context->warnings = Clay__WarningArray_Allocate_Arena(100, arena);

    context->layoutConfigs = Clay__LayoutConfigArray_Allocate_Arena(maxElementCount, arena);
    context->elementConfigBuffer = Clay__ElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->elementConfigs = Clay__ElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->rectangleElementConfigs = Clay__RectangleElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->textElementConfigs = Clay__TextElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->imageElementConfigs = Clay__ImageElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->floatingElementConfigs = Clay__FloatingElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->scrollElementConfigs = Clay__ScrollElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->customElementConfigs = Clay__CustomElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->borderElementConfigs = Clay__BorderElementConfigArray_Allocate_Arena(maxElementCount, arena);

    context->layoutElementIdStrings = Clay__StringArray_Allocate_Arena(maxElementCount, arena);
    context->wrappedTextLines = Clay__WrappedTextLineArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementTreeNodeArray1 = Clay__LayoutElementTreeNodeArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementTreeRoots = Clay__LayoutElementTreeRootArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementChildren = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->openLayoutElementStack = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->textElementData = Clay__TextElementDataArray_Allocate_Arena(maxElementCount, arena);
    context->imageElementPointers = Clay__LayoutElementPointerArray_Allocate_Arena(maxElementCount, arena);
    context->renderCommands = Clay_RenderCommandArray_Allocate_Arena(maxElementCount, arena);
    context->treeNodeVisited = Clay__BoolArray_Allocate_Arena(maxElementCount, arena);
    context->treeNodeVisited.length = context->treeNodeVisited.capacity; // This array is accessed directly rather than behaving as a list
    context->openClipElementStack = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->reusableElementIndexBuffer = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementClipElementIds = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->dynamicStringData = Clay__CharArray_Allocate_Arena(maxElementCount, arena);
}

void Clay__InitializePersistentMemory(Clay_Context* context) {
    // Persistent memory - initialized once and not reset
    int32_t maxElementCount = context->maxElementCount;
    int32_t maxMeasureTextCacheWordCount = context->maxMeasureTextCacheWordCount;
    Clay_Arena *arena = &context->internalArena;
    
    context->scrollContainerDatas = Clay__ScrollContainerDataInternalArray_Allocate_Arena(10, arena);
    context->layoutElementsHashMapInternal = Clay__LayoutElementHashMapItemArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementsHashMap = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->measureTextHashMapInternal = Clay__MeasureTextCacheItemArray_Allocate_Arena(maxElementCount, arena);
    context->measureTextHashMapInternalFreeList = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->measuredWordsFreeList = Clay__int32_tArray_Allocate_Arena(maxMeasureTextCacheWordCount, arena);
    context->measureTextHashMap = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->measuredWords = Clay__MeasuredWordArray_Allocate_Arena(maxMeasureTextCacheWordCount, arena);
    context->pointerOverIds = Clay__ElementIdArray_Allocate_Arena(maxElementCount, arena);
    context->debugElementData = Clay__DebugElementDataArray_Allocate_Arena(maxElementCount, arena);
    context->arenaResetOffset = arena->nextAllocation;
}


void Clay__CompressChildrenAlongAxis(bool xAxis, float totalSizeToDistribute, Clay__int32_tArray resizableContainerBuffer) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__int32_tArray largestContainers = context->openClipElementStack;

    while (totalSizeToDistribute > 0.1) {
        largestContainers.length = 0;
        float largestSize = 0;
        float targetSize = 0;
        for (int32_t i = 0; i < resizableContainerBuffer.length; ++i) {
            Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_Get(&resizableContainerBuffer, i));
            if (!xAxis && Clay__ElementHasConfig(childElement, CLAY__ELEMENT_CONFIG_TYPE_IMAGE)) {
                continue;
            }
            float childSize = xAxis ? childElement->dimensions.width : childElement->dimensions.height;
            if ((childSize - largestSize) < 0.1 && (childSize - largestSize) > -0.1) {
                Clay__int32_tArray_Add(&largestContainers, Clay__int32_tArray_Get(&resizableContainerBuffer, i));
            } else if (childSize > largestSize) {
                targetSize = largestSize;
                largestSize = childSize;
                largestContainers.length = 0;
                Clay__int32_tArray_Add(&largestContainers, Clay__int32_tArray_Get(&resizableContainerBuffer, i));
            }
            else if (childSize > targetSize) {
                targetSize = childSize;
            }
        }

        if (largestContainers.length == 0) {
            return;
        }

        targetSize = CLAY__MAX(targetSize, (largestSize * largestContainers.length) - totalSizeToDistribute) / largestContainers.length;

        for (int32_t childOffset = 0; childOffset < largestContainers.length; childOffset++) {
            int32_t childIndex = Clay__int32_tArray_Get(&largestContainers, childOffset);
            Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, childIndex);
            float *childSize = xAxis ? &childElement->dimensions.width : &childElement->dimensions.height;
            float childMinSize = xAxis ? childElement->minDimensions.width : childElement->minDimensions.height;
            float oldChildSize = *childSize;
            *childSize = CLAY__MAX(childMinSize, targetSize);
            totalSizeToDistribute -= (oldChildSize - *childSize);
            if (*childSize == childMinSize) {
                for (int32_t i = 0; i < resizableContainerBuffer.length; i++) {
                    if (Clay__int32_tArray_Get(&resizableContainerBuffer, i) == childIndex) {
                        Clay__int32_tArray_RemoveSwapback(&resizableContainerBuffer, i);
                        break;
                    }
                }
            }
        }
    }
}

void Clay__SizeContainersAlongAxis(bool xAxis) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__int32_tArray bfsBuffer = context->layoutElementChildrenBuffer;
    Clay__int32_tArray resizableContainerBuffer = context->openLayoutElementStack;
    for (int32_t rootIndex = 0; rootIndex < context->layoutElementTreeRoots.length; ++rootIndex) {
        bfsBuffer.length = 0;
        Clay__LayoutElementTreeRoot *root = Clay__LayoutElementTreeRootArray_Get(&context->layoutElementTreeRoots, rootIndex);
        Clay_LayoutElement *rootElement = Clay_LayoutElementArray_Get(&context->layoutElements, (int)root->layoutElementIndex);
        Clay__int32_tArray_Add(&bfsBuffer, (int32_t)root->layoutElementIndex);

        // Size floating containers to their parents
        if (Clay__ElementHasConfig(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER)) {
            Clay_FloatingElementConfig *floatingElementConfig = Clay__FindElementConfigWithType(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER).floatingElementConfig;
            Clay_LayoutElementHashMapItem *parentItem = Clay__GetHashMapItem(floatingElementConfig->parentId);
            if (parentItem) {
                Clay_LayoutElement *parentLayoutElement = parentItem->layoutElement;
                if (rootElement->layoutConfig->sizing.width.type == CLAY__SIZING_TYPE_GROW) {
                    rootElement->dimensions.width = parentLayoutElement->dimensions.width;
                }
                if (rootElement->layoutConfig->sizing.height.type == CLAY__SIZING_TYPE_GROW) {
                    rootElement->dimensions.height = parentLayoutElement->dimensions.height;
                }
            }
        }

        rootElement->dimensions.width = CLAY__MIN(CLAY__MAX(rootElement->dimensions.width, rootElement->layoutConfig->sizing.width.size.minMax.min), rootElement->layoutConfig->sizing.width.size.minMax.max);
        rootElement->dimensions.height = CLAY__MIN(CLAY__MAX(rootElement->dimensions.height, rootElement->layoutConfig->sizing.height.size.minMax.min), rootElement->layoutConfig->sizing.height.size.minMax.max);

        for (int32_t i = 0; i < bfsBuffer.length; ++i) {
            int32_t parentIndex = Clay__int32_tArray_Get(&bfsBuffer, i);
            Clay_LayoutElement *parent = Clay_LayoutElementArray_Get(&context->layoutElements, parentIndex);
            Clay_LayoutConfig *parentStyleConfig = parent->layoutConfig;
            int32_t growContainerCount = 0;
            float parentSize = xAxis ? parent->dimensions.width : parent->dimensions.height;
            float parentPadding = (float)(xAxis ? (parent->layoutConfig->padding.left + parent->layoutConfig->padding.right) : (parent->layoutConfig->padding.top + parent->layoutConfig->padding.bottom));
            float innerContentSize = 0, growContainerContentSize = 0, totalPaddingAndChildGaps = parentPadding;
            bool sizingAlongAxis = (xAxis && parentStyleConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) || (!xAxis && parentStyleConfig->layoutDirection == CLAY_TOP_TO_BOTTOM);
            resizableContainerBuffer.length = 0;
            float parentChildGap = parentStyleConfig->childGap;

            for (int32_t childOffset = 0; childOffset < parent->childrenOrTextContent.children.length; childOffset++) {
                int32_t childElementIndex = parent->childrenOrTextContent.children.elements[childOffset];
                Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, childElementIndex);
                Clay_SizingAxis childSizing = xAxis ? childElement->layoutConfig->sizing.width : childElement->layoutConfig->sizing.height;
                float childSize = xAxis ? childElement->dimensions.width : childElement->dimensions.height;

                if (!Clay__ElementHasConfig(childElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT) && childElement->childrenOrTextContent.children.length > 0) {
                    Clay__int32_tArray_Add(&bfsBuffer, childElementIndex);
                }

                if (childSizing.type != CLAY__SIZING_TYPE_PERCENT && childSizing.type != CLAY__SIZING_TYPE_FIXED && (!Clay__ElementHasConfig(childElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT) || (Clay__FindElementConfigWithType(childElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT).textElementConfig->wrapMode == CLAY_TEXT_WRAP_WORDS))) {
                    Clay__int32_tArray_Add(&resizableContainerBuffer, childElementIndex);
                }

                if (sizingAlongAxis) {
                    innerContentSize += (childSizing.type == CLAY__SIZING_TYPE_PERCENT ? 0 : childSize);
                    if (childSizing.type == CLAY__SIZING_TYPE_GROW) {
                        growContainerContentSize += childSize;
                        growContainerCount++;
                    }
                    if (childOffset > 0) {
                        innerContentSize += parentChildGap; // For children after index 0, the childAxisOffset is the gap from the previous child
                        totalPaddingAndChildGaps += parentChildGap;
                    }
                } else {
                    innerContentSize = CLAY__MAX(childSize, innerContentSize);
                }
            }

            // Expand percentage containers to size
            for (int32_t childOffset = 0; childOffset < parent->childrenOrTextContent.children.length; childOffset++) {
                int32_t childElementIndex = parent->childrenOrTextContent.children.elements[childOffset];
                Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, childElementIndex);
                Clay_SizingAxis childSizing = xAxis ? childElement->layoutConfig->sizing.width : childElement->layoutConfig->sizing.height;
                float *childSize = xAxis ? &childElement->dimensions.width : &childElement->dimensions.height;
                if (childSizing.type == CLAY__SIZING_TYPE_PERCENT) {
                    *childSize = (parentSize - totalPaddingAndChildGaps) * childSizing.size.percent;
                    if (sizingAlongAxis) {
                        innerContentSize += *childSize;
                    }
                }
            }

            if (sizingAlongAxis) {
                float sizeToDistribute = parentSize - parentPadding - innerContentSize;
                // The content is too large, compress the children as much as possible
                if (sizeToDistribute < 0) {
                    // If the parent can scroll in the axis direction in this direction, don't compress children, just leave them alone
                    if (Clay__ElementHasConfig(parent, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER)) {
                        Clay_ScrollElementConfig *scrollElementConfig = Clay__FindElementConfigWithType(parent, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER).scrollElementConfig;
                        if (((xAxis && scrollElementConfig->horizontal) || (!xAxis && scrollElementConfig->vertical))) {
                            continue;
                        }
                    }
                    // Scrolling containers preferentially compress before others
                    Clay__CompressChildrenAlongAxis(xAxis, -sizeToDistribute, resizableContainerBuffer);
                // The content is too small, allow SIZING_GROW containers to expand
                } else if (sizeToDistribute > 0 && growContainerCount > 0) {
                    float targetSize = (sizeToDistribute + growContainerContentSize) / (float)growContainerCount;
                    for (int32_t childOffset = 0; childOffset < resizableContainerBuffer.length; childOffset++) {
                        Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_Get(&resizableContainerBuffer, childOffset));
                        Clay_SizingAxis childSizing = xAxis ? childElement->layoutConfig->sizing.width : childElement->layoutConfig->sizing.height;
                        if (childSizing.type == CLAY__SIZING_TYPE_GROW) {
                            float *childSize = xAxis ? &childElement->dimensions.width : &childElement->dimensions.height;
                            float *minSize = xAxis ? &childElement->minDimensions.width : &childElement->minDimensions.height;
                            if (targetSize < *minSize) {
                                growContainerContentSize -= *minSize;
                                Clay__int32_tArray_RemoveSwapback(&resizableContainerBuffer, childOffset);
                                growContainerCount--;
                                targetSize = (sizeToDistribute + growContainerContentSize) / (float)growContainerCount;
                                childOffset = -1;
                                continue;
                            }
                            *childSize = targetSize;
                        }
                    }
                }
            // Sizing along the non layout axis ("off axis")
            } else {
                for (int32_t childOffset = 0; childOffset < resizableContainerBuffer.length; childOffset++) {
                    Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_Get(&resizableContainerBuffer, childOffset));
                    Clay_SizingAxis childSizing = xAxis ? childElement->layoutConfig->sizing.width : childElement->layoutConfig->sizing.height;
                    float *childSize = xAxis ? &childElement->dimensions.width : &childElement->dimensions.height;

                    if (!xAxis && Clay__ElementHasConfig(childElement, CLAY__ELEMENT_CONFIG_TYPE_IMAGE)) {
                        continue; // Currently we don't support resizing aspect ratio images on the Y axis because it would break the ratio
                    }

                    // If we're laying out the children of a scroll panel, grow containers expand to the height of the inner content, not the outer container
                    float maxSize = parentSize - parentPadding;
                    if (Clay__ElementHasConfig(parent, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER)) {
                        Clay_ScrollElementConfig *scrollElementConfig = Clay__FindElementConfigWithType(parent, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER).scrollElementConfig;
                        if (((xAxis && scrollElementConfig->horizontal) || (!xAxis && scrollElementConfig->vertical))) {
                            maxSize = CLAY__MAX(maxSize, innerContentSize);
                        }
                    }
                    if (childSizing.type == CLAY__SIZING_TYPE_FIT) {
                        *childSize = CLAY__MAX(childSizing.size.minMax.min, CLAY__MIN(*childSize, maxSize));
                    } else if (childSizing.type == CLAY__SIZING_TYPE_GROW) {
                        *childSize = CLAY__MIN(maxSize, childSizing.size.minMax.max);
                    }
                }
            }
        }
    }
}

Clay_String Clay__IntToString(int32_t integer) {
    if (integer == 0) {
        return CLAY__INIT(Clay_String) { .length = 1, .chars = "0" };
    }
    Clay_Context* context = Clay_GetCurrentContext();
    char *chars = (char *)(context->dynamicStringData.internalArray + context->dynamicStringData.length);
    int32_t length = 0;
    int32_t sign = integer;

    if (integer < 0) {
        integer = -integer;
    }
    while (integer > 0) {
        chars[length++] = (char)(integer % 10 + '0');
        integer /= 10;
    }

    if (sign < 0) {
        chars[length++] = '-';
    }

    // Reverse the string to get the correct order
    for (int32_t j = 0, k = length - 1; j < k; j++, k--) {
        char temp = chars[j];
        chars[j] = chars[k];
        chars[k] = temp;
    }
    context->dynamicStringData.length += length;
    return CLAY__INIT(Clay_String) { .length = length, .chars = chars };
}

void Clay__AddRenderCommand(Clay_RenderCommand renderCommand) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->renderCommands.length < context->renderCommands.capacity - 1) {
        Clay_RenderCommandArray_Add(&context->renderCommands, renderCommand);
    } else {
        if (!context->booleanWarnings.maxRenderCommandsExceeded) {
            context->booleanWarnings.maxRenderCommandsExceeded = true;
            context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                .errorType = CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED,
                .errorText = CLAY_STRING("Clay ran out of capacity while attempting to create render commands. This is usually caused by a large amount of wrapping text elements while close to the max element capacity. Try using Clay_SetMaxElementCount() with a higher value."),
                .userData = context->errorHandler.userData });
        }
    }
}

bool Clay__ElementIsOffscreen(Clay_BoundingBox *boundingBox) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->disableCulling) {
        return false;
    }

    return (boundingBox->x > (float)context->layoutDimensions.width) ||
           (boundingBox->y > (float)context->layoutDimensions.height) ||
           (boundingBox->x + boundingBox->width < 0) ||
           (boundingBox->y + boundingBox->height < 0);
}

void Clay__CalculateFinalLayout() {
    Clay_Context* context = Clay_GetCurrentContext();
    // Calculate sizing along the X axis
    Clay__SizeContainersAlongAxis(true);

    // Wrap text
    for (int32_t textElementIndex = 0; textElementIndex < context->textElementData.length; ++textElementIndex) {
        Clay__TextElementData *textElementData = Clay__TextElementDataArray_Get(&context->textElementData, textElementIndex);
        textElementData->wrappedLines = CLAY__INIT(Clay__WrappedTextLineArraySlice) { .length = 0, .internalArray = &context->wrappedTextLines.internalArray[context->wrappedTextLines.length] };
        Clay_LayoutElement *containerElement = Clay_LayoutElementArray_Get(&context->layoutElements, (int)textElementData->elementIndex);
        Clay_TextElementConfig *textConfig = Clay__FindElementConfigWithType(containerElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT).textElementConfig;
        Clay__MeasureTextCacheItem *measureTextCacheItem = Clay__MeasureTextCached(&textElementData->text, textConfig);
        float lineWidth = 0;
        float lineHeight = textConfig->lineHeight > 0 ? (float)textConfig->lineHeight : textElementData->preferredDimensions.height;
        int32_t lineLengthChars = 0;
        int32_t lineStartOffset = 0;
        if (!measureTextCacheItem->containsNewlines && textElementData->preferredDimensions.width <= containerElement->dimensions.width) {
            Clay__WrappedTextLineArray_Add(&context->wrappedTextLines, CLAY__INIT(Clay__WrappedTextLine) { containerElement->dimensions,  textElementData->text });
            textElementData->wrappedLines.length++;
            continue;
        }
        int32_t wordIndex = measureTextCacheItem->measuredWordsStartIndex;
        while (wordIndex != -1) {
            if (context->wrappedTextLines.length > context->wrappedTextLines.capacity - 1) {
                break;
            }
            Clay__MeasuredWord *measuredWord = Clay__MeasuredWordArray_Get(&context->measuredWords, wordIndex);
            // Only word on the line is too large, just render it anyway
            if (lineLengthChars == 0 && lineWidth + measuredWord->width > containerElement->dimensions.width) {
                Clay__WrappedTextLineArray_Add(&context->wrappedTextLines, CLAY__INIT(Clay__WrappedTextLine) { { measuredWord->width, lineHeight }, { .length = measuredWord->length, .chars = &textElementData->text.chars[measuredWord->startOffset] } });
                textElementData->wrappedLines.length++;
                wordIndex = measuredWord->next;
                lineStartOffset = measuredWord->startOffset + measuredWord->length;
            }
            // measuredWord->length == 0 means a newline character
            else if (measuredWord->length == 0 || lineWidth + measuredWord->width > containerElement->dimensions.width) {
                // Wrapped text lines list has overflowed, just render out the line
                Clay__WrappedTextLineArray_Add(&context->wrappedTextLines, CLAY__INIT(Clay__WrappedTextLine) { { lineWidth, lineHeight }, { .length = lineLengthChars, .chars = &textElementData->text.chars[lineStartOffset] } });
                textElementData->wrappedLines.length++;
                if (lineLengthChars == 0 || measuredWord->length == 0) {
                    wordIndex = measuredWord->next;
                }
                lineWidth = 0;
                lineLengthChars = 0;
                lineStartOffset = measuredWord->startOffset;
            } else {
                lineWidth += measuredWord->width;
                lineLengthChars += measuredWord->length;
                wordIndex = measuredWord->next;
            }
        }
        if (lineLengthChars > 0) {
            Clay__WrappedTextLineArray_Add(&context->wrappedTextLines, CLAY__INIT(Clay__WrappedTextLine) { { lineWidth, lineHeight }, {.length = lineLengthChars, .chars = &textElementData->text.chars[lineStartOffset] } });
            textElementData->wrappedLines.length++;
        }
        containerElement->dimensions.height = lineHeight * (float)textElementData->wrappedLines.length;
    }

    // Scale vertical image heights according to aspect ratio
    for (int32_t i = 0; i < context->imageElementPointers.length; ++i) {
        Clay_LayoutElement* imageElement = Clay__LayoutElementPointerArray_Get(&context->imageElementPointers, i);
        Clay_ImageElementConfig *config = Clay__FindElementConfigWithType(imageElement, CLAY__ELEMENT_CONFIG_TYPE_IMAGE).imageElementConfig;
        imageElement->dimensions.height = (config->sourceDimensions.height / CLAY__MAX(config->sourceDimensions.width, 1)) * imageElement->dimensions.width;
    }

    // Propagate effect of text wrapping, image aspect scaling etc. on height of parents
    Clay__LayoutElementTreeNodeArray dfsBuffer = context->layoutElementTreeNodeArray1;
    dfsBuffer.length = 0;
    for (int32_t i = 0; i < context->layoutElementTreeRoots.length; ++i) {
        Clay__LayoutElementTreeRoot *root = Clay__LayoutElementTreeRootArray_Get(&context->layoutElementTreeRoots, i);
        context->treeNodeVisited.internalArray[dfsBuffer.length] = false;
        Clay__LayoutElementTreeNodeArray_Add(&dfsBuffer, CLAY__INIT(Clay__LayoutElementTreeNode) { .layoutElement = Clay_LayoutElementArray_Get(&context->layoutElements, (int)root->layoutElementIndex) });
    }
    while (dfsBuffer.length > 0) {
        Clay__LayoutElementTreeNode *currentElementTreeNode = Clay__LayoutElementTreeNodeArray_Get(&dfsBuffer, (int)dfsBuffer.length - 1);
        Clay_LayoutElement *currentElement = currentElementTreeNode->layoutElement;
        if (!context->treeNodeVisited.internalArray[dfsBuffer.length - 1]) {
            context->treeNodeVisited.internalArray[dfsBuffer.length - 1] = true;
            // If the element has no children or is the container for a text element, don't bother inspecting it
            if (Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT) || currentElement->childrenOrTextContent.children.length == 0) {
                dfsBuffer.length--;
                continue;
            }
            // Add the children to the DFS buffer (needs to be pushed in reverse so that stack traversal is in correct layout order)
            for (int32_t i = 0; i < currentElement->childrenOrTextContent.children.length; i++) {
                context->treeNodeVisited.internalArray[dfsBuffer.length] = false;
                Clay__LayoutElementTreeNodeArray_Add(&dfsBuffer, CLAY__INIT(Clay__LayoutElementTreeNode) { .layoutElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->childrenOrTextContent.children.elements[i]) });
            }
            continue;
        }
        dfsBuffer.length--;

        // DFS node has been visited, this is on the way back up to the root
        Clay_LayoutConfig *layoutConfig = currentElement->layoutConfig;
        if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
            // Resize any parent containers that have grown in height along their non layout axis
            for (int32_t j = 0; j < currentElement->childrenOrTextContent.children.length; ++j) {
                Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->childrenOrTextContent.children.elements[j]);
                float childHeightWithPadding = CLAY__MAX(childElement->dimensions.height + layoutConfig->padding.top + layoutConfig->padding.bottom, currentElement->dimensions.height);
                currentElement->dimensions.height = CLAY__MIN(CLAY__MAX(childHeightWithPadding, layoutConfig->sizing.height.size.minMax.min), layoutConfig->sizing.height.size.minMax.max);
            }
        } else if (layoutConfig->layoutDirection == CLAY_TOP_TO_BOTTOM) {
            // Resizing along the layout axis
            float contentHeight = (float)(layoutConfig->padding.top + layoutConfig->padding.bottom);
            for (int32_t j = 0; j < currentElement->childrenOrTextContent.children.length; ++j) {
                Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->childrenOrTextContent.children.elements[j]);
                contentHeight += childElement->dimensions.height;
            }
            contentHeight += (float)(CLAY__MAX(currentElement->childrenOrTextContent.children.length - 1, 0) * layoutConfig->childGap);
            currentElement->dimensions.height = CLAY__MIN(CLAY__MAX(contentHeight, layoutConfig->sizing.height.size.minMax.min), layoutConfig->sizing.height.size.minMax.max);
        }
    }

    // Calculate sizing along the Y axis
    Clay__SizeContainersAlongAxis(false);

    // Sort tree roots by z-index
    int32_t sortMax = context->layoutElementTreeRoots.length - 1;
    while (sortMax > 0) { // todo dumb bubble sort
        for (int32_t i = 0; i < sortMax; ++i) {
            Clay__LayoutElementTreeRoot current = *Clay__LayoutElementTreeRootArray_Get(&context->layoutElementTreeRoots, i);
            Clay__LayoutElementTreeRoot next = *Clay__LayoutElementTreeRootArray_Get(&context->layoutElementTreeRoots, i + 1);
            if (next.zIndex < current.zIndex) {
                Clay__LayoutElementTreeRootArray_Set(&context->layoutElementTreeRoots, i, next);
                Clay__LayoutElementTreeRootArray_Set(&context->layoutElementTreeRoots, i + 1, current);
            }
        }
        sortMax--;
    }

    // Calculate final positions and generate render commands
    context->renderCommands.length = 0;
    dfsBuffer.length = 0;
    for (int32_t rootIndex = 0; rootIndex < context->layoutElementTreeRoots.length; ++rootIndex) {
        dfsBuffer.length = 0;
        Clay__LayoutElementTreeRoot *root = Clay__LayoutElementTreeRootArray_Get(&context->layoutElementTreeRoots, rootIndex);
        Clay_LayoutElement *rootElement = Clay_LayoutElementArray_Get(&context->layoutElements, (int)root->layoutElementIndex);
        Clay_Vector2 rootPosition = CLAY__DEFAULT_STRUCT;
        Clay_LayoutElementHashMapItem *parentHashMapItem = Clay__GetHashMapItem(root->parentId);
        // Position root floating containers
        if (Clay__ElementHasConfig(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER) && parentHashMapItem) {
            Clay_FloatingElementConfig *config = Clay__FindElementConfigWithType(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER).floatingElementConfig;
            Clay_Dimensions rootDimensions = rootElement->dimensions;
            Clay_BoundingBox parentBoundingBox = parentHashMapItem->boundingBox;
            // Set X position
            Clay_Vector2 targetAttachPosition = CLAY__DEFAULT_STRUCT;
            switch (config->attachment.parent) {
                case CLAY_ATTACH_POINT_LEFT_TOP:
                case CLAY_ATTACH_POINT_LEFT_CENTER:
                case CLAY_ATTACH_POINT_LEFT_BOTTOM: targetAttachPosition.x = parentBoundingBox.x; break;
                case CLAY_ATTACH_POINT_CENTER_TOP:
                case CLAY_ATTACH_POINT_CENTER_CENTER:
                case CLAY_ATTACH_POINT_CENTER_BOTTOM: targetAttachPosition.x = parentBoundingBox.x + (parentBoundingBox.width / 2); break;
                case CLAY_ATTACH_POINT_RIGHT_TOP:
                case CLAY_ATTACH_POINT_RIGHT_CENTER:
                case CLAY_ATTACH_POINT_RIGHT_BOTTOM: targetAttachPosition.x = parentBoundingBox.x + parentBoundingBox.width; break;
            }
            switch (config->attachment.element) {
                case CLAY_ATTACH_POINT_LEFT_TOP:
                case CLAY_ATTACH_POINT_LEFT_CENTER:
                case CLAY_ATTACH_POINT_LEFT_BOTTOM: break;
                case CLAY_ATTACH_POINT_CENTER_TOP:
                case CLAY_ATTACH_POINT_CENTER_CENTER:
                case CLAY_ATTACH_POINT_CENTER_BOTTOM: targetAttachPosition.x -= (rootDimensions.width / 2); break;
                case CLAY_ATTACH_POINT_RIGHT_TOP:
                case CLAY_ATTACH_POINT_RIGHT_CENTER:
                case CLAY_ATTACH_POINT_RIGHT_BOTTOM: targetAttachPosition.x -= rootDimensions.width; break;
            }
            switch (config->attachment.parent) { // I know I could merge the x and y switch statements, but this is easier to read
                case CLAY_ATTACH_POINT_LEFT_TOP:
                case CLAY_ATTACH_POINT_RIGHT_TOP:
                case CLAY_ATTACH_POINT_CENTER_TOP: targetAttachPosition.y = parentBoundingBox.y; break;
                case CLAY_ATTACH_POINT_LEFT_CENTER:
                case CLAY_ATTACH_POINT_CENTER_CENTER:
                case CLAY_ATTACH_POINT_RIGHT_CENTER: targetAttachPosition.y = parentBoundingBox.y + (parentBoundingBox.height / 2); break;
                case CLAY_ATTACH_POINT_LEFT_BOTTOM:
                case CLAY_ATTACH_POINT_CENTER_BOTTOM:
                case CLAY_ATTACH_POINT_RIGHT_BOTTOM: targetAttachPosition.y = parentBoundingBox.y + parentBoundingBox.height; break;
            }
            switch (config->attachment.element) {
                case CLAY_ATTACH_POINT_LEFT_TOP:
                case CLAY_ATTACH_POINT_RIGHT_TOP:
                case CLAY_ATTACH_POINT_CENTER_TOP: break;
                case CLAY_ATTACH_POINT_LEFT_CENTER:
                case CLAY_ATTACH_POINT_CENTER_CENTER:
                case CLAY_ATTACH_POINT_RIGHT_CENTER: targetAttachPosition.y -= (rootDimensions.height / 2); break;
                case CLAY_ATTACH_POINT_LEFT_BOTTOM:
                case CLAY_ATTACH_POINT_CENTER_BOTTOM:
                case CLAY_ATTACH_POINT_RIGHT_BOTTOM: targetAttachPosition.y -= rootDimensions.height; break;
            }
            targetAttachPosition.x += config->offset.x;
            targetAttachPosition.y += config->offset.y;
            rootPosition = targetAttachPosition;
        }
        if (root->clipElementId) {
            Clay_LayoutElementHashMapItem *clipHashMapItem = Clay__GetHashMapItem(root->clipElementId);
            if (clipHashMapItem) {
                // Floating elements that are attached to scrolling contents won't be correctly positioned if external scroll handling is enabled, fix here
                if (context->externalScrollHandlingEnabled) {
                    Clay_ScrollElementConfig *scrollConfig = Clay__FindElementConfigWithType(clipHashMapItem->layoutElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER).scrollElementConfig;
                    for (int32_t i = 0; i < context->scrollContainerDatas.length; i++) {
                        Clay__ScrollContainerDataInternal *mapping = Clay__ScrollContainerDataInternalArray_Get(&context->scrollContainerDatas, i);
                        if (mapping->layoutElement == clipHashMapItem->layoutElement) {
                            root->pointerOffset = mapping->scrollPosition;
                            if (scrollConfig->horizontal) {
                                rootPosition.x += mapping->scrollPosition.x;
                            }
                            if (scrollConfig->vertical) {
                                rootPosition.y += mapping->scrollPosition.y;
                            }
                            break;
                        }
                    }
                }
                Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                    .boundingBox = clipHashMapItem->boundingBox,
                    .config = { .scrollElementConfig = Clay__StoreScrollElementConfig(CLAY__INIT(Clay_ScrollElementConfig)CLAY__DEFAULT_STRUCT) },
                    .id = Clay__RehashWithNumber(rootElement->id, 10), // TODO need a better strategy for managing derived ids
                    .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_START,
                });
            }
        }
        Clay__LayoutElementTreeNodeArray_Add(&dfsBuffer, CLAY__INIT(Clay__LayoutElementTreeNode) { .layoutElement = rootElement, .position = rootPosition, .nextChildOffset = { .x = (float)rootElement->layoutConfig->padding.left, .y = (float)rootElement->layoutConfig->padding.top } });

        context->treeNodeVisited.internalArray[0] = false;
        while (dfsBuffer.length > 0) {
            Clay__LayoutElementTreeNode *currentElementTreeNode = Clay__LayoutElementTreeNodeArray_Get(&dfsBuffer, (int)dfsBuffer.length - 1);
            Clay_LayoutElement *currentElement = currentElementTreeNode->layoutElement;
            Clay_LayoutConfig *layoutConfig = currentElement->layoutConfig;
            Clay_Vector2 scrollOffset = CLAY__DEFAULT_STRUCT;

            // This will only be run a single time for each element in downwards DFS order
            if (!context->treeNodeVisited.internalArray[dfsBuffer.length - 1]) {
                context->treeNodeVisited.internalArray[dfsBuffer.length - 1] = true;

                Clay_BoundingBox currentElementBoundingBox = { currentElementTreeNode->position.x, currentElementTreeNode->position.y, currentElement->dimensions.width, currentElement->dimensions.height };
                if (Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER)) {
                    Clay_FloatingElementConfig *floatingElementConfig = Clay__FindElementConfigWithType(currentElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER).floatingElementConfig;
                    Clay_Dimensions expand = floatingElementConfig->expand;
                    currentElementBoundingBox.x -= expand.width;
                    currentElementBoundingBox.width += expand.width * 2;
                    currentElementBoundingBox.y -= expand.height;
                    currentElementBoundingBox.height += expand.height * 2;
                }

                Clay__ScrollContainerDataInternal *scrollContainerData = CLAY__NULL;
                // Apply scroll offsets to container
                if (Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER)) {
                    Clay_ScrollElementConfig *scrollConfig = Clay__FindElementConfigWithType(currentElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER).scrollElementConfig;

                    // This linear scan could theoretically be slow under very strange conditions, but I can't imagine a real UI with more than a few 10's of scroll containers
                    for (int32_t i = 0; i < context->scrollContainerDatas.length; i++) {
                        Clay__ScrollContainerDataInternal *mapping = Clay__ScrollContainerDataInternalArray_Get(&context->scrollContainerDatas, i);
                        if (mapping->layoutElement == currentElement) {
                            scrollContainerData = mapping;
                            mapping->boundingBox = currentElementBoundingBox;
                            if (scrollConfig->horizontal) {
                                scrollOffset.x = mapping->scrollPosition.x;
                            }
                            if (scrollConfig->vertical) {
                                scrollOffset.y = mapping->scrollPosition.y;
                            }
                            if (context->externalScrollHandlingEnabled) {
                                scrollOffset = CLAY__INIT(Clay_Vector2) CLAY__DEFAULT_STRUCT;
                            }
                            break;
                        }
                    }
                }

                Clay_LayoutElementHashMapItem *hashMapItem = Clay__GetHashMapItem(currentElement->id);
                if (hashMapItem) {
                    hashMapItem->boundingBox = currentElementBoundingBox;
                }

                int32_t sortedConfigIndexes[20];
                for (int32_t elementConfigIndex = 0; elementConfigIndex < currentElement->elementConfigs.length; ++elementConfigIndex) {
                    sortedConfigIndexes[elementConfigIndex] = elementConfigIndex;
                }
                sortMax = currentElement->elementConfigs.length - 1;
                while (sortMax > 0) { // todo dumb bubble sort
                    for (int32_t i = 0; i < sortMax; ++i) {
                        int32_t current = sortedConfigIndexes[i];
                        int32_t next = sortedConfigIndexes[i + 1];
                        Clay__ElementConfigType currentType = Clay__ElementConfigArraySlice_Get(&currentElement->elementConfigs, current)->type;
                        Clay__ElementConfigType nextType = Clay__ElementConfigArraySlice_Get(&currentElement->elementConfigs, next)->type;
                        if (nextType == CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER || currentType == CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER) {
                            sortedConfigIndexes[i] = next;
                            sortedConfigIndexes[i + 1] = current;
                        }
                    }
                    sortMax--;
                }

                // Create the render commands for this element
                for (int32_t elementConfigIndex = 0; elementConfigIndex < currentElement->elementConfigs.length; ++elementConfigIndex) {
                    Clay_ElementConfig *elementConfig = Clay__ElementConfigArraySlice_Get(&currentElement->elementConfigs, sortedConfigIndexes[elementConfigIndex]);
                    Clay_RenderCommand renderCommand = {
                        .boundingBox = currentElementBoundingBox,
                        .config = elementConfig->config,
                        .id = currentElement->id,
                    };

                    bool offscreen = Clay__ElementIsOffscreen(&currentElementBoundingBox);
                    // Culling - Don't bother to generate render commands for rectangles entirely outside the screen - this won't stop their children from being rendered if they overflow
                    bool shouldRender = !offscreen;
                    switch (elementConfig->type) {
                        case CLAY__ELEMENT_CONFIG_TYPE_RECTANGLE: {
                            renderCommand.commandType = CLAY_RENDER_COMMAND_TYPE_RECTANGLE;
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER: {
                            shouldRender = false;
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER: {
                            renderCommand.commandType = CLAY_RENDER_COMMAND_TYPE_NONE;
                            shouldRender = false;
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER: {
                            renderCommand.commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_START;
                            shouldRender = true;
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_IMAGE: {
                            renderCommand.commandType = CLAY_RENDER_COMMAND_TYPE_IMAGE;
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_TEXT: {
                            if (!shouldRender) {
                                break;
                            }
                            shouldRender = false;
                            Clay_ElementConfigUnion configUnion = elementConfig->config;
                            Clay_TextElementConfig *textElementConfig = configUnion.textElementConfig;
                            float naturalLineHeight = currentElement->childrenOrTextContent.textElementData->preferredDimensions.height;
                            float finalLineHeight = textElementConfig->lineHeight > 0 ? (float)textElementConfig->lineHeight : naturalLineHeight;
                            float lineHeightOffset = (finalLineHeight - naturalLineHeight) / 2;
                            float yPosition = lineHeightOffset;
                            for (int32_t lineIndex = 0; lineIndex < currentElement->childrenOrTextContent.textElementData->wrappedLines.length; ++lineIndex) {
                                Clay__WrappedTextLine wrappedLine = currentElement->childrenOrTextContent.textElementData->wrappedLines.internalArray[lineIndex]; // todo range check
                                if (wrappedLine.line.length == 0) {
                                    yPosition += finalLineHeight;
                                    continue;
                                }
                                Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                                    .boundingBox = { currentElementBoundingBox.x, currentElementBoundingBox.y + yPosition, wrappedLine.dimensions.width, wrappedLine.dimensions.height }, // TODO width
                                    .config = configUnion,
                                    .text = wrappedLine.line,
                                    .id = Clay__HashNumber(lineIndex, currentElement->id).id,
                                    .commandType = CLAY_RENDER_COMMAND_TYPE_TEXT,
                                });
                                yPosition += finalLineHeight;

                                if (!context->disableCulling && (currentElementBoundingBox.y + yPosition > context->layoutDimensions.height)) {
                                    break;
                                }
                            }
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_CUSTOM: {
                            renderCommand.commandType = CLAY_RENDER_COMMAND_TYPE_CUSTOM;
                            break;
                        }
                        default: break;
                    }
                    if (shouldRender) {
                        Clay__AddRenderCommand(renderCommand);
                    }
                    if (offscreen) {
                        // NOTE: You may be tempted to try an early return / continue if an element is off screen. Why bother calculating layout for its children, right?
                        // Unfortunately, a FLOATING_CONTAINER may be defined that attaches to a child or grandchild of this element, which is large enough to still
                        // be on screen, even if this element isn't. That depends on this element and it's children being laid out correctly (even if they are entirely off screen)
                    }
                }

                // Setup initial on-axis alignment
                if (!Clay__ElementHasConfig(currentElementTreeNode->layoutElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT)) {
                    Clay_Dimensions contentSize = {0,0};
                    if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                        for (int32_t i = 0; i < currentElement->childrenOrTextContent.children.length; ++i) {
                            Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->childrenOrTextContent.children.elements[i]);
                            contentSize.width += childElement->dimensions.width;
                            contentSize.height = CLAY__MAX(contentSize.height, childElement->dimensions.height);
                        }
                        contentSize.width += (float)(CLAY__MAX(currentElement->childrenOrTextContent.children.length - 1, 0) * layoutConfig->childGap);
                        float extraSpace = currentElement->dimensions.width - (float)(layoutConfig->padding.left + layoutConfig->padding.right) - contentSize.width;
                        switch (layoutConfig->childAlignment.x) {
                            case CLAY_ALIGN_X_LEFT: extraSpace = 0; break;
                            case CLAY_ALIGN_X_CENTER: extraSpace /= 2; break;
                            default: break;
                        }
                        currentElementTreeNode->nextChildOffset.x += extraSpace;
                    } else {
                        for (int32_t i = 0; i < currentElement->childrenOrTextContent.children.length; ++i) {
                            Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->childrenOrTextContent.children.elements[i]);
                            contentSize.width = CLAY__MAX(contentSize.width, childElement->dimensions.width);
                            contentSize.height += childElement->dimensions.height;
                        }
                        contentSize.height += (float)(CLAY__MAX(currentElement->childrenOrTextContent.children.length - 1, 0) * layoutConfig->childGap);
                        float extraSpace = currentElement->dimensions.height - (float)(layoutConfig->padding.top + layoutConfig->padding.bottom) - contentSize.height;
                        switch (layoutConfig->childAlignment.y) {
                            case CLAY_ALIGN_Y_TOP: extraSpace = 0; break;
                            case CLAY_ALIGN_Y_CENTER: extraSpace /= 2; break;
                            default: break;
                        }
                        currentElementTreeNode->nextChildOffset.y += extraSpace;
                    }

                    if (scrollContainerData) {
                        scrollContainerData->contentSize = CLAY__INIT(Clay_Dimensions) { contentSize.width + (float)(layoutConfig->padding.left + layoutConfig->padding.right), contentSize.height + (float)(layoutConfig->padding.top + layoutConfig->padding.bottom) };
                    }
                }
            }
            else {
                // DFS is returning upwards backwards
                bool closeScrollElement = false;
                if (Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER)) {
                    closeScrollElement = true;
                    Clay_ScrollElementConfig *scrollConfig = Clay__FindElementConfigWithType(currentElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER).scrollElementConfig;
                    for (int32_t i = 0; i < context->scrollContainerDatas.length; i++) {
                        Clay__ScrollContainerDataInternal *mapping = Clay__ScrollContainerDataInternalArray_Get(&context->scrollContainerDatas, i);
                        if (mapping->layoutElement == currentElement) {
                            if (scrollConfig->horizontal) { scrollOffset.x = mapping->scrollPosition.x; }
                            if (scrollConfig->vertical) { scrollOffset.y = mapping->scrollPosition.y; }
                            if (context->externalScrollHandlingEnabled) {
                                scrollOffset = CLAY__INIT(Clay_Vector2) CLAY__DEFAULT_STRUCT;
                            }
                            break;
                        }
                    }
                }

                if (Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER)) {
                    Clay_LayoutElementHashMapItem *currentElementData = Clay__GetHashMapItem(currentElement->id);
                    Clay_BoundingBox currentElementBoundingBox = currentElementData->boundingBox;

                    // Culling - Don't bother to generate render commands for rectangles entirely outside the screen - this won't stop their children from being rendered if they overflow
                    if (!Clay__ElementIsOffscreen(&currentElementBoundingBox)) {
                        Clay_BorderElementConfig *borderConfig = Clay__FindElementConfigWithType(currentElement, CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER).borderElementConfig;
                        Clay_RenderCommand renderCommand = {
                                .boundingBox = currentElementBoundingBox,
                                .config = { .borderElementConfig = borderConfig },
                                .id = Clay__RehashWithNumber(currentElement->id, 4),
                                .commandType = CLAY_RENDER_COMMAND_TYPE_BORDER,
                        };
                        Clay__AddRenderCommand(renderCommand);
                        if (borderConfig->betweenChildren.width > 0 && borderConfig->betweenChildren.color.a > 0) {
                            Clay_RectangleElementConfig *rectangleConfig = Clay__StoreRectangleElementConfig(CLAY__INIT(Clay_RectangleElementConfig) {.color = borderConfig->betweenChildren.color});
                            float halfGap = layoutConfig->childGap / 2;
                            Clay_Vector2 borderOffset = { (float)layoutConfig->padding.left - halfGap, (float)layoutConfig->padding.top - halfGap };
                            if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                                for (int32_t i = 0; i < currentElement->childrenOrTextContent.children.length; ++i) {
                                    Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->childrenOrTextContent.children.elements[i]);
                                    if (i > 0) {
                                        Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                                            .boundingBox = { currentElementBoundingBox.x + borderOffset.x + scrollOffset.x, currentElementBoundingBox.y + scrollOffset.y, (float)borderConfig->betweenChildren.width, currentElement->dimensions.height },
                                            .config = { rectangleConfig },
                                            .id = Clay__RehashWithNumber(currentElement->id, 5 + i),
                                            .commandType = CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
                                        });
                                    }
                                    borderOffset.x += (childElement->dimensions.width + (float)layoutConfig->childGap);
                                }
                            } else {
                                for (int32_t i = 0; i < currentElement->childrenOrTextContent.children.length; ++i) {
                                    Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->childrenOrTextContent.children.elements[i]);
                                    if (i > 0) {
                                        Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                                                .boundingBox = { currentElementBoundingBox.x + scrollOffset.x, currentElementBoundingBox.y + borderOffset.y + scrollOffset.y, currentElement->dimensions.width, (float)borderConfig->betweenChildren.width },
                                                .config = { rectangleConfig },
                                                .id = Clay__RehashWithNumber(currentElement->id, 5 + i),
                                                .commandType = CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
                                        });
                                    }
                                    borderOffset.y += (childElement->dimensions.height + (float)layoutConfig->childGap);
                                }
                            }
                        }
                    }
                }
                // This exists because the scissor needs to end _after_ borders between elements
                if (closeScrollElement) {
                    Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                        .id = Clay__RehashWithNumber(currentElement->id, 11),
                       .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_END,
                    });
                }

                dfsBuffer.length--;
                continue;
            }

            // Add children to the DFS buffer
            if (!Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT)) {
                dfsBuffer.length += currentElement->childrenOrTextContent.children.length;
                for (int32_t i = 0; i < currentElement->childrenOrTextContent.children.length; ++i) {
                    Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->childrenOrTextContent.children.elements[i]);
                    // Alignment along non layout axis
                    if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                        currentElementTreeNode->nextChildOffset.y = currentElement->layoutConfig->padding.top;
                        float whiteSpaceAroundChild = currentElement->dimensions.height - (float)(layoutConfig->padding.top + layoutConfig->padding.bottom) - childElement->dimensions.height;
                        switch (layoutConfig->childAlignment.y) {
                            case CLAY_ALIGN_Y_TOP: break;
                            case CLAY_ALIGN_Y_CENTER: currentElementTreeNode->nextChildOffset.y += whiteSpaceAroundChild / 2; break;
                            case CLAY_ALIGN_Y_BOTTOM: currentElementTreeNode->nextChildOffset.y += whiteSpaceAroundChild; break;
                        }
                    } else {
                        currentElementTreeNode->nextChildOffset.x = currentElement->layoutConfig->padding.left;
                        float whiteSpaceAroundChild = currentElement->dimensions.width - (float)(layoutConfig->padding.left + layoutConfig->padding.right) - childElement->dimensions.width;
                        switch (layoutConfig->childAlignment.x) {
                            case CLAY_ALIGN_X_LEFT: break;
                            case CLAY_ALIGN_X_CENTER: currentElementTreeNode->nextChildOffset.x += whiteSpaceAroundChild / 2; break;
                            case CLAY_ALIGN_X_RIGHT: currentElementTreeNode->nextChildOffset.x += whiteSpaceAroundChild; break;
                        }
                    }

                    Clay_Vector2 childPosition = {
                        currentElementTreeNode->position.x + currentElementTreeNode->nextChildOffset.x + scrollOffset.x,
                        currentElementTreeNode->position.y + currentElementTreeNode->nextChildOffset.y + scrollOffset.y,
                    };

                    // DFS buffer elements need to be added in reverse because stack traversal happens backwards
                    uint32_t newNodeIndex = dfsBuffer.length - 1 - i;
                    dfsBuffer.internalArray[newNodeIndex] = CLAY__INIT(Clay__LayoutElementTreeNode) {
                        .layoutElement = childElement,
                        .position = { childPosition.x, childPosition.y },
                        .nextChildOffset = { .x = (float)childElement->layoutConfig->padding.left, .y = (float)childElement->layoutConfig->padding.top },
                    };
                    context->treeNodeVisited.internalArray[newNodeIndex] = false;

                    // Update parent offsets
                    if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                        currentElementTreeNode->nextChildOffset.x += childElement->dimensions.width + (float)layoutConfig->childGap;
                    } else {
                        currentElementTreeNode->nextChildOffset.y += childElement->dimensions.height + (float)layoutConfig->childGap;
                    }
                }
            }
        }

        if (root->clipElementId) {
            Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) { .id = Clay__RehashWithNumber(rootElement->id, 11), .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_END });
        }
    }
}

void Clay__AttachId(Clay_ElementId elementId) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    openLayoutElement->id = elementId.id;
    Clay__AddHashMapItem(elementId, openLayoutElement);
    Clay__StringArray_Add(&context->layoutElementIdStrings, elementId.stringId);
}

void Clay__AttachLayoutConfig(Clay_LayoutConfig *config) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return;
    }
    Clay__GetOpenLayoutElement()->layoutConfig = config;
}
void Clay__AttachElementConfig(Clay_ElementConfigUnion config, Clay__ElementConfigType type) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    openLayoutElement->elementConfigs.length++;
    Clay__ElementConfigArray_Add(&context->elementConfigBuffer, CLAY__INIT(Clay_ElementConfig) { .type = type, .config = config });
}
Clay_LayoutConfig * Clay__StoreLayoutConfig(Clay_LayoutConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &CLAY_LAYOUT_DEFAULT : Clay__LayoutConfigArray_Add(&Clay_GetCurrentContext()->layoutConfigs, config); }
Clay_RectangleElementConfig * Clay__StoreRectangleElementConfig(Clay_RectangleElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &CLAY__RECTANGLE_ELEMENT_CONFIG_DEFAULT : Clay__RectangleElementConfigArray_Add(&Clay_GetCurrentContext()->rectangleElementConfigs, config); }
Clay_TextElementConfig * Clay__StoreTextElementConfig(Clay_TextElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &CLAY__TEXT_ELEMENT_CONFIG_DEFAULT : Clay__TextElementConfigArray_Add(&Clay_GetCurrentContext()->textElementConfigs, config); }
Clay_ImageElementConfig * Clay__StoreImageElementConfig(Clay_ImageElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &CLAY__IMAGE_ELEMENT_CONFIG_DEFAULT : Clay__ImageElementConfigArray_Add(&Clay_GetCurrentContext()->imageElementConfigs, config); }
Clay_FloatingElementConfig * Clay__StoreFloatingElementConfig(Clay_FloatingElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &CLAY__FLOATING_ELEMENT_CONFIG_DEFAULT : Clay__FloatingElementConfigArray_Add(&Clay_GetCurrentContext()->floatingElementConfigs, config); }
Clay_CustomElementConfig * Clay__StoreCustomElementConfig(Clay_CustomElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &CLAY__CUSTOM_ELEMENT_CONFIG_DEFAULT : Clay__CustomElementConfigArray_Add(&Clay_GetCurrentContext()->customElementConfigs, config); }
Clay_ScrollElementConfig * Clay__StoreScrollElementConfig(Clay_ScrollElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &CLAY__SCROLL_ELEMENT_CONFIG_DEFAULT : Clay__ScrollElementConfigArray_Add(&Clay_GetCurrentContext()->scrollElementConfigs, config); }
Clay_BorderElementConfig * Clay__StoreBorderElementConfig(Clay_BorderElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &CLAY__BORDER_ELEMENT_CONFIG_DEFAULT : Clay__BorderElementConfigArray_Add(&Clay_GetCurrentContext()->borderElementConfigs, config); }

#pragma region DebugTools
Clay_Color CLAY__DEBUGVIEW_COLOR_1 = {58, 56, 52, 255};
Clay_Color CLAY__DEBUGVIEW_COLOR_2 = {62, 60, 58, 255};
Clay_Color CLAY__DEBUGVIEW_COLOR_3 = {141, 133, 135, 255};
Clay_Color CLAY__DEBUGVIEW_COLOR_4 = {238, 226, 231, 255};
Clay_Color CLAY__DEBUGVIEW_COLOR_SELECTED_ROW = {102, 80, 78, 255};
const int32_t CLAY__DEBUGVIEW_ROW_HEIGHT = 30;
const int32_t CLAY__DEBUGVIEW_OUTER_PADDING = 10;
const int32_t CLAY__DEBUGVIEW_INDENT_WIDTH = 16;
Clay_TextElementConfig Clay__DebugView_TextNameConfig = {.textColor = {238, 226, 231, 255}, .fontSize = 16, .wrapMode = CLAY_TEXT_WRAP_NONE };
Clay_LayoutConfig Clay__DebugView_ScrollViewItemLayoutConfig = CLAY__DEFAULT_STRUCT;

CLAY__TYPEDEF(Clay__DebugElementConfigTypeLabelConfig, struct {
    Clay_String label;
    Clay_Color color;
});

Clay__DebugElementConfigTypeLabelConfig Clay__DebugGetElementConfigTypeLabel(Clay__ElementConfigType type) {
    switch (type) {
        case CLAY__ELEMENT_CONFIG_TYPE_RECTANGLE: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Rectangle"), {243,134,48,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_TEXT: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Text"), {105,210,231,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_IMAGE: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Image"), {121,189,154,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Floating"), {250,105,0,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Scroll"), {242,196,90,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Border"), {108,91,123, 255} };
        case CLAY__ELEMENT_CONFIG_TYPE_CUSTOM: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Custom"), {11,72,107,255} };
        default: break;
    }
    return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Error"), {0,0,0,255} };
}

CLAY__TYPEDEF(Clay__RenderDebugLayoutData, struct {
    int32_t rowCount;
    int32_t selectedElementRowIndex;
});

// Returns row count
Clay__RenderDebugLayoutData Clay__RenderDebugLayoutElementsList(int32_t initialRootsLength, int32_t highlightedRowIndex) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__int32_tArray dfsBuffer = context->reusableElementIndexBuffer;
    Clay__DebugView_ScrollViewItemLayoutConfig = CLAY__INIT(Clay_LayoutConfig) { .sizing = { .height = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT) }, .childGap = 6, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER }};
    Clay__RenderDebugLayoutData layoutData = CLAY__DEFAULT_STRUCT;

    uint32_t highlightedElementId = 0;

    for (int32_t rootIndex = 0; rootIndex < initialRootsLength; ++rootIndex) {
        dfsBuffer.length = 0;
        Clay__LayoutElementTreeRoot *root = Clay__LayoutElementTreeRootArray_Get(&context->layoutElementTreeRoots, rootIndex);
        Clay__int32_tArray_Add(&dfsBuffer, (int32_t)root->layoutElementIndex);
        context->treeNodeVisited.internalArray[0] = false;
        if (rootIndex > 0) {
            CLAY(CLAY_IDI("Clay__DebugView_EmptyRowOuter", rootIndex), CLAY_LAYOUT({ .sizing = {.width = CLAY_SIZING_GROW(0)}, .padding = {CLAY__DEBUGVIEW_INDENT_WIDTH / 2, 0} })) {
                CLAY(CLAY_IDI("Clay__DebugView_EmptyRow", rootIndex), CLAY_LAYOUT({ .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED((float)CLAY__DEBUGVIEW_ROW_HEIGHT) }}), CLAY_BORDER({ .top = { .width = 1, .color = CLAY__DEBUGVIEW_COLOR_3 } })) {}
            }
            layoutData.rowCount++;
        }
        while (dfsBuffer.length > 0) {
            int32_t currentElementIndex = Clay__int32_tArray_Get(&dfsBuffer, (int)dfsBuffer.length - 1);
            Clay_LayoutElement *currentElement = Clay_LayoutElementArray_Get(&context->layoutElements, (int)currentElementIndex);
            if (context->treeNodeVisited.internalArray[dfsBuffer.length - 1]) {
                if (!Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT) && currentElement->childrenOrTextContent.children.length > 0) {
                    Clay__CloseElement();
                    Clay__CloseElement();
                    Clay__CloseElement();
                }
                dfsBuffer.length--;
                continue;
            }

            if (highlightedRowIndex == layoutData.rowCount) {
                if (context->pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
                    context->debugSelectedElementId = currentElement->id;
                }
                highlightedElementId = currentElement->id;
            }

            context->treeNodeVisited.internalArray[dfsBuffer.length - 1] = true;
            Clay_LayoutElementHashMapItem *currentElementData = Clay__GetHashMapItem(currentElement->id);
            bool offscreen = Clay__ElementIsOffscreen(&currentElementData->boundingBox);
            if (context->debugSelectedElementId == currentElement->id) {
                layoutData.selectedElementRowIndex = layoutData.rowCount;
            }
            CLAY(CLAY_IDI("Clay__DebugView_ElementOuter", currentElement->id), Clay__AttachLayoutConfig(&Clay__DebugView_ScrollViewItemLayoutConfig)) {
                // Collapse icon / button
                if (!(Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT) || currentElement->childrenOrTextContent.children.length == 0)) {
                    CLAY(CLAY_IDI("Clay__DebugView_CollapseElement", currentElement->id),
                        CLAY_LAYOUT({ .sizing = {CLAY_SIZING_FIXED(16), CLAY_SIZING_FIXED(16)}, .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER} }),
                        CLAY_BORDER_OUTSIDE_RADIUS(1, CLAY__DEBUGVIEW_COLOR_3, 4)
                    ) {
                        CLAY_TEXT((currentElementData && currentElementData->debugData->collapsed) ? CLAY_STRING("+") : CLAY_STRING("-"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
                    }
                } else { // Square dot for empty containers
                    CLAY(CLAY_LAYOUT({ .sizing = {CLAY_SIZING_FIXED(16), CLAY_SIZING_FIXED(16)}, .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER } })) {
                        CLAY(CLAY_LAYOUT({ .sizing = {CLAY_SIZING_FIXED(8), CLAY_SIZING_FIXED(8)} }), CLAY_RECTANGLE({ .color = CLAY__DEBUGVIEW_COLOR_3, .cornerRadius = CLAY_CORNER_RADIUS(2) })) {}
                    }
                }
                // Collisions and offscreen info
                if (currentElementData) {
                    if (currentElementData->debugData->collision) {
                        CLAY(CLAY_LAYOUT({ .padding = { 8, 8, 2, 2 } }), CLAY_BORDER_OUTSIDE_RADIUS(1, (CLAY__INIT(Clay_Color){177, 147, 8, 255}), 4)) {
                            CLAY_TEXT(CLAY_STRING("Duplicate ID"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16 }));
                        }
                    }
                    if (offscreen) {
                        CLAY(CLAY_LAYOUT({ .padding = { 8, 8, 2, 2 } }), CLAY_BORDER_OUTSIDE_RADIUS(1, CLAY__DEBUGVIEW_COLOR_3, 4)) {
                            CLAY_TEXT(CLAY_STRING("Offscreen"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16 }));
                        }
                    }
                }
                Clay_String idString = context->layoutElementIdStrings.internalArray[currentElementIndex];
                if (idString.length > 0) {
                    CLAY_TEXT(idString, offscreen ? CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16 }) : &Clay__DebugView_TextNameConfig);
                }
                for (int32_t elementConfigIndex = 0; elementConfigIndex < currentElement->elementConfigs.length; ++elementConfigIndex) {
                    Clay_ElementConfig *elementConfig = Clay__ElementConfigArraySlice_Get(&currentElement->elementConfigs, elementConfigIndex);
                    Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(elementConfig->type);
                    Clay_Color backgroundColor = config.color;
                    backgroundColor.a = 90;
                    CLAY(CLAY_LAYOUT({ .padding = { 8, 8, 2, 2 } }), CLAY_RECTANGLE({ .color = backgroundColor, .cornerRadius = CLAY_CORNER_RADIUS(4) }), CLAY_BORDER_OUTSIDE_RADIUS(1, config.color, 4)) {
                        CLAY_TEXT(config.label, CLAY_TEXT_CONFIG({ .textColor = offscreen ? CLAY__DEBUGVIEW_COLOR_3 : CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
                    }
                }
            }

            // Render the text contents below the element as a non-interactive row
            if (Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT)) {
                layoutData.rowCount++;
                Clay__TextElementData *textElementData = currentElement->childrenOrTextContent.textElementData;
                Clay_TextElementConfig *rawTextConfig = offscreen ? CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16 }) : &Clay__DebugView_TextNameConfig;
                CLAY(CLAY_LAYOUT({ .sizing = { .height = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } }), CLAY_RECTANGLE(CLAY__DEFAULT_STRUCT)) {
                    CLAY(CLAY_LAYOUT({ .sizing = {CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_INDENT_WIDTH + 16), CLAY__DEFAULT_STRUCT} })) {}
                    CLAY_TEXT(CLAY_STRING("\""), rawTextConfig);
                    CLAY_TEXT(textElementData->text.length > 40 ? (CLAY__INIT(Clay_String) { .length = 40, .chars = textElementData->text.chars }) : textElementData->text, rawTextConfig);
                    if (textElementData->text.length > 40) {
                        CLAY_TEXT(CLAY_STRING("..."), rawTextConfig);
                    }
                    CLAY_TEXT(CLAY_STRING("\""), rawTextConfig);
                }
            } else if (currentElement->childrenOrTextContent.children.length > 0) {
                Clay__OpenElement();
                CLAY_LAYOUT({ .padding = { 8 } });
                Clay__ElementPostConfiguration();
                Clay__OpenElement();
                CLAY_BORDER({ .left = { .width = 1, .color = CLAY__DEBUGVIEW_COLOR_3 }});
                Clay__ElementPostConfiguration();
                CLAY(CLAY_LAYOUT({ .sizing = {CLAY_SIZING_FIXED( CLAY__DEBUGVIEW_INDENT_WIDTH), CLAY__DEFAULT_STRUCT}, .childAlignment = { .x = CLAY_ALIGN_X_RIGHT } })) {}
                Clay__OpenElement();
                CLAY_LAYOUT({ .layoutDirection = CLAY_TOP_TO_BOTTOM });
                Clay__ElementPostConfiguration();
            }

            layoutData.rowCount++;
            if (!(Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT) || (currentElementData && currentElementData->debugData->collapsed))) {
                for (int32_t i = currentElement->childrenOrTextContent.children.length - 1; i >= 0; --i) {
                    Clay__int32_tArray_Add(&dfsBuffer, currentElement->childrenOrTextContent.children.elements[i]);
                    context->treeNodeVisited.internalArray[dfsBuffer.length - 1] = false; // TODO needs to be ranged checked
                }
            }
        }
    }

    if (context->pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        Clay_ElementId collapseButtonId = Clay__HashString(CLAY_STRING("Clay__DebugView_CollapseElement"), 0, 0);
        for (int32_t i = (int)context->pointerOverIds.length - 1; i >= 0; i--) {
            Clay_ElementId *elementId = Clay__ElementIdArray_Get(&context->pointerOverIds, i);
            if (elementId->baseId == collapseButtonId.baseId) {
                Clay_LayoutElementHashMapItem *highlightedItem = Clay__GetHashMapItem(elementId->offset);
                highlightedItem->debugData->collapsed = !highlightedItem->debugData->collapsed;
                break;
            }
        }
    }

    if (highlightedElementId) {
        CLAY(CLAY_ID("Clay__DebugView_ElementHighlight"), CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }), CLAY_FLOATING({ .zIndex = 65535, .parentId = highlightedElementId })) {
            CLAY(CLAY_ID("Clay__DebugView_ElementHighlightRectangle"), CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }), CLAY_RECTANGLE({.color = Clay__debugViewHighlightColor })) {}
        }
    }
    return layoutData;
}

void Clay__RenderDebugLayoutSizing(Clay_SizingAxis sizing, Clay_TextElementConfig *infoTextConfig) {
    Clay_String sizingLabel = CLAY_STRING("GROW");
    if (sizing.type == CLAY__SIZING_TYPE_FIT) {
        sizingLabel = CLAY_STRING("FIT");
    } else if (sizing.type == CLAY__SIZING_TYPE_PERCENT) {
        sizingLabel = CLAY_STRING("PERCENT");
    }
    CLAY_TEXT(sizingLabel, infoTextConfig);
    if (sizing.type == CLAY__SIZING_TYPE_GROW || sizing.type == CLAY__SIZING_TYPE_FIT) {
        CLAY_TEXT(CLAY_STRING("("), infoTextConfig);
        if (sizing.size.minMax.min != 0) {
            CLAY_TEXT(CLAY_STRING("min: "), infoTextConfig);
            CLAY_TEXT(Clay__IntToString(sizing.size.minMax.min), infoTextConfig);
            if (sizing.size.minMax.max != CLAY__MAXFLOAT) {
                CLAY_TEXT(CLAY_STRING(", "), infoTextConfig);
            }
        }
        if (sizing.size.minMax.max != CLAY__MAXFLOAT) {
            CLAY_TEXT(CLAY_STRING("max: "), infoTextConfig);
            CLAY_TEXT(Clay__IntToString(sizing.size.minMax.max), infoTextConfig);
        }
        CLAY_TEXT(CLAY_STRING(")"), infoTextConfig);
    }
}

void Clay__RenderDebugViewElementConfigHeader(Clay_String elementId, Clay__ElementConfigType type) {
    Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(type);
    Clay_Color backgroundColor = config.color;
    backgroundColor.a = 90;
    CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_GROW(0)}, .padding = CLAY_PADDING_ALL(CLAY__DEBUGVIEW_OUTER_PADDING), .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } })) {
        CLAY(CLAY_LAYOUT({ .padding = { 8, 8, 2, 2 } }), CLAY_RECTANGLE({ .color = backgroundColor, .cornerRadius = CLAY_CORNER_RADIUS(4) }), CLAY_BORDER_OUTSIDE_RADIUS(1, config.color, 4)) {
            CLAY_TEXT(config.label, CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
        }
        CLAY(CLAY_LAYOUT({ .sizing = { .width = CLAY_SIZING_GROW(0) } })) {}
        CLAY_TEXT(elementId, CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16, .wrapMode = CLAY_TEXT_WRAP_NONE }));
    }
}

void Clay__RenderDebugViewColor(Clay_Color color, Clay_TextElementConfig *textConfig) {
    CLAY(CLAY_LAYOUT({ .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} })) {
        CLAY_TEXT(CLAY_STRING("{ r: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.r), textConfig);
        CLAY_TEXT(CLAY_STRING(", g: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.g), textConfig);
        CLAY_TEXT(CLAY_STRING(", b: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.b), textConfig);
        CLAY_TEXT(CLAY_STRING(", a: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.a), textConfig);
        CLAY_TEXT(CLAY_STRING(" }"), textConfig);
        CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_FIXED(10), CLAY__DEFAULT_STRUCT } })) {}
        CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 8), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 8)} }), CLAY_BORDER_OUTSIDE_RADIUS(1, CLAY__DEBUGVIEW_COLOR_4, 4)) {
            CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 8), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 8)} }), CLAY_RECTANGLE({ .color = color, .cornerRadius = CLAY_CORNER_RADIUS(4) })) {}
        }
    }
}

void Clay__RenderDebugViewCornerRadius(Clay_CornerRadius cornerRadius, Clay_TextElementConfig *textConfig) {
    CLAY(CLAY_LAYOUT({ .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} })) {
        CLAY_TEXT(CLAY_STRING("{ topLeft: "), textConfig);
        CLAY_TEXT(Clay__IntToString(cornerRadius.topLeft), textConfig);
        CLAY_TEXT(CLAY_STRING(", topRight: "), textConfig);
        CLAY_TEXT(Clay__IntToString(cornerRadius.topRight), textConfig);
        CLAY_TEXT(CLAY_STRING(", bottomLeft: "), textConfig);
        CLAY_TEXT(Clay__IntToString(cornerRadius.bottomLeft), textConfig);
        CLAY_TEXT(CLAY_STRING(", bottomRight: "), textConfig);
        CLAY_TEXT(Clay__IntToString(cornerRadius.bottomRight), textConfig);
        CLAY_TEXT(CLAY_STRING(" }"), textConfig);
    }
}

void Clay__RenderDebugViewBorder(int32_t index, Clay_Border border, Clay_TextElementConfig *textConfig) {
    (void) index;
    CLAY(CLAY_LAYOUT({ .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} })) {
        CLAY_TEXT(CLAY_STRING("{ width: "), textConfig);
        CLAY_TEXT(Clay__IntToString(border.width), textConfig);
        CLAY_TEXT(CLAY_STRING(", color: "), textConfig);
        Clay__RenderDebugViewColor(border.color, textConfig);
        CLAY_TEXT(CLAY_STRING(" }"), textConfig);
    }
}

void HandleDebugViewCloseButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerInfo, intptr_t userData) {
    Clay_Context* context = Clay_GetCurrentContext();
    (void) elementId; (void) pointerInfo; (void) userData;
    if (pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        context->debugModeEnabled = false;
    }
}

void Clay__RenderDebugView() {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay_ElementId closeButtonId = Clay__HashString(CLAY_STRING("Clay__DebugViewTopHeaderCloseButtonOuter"), 0, 0);
    if (context->pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        for (int32_t i = 0; i < context->pointerOverIds.length; ++i) {
            Clay_ElementId *elementId = Clay__ElementIdArray_Get(&context->pointerOverIds, i);
            if (elementId->id == closeButtonId.id) {
                context->debugModeEnabled = false;
                return;
            }
        }
    }

    uint32_t initialRootsLength = context->layoutElementTreeRoots.length;
    uint32_t initialElementsLength = context->layoutElements.length;
    Clay_TextElementConfig *infoTextConfig = CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16, .wrapMode = CLAY_TEXT_WRAP_NONE });
    Clay_TextElementConfig *infoTitleConfig = CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16, .wrapMode = CLAY_TEXT_WRAP_NONE });
    Clay_ElementId scrollId = Clay__HashString(CLAY_STRING("Clay__DebugViewOuterScrollPane"), 0, 0);
    float scrollYOffset = 0;
    bool pointerInDebugView = context->pointerInfo.position.y < context->layoutDimensions.height - 300;
    for (int32_t i = 0; i < context->scrollContainerDatas.length; ++i) {
        Clay__ScrollContainerDataInternal *scrollContainerData = Clay__ScrollContainerDataInternalArray_Get(&context->scrollContainerDatas, i);
        if (scrollContainerData->elementId == scrollId.id) {
            if (!context->externalScrollHandlingEnabled) {
                scrollYOffset = scrollContainerData->scrollPosition.y;
            } else {
                pointerInDebugView = context->pointerInfo.position.y + scrollContainerData->scrollPosition.y < context->layoutDimensions.height - 300;
            }
            break;
        }
    }
    int32_t highlightedRow = pointerInDebugView
            ? (int32_t)((context->pointerInfo.position.y - scrollYOffset) / (float)CLAY__DEBUGVIEW_ROW_HEIGHT) - 1
            : -1;
    if (context->pointerInfo.position.x < context->layoutDimensions.width - (float)Clay__debugViewWidth) {
        highlightedRow = -1;
    }
    Clay__RenderDebugLayoutData layoutData = CLAY__DEFAULT_STRUCT;
    CLAY(CLAY_ID("Clay__DebugView"),
        CLAY_FLOATING({ .zIndex = 65000, .parentId = Clay__HashString(CLAY_STRING("Clay__RootContainer"), 0, 0).id, .attachment = { .element = CLAY_ATTACH_POINT_LEFT_CENTER, .parent = CLAY_ATTACH_POINT_RIGHT_CENTER }}),
        CLAY_LAYOUT({ .sizing = { CLAY_SIZING_FIXED((float)Clay__debugViewWidth) , CLAY_SIZING_FIXED(context->layoutDimensions.height) }, .layoutDirection = CLAY_TOP_TO_BOTTOM }),
        CLAY_BORDER({ .bottom = { .width = 1, .color = CLAY__DEBUGVIEW_COLOR_3 }})
    ) {
        CLAY(CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} }), CLAY_RECTANGLE({ .color = CLAY__DEBUGVIEW_COLOR_2 })) {
            CLAY_TEXT(CLAY_STRING("Clay Debug Tools"), infoTextConfig);
            CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_GROW(0) } })) {}
            // Close button
            CLAY(CLAY_BORDER_OUTSIDE_RADIUS(1, (CLAY__INIT(Clay_Color){217,91,67,255}), 4),
                CLAY_LAYOUT({ .sizing = {CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 10), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 10)}, .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER} }),
                CLAY_RECTANGLE({ .color = {217,91,67,80} }),
                Clay_OnHover(HandleDebugViewCloseButtonInteraction, 0)
            ) {
                CLAY_TEXT(CLAY_STRING("x"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
            }
        }
        CLAY(CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1)} }), CLAY_RECTANGLE({ .color = CLAY__DEBUGVIEW_COLOR_3 })) {}
        CLAY(Clay__AttachId(scrollId), CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }), CLAY_SCROLL({ .horizontal = true, .vertical = true })) {
            CLAY(CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .layoutDirection = CLAY_TOP_TO_BOTTOM }), CLAY_RECTANGLE({ .color = ((initialElementsLength + initialRootsLength) & 1) == 0 ? CLAY__DEBUGVIEW_COLOR_2 : CLAY__DEBUGVIEW_COLOR_1 })) {
                Clay_ElementId panelContentsId = Clay__HashString(CLAY_STRING("Clay__DebugViewPaneOuter"), 0, 0);
                // Element list
                CLAY(Clay__AttachId(panelContentsId), CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }), CLAY_FLOATING({ .zIndex = 65001, .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH })) {
                    CLAY(CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = { CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING }, .layoutDirection = CLAY_TOP_TO_BOTTOM })) {
                        layoutData = Clay__RenderDebugLayoutElementsList((int32_t)initialRootsLength, highlightedRow);
                    }
                }
                float contentWidth = Clay__GetHashMapItem(panelContentsId.id)->layoutElement->dimensions.width;
                CLAY(CLAY_LAYOUT({ .sizing = {CLAY_SIZING_FIXED(contentWidth), CLAY__DEFAULT_STRUCT}, .layoutDirection = CLAY_TOP_TO_BOTTOM })) {}
                for (int32_t i = 0; i < layoutData.rowCount; i++) {
                    Clay_Color rowColor = (i & 1) == 0 ? CLAY__DEBUGVIEW_COLOR_2 : CLAY__DEBUGVIEW_COLOR_1;
                    if (i == layoutData.selectedElementRowIndex) {
                        rowColor = CLAY__DEBUGVIEW_COLOR_SELECTED_ROW;
                    }
                    if (i == highlightedRow) {
                        rowColor.r *= 1.25f;
                        rowColor.g *= 1.25f;
                        rowColor.b *= 1.25f;
                    }
                    CLAY(CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .layoutDirection = CLAY_TOP_TO_BOTTOM }), CLAY_RECTANGLE({ .color = rowColor })) {}
                }
            }
        }
        CLAY(CLAY_LAYOUT({ .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(1)} }), CLAY_RECTANGLE({ .color = CLAY__DEBUGVIEW_COLOR_3 })) {}
        if (context->debugSelectedElementId != 0) {
            Clay_LayoutElementHashMapItem *selectedItem = Clay__GetHashMapItem(context->debugSelectedElementId);
            CLAY(
                CLAY_SCROLL({ .vertical = true }),
                CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(300)}, .layoutDirection = CLAY_TOP_TO_BOTTOM }),
                CLAY_RECTANGLE({ .color = CLAY__DEBUGVIEW_COLOR_2 }),
                CLAY_BORDER({ .betweenChildren = { .width = 1, .color = CLAY__DEBUGVIEW_COLOR_3 }})
            ) {
                CLAY(CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT + 8)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING}, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} })) {
                    CLAY_TEXT(CLAY_STRING("Layout Config"), infoTextConfig);
                    CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_GROW(0) } })) {}
                    if (selectedItem->elementId.stringId.length != 0) {
                        CLAY_TEXT(selectedItem->elementId.stringId, infoTitleConfig);
                        if (selectedItem->elementId.offset != 0) {
                            CLAY_TEXT(CLAY_STRING(" ("), infoTitleConfig);
                            CLAY_TEXT(Clay__IntToString(selectedItem->elementId.offset), infoTitleConfig);
                            CLAY_TEXT(CLAY_STRING(")"), infoTitleConfig);
                        }
                    }
                }
                Clay_Padding attributeConfigPadding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING, 8, 8};
                // Clay_LayoutConfig debug info
                CLAY(CLAY_LAYOUT({ .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM })) {
                    // .boundingBox
                    CLAY_TEXT(CLAY_STRING("Bounding Box"), infoTitleConfig);
                    CLAY(CLAY_LAYOUT(CLAY__DEFAULT_STRUCT)) {
                        CLAY_TEXT(CLAY_STRING("{ x: "), infoTextConfig);
                        CLAY_TEXT(Clay__IntToString(selectedItem->boundingBox.x), infoTextConfig);
                        CLAY_TEXT(CLAY_STRING(", y: "), infoTextConfig);
                        CLAY_TEXT(Clay__IntToString(selectedItem->boundingBox.y), infoTextConfig);
                        CLAY_TEXT(CLAY_STRING(", width: "), infoTextConfig);
                        CLAY_TEXT(Clay__IntToString(selectedItem->boundingBox.width), infoTextConfig);
                        CLAY_TEXT(CLAY_STRING(", height: "), infoTextConfig);
                        CLAY_TEXT(Clay__IntToString(selectedItem->boundingBox.height), infoTextConfig);
                        CLAY_TEXT(CLAY_STRING(" }"), infoTextConfig);
                    }
                    // .layoutDirection
                    CLAY_TEXT(CLAY_STRING("Layout Direction"), infoTitleConfig);
                    Clay_LayoutConfig *layoutConfig = selectedItem->layoutElement->layoutConfig;
                    CLAY_TEXT(layoutConfig->layoutDirection == CLAY_TOP_TO_BOTTOM ? CLAY_STRING("TOP_TO_BOTTOM") : CLAY_STRING("LEFT_TO_RIGHT"), infoTextConfig);
                    // .sizing
                    CLAY_TEXT(CLAY_STRING("Sizing"), infoTitleConfig);
                    CLAY(CLAY_LAYOUT(CLAY__DEFAULT_STRUCT)) {
                        CLAY_TEXT(CLAY_STRING("width: "), infoTextConfig);
                        Clay__RenderDebugLayoutSizing(layoutConfig->sizing.width, infoTextConfig);
                    }
                    CLAY(CLAY_LAYOUT(CLAY__DEFAULT_STRUCT)) {
                        CLAY_TEXT(CLAY_STRING("height: "), infoTextConfig);
                        Clay__RenderDebugLayoutSizing(layoutConfig->sizing.height, infoTextConfig);
                    }
                    // .padding
                    CLAY_TEXT(CLAY_STRING("Padding"), infoTitleConfig);
                    CLAY(CLAY_ID("Clay__DebugViewElementInfoPadding")) {
                        CLAY_TEXT(CLAY_STRING("{ left: "), infoTextConfig);
                        CLAY_TEXT(Clay__IntToString(layoutConfig->padding.left), infoTextConfig);
                        CLAY_TEXT(CLAY_STRING(", right: "), infoTextConfig);
                        CLAY_TEXT(Clay__IntToString(layoutConfig->padding.right), infoTextConfig);
                        CLAY_TEXT(CLAY_STRING(", top: "), infoTextConfig);
                        CLAY_TEXT(Clay__IntToString(layoutConfig->padding.top), infoTextConfig);
                        CLAY_TEXT(CLAY_STRING(", bottom: "), infoTextConfig);
                        CLAY_TEXT(Clay__IntToString(layoutConfig->padding.bottom), infoTextConfig);
                        CLAY_TEXT(CLAY_STRING(" }"), infoTextConfig);
                    }
                    // .childGap
                    CLAY_TEXT(CLAY_STRING("Child Gap"), infoTitleConfig);
                    CLAY_TEXT(Clay__IntToString(layoutConfig->childGap), infoTextConfig);
                    // .childAlignment
                    CLAY_TEXT(CLAY_STRING("Child Alignment"), infoTitleConfig);
                    CLAY(CLAY_LAYOUT(CLAY__DEFAULT_STRUCT)) {
                        CLAY_TEXT(CLAY_STRING("{ x: "), infoTextConfig);
                        Clay_String alignX = CLAY_STRING("LEFT");
                        if (layoutConfig->childAlignment.x == CLAY_ALIGN_X_CENTER) {
                            alignX = CLAY_STRING("CENTER");
                        } else if (layoutConfig->childAlignment.x == CLAY_ALIGN_X_RIGHT) {
                            alignX = CLAY_STRING("RIGHT");
                        }
                        CLAY_TEXT(alignX, infoTextConfig);
                        CLAY_TEXT(CLAY_STRING(", y: "), infoTextConfig);
                        Clay_String alignY = CLAY_STRING("TOP");
                        if (layoutConfig->childAlignment.y == CLAY_ALIGN_Y_CENTER) {
                            alignY = CLAY_STRING("CENTER");
                        } else if (layoutConfig->childAlignment.y == CLAY_ALIGN_Y_BOTTOM) {
                            alignY = CLAY_STRING("BOTTOM");
                        }
                        CLAY_TEXT(alignY, infoTextConfig);
                        CLAY_TEXT(CLAY_STRING(" }"), infoTextConfig);
                    }
                }
                for (int32_t elementConfigIndex = 0; elementConfigIndex < selectedItem->layoutElement->elementConfigs.length; ++elementConfigIndex) {
                    Clay_ElementConfig *elementConfig = Clay__ElementConfigArraySlice_Get(&selectedItem->layoutElement->elementConfigs, elementConfigIndex);
                    Clay__RenderDebugViewElementConfigHeader(selectedItem->elementId.stringId, elementConfig->type);
                    switch (elementConfig->type) {
                        case CLAY__ELEMENT_CONFIG_TYPE_RECTANGLE: {
                            Clay_RectangleElementConfig *rectangleConfig = elementConfig->config.rectangleElementConfig;
                            CLAY(CLAY_LAYOUT({ .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM })) {
                                // .color
                                CLAY_TEXT(CLAY_STRING("Color"), infoTitleConfig);
                                Clay__RenderDebugViewColor(rectangleConfig->color, infoTextConfig);
                                // .cornerRadius
                                CLAY_TEXT(CLAY_STRING("Corner Radius"), infoTitleConfig);
                                Clay__RenderDebugViewCornerRadius(rectangleConfig->cornerRadius, infoTextConfig);
                            }
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_TEXT: {
                            Clay_TextElementConfig *textConfig = elementConfig->config.textElementConfig;
                            CLAY(CLAY_LAYOUT({ .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM })) {
                                // .fontSize
                                CLAY_TEXT(CLAY_STRING("Font Size"), infoTitleConfig);
                                CLAY_TEXT(Clay__IntToString(textConfig->fontSize), infoTextConfig);
                                // .fontId
                                CLAY_TEXT(CLAY_STRING("Font ID"), infoTitleConfig);
                                CLAY_TEXT(Clay__IntToString(textConfig->fontId), infoTextConfig);
                                // .lineHeight
                                CLAY_TEXT(CLAY_STRING("Line Height"), infoTitleConfig);
                                CLAY_TEXT(textConfig->lineHeight == 0 ? CLAY_STRING("auto") : Clay__IntToString(textConfig->lineHeight), infoTextConfig);
                                // .letterSpacing
                                CLAY_TEXT(CLAY_STRING("Letter Spacing"), infoTitleConfig);
                                CLAY_TEXT(Clay__IntToString(textConfig->letterSpacing), infoTextConfig);
                                // .lineSpacing
                                CLAY_TEXT(CLAY_STRING("Wrap Mode"), infoTitleConfig);
                                Clay_String wrapMode = CLAY_STRING("WORDS");
                                if (textConfig->wrapMode == CLAY_TEXT_WRAP_NONE) {
                                    wrapMode = CLAY_STRING("NONE");
                                } else if (textConfig->wrapMode == CLAY_TEXT_WRAP_NEWLINES) {
                                    wrapMode = CLAY_STRING("NEWLINES");
                                }
                                CLAY_TEXT(wrapMode, infoTextConfig);
                                // .textColor
                                CLAY_TEXT(CLAY_STRING("Text Color"), infoTitleConfig);
                                Clay__RenderDebugViewColor(textConfig->textColor, infoTextConfig);
                            }
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_IMAGE: {
                            Clay_ImageElementConfig *imageConfig = elementConfig->config.imageElementConfig;
                            CLAY(CLAY_ID("Clay__DebugViewElementInfoImageBody"), CLAY_LAYOUT({ .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM })) {
                                // .sourceDimensions
                                CLAY_TEXT(CLAY_STRING("Source Dimensions"), infoTitleConfig);
                                CLAY(CLAY_ID("Clay__DebugViewElementInfoImageDimensions")) {
                                    CLAY_TEXT(CLAY_STRING("{ width: "), infoTextConfig);
                                    CLAY_TEXT(Clay__IntToString(imageConfig->sourceDimensions.width), infoTextConfig);
                                    CLAY_TEXT(CLAY_STRING(", height: "), infoTextConfig);
                                    CLAY_TEXT(Clay__IntToString(imageConfig->sourceDimensions.height), infoTextConfig);
                                    CLAY_TEXT(CLAY_STRING(" }"), infoTextConfig);
                                }
                                // Image Preview
                                CLAY_TEXT(CLAY_STRING("Preview"), infoTitleConfig);
                                CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_GROW(0, imageConfig->sourceDimensions.width) }}), Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .imageElementConfig = imageConfig }, CLAY__ELEMENT_CONFIG_TYPE_IMAGE)) {}
                            }
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER: {
                            Clay_ScrollElementConfig *scrollConfig = elementConfig->config.scrollElementConfig;
                            CLAY(CLAY_LAYOUT({ .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM })) {
                                // .vertical
                                CLAY_TEXT(CLAY_STRING("Vertical"), infoTitleConfig);
                                CLAY_TEXT(scrollConfig->vertical ? CLAY_STRING("true") : CLAY_STRING("false") , infoTextConfig);
                                // .horizontal
                                CLAY_TEXT(CLAY_STRING("Horizontal"), infoTitleConfig);
                                CLAY_TEXT(scrollConfig->horizontal ? CLAY_STRING("true") : CLAY_STRING("false") , infoTextConfig);
                            }
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER: {
                            Clay_FloatingElementConfig *floatingConfig = elementConfig->config.floatingElementConfig;
                            CLAY(CLAY_LAYOUT({ .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM })) {
                                // .offset
                                CLAY_TEXT(CLAY_STRING("Offset"), infoTitleConfig);
                                CLAY(CLAY_LAYOUT(CLAY__DEFAULT_STRUCT)) {
                                    CLAY_TEXT(CLAY_STRING("{ x: "), infoTextConfig);
                                    CLAY_TEXT(Clay__IntToString(floatingConfig->offset.x), infoTextConfig);
                                    CLAY_TEXT(CLAY_STRING(", y: "), infoTextConfig);
                                    CLAY_TEXT(Clay__IntToString(floatingConfig->offset.y), infoTextConfig);
                                    CLAY_TEXT(CLAY_STRING(" }"), infoTextConfig);
                                }
                                // .expand
                                CLAY_TEXT(CLAY_STRING("Expand"), infoTitleConfig);
                                CLAY(CLAY_LAYOUT(CLAY__DEFAULT_STRUCT)) {
                                    CLAY_TEXT(CLAY_STRING("{ width: "), infoTextConfig);
                                    CLAY_TEXT(Clay__IntToString(floatingConfig->expand.width), infoTextConfig);
                                    CLAY_TEXT(CLAY_STRING(", height: "), infoTextConfig);
                                    CLAY_TEXT(Clay__IntToString(floatingConfig->expand.height), infoTextConfig);
                                    CLAY_TEXT(CLAY_STRING(" }"), infoTextConfig);
                                }
                                // .zIndex
                                CLAY_TEXT(CLAY_STRING("z-index"), infoTitleConfig);
                                CLAY_TEXT(Clay__IntToString(floatingConfig->zIndex), infoTextConfig);
                                // .parentId
                                CLAY_TEXT(CLAY_STRING("Parent"), infoTitleConfig);
                                Clay_LayoutElementHashMapItem *hashItem = Clay__GetHashMapItem(floatingConfig->parentId);
                                CLAY_TEXT(hashItem->elementId.stringId, infoTextConfig);
                            }
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER: {
                            Clay_BorderElementConfig *borderConfig = elementConfig->config.borderElementConfig;
                            CLAY(CLAY_ID("Clay__DebugViewElementInfoBorderBody"), CLAY_LAYOUT({ .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM })) {
                                // .left
                                CLAY_TEXT(CLAY_STRING("Left Border"), infoTitleConfig);
                                Clay__RenderDebugViewBorder(1, borderConfig->left, infoTextConfig);
                                // .right
                                CLAY_TEXT(CLAY_STRING("Right Border"), infoTitleConfig);
                                Clay__RenderDebugViewBorder(2, borderConfig->right, infoTextConfig);
                                // .top
                                CLAY_TEXT(CLAY_STRING("Top Border"), infoTitleConfig);
                                Clay__RenderDebugViewBorder(3, borderConfig->top, infoTextConfig);
                                // .bottom
                                CLAY_TEXT(CLAY_STRING("Bottom Border"), infoTitleConfig);
                                Clay__RenderDebugViewBorder(4, borderConfig->bottom, infoTextConfig);
                                // .betweenChildren
                                CLAY_TEXT(CLAY_STRING("Border Between Children"), infoTitleConfig);
                                Clay__RenderDebugViewBorder(5, borderConfig->betweenChildren, infoTextConfig);
                                // .cornerRadius
                                CLAY_TEXT(CLAY_STRING("Corner Radius"), infoTitleConfig);
                                Clay__RenderDebugViewCornerRadius(borderConfig->cornerRadius, infoTextConfig);
                            }
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_CUSTOM:
                        default: break;
                    }
                }
            }
        } else {
            CLAY(CLAY_ID("Clay__DebugViewWarningsScrollPane"), CLAY_LAYOUT({ .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(300)}, .childGap = 6, .layoutDirection = CLAY_TOP_TO_BOTTOM }), CLAY_SCROLL({ .horizontal = true, .vertical = true }), CLAY_RECTANGLE({ .color = CLAY__DEBUGVIEW_COLOR_2 })) {
                Clay_TextElementConfig *warningConfig = CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16, .wrapMode = CLAY_TEXT_WRAP_NONE });
                CLAY(CLAY_ID("Clay__DebugViewWarningItemHeader"), CLAY_LAYOUT({ .sizing = {.height = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING}, .childGap = 8, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} })) {
                    CLAY_TEXT(CLAY_STRING("Warnings"), warningConfig);
                }
                CLAY(CLAY_ID("Clay__DebugViewWarningsTopBorder"), CLAY_LAYOUT({ .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(1)} }), CLAY_RECTANGLE({ .color = {200, 200, 200, 255} })) {}
                int32_t previousWarningsLength = context->warnings.length;
                for (int32_t i = 0; i < previousWarningsLength; i++) {
                    Clay__Warning warning = context->warnings.internalArray[i];
                    CLAY(CLAY_IDI("Clay__DebugViewWarningItem", i), CLAY_LAYOUT({ .sizing = {.height = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING}, .childGap = 8, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} })) {
                        CLAY_TEXT(warning.baseMessage, warningConfig);
                        if (warning.dynamicMessage.length > 0) {
                            CLAY_TEXT(warning.dynamicMessage, warningConfig);
                        }
                    }
                }
            }
        }
    }
}
#pragma endregion

uint32_t Clay__debugViewWidth = 400;
Clay_Color Clay__debugViewHighlightColor = { 168, 66, 28, 100 };

Clay__WarningArray Clay__WarningArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {
    size_t totalSizeBytes = capacity * sizeof(Clay_String);
    Clay__WarningArray array = {.capacity = capacity, .length = 0};
    uintptr_t nextAllocAddress = arena->nextAllocation + (uintptr_t)arena->memory;
    uintptr_t arenaOffsetAligned = nextAllocAddress + (CLAY__ALIGNMENT(Clay_String) - (nextAllocAddress % CLAY__ALIGNMENT(Clay_String)));
    arenaOffsetAligned -= (uintptr_t)arena->memory;
    if (arenaOffsetAligned + totalSizeBytes <= arena->capacity) {
        array.internalArray = (Clay__Warning*)((uintptr_t)arena->memory + (uintptr_t)arenaOffsetAligned);
        arena->nextAllocation = arenaOffsetAligned + totalSizeBytes;
    }
    else {
        Clay__currentContext->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
            .errorType = CLAY_ERROR_TYPE_ARENA_CAPACITY_EXCEEDED,
            .errorText = CLAY_STRING("Clay attempted to allocate memory in its arena, but ran out of capacity. Try increasing the capacity of the arena passed to Clay_Initialize()"),
            .userData = Clay__currentContext->errorHandler.userData });
    }
    return array;
}

Clay__Warning *Clay__WarningArray_Add(Clay__WarningArray *array, Clay__Warning item)
{
    if (array->length < array->capacity) {
        array->internalArray[array->length++] = item;
        return &array->internalArray[array->length - 1];
    }
    return &CLAY__WARNING_DEFAULT;
}

void* Clay__Array_Allocate_Arena(int32_t capacity, uint32_t itemSize, uint32_t alignment, Clay_Arena *arena)
{
    size_t totalSizeBytes = capacity * itemSize;
    uintptr_t nextAllocAddress = arena->nextAllocation + (uintptr_t)arena->memory;
    uintptr_t arenaOffsetAligned = nextAllocAddress + (alignment - (nextAllocAddress % alignment));
    arenaOffsetAligned -= (uintptr_t)arena->memory;
    if (arenaOffsetAligned + totalSizeBytes <= arena->capacity) {
        arena->nextAllocation = arenaOffsetAligned + totalSizeBytes;
        return (void*)((uintptr_t)arena->memory + (uintptr_t)arenaOffsetAligned);
    }
    else {
        Clay__currentContext->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                .errorType = CLAY_ERROR_TYPE_ARENA_CAPACITY_EXCEEDED,
                .errorText = CLAY_STRING("Clay attempted to allocate memory in its arena, but ran out of capacity. Try increasing the capacity of the arena passed to Clay_Initialize()"),
                .userData = Clay__currentContext->errorHandler.userData });
    }
    return CLAY__NULL;
}

bool Clay__Array_RangeCheck(int32_t index, int32_t length)
{
    if (index < length && index >= 0) {
        return true;
    }
    Clay_Context* context = Clay_GetCurrentContext();
    context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
            .errorType = CLAY_ERROR_TYPE_INTERNAL_ERROR,
            .errorText = CLAY_STRING("Clay attempted to make an out of bounds array access. This is an internal error and is likely a bug."),
            .userData = context->errorHandler.userData });
    return false;
}

bool Clay__Array_AddCapacityCheck(int32_t length, int32_t capacity)
{
    if (length < capacity) {
        return true;
    }
    Clay_Context* context = Clay_GetCurrentContext();
    context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
        .errorType = CLAY_ERROR_TYPE_INTERNAL_ERROR,
        .errorText = CLAY_STRING("Clay attempted to make an out of bounds array access. This is an internal error and is likely a bug."),
        .userData = context->errorHandler.userData });
    return false;
}

// PUBLIC API FROM HERE ---------------------------------------

CLAY_WASM_EXPORT("Clay_MinMemorySize")
uint32_t Clay_MinMemorySize(void) {
    Clay_Context fakeContext = {
        .maxElementCount = Clay__defaultMaxElementCount,
        .maxMeasureTextCacheWordCount = Clay__defaultMaxMeasureTextWordCacheCount,
        .internalArena = {
            .capacity = SIZE_MAX,
            .memory = NULL,
        }
    };
    Clay_Context* currentContext = Clay_GetCurrentContext();
    if (currentContext) {
        fakeContext.maxElementCount = currentContext->maxElementCount;
        fakeContext.maxMeasureTextCacheWordCount = currentContext->maxElementCount;
    }
    // Reserve space in the arena for the context, important for calculating min memory size correctly
    Clay__Context_Allocate_Arena(&fakeContext.internalArena);
    Clay__InitializePersistentMemory(&fakeContext);
    Clay__InitializeEphemeralMemory(&fakeContext);
    return fakeContext.internalArena.nextAllocation;
}

CLAY_WASM_EXPORT("Clay_CreateArenaWithCapacityAndMemory")
Clay_Arena Clay_CreateArenaWithCapacityAndMemory(uint32_t capacity, void *offset) {
    Clay_Arena arena = {
        .capacity = capacity,
        .memory = (char *)offset
    };
    return arena;
}

#ifndef CLAY_WASM
void Clay_SetMeasureTextFunction(Clay_Dimensions (*measureTextFunction)(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData), uintptr_t userData) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__MeasureText = measureTextFunction;
    context->mesureTextUserData = userData;
}
void Clay_SetQueryScrollOffsetFunction(Clay_Vector2 (*queryScrollOffsetFunction)(uint32_t elementId, uintptr_t userData), uintptr_t userData) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__QueryScrollOffset = queryScrollOffsetFunction;
    context->queryScrollOffsetUserData = userData;
}
#endif

CLAY_WASM_EXPORT("Clay_SetLayoutDimensions")
void Clay_SetLayoutDimensions(Clay_Dimensions dimensions) {
    Clay_GetCurrentContext()->layoutDimensions = dimensions;
}

CLAY_WASM_EXPORT("Clay_SetPointerState")
void Clay_SetPointerState(Clay_Vector2 position, bool isPointerDown) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return;
    }
    context->pointerInfo.position = position;
    context->pointerOverIds.length = 0;
    Clay__int32_tArray dfsBuffer = context->layoutElementChildrenBuffer;
    for (int32_t rootIndex = context->layoutElementTreeRoots.length - 1; rootIndex >= 0; --rootIndex) {
        dfsBuffer.length = 0;
        Clay__LayoutElementTreeRoot *root = Clay__LayoutElementTreeRootArray_Get(&context->layoutElementTreeRoots, rootIndex);
        Clay__int32_tArray_Add(&dfsBuffer, (int32_t)root->layoutElementIndex);
        context->treeNodeVisited.internalArray[0] = false;
        bool found = false;
        while (dfsBuffer.length > 0) {
            if (context->treeNodeVisited.internalArray[dfsBuffer.length - 1]) {
                dfsBuffer.length--;
                continue;
            }
            context->treeNodeVisited.internalArray[dfsBuffer.length - 1] = true;
            Clay_LayoutElement *currentElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_Get(&dfsBuffer, (int)dfsBuffer.length - 1));
            Clay_LayoutElementHashMapItem *mapItem = Clay__GetHashMapItem(currentElement->id); // TODO think of a way around this, maybe the fact that it's essentially a binary tree limits the cost, but the worst case is not great
            Clay_BoundingBox elementBox = mapItem->boundingBox;
            elementBox.x -= root->pointerOffset.x;
            elementBox.y -= root->pointerOffset.y;
            if (mapItem) {
                if ((Clay__PointIsInsideRect(position, elementBox))) {
                    if (mapItem->onHoverFunction) {
                        mapItem->onHoverFunction(mapItem->elementId, context->pointerInfo, mapItem->hoverFunctionUserData);
                    }
                    Clay__ElementIdArray_Add(&context->pointerOverIds, mapItem->elementId);
                    found = true;
                }
                if (Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT)) {
                    dfsBuffer.length--;
                    continue;
                }
                for (int32_t i = currentElement->childrenOrTextContent.children.length - 1; i >= 0; --i) {
                    Clay__int32_tArray_Add(&dfsBuffer, currentElement->childrenOrTextContent.children.elements[i]);
                    context->treeNodeVisited.internalArray[dfsBuffer.length - 1] = false; // TODO needs to be ranged checked
                }
            } else {
                dfsBuffer.length--;
            }
        }

        Clay_LayoutElement *rootElement = Clay_LayoutElementArray_Get(&context->layoutElements, root->layoutElementIndex);
        if (found && Clay__ElementHasConfig(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER) &&
                Clay__FindElementConfigWithType(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER).floatingElementConfig->pointerCaptureMode == CLAY_POINTER_CAPTURE_MODE_CAPTURE) {
            break;
        }
    }

    if (isPointerDown) {
        if (context->pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
            context->pointerInfo.state = CLAY_POINTER_DATA_PRESSED;
        } else if (context->pointerInfo.state != CLAY_POINTER_DATA_PRESSED) {
            context->pointerInfo.state = CLAY_POINTER_DATA_PRESSED_THIS_FRAME;
        }
    } else {
        if (context->pointerInfo.state == CLAY_POINTER_DATA_RELEASED_THIS_FRAME) {
            context->pointerInfo.state = CLAY_POINTER_DATA_RELEASED;
        } else if (context->pointerInfo.state != CLAY_POINTER_DATA_RELEASED)  {
            context->pointerInfo.state = CLAY_POINTER_DATA_RELEASED_THIS_FRAME;
        }
    }
}

CLAY_WASM_EXPORT("Clay_Initialize")
Clay_Context* Clay_Initialize(Clay_Arena arena, Clay_Dimensions layoutDimensions, Clay_ErrorHandler errorHandler) {
    Clay_Context *context = Clay__Context_Allocate_Arena(&arena);
    if (context == NULL) return NULL;
    // DEFAULTS
    Clay_Context *oldContext = Clay_GetCurrentContext();
    *context = CLAY__INIT(Clay_Context) {
        .maxElementCount = oldContext ? oldContext->maxElementCount : Clay__defaultMaxElementCount,
        .maxMeasureTextCacheWordCount = oldContext ? oldContext->maxMeasureTextCacheWordCount : Clay__defaultMaxElementCount * 2,
        .errorHandler = errorHandler.errorHandlerFunction ? errorHandler : CLAY__INIT(Clay_ErrorHandler) { Clay__ErrorHandlerFunctionDefault },
        .layoutDimensions = layoutDimensions,
        .internalArena = arena,
    };
    Clay_SetCurrentContext(context);
    Clay__InitializePersistentMemory(context);
    Clay__InitializeEphemeralMemory(context);
    for (int32_t i = 0; i < context->layoutElementsHashMap.capacity; ++i) {
        context->layoutElementsHashMap.internalArray[i] = -1;
    }
    for (int32_t i = 0; i < context->measureTextHashMap.capacity; ++i) {
        context->measureTextHashMap.internalArray[i] = 0;
    }
    context->measureTextHashMapInternal.length = 1; // Reserve the 0 value to mean "no next element"
    context->layoutDimensions = layoutDimensions;
    return context;
}

CLAY_WASM_EXPORT("Clay_GetCurrentContext")
Clay_Context* Clay_GetCurrentContext(void) {
    return Clay__currentContext;
}

CLAY_WASM_EXPORT("Clay_SetCurrentContext")
void Clay_SetCurrentContext(Clay_Context* context) {
    Clay__currentContext = context;
}

CLAY_WASM_EXPORT("Clay_UpdateScrollContainers")
void Clay_UpdateScrollContainers(bool enableDragScrolling, Clay_Vector2 scrollDelta, float deltaTime) {
    Clay_Context* context = Clay_GetCurrentContext();
    bool isPointerActive = enableDragScrolling && (context->pointerInfo.state == CLAY_POINTER_DATA_PRESSED || context->pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME);
    // Don't apply scroll events to ancestors of the inner element
    int32_t highestPriorityElementIndex = -1;
    Clay__ScrollContainerDataInternal *highestPriorityScrollData = CLAY__NULL;
    for (int32_t i = 0; i < context->scrollContainerDatas.length; i++) {
        Clay__ScrollContainerDataInternal *scrollData = Clay__ScrollContainerDataInternalArray_Get(&context->scrollContainerDatas, i);
        if (!scrollData->openThisFrame) {
            Clay__ScrollContainerDataInternalArray_RemoveSwapback(&context->scrollContainerDatas, i);
            continue;
        }
        scrollData->openThisFrame = false;
        Clay_LayoutElementHashMapItem *hashMapItem = Clay__GetHashMapItem(scrollData->elementId);
        // Element isn't rendered this frame but scroll offset has been retained
        if (!hashMapItem) {
            Clay__ScrollContainerDataInternalArray_RemoveSwapback(&context->scrollContainerDatas, i);
            continue;
        }

        // Touch / click is released
        if (!isPointerActive && scrollData->pointerScrollActive) {
            float xDiff = scrollData->scrollPosition.x - scrollData->scrollOrigin.x;
            if (xDiff < -10 || xDiff > 10) {
                scrollData->scrollMomentum.x = (scrollData->scrollPosition.x - scrollData->scrollOrigin.x) / (scrollData->momentumTime * 25);
            }
            float yDiff = scrollData->scrollPosition.y - scrollData->scrollOrigin.y;
            if (yDiff < -10 || yDiff > 10) {
                scrollData->scrollMomentum.y = (scrollData->scrollPosition.y - scrollData->scrollOrigin.y) / (scrollData->momentumTime * 25);
            }
            scrollData->pointerScrollActive = false;

            scrollData->pointerOrigin = CLAY__INIT(Clay_Vector2){0,0};
            scrollData->scrollOrigin = CLAY__INIT(Clay_Vector2){0,0};
            scrollData->momentumTime = 0;
        }

        // Apply existing momentum
        scrollData->scrollPosition.x += scrollData->scrollMomentum.x;
        scrollData->scrollMomentum.x *= 0.95f;
        bool scrollOccurred = scrollDelta.x != 0 || scrollDelta.y != 0;
        if ((scrollData->scrollMomentum.x > -0.1f && scrollData->scrollMomentum.x < 0.1f) || scrollOccurred) {
            scrollData->scrollMomentum.x = 0;
        }
        scrollData->scrollPosition.x = CLAY__MIN(CLAY__MAX(scrollData->scrollPosition.x, -(CLAY__MAX(scrollData->contentSize.width - scrollData->layoutElement->dimensions.width, 0))), 0);

        scrollData->scrollPosition.y += scrollData->scrollMomentum.y;
        scrollData->scrollMomentum.y *= 0.95f;
        if ((scrollData->scrollMomentum.y > -0.1f && scrollData->scrollMomentum.y < 0.1f) || scrollOccurred) {
            scrollData->scrollMomentum.y = 0;
        }
        scrollData->scrollPosition.y = CLAY__MIN(CLAY__MAX(scrollData->scrollPosition.y, -(CLAY__MAX(scrollData->contentSize.height - scrollData->layoutElement->dimensions.height, 0))), 0);

        for (int32_t j = 0; j < context->pointerOverIds.length; ++j) { // TODO n & m are small here but this being n*m gives me the creeps
            if (scrollData->layoutElement->id == Clay__ElementIdArray_Get(&context->pointerOverIds, j)->id) {
                highestPriorityElementIndex = j;
                highestPriorityScrollData = scrollData;
            }
        }
    }

    if (highestPriorityElementIndex > -1 && highestPriorityScrollData) {
        Clay_LayoutElement *scrollElement = highestPriorityScrollData->layoutElement;
        Clay_ScrollElementConfig *scrollConfig = Clay__FindElementConfigWithType(scrollElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER).scrollElementConfig;
        bool canScrollVertically = scrollConfig->vertical && highestPriorityScrollData->contentSize.height > scrollElement->dimensions.height;
        bool canScrollHorizontally = scrollConfig->horizontal && highestPriorityScrollData->contentSize.width > scrollElement->dimensions.width;
        // Handle wheel scroll
        if (canScrollVertically) {
            highestPriorityScrollData->scrollPosition.y = highestPriorityScrollData->scrollPosition.y + scrollDelta.y * 10;
        }
        if (canScrollHorizontally) {
            highestPriorityScrollData->scrollPosition.x = highestPriorityScrollData->scrollPosition.x + scrollDelta.x * 10;
        }
        // Handle click / touch scroll
        if (isPointerActive) {
            highestPriorityScrollData->scrollMomentum = CLAY__INIT(Clay_Vector2)CLAY__DEFAULT_STRUCT;
            if (!highestPriorityScrollData->pointerScrollActive) {
                highestPriorityScrollData->pointerOrigin = context->pointerInfo.position;
                highestPriorityScrollData->scrollOrigin = highestPriorityScrollData->scrollPosition;
                highestPriorityScrollData->pointerScrollActive = true;
            } else {
                float scrollDeltaX = 0, scrollDeltaY = 0;
                if (canScrollHorizontally) {
                    float oldXScrollPosition = highestPriorityScrollData->scrollPosition.x;
                    highestPriorityScrollData->scrollPosition.x = highestPriorityScrollData->scrollOrigin.x + (context->pointerInfo.position.x - highestPriorityScrollData->pointerOrigin.x);
                    highestPriorityScrollData->scrollPosition.x = CLAY__MAX(CLAY__MIN(highestPriorityScrollData->scrollPosition.x, 0), -(highestPriorityScrollData->contentSize.width - highestPriorityScrollData->boundingBox.width));
                    scrollDeltaX = highestPriorityScrollData->scrollPosition.x - oldXScrollPosition;
                }
                if (canScrollVertically) {
                    float oldYScrollPosition = highestPriorityScrollData->scrollPosition.y;
                    highestPriorityScrollData->scrollPosition.y = highestPriorityScrollData->scrollOrigin.y + (context->pointerInfo.position.y - highestPriorityScrollData->pointerOrigin.y);
                    highestPriorityScrollData->scrollPosition.y = CLAY__MAX(CLAY__MIN(highestPriorityScrollData->scrollPosition.y, 0), -(highestPriorityScrollData->contentSize.height - highestPriorityScrollData->boundingBox.height));
                    scrollDeltaY = highestPriorityScrollData->scrollPosition.y - oldYScrollPosition;
                }
                if (scrollDeltaX > -0.1f && scrollDeltaX < 0.1f && scrollDeltaY > -0.1f && scrollDeltaY < 0.1f && highestPriorityScrollData->momentumTime > 0.15f) {
                    highestPriorityScrollData->momentumTime = 0;
                    highestPriorityScrollData->pointerOrigin = context->pointerInfo.position;
                    highestPriorityScrollData->scrollOrigin = highestPriorityScrollData->scrollPosition;
                } else {
                     highestPriorityScrollData->momentumTime += deltaTime;
                }
            }
        }
        // Clamp any changes to scroll position to the maximum size of the contents
        if (canScrollVertically) {
            highestPriorityScrollData->scrollPosition.y = CLAY__MAX(CLAY__MIN(highestPriorityScrollData->scrollPosition.y, 0), -(highestPriorityScrollData->contentSize.height - scrollElement->dimensions.height));
        }
        if (canScrollHorizontally) {
            highestPriorityScrollData->scrollPosition.x = CLAY__MAX(CLAY__MIN(highestPriorityScrollData->scrollPosition.x, 0), -(highestPriorityScrollData->contentSize.width - scrollElement->dimensions.width));
        }
    }
}

CLAY_WASM_EXPORT("Clay_BeginLayout")
void Clay_BeginLayout(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__InitializeEphemeralMemory(context);
    context->generation++;
    context->dynamicElementIndex = 0;
    // Set up the root container that covers the entire window
    Clay_Dimensions rootDimensions = {context->layoutDimensions.width, context->layoutDimensions.height};
    if (context->debugModeEnabled) {
        rootDimensions.width -= (float)Clay__debugViewWidth;
    }
    context->booleanWarnings = CLAY__INIT(Clay_BooleanWarnings) CLAY__DEFAULT_STRUCT;
    Clay__OpenElement();
    CLAY_ID("Clay__RootContainer");
    CLAY_LAYOUT({ .sizing = {CLAY_SIZING_FIXED((rootDimensions.width)), CLAY_SIZING_FIXED(rootDimensions.height)} });
    Clay__ElementPostConfiguration();
    Clay__int32_tArray_Add(&context->openLayoutElementStack, 0);
    Clay__LayoutElementTreeRootArray_Add(&context->layoutElementTreeRoots, CLAY__INIT(Clay__LayoutElementTreeRoot) { .layoutElementIndex = 0 });
}

Clay_TextElementConfig Clay__DebugView_ErrorTextConfig = {.textColor = {255, 0, 0, 255}, .fontSize = 16, .wrapMode = CLAY_TEXT_WRAP_NONE };

CLAY_WASM_EXPORT("Clay_EndLayout")
Clay_RenderCommandArray Clay_EndLayout() {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__CloseElement();
    if (context->debugModeEnabled) {
        context->warningsEnabled = false;
        Clay__RenderDebugView();
        context->warningsEnabled = true;
    }
    if (context->booleanWarnings.maxElementsExceeded) {
        Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand ) { .boundingBox = { context->layoutDimensions.width / 2 - 59 * 4, context->layoutDimensions.height / 2, 0, 0 },  .config = { .textElementConfig = &Clay__DebugView_ErrorTextConfig }, .text = CLAY_STRING("Clay Error: Layout elements exceeded Clay__maxElementCount"), .commandType = CLAY_RENDER_COMMAND_TYPE_TEXT });
    } else {
        Clay__CalculateFinalLayout();
    }
    return context->renderCommands;
}

CLAY_WASM_EXPORT("Clay_GetElementId")
Clay_ElementId Clay_GetElementId(Clay_String idString) {
    return Clay__HashString(idString, 0, 0);
}

CLAY_WASM_EXPORT("Clay_GetElementIdWithIndex")
Clay_ElementId Clay_GetElementIdWithIndex(Clay_String idString, uint32_t index) {
    return Clay__HashString(idString, index, 0);
}

bool Clay_Hovered(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return false;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    // If the element has no id attached at this point, we need to generate one
    if (openLayoutElement->id == 0) {
        Clay__GenerateIdForAnonymousElement(openLayoutElement);
    }
    for (int32_t i = 0; i < context->pointerOverIds.length; ++i) {
        if (Clay__ElementIdArray_Get(&context->pointerOverIds, i)->id == openLayoutElement->id) {
            return true;
        }
    }
    return false;
}

void Clay_OnHover(void (*onHoverFunction)(Clay_ElementId elementId, Clay_PointerData pointerInfo, intptr_t userData), intptr_t userData) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    if (openLayoutElement->id == 0) {
        Clay__GenerateIdForAnonymousElement(openLayoutElement);
    }
    Clay_LayoutElementHashMapItem *hashMapItem = Clay__GetHashMapItem(openLayoutElement->id);
    hashMapItem->onHoverFunction = onHoverFunction;
    hashMapItem->hoverFunctionUserData = userData;
}

CLAY_WASM_EXPORT("Clay_PointerOver")
bool Clay_PointerOver(Clay_ElementId elementId) { // TODO return priority for separating multiple results
    Clay_Context* context = Clay_GetCurrentContext();
    for (int32_t i = 0; i < context->pointerOverIds.length; ++i) {
        if (Clay__ElementIdArray_Get(&context->pointerOverIds, i)->id == elementId.id) {
            return true;
        }
    }
    return false;
}

CLAY_WASM_EXPORT("Clay_GetScrollContainerData")
Clay_ScrollContainerData Clay_GetScrollContainerData(Clay_ElementId id) {
    Clay_Context* context = Clay_GetCurrentContext();
    for (int32_t i = 0; i < context->scrollContainerDatas.length; ++i) {
        Clay__ScrollContainerDataInternal *scrollContainerData = Clay__ScrollContainerDataInternalArray_Get(&context->scrollContainerDatas, i);
        if (scrollContainerData->elementId == id.id) {
            return CLAY__INIT(Clay_ScrollContainerData) {
                .scrollPosition = &scrollContainerData->scrollPosition,
                .scrollContainerDimensions = { scrollContainerData->boundingBox.width, scrollContainerData->boundingBox.height },
                .contentDimensions = scrollContainerData->contentSize,
                .config = *Clay__FindElementConfigWithType(scrollContainerData->layoutElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER).scrollElementConfig,
                .found = true
            };
        }
    }
    return CLAY__INIT(Clay_ScrollContainerData) CLAY__DEFAULT_STRUCT;
}

CLAY_WASM_EXPORT("Clay_GetElementData")
Clay_ElementData Clay_GetElementData(Clay_ElementId id){
    Clay_LayoutElementHashMapItem * item = Clay__GetHashMapItem(id.id);
    if(item == &CLAY__LAYOUT_ELEMENT_HASH_MAP_ITEM_DEFAULT) {
        return CLAY__INIT(Clay_ElementData) CLAY__DEFAULT_STRUCT;
    }

    return CLAY__INIT(Clay_ElementData){
        .boundingBox = item->boundingBox,
        .found = true
    };
}

CLAY_WASM_EXPORT("Clay_SetDebugModeEnabled")
void Clay_SetDebugModeEnabled(bool enabled) {
    Clay_Context* context = Clay_GetCurrentContext();
    context->debugModeEnabled = enabled;
}

CLAY_WASM_EXPORT("Clay_IsDebugModeEnabled")
bool Clay_IsDebugModeEnabled(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    return context->debugModeEnabled;
}

CLAY_WASM_EXPORT("Clay_SetCullingEnabled")
void Clay_SetCullingEnabled(bool enabled) {
    Clay_Context* context = Clay_GetCurrentContext();
    context->disableCulling = !enabled;
}

CLAY_WASM_EXPORT("Clay_SetExternalScrollHandlingEnabled")
void Clay_SetExternalScrollHandlingEnabled(bool enabled) {
    Clay_Context* context = Clay_GetCurrentContext();
    context->externalScrollHandlingEnabled = enabled;
}

CLAY_WASM_EXPORT("Clay_GetMaxElementCount")
int32_t Clay_GetMaxElementCount(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    return context->maxElementCount;
}

CLAY_WASM_EXPORT("Clay_SetMaxElementCount")
void Clay_SetMaxElementCount(int32_t maxElementCount) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context) {
        context->maxElementCount = maxElementCount;
    } else {
        Clay__defaultMaxElementCount = maxElementCount; // TODO: Fix this
    }
}

CLAY_WASM_EXPORT("Clay_GetMaxMeasureTextCacheWordCount")
int32_t Clay_GetMaxMeasureTextCacheWordCount(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    return context->maxMeasureTextCacheWordCount;
}

CLAY_WASM_EXPORT("Clay_SetMaxMeasureTextCacheWordCount")
void Clay_SetMaxMeasureTextCacheWordCount(int32_t maxMeasureTextCacheWordCount) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context) {
        Clay__currentContext->maxMeasureTextCacheWordCount = maxMeasureTextCacheWordCount;
    } else {
        Clay__defaultMaxMeasureTextWordCacheCount = maxMeasureTextCacheWordCount; // TODO: Fix this
    }
}

CLAY_WASM_EXPORT("Clay_ResetMeasureTextCache")
void Clay_ResetMeasureTextCache(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    context->measureTextHashMapInternal.length = 0;
    context->measureTextHashMapInternalFreeList.length = 0;
    context->measureTextHashMap.length = 0;
    context->measuredWords.length = 0;
    context->measuredWordsFreeList.length = 0;
    
    for (int32_t i = 0; i < context->measureTextHashMap.capacity; ++i) {
        context->measureTextHashMap.internalArray[i] = 0;
    }
    context->measureTextHashMapInternal.length = 1; // Reserve the 0 value to mean "no next element"
}

#endif // CLAY_IMPLEMENTATION

/*
LICENSE
zlib/libpng license

Copyright (c) 2024 Nic Barker

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software in a
    product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not
    be misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/
