#pragma once
// VERSION: 0.10

#ifndef CLAY_IMPLEMENTATION
#define CLAY_IMPLEMENTATION

#ifdef CLAY_WASM
#define CLAY_WASM_EXPORT(name) __attribute__((export_name(name)))
#else
#define CLAY_WASM_EXPORT(null)
#endif

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#ifdef CLAY_OVERFLOW_TRAP
    #include "signal.h"
#endif

#ifndef CLAY_MAX_ELEMENT_COUNT
#define CLAY_MAX_ELEMENT_COUNT 8192
#endif

#ifndef CLAY__NULL
#define CLAY__NULL 0
#endif

#ifndef CLAY__MAXFLOAT
#define CLAY__MAXFLOAT 3.40282346638528859812e+38F
#endif

#define CLAY__MAX(x, y) (((x) > (y)) ? (x) : (y))
#define CLAY__MIN(x, y) (((x) < (y)) ? (x) : (y))

#define CLAY__ALIGNMENT(type) (offsetof(struct { char c; type x; }, x))

// Publicly visible config macros -----------------------------------------------------

#define CLAY_LAYOUT(...) Clay__LayoutConfigArray_Add(&Clay__layoutConfigs, (Clay_LayoutConfig) {__VA_ARGS__ })

#define CLAY_RECTANGLE_CONFIG(...) Clay__RectangleElementConfigArray_Add(&Clay__rectangleElementConfigs, (Clay_RectangleElementConfig) {__VA_ARGS__ })

#define CLAY_TEXT_CONFIG(...) Clay__TextElementConfigArray_Add(&Clay__textElementConfigs, (Clay_TextElementConfig) {__VA_ARGS__ })

#define CLAY_IMAGE_CONFIG(...) Clay__ImageElementConfigArray_Add(&Clay__imageElementConfigs, (Clay_ImageElementConfig) {__VA_ARGS__ })

#define CLAY_FLOATING_CONFIG(...) Clay__FloatingElementConfigArray_Add(&Clay__floatingElementConfigs, (Clay_FloatingElementConfig) {__VA_ARGS__ })

#define CLAY_CUSTOM_ELEMENT_CONFIG(...) Clay__CustomElementConfigArray_Add(&Clay__customElementConfigs, (Clay_CustomElementConfig) {__VA_ARGS__ })

#define CLAY_SCROLL_CONFIG(...) Clay__ScrollElementConfigArray_Add(&Clay__scrollElementConfigs, (Clay_ScrollElementConfig) {__VA_ARGS__ })

#define CLAY_BORDER_CONFIG(...)  Clay__BorderElementConfigArray_Add(&Clay__borderElementConfigs, (Clay_BorderElementConfig ) { __VA_ARGS__ })

#define CLAY_BORDER_CONFIG_OUTSIDE(...)  Clay__BorderElementConfigArray_Add(&Clay__borderElementConfigs, (Clay_BorderElementConfig ) { .left = { __VA_ARGS__ }, .right = { __VA_ARGS__ }, .top = { __VA_ARGS__ }, .bottom = { __VA_ARGS__ } })

#define CLAY_BORDER_CONFIG_OUTSIDE_RADIUS(width, color, radius)  Clay__BorderElementConfigArray_Add(&Clay__borderElementConfigs, (Clay_BorderElementConfig ) { .left = { width, color }, .right = { width, color }, .top = { width, color }, .bottom = { width, color }, .cornerRadius = { radius, radius, radius, radius } })

#define CLAY_BORDER_CONFIG_ALL(...)  Clay__BorderElementConfigArray_Add(&Clay__borderElementConfigs, (Clay_BorderElementConfig ) { .left = { __VA_ARGS__ }, .right = { __VA_ARGS__ }, .top = { __VA_ARGS__ }, .bottom = { __VA_ARGS__ }, .betweenChildren = { __VA_ARGS__ } })

#define CLAY_BORDER_CONFIG_ALL_RADIUS(width, color, radius)  Clay__BorderElementConfigArray_Add(&Clay__borderElementConfigs, (Clay_BorderElementConfig ) { .left = { __VA_ARGS__ }, .right = { __VA_ARGS__ }, .top = { __VA_ARGS__ }, .bottom = { __VA_ARGS__ }, .betweenChildren = { __VA_ARGS__ }, .cornerRadius = { radius, radius, radius, radius }})

#define CLAY_CORNER_RADIUS(radius) (Clay_CornerRadius) { radius, radius, radius, radius }

#define CLAY_SIZING_FIT(...) (Clay_SizingAxis) { .type = CLAY__SIZING_TYPE_FIT, .sizeMinMax = (Clay_SizingMinMax) {__VA_ARGS__} }

#define CLAY_SIZING_GROW(...) (Clay_SizingAxis) { .type = CLAY__SIZING_TYPE_GROW, .sizeMinMax = (Clay_SizingMinMax) {__VA_ARGS__} }

#define CLAY_SIZING_FIXED(fixedSize) (Clay_SizingAxis) { .type = CLAY__SIZING_TYPE_GROW, .sizeMinMax = { fixedSize, fixedSize } }

#define CLAY_SIZING_PERCENT(percentOfParent) (Clay_SizingAxis) { .type = CLAY__SIZING_TYPE_PERCENT, .sizePercent = percentOfParent }

#define CLAY_ID(label) Clay__HashString(CLAY_STRING(label), 0)

#define CLAY_IDI(label, index) Clay__HashString(CLAY_STRING(label), index)

#define CLAY__STRING_LENGTH(s) ((sizeof(s) / sizeof(s[0])) - sizeof(s[0]))

#define CLAY_STRING(string) (Clay_String) { .length = CLAY__STRING_LENGTH(string), .chars = string }

// Publicly visible layout element macros -----------------------------------------------------
#define CLAY_CONTAINER(id, layoutConfig, children)  \
    Clay__OpenContainerElement(id, layoutConfig);   \
    children                                        \
    Clay__CloseElementWithChildren()

#define CLAY_RECTANGLE(id, layoutConfig, rectangleConfig, children)     \
    Clay__OpenRectangleElement(id, layoutConfig, rectangleConfig);      \
    children                                                            \
    Clay__CloseElementWithChildren()

#define CLAY_TEXT(id, text, textConfig) Clay__OpenTextElement(id, text, textConfig)

#define CLAY_IMAGE(id, layoutConfig, imageConfig, children)         \
    Clay__OpenImageElement(id, layoutConfig, imageConfig); \
    children                                                        \
    Clay__CloseElementWithChildren()

#define CLAY_SCROLL_CONTAINER(id, layoutConfig, scrollConfig, children)     \
    Clay__OpenScrollElement(id, layoutConfig, scrollConfig);                \
    children                                                                \
    Clay__CloseScrollElement()

#define CLAY_FLOATING_CONTAINER(id, layoutConfig, floatingConfig, children)   \
    Clay__OpenFloatingElement(id, layoutConfig, floatingConfig);     \
    children                                                                  \
    Clay__CloseFloatingElement()

#define CLAY_BORDER_CONTAINER(id, layoutConfig, borderConfig, children)  \
    Clay__OpenBorderElement(id, layoutConfig, borderConfig);    \
    children                                                             \
    Clay__CloseElementWithChildren()

#define CLAY_CUSTOM_ELEMENT(id, layoutConfig, customElementConfig, children)    \
    Clay__OpenCustomElement(id, layoutConfig, customElementConfig);             \
    children                                                                    \
    Clay__CloseElementWithChildren()

// Note: Clay_String is not guaranteed to be null terminated. It may be if created from a literal C string,
// but it is also used to represent slices.
typedef struct {
    int length;
    const char *chars;
} Clay_String;

Clay_String CLAY__SPACECHAR = (Clay_String) { .length = 1, .chars = " " };
Clay_String CLAY__STRING_DEFAULT = (Clay_String) { .length = 0, .chars = "" };

typedef struct {
    Clay_String label;
    uint64_t nextAllocation;
    uint64_t capacity;
    char *memory;
} Clay_Arena;

typedef struct
{
    uint32_t capacity;
    uint32_t length;
    Clay_String *internalArray;
} Clay_StringArray;

Clay_StringArray Clay_warnings = (Clay_StringArray) {};

Clay_String *Clay__StringArray_Add(Clay_StringArray *array, Clay_String item)
{
    if (array->length < array->capacity) {
        array->internalArray[array->length++] = item;
        return &array->internalArray[array->length - 1];
    }
    else {
        #ifdef CLAY_OVERFLOW_TRAP
        raise(SIGTRAP);
        #endif
    }
    return &CLAY__STRING_DEFAULT;
}

Clay_StringArray Clay__StringArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena)
{
    uint64_t totalSizeBytes = capacity * sizeof(Clay_String);
    Clay_StringArray array = (Clay_StringArray){.capacity = capacity, .length = 0};
    uint64_t nextAllocAddress = (uint64_t)(arena->nextAllocation + arena->memory);
    uint64_t arenaOffsetAligned = nextAllocAddress + (CLAY__ALIGNMENT(Clay_String) - (nextAllocAddress % CLAY__ALIGNMENT(Clay_String)));
    arenaOffsetAligned -= (uint64_t)arena->memory;
    if (arenaOffsetAligned + totalSizeBytes <= arena->capacity) {
        array.internalArray = (Clay_String*)(arena->memory + arenaOffsetAligned);
        arena->nextAllocation = arenaOffsetAligned + totalSizeBytes;
    }
    else {
        Clay__StringArray_Add(&Clay_warnings, CLAY_STRING("Attempting to allocate array in arena, but arena is already at capacity and would overflow."));
        #ifdef CLAY_OVERFLOW_TRAP
        raise(SIGTRAP);
        #endif
    }
    return array;
}

void* Clay__Array_Allocate_Arena(uint32_t capacity, uint32_t itemSize, uint32_t alignment, Clay_Arena *arena)
{
    uint64_t totalSizeBytes = capacity * itemSize;
    uint64_t nextAllocAddress = (uint64_t)(arena->nextAllocation + arena->memory);
    uint64_t arenaOffsetAligned = nextAllocAddress + (alignment - (nextAllocAddress % alignment));
    arenaOffsetAligned -= (uint64_t)arena->memory;
    if (arenaOffsetAligned + totalSizeBytes <= arena->capacity) {
        arena->nextAllocation = arenaOffsetAligned + totalSizeBytes;
        return (void*)(arena->memory + arenaOffsetAligned);
    }
    else {
        Clay__StringArray_Add(&Clay_warnings, CLAY_STRING("Attempting to allocate array in arena, but arena is already at capacity and would overflow."));
        #ifdef CLAY_OVERFLOW_TRAP
        raise(SIGTRAP);
        #endif
    }
    return CLAY__NULL;
}

bool Clay__Array_RangeCheck(int index, uint32_t length)
{
    if (index < length && index >= 0) {
        return true;
    }
    Clay__StringArray_Add(&Clay_warnings, CLAY_STRING("Array access out of bounds."));
    #ifdef CLAY_OVERFLOW_TRAP
    raise(SIGTRAP);
    #endif
    return false;
}

bool Clay__Array_IncrementCapacityCheck(uint32_t length, uint32_t capacity)
{
    if (length < capacity) {
        return true;
    }
    Clay__StringArray_Add(&Clay_warnings, CLAY_STRING("Attempting to add to array that is already at capacity."));
    #ifdef CLAY_OVERFLOW_TRAP
    raise(SIGTRAP);
    #endif
    return false;
}

bool CLAY__BOOL_DEFAULT = false;

// __GENERATED__ template array_define TYPE=bool NAME=Clay__BoolArray
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	bool *internalArray;
} Clay__BoolArray;

Clay__BoolArray Clay__BoolArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__BoolArray){.capacity = capacity, .length = 0, .internalArray = (bool *)Clay__Array_Allocate_Arena(capacity, sizeof(bool), CLAY__ALIGNMENT(bool), arena)};
}
#pragma endregion
// __GENERATED__ template

typedef struct {
    float r, g, b, a;
} Clay_Color;

typedef struct {
    float x, y, width, height;
} Clay_BoundingBox;

typedef struct {
    float width, height;
} Clay_Dimensions;

typedef struct {
    float x, y;
} Clay_Vector2;

typedef enum __attribute__((__packed__)) {
    CLAY_LEFT_TO_RIGHT,
    CLAY_TOP_TO_BOTTOM,
} Clay_LayoutDirection;

typedef enum __attribute__((__packed__)) {
    CLAY_ALIGN_X_LEFT,
    CLAY_ALIGN_X_RIGHT,
    CLAY_ALIGN_X_CENTER,
} Clay_LayoutAlignmentX;

typedef enum __attribute__((__packed__)) {
    CLAY_ALIGN_Y_TOP,
    CLAY_ALIGN_Y_BOTTOM,
    CLAY_ALIGN_Y_CENTER,
} Clay_LayoutAlignmentY;

typedef enum __attribute__((__packed__)) {
    CLAY__SIZING_TYPE_FIT,
    CLAY__SIZING_TYPE_GROW,
    CLAY__SIZING_TYPE_PERCENT,
} Clay__SizingType;

typedef enum {
    CLAY_RENDER_COMMAND_TYPE_NONE,
    CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
    CLAY_RENDER_COMMAND_TYPE_BORDER,
    CLAY_RENDER_COMMAND_TYPE_TEXT,
    CLAY_RENDER_COMMAND_TYPE_IMAGE,
    CLAY_RENDER_COMMAND_TYPE_SCISSOR_START,
    CLAY_RENDER_COMMAND_TYPE_SCISSOR_END,
    CLAY_RENDER_COMMAND_TYPE_CUSTOM,
} Clay_RenderCommandType;

typedef enum __attribute__((__packed__)) {
    CLAY__LAYOUT_ELEMENT_TYPE_CONTAINER,
    CLAY__LAYOUT_ELEMENT_TYPE_RECTANGLE,
    CLAY__LAYOUT_ELEMENT_TYPE_BORDER_CONTAINER,
    CLAY__LAYOUT_ELEMENT_TYPE_FLOATING_CONTAINER,
    CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER,
    CLAY__LAYOUT_ELEMENT_TYPE_IMAGE,
    CLAY__LAYOUT_ELEMENT_TYPE_TEXT,
    CLAY__LAYOUT_ELEMENT_TYPE_CUSTOM,
} Clay__LayoutElementType;

Clay_RenderCommandType Clay__LayoutElementTypeToRenderCommandType[] = {
    [CLAY__LAYOUT_ELEMENT_TYPE_CONTAINER] = CLAY_RENDER_COMMAND_TYPE_NONE,
    [CLAY__LAYOUT_ELEMENT_TYPE_RECTANGLE] = CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
    [CLAY__LAYOUT_ELEMENT_TYPE_FLOATING_CONTAINER] = CLAY_RENDER_COMMAND_TYPE_NONE,
    [CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER] = CLAY_RENDER_COMMAND_TYPE_NONE,
    [CLAY__LAYOUT_ELEMENT_TYPE_BORDER_CONTAINER] = CLAY_RENDER_COMMAND_TYPE_BORDER,
    [CLAY__LAYOUT_ELEMENT_TYPE_IMAGE] = CLAY_RENDER_COMMAND_TYPE_IMAGE,
    [CLAY__LAYOUT_ELEMENT_TYPE_TEXT] = CLAY_RENDER_COMMAND_TYPE_TEXT,
    [CLAY__LAYOUT_ELEMENT_TYPE_CUSTOM] = CLAY_RENDER_COMMAND_TYPE_CUSTOM,
};

typedef enum __attribute__((__packed__)) {
    CLAY_ATTACH_POINT_LEFT_TOP,
    CLAY_ATTACH_POINT_LEFT_CENTER,
    CLAY_ATTACH_POINT_LEFT_BOTTOM,
    CLAY_ATTACH_POINT_CENTER_TOP,
    CLAY_ATTACH_POINT_CENTER_CENTER,
    CLAY_ATTACH_POINT_CENTER_BOTTOM,
    CLAY_ATTACH_POINT_RIGHT_TOP,
    CLAY_ATTACH_POINT_RIGHT_CENTER,
    CLAY_ATTACH_POINT_RIGHT_BOTTOM,
} Clay_FloatingAttachPointType;

typedef struct
{
    Clay_FloatingAttachPointType element;
    Clay_FloatingAttachPointType parent;
} Clay_FloatingAttachPoints;

typedef struct {
    Clay_LayoutAlignmentX x;
    Clay_LayoutAlignmentY y;
} Clay_ChildAlignment;

typedef struct {
    float min;
    float max;
} Clay_SizingMinMax;

typedef struct {
    union {
        Clay_SizingMinMax sizeMinMax;
        float sizePercent;
    };
    Clay__SizingType type;
} Clay_SizingAxis;

typedef struct {
    Clay_SizingAxis width;
    Clay_SizingAxis height;
} Clay_Sizing;

typedef struct {
    uint16_t x;
    uint16_t y;
} Clay_Padding;

typedef struct {
    float topLeft;
    float topRight;
    float bottomLeft;
    float bottomRight;
} Clay_CornerRadius;

typedef struct {
    Clay_Sizing sizing;
    Clay_Padding padding;
    uint16_t childGap;
    Clay_LayoutDirection layoutDirection;
    Clay_ChildAlignment childAlignment;
} Clay_LayoutConfig;

Clay_LayoutConfig CLAY_LAYOUT_DEFAULT = (Clay_LayoutConfig){};

// __GENERATED__ template array_define,array_add TYPE=Clay_LayoutConfig NAME=Clay__LayoutConfigArray DEFAULT_VALUE=&CLAY_LAYOUT_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_LayoutConfig *internalArray;
} Clay__LayoutConfigArray;

Clay__LayoutConfigArray Clay__LayoutConfigArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__LayoutConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_LayoutConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_LayoutConfig), CLAY__ALIGNMENT(Clay_LayoutConfig), arena)};
}
Clay_LayoutConfig *Clay__LayoutConfigArray_Add(Clay__LayoutConfigArray *array, Clay_LayoutConfig item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY_LAYOUT_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

typedef struct {
    Clay_Color color;
    Clay_CornerRadius cornerRadius;
    #ifdef CLAY_EXTEND_CONFIG_RECTANGLE
    CLAY_EXTEND_CONFIG_RECTANGLE
    #endif
} Clay_RectangleElementConfig;

Clay_RectangleElementConfig CLAY__RECTANGLE_ELEMENT_CONFIG_DEFAULT = (Clay_RectangleElementConfig){0};

// __GENERATED__ template array_define,array_add TYPE=Clay_RectangleElementConfig NAME=Clay__RectangleElementConfigArray DEFAULT_VALUE=&CLAY__RECTANGLE_ELEMENT_CONFIG_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_RectangleElementConfig *internalArray;
} Clay__RectangleElementConfigArray;

Clay__RectangleElementConfigArray Clay__RectangleElementConfigArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__RectangleElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_RectangleElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_RectangleElementConfig), CLAY__ALIGNMENT(Clay_RectangleElementConfig), arena)};
}
Clay_RectangleElementConfig *Clay__RectangleElementConfigArray_Add(Clay__RectangleElementConfigArray *array, Clay_RectangleElementConfig item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__RECTANGLE_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

typedef struct
{
    Clay_Color textColor;
    uint16_t fontId;
    uint16_t fontSize;
    uint16_t letterSpacing;
    uint16_t lineSpacing;
    #ifdef CLAY_EXTEND_CONFIG_TEXT
    CLAY_EXTEND_CONFIG_TEXT
    #endif
} Clay_TextElementConfig;

Clay_TextElementConfig CLAY__TEXT_ELEMENT_CONFIG_DEFAULT = (Clay_TextElementConfig) {};

// __GENERATED__ template array_define,array_add TYPE=Clay_TextElementConfig NAME=Clay__TextElementConfigArray DEFAULT_VALUE=&CLAY__TEXT_ELEMENT_CONFIG_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_TextElementConfig *internalArray;
} Clay__TextElementConfigArray;

Clay__TextElementConfigArray Clay__TextElementConfigArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__TextElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_TextElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_TextElementConfig), CLAY__ALIGNMENT(Clay_TextElementConfig), arena)};
}
Clay_TextElementConfig *Clay__TextElementConfigArray_Add(Clay__TextElementConfigArray *array, Clay_TextElementConfig item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__TEXT_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

typedef struct
{
    void * imageData;
    Clay_Dimensions sourceDimensions;
    #ifdef CLAY_EXTEND_CONFIG_IMAGE
    CLAY_EXTEND_CONFIG_IMAGE
    #endif
} Clay_ImageElementConfig;

Clay_ImageElementConfig CLAY__IMAGE_ELEMENT_CONFIG_DEFAULT = (Clay_ImageElementConfig) {};

// __GENERATED__ template array_define,array_add TYPE=Clay_ImageElementConfig NAME=Clay__ImageElementConfigArray DEFAULT_VALUE=&CLAY__IMAGE_ELEMENT_CONFIG_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_ImageElementConfig *internalArray;
} Clay__ImageElementConfigArray;

Clay__ImageElementConfigArray Clay__ImageElementConfigArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__ImageElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_ImageElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_ImageElementConfig), CLAY__ALIGNMENT(Clay_ImageElementConfig), arena)};
}
Clay_ImageElementConfig *Clay__ImageElementConfigArray_Add(Clay__ImageElementConfigArray *array, Clay_ImageElementConfig item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__IMAGE_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

typedef struct
{
    Clay_Vector2 offset;
    Clay_Dimensions expand;
    uint16_t zIndex;
    uint32_t parentId;
    Clay_FloatingAttachPoints attachment;
} Clay_FloatingElementConfig;

Clay_FloatingElementConfig CLAY__FLOATING_ELEMENT_CONFIG_DEFAULT = (Clay_FloatingElementConfig) {};

// __GENERATED__ template array_define,array_add TYPE=Clay_FloatingElementConfig NAME=Clay__FloatingElementConfigArray DEFAULT_VALUE=&CLAY__FLOATING_ELEMENT_CONFIG_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_FloatingElementConfig *internalArray;
} Clay__FloatingElementConfigArray;

Clay__FloatingElementConfigArray Clay__FloatingElementConfigArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__FloatingElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_FloatingElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_FloatingElementConfig), CLAY__ALIGNMENT(Clay_FloatingElementConfig), arena)};
}
Clay_FloatingElementConfig *Clay__FloatingElementConfigArray_Add(Clay__FloatingElementConfigArray *array, Clay_FloatingElementConfig item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__FLOATING_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

typedef struct
{
    #ifndef CLAY_EXTEND_CONFIG_CUSTOM
    void* customData;
    #else
    CLAY_EXTEND_CONFIG_CUSTOM
    #endif
} Clay_CustomElementConfig;

Clay_CustomElementConfig CLAY__CUSTOM_ELEMENT_CONFIG_DEFAULT = (Clay_CustomElementConfig) {};

// __GENERATED__ template array_define,array_add TYPE=Clay_CustomElementConfig NAME=Clay__CustomElementConfigArray DEFAULT_VALUE=&CLAY__CUSTOM_ELEMENT_CONFIG_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_CustomElementConfig *internalArray;
} Clay__CustomElementConfigArray;

Clay__CustomElementConfigArray Clay__CustomElementConfigArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__CustomElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_CustomElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_CustomElementConfig), CLAY__ALIGNMENT(Clay_CustomElementConfig), arena)};
}
Clay_CustomElementConfig *Clay__CustomElementConfigArray_Add(Clay__CustomElementConfigArray *array, Clay_CustomElementConfig item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__CUSTOM_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

typedef struct
{
    bool horizontal;
    bool vertical;
} Clay_ScrollElementConfig;

Clay_ScrollElementConfig CLAY__SCROLL_CONTAINER_ELEMENT_CONFIG_DEFAULT = (Clay_ScrollElementConfig ) {};

// __GENERATED__ template array_define,array_add TYPE=Clay_ScrollElementConfig NAME=Clay__ScrollElementConfigArray DEFAULT_VALUE=&CLAY__SCROLL_CONTAINER_ELEMENT_CONFIG_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_ScrollElementConfig *internalArray;
} Clay__ScrollElementConfigArray;

Clay__ScrollElementConfigArray Clay__ScrollElementConfigArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__ScrollElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_ScrollElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_ScrollElementConfig), CLAY__ALIGNMENT(Clay_ScrollElementConfig), arena)};
}
Clay_ScrollElementConfig *Clay__ScrollElementConfigArray_Add(Clay__ScrollElementConfigArray *array, Clay_ScrollElementConfig item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__SCROLL_CONTAINER_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

typedef struct
{
    uint32_t width;
    Clay_Color color;
} Clay_Border;

typedef struct
{
    Clay_Border left;
    Clay_Border right;
    Clay_Border top;
    Clay_Border bottom;
    Clay_Border betweenChildren;
    Clay_CornerRadius cornerRadius;
} Clay_BorderElementConfig;

Clay_BorderElementConfig CLAY__BORDER_CONTAINER_ELEMENT_CONFIG_DEFAULT = (Clay_BorderElementConfig ) {};

// __GENERATED__ template array_define,array_add TYPE=Clay_BorderElementConfig NAME=Clay__BorderElementConfigArray DEFAULT_VALUE=&CLAY__BORDER_CONTAINER_ELEMENT_CONFIG_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_BorderElementConfig *internalArray;
} Clay__BorderElementConfigArray;

Clay__BorderElementConfigArray Clay__BorderElementConfigArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__BorderElementConfigArray){.capacity = capacity, .length = 0, .internalArray = (Clay_BorderElementConfig *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_BorderElementConfig), CLAY__ALIGNMENT(Clay_BorderElementConfig), arena)};
}
Clay_BorderElementConfig *Clay__BorderElementConfigArray_Add(Clay__BorderElementConfigArray *array, Clay_BorderElementConfig item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__BORDER_CONTAINER_ELEMENT_CONFIG_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

typedef struct
{
    struct t_Clay_LayoutElement **elements;
    uint16_t length;
} Clay__LayoutElementChildren;

typedef union
{
    Clay_RectangleElementConfig *rectangleElementConfig;
    Clay_TextElementConfig *textElementConfig;
    Clay_ImageElementConfig *imageElementConfig;
    Clay_FloatingElementConfig *floatingElementConfig;
    Clay_CustomElementConfig *customElementConfig;
    Clay_ScrollElementConfig *scrollElementConfig;
    Clay_BorderElementConfig *borderElementConfig;
} Clay_ElementConfigUnion;

typedef struct t_Clay_LayoutElement
{
    #ifdef CLAY_DEBUG
    Clay_String name;
    #endif
    union {
        Clay__LayoutElementChildren children;
        Clay_String text;
    };
    Clay_Dimensions dimensions;
    Clay_Dimensions minDimensions;
    Clay_LayoutConfig *layoutConfig;
    Clay_ElementConfigUnion elementConfig;
    uint32_t id;
    Clay__LayoutElementType elementType;
} Clay_LayoutElement;

Clay_LayoutElement CLAY__LAYOUT_ELEMENT_DEFAULT = (Clay_LayoutElement) {};

// __GENERATED__ template array_define,array_add,array_get TYPE=Clay_LayoutElement NAME=Clay_LayoutElementArray DEFAULT_VALUE=&CLAY__LAYOUT_ELEMENT_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_LayoutElement *internalArray;
} Clay_LayoutElementArray;

Clay_LayoutElementArray Clay_LayoutElementArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay_LayoutElementArray){.capacity = capacity, .length = 0, .internalArray = (Clay_LayoutElement *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_LayoutElement), CLAY__ALIGNMENT(Clay_LayoutElement), arena)};
}
Clay_LayoutElement *Clay_LayoutElementArray_Add(Clay_LayoutElementArray *array, Clay_LayoutElement item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__LAYOUT_ELEMENT_DEFAULT;
}
Clay_LayoutElement *Clay_LayoutElementArray_Get(Clay_LayoutElementArray *array, int index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__LAYOUT_ELEMENT_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

// __GENERATED__ template array_define,array_add,array_get,array_remove_swapback TYPE=Clay_LayoutElement* NAME=Clay__LayoutElementPointerArray DEFAULT_VALUE=CLAY__NULL
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_LayoutElement* *internalArray;
} Clay__LayoutElementPointerArray;

Clay__LayoutElementPointerArray Clay__LayoutElementPointerArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__LayoutElementPointerArray){.capacity = capacity, .length = 0, .internalArray = (Clay_LayoutElement* *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_LayoutElement*), CLAY__ALIGNMENT(Clay_LayoutElement*), arena)};
}
Clay_LayoutElement* *Clay__LayoutElementPointerArray_Add(Clay__LayoutElementPointerArray *array, Clay_LayoutElement* item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return CLAY__NULL;
}
Clay_LayoutElement* *Clay__LayoutElementPointerArray_Get(Clay__LayoutElementPointerArray *array, int index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : CLAY__NULL;
}
Clay_LayoutElement* Clay__LayoutElementPointerArray_RemoveSwapback(Clay__LayoutElementPointerArray *array, int index) {
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

typedef struct
{
    Clay_BoundingBox boundingBox;
    Clay_ElementConfigUnion config;
    Clay_String text; // TODO I wish there was a way to avoid having to have this on every render command
    uint32_t id;
    Clay_RenderCommandType commandType;
} Clay_RenderCommand;

Clay_RenderCommand CLAY__RENDER_COMMAND_DEFAULT = (Clay_RenderCommand) {};

// __GENERATED__ template array_define TYPE=Clay_RenderCommand NAME=Clay_RenderCommandArray
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_RenderCommand *internalArray;
} Clay_RenderCommandArray;

Clay_RenderCommandArray Clay_RenderCommandArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay_RenderCommandArray){.capacity = capacity, .length = 0, .internalArray = (Clay_RenderCommand *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_RenderCommand), CLAY__ALIGNMENT(Clay_RenderCommand), arena)};
}
#pragma endregion
// __GENERATED__ template

// __GENERATED__ template array_add TYPE=Clay_RenderCommand NAME=Clay_RenderCommandArray DEFAULT_VALUE=&CLAY__RENDER_COMMAND_DEFAULT
#pragma region generated
Clay_RenderCommand *Clay_RenderCommandArray_Add(Clay_RenderCommandArray *array, Clay_RenderCommand item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__RENDER_COMMAND_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

// __GENERATED__ template array_get TYPE=Clay_RenderCommand NAME=Clay_RenderCommandArray DEFAULT_VALUE=&CLAY__RENDER_COMMAND_DEFAULT
#pragma region generated
Clay_RenderCommand *Clay_RenderCommandArray_Get(Clay_RenderCommandArray *array, int index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__RENDER_COMMAND_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

typedef struct
{
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
} Clay__ScrollContainerDataInternal;

Clay__ScrollContainerDataInternal CLAY__SCROLL_CONTAINER_DEFAULT = (Clay__ScrollContainerDataInternal) {};

// __GENERATED__ template define,array_add,array_get TYPE=Clay__ScrollContainerDataInternal NAME=Clay__ScrollContainerDataInternalArray DEFAULT_VALUE=&CLAY__SCROLL_CONTAINER_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay__ScrollContainerDataInternal *internalArray;
} Clay__ScrollContainerDataInternalArray;

Clay__ScrollContainerDataInternalArray Clay__ScrollContainerDataInternalArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__ScrollContainerDataInternalArray){.capacity = capacity, .length = 0, .internalArray = (Clay__ScrollContainerDataInternal *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__ScrollContainerDataInternal), CLAY__ALIGNMENT(Clay__ScrollContainerDataInternal), arena)};
}
Clay__ScrollContainerDataInternal *Clay__ScrollContainerDataInternalArray_Add(Clay__ScrollContainerDataInternalArray *array, Clay__ScrollContainerDataInternal item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__SCROLL_CONTAINER_DEFAULT;
}
Clay__ScrollContainerDataInternal *Clay__ScrollContainerDataInternalArray_Get(Clay__ScrollContainerDataInternalArray *array, int index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__SCROLL_CONTAINER_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

// __GENERATED__ template array_remove_swapback TYPE=Clay__ScrollContainerDataInternal NAME=Clay__ScrollContainerDataInternalArray DEFAULT_VALUE=CLAY__SCROLL_CONTAINER_DEFAULT
#pragma region generated
Clay__ScrollContainerDataInternal Clay__ScrollContainerDataInternalArray_RemoveSwapback(Clay__ScrollContainerDataInternalArray *array, int index) {
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

typedef struct
{
    Clay_BoundingBox boundingBox;
    uint32_t id;
    Clay_LayoutElement* layoutElement;
    int32_t nextIndex;
} Clay_LayoutElementHashMapItem;

Clay_LayoutElementHashMapItem CLAY__LAYOUT_ELEMENT_HASH_MAP_ITEM_DEFAULT = (Clay_LayoutElementHashMapItem) { .layoutElement = &CLAY__LAYOUT_ELEMENT_DEFAULT };

// __GENERATED__ template array_define,array_get,array_add,array_set TYPE=Clay_LayoutElementHashMapItem NAME=Clay__LayoutElementHashMapItemArray DEFAULT_VALUE=&CLAY__LAYOUT_ELEMENT_HASH_MAP_ITEM_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_LayoutElementHashMapItem *internalArray;
} Clay__LayoutElementHashMapItemArray;

Clay__LayoutElementHashMapItemArray Clay__LayoutElementHashMapItemArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__LayoutElementHashMapItemArray){.capacity = capacity, .length = 0, .internalArray = (Clay_LayoutElementHashMapItem *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay_LayoutElementHashMapItem), CLAY__ALIGNMENT(Clay_LayoutElementHashMapItem), arena)};
}
Clay_LayoutElementHashMapItem *Clay__LayoutElementHashMapItemArray_Get(Clay__LayoutElementHashMapItemArray *array, int index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__LAYOUT_ELEMENT_HASH_MAP_ITEM_DEFAULT;
}
Clay_LayoutElementHashMapItem *Clay__LayoutElementHashMapItemArray_Add(Clay__LayoutElementHashMapItemArray *array, Clay_LayoutElementHashMapItem item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__LAYOUT_ELEMENT_HASH_MAP_ITEM_DEFAULT;
}
void Clay__LayoutElementHashMapItemArray_Set(Clay__LayoutElementHashMapItemArray *array, int index, Clay_LayoutElementHashMapItem value) {
	if (index < array->capacity && index >= 0) {
		array->internalArray[index] = value;
		array->length = index < array->length ? array->length : index + 1;
	} else {
        Clay__StringArray_Add(&Clay_warnings, CLAY_STRING("Attempting to allocate array in arena, but arena is already at capacity and would overflow."));
        #ifdef CLAY_OVERFLOW_TRAP
        raise(SIGTRAP);
        #endif
	}
}
#pragma endregion
// __GENERATED__ template

typedef struct
{
    Clay_Dimensions dimensions;
    uint32_t id;
    int32_t nextIndex;
} Clay__MeasureTextCacheItem;

Clay__MeasureTextCacheItem CLAY__MEASURE_TEXT_CACHE_ITEM_DEFAULT = (Clay__MeasureTextCacheItem) { };

// __GENERATED__ template array_define,array_get,array_add,array_set TYPE=Clay__MeasureTextCacheItem NAME=Clay__MeasureTextCacheItemArray DEFAULT_VALUE=&CLAY__MEASURE_TEXT_CACHE_ITEM_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay__MeasureTextCacheItem *internalArray;
} Clay__MeasureTextCacheItemArray;

Clay__MeasureTextCacheItemArray Clay__MeasureTextCacheItemArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__MeasureTextCacheItemArray){.capacity = capacity, .length = 0, .internalArray = (Clay__MeasureTextCacheItem *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__MeasureTextCacheItem), CLAY__ALIGNMENT(Clay__MeasureTextCacheItem), arena)};
}
Clay__MeasureTextCacheItem *Clay__MeasureTextCacheItemArray_Get(Clay__MeasureTextCacheItemArray *array, int index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__MEASURE_TEXT_CACHE_ITEM_DEFAULT;
}
Clay__MeasureTextCacheItem *Clay__MeasureTextCacheItemArray_Add(Clay__MeasureTextCacheItemArray *array, Clay__MeasureTextCacheItem item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__MEASURE_TEXT_CACHE_ITEM_DEFAULT;
}
void Clay__MeasureTextCacheItemArray_Set(Clay__MeasureTextCacheItemArray *array, int index, Clay__MeasureTextCacheItem value) {
	if (index < array->capacity && index >= 0) {
		array->internalArray[index] = value;
		array->length = index < array->length ? array->length : index + 1;
	} else {
        Clay__StringArray_Add(&Clay_warnings, CLAY_STRING("Attempting to allocate array in arena, but arena is already at capacity and would overflow."));
        #ifdef CLAY_OVERFLOW_TRAP
        raise(SIGTRAP);
        #endif
	}
}
#pragma endregion
// __GENERATED__ template

int32_t CLAY__INDEX_ARRAY_DEFAULT_VALUE = -1;

// __GENERATED__ template array_define,array_get,array_add,array_set TYPE=int32_t NAME=Clay__int32_tArray DEFAULT_VALUE=&CLAY__INDEX_ARRAY_DEFAULT_VALUE
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	int32_t *internalArray;
} Clay__int32_tArray;

Clay__int32_tArray Clay__int32_tArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__int32_tArray){.capacity = capacity, .length = 0, .internalArray = (int32_t *)Clay__Array_Allocate_Arena(capacity, sizeof(int32_t), CLAY__ALIGNMENT(int32_t), arena)};
}
int32_t *Clay__int32_tArray_Get(Clay__int32_tArray *array, int index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__INDEX_ARRAY_DEFAULT_VALUE;
}
int32_t *Clay__int32_tArray_Add(Clay__int32_tArray *array, int32_t item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__INDEX_ARRAY_DEFAULT_VALUE;
}
void Clay__int32_tArray_Set(Clay__int32_tArray *array, int index, int32_t value) {
	if (index < array->capacity && index >= 0) {
		array->internalArray[index] = value;
		array->length = index < array->length ? array->length : index + 1;
	} else {
        Clay__StringArray_Add(&Clay_warnings, CLAY_STRING("Attempting to allocate array in arena, but arena is already at capacity and would overflow."));
        #ifdef CLAY_OVERFLOW_TRAP
        raise(SIGTRAP);
        #endif
	}
}
#pragma endregion
// __GENERATED__ template

Clay_LayoutElement *Clay__openLayoutElement = CLAY__NULL;

typedef struct
{
    Clay_LayoutElement *layoutElement;
    Clay_Vector2 position;
    Clay_Vector2 nextChildOffset;
} Clay__LayoutElementTreeNode;

Clay__LayoutElementTreeNode CLAY__LAYOUT_ELEMENT_TREE_NODE_DEFAULT = (Clay__LayoutElementTreeNode) {};

// __GENERATED__ template array_define,array_add,array_get TYPE=Clay__LayoutElementTreeNode NAME=Clay__LayoutElementTreeNodeArray DEFAULT_VALUE=&CLAY__LAYOUT_ELEMENT_TREE_NODE_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay__LayoutElementTreeNode *internalArray;
} Clay__LayoutElementTreeNodeArray;

Clay__LayoutElementTreeNodeArray Clay__LayoutElementTreeNodeArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__LayoutElementTreeNodeArray){.capacity = capacity, .length = 0, .internalArray = (Clay__LayoutElementTreeNode *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__LayoutElementTreeNode), CLAY__ALIGNMENT(Clay__LayoutElementTreeNode), arena)};
}
Clay__LayoutElementTreeNode *Clay__LayoutElementTreeNodeArray_Add(Clay__LayoutElementTreeNodeArray *array, Clay__LayoutElementTreeNode item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__LAYOUT_ELEMENT_TREE_NODE_DEFAULT;
}
Clay__LayoutElementTreeNode *Clay__LayoutElementTreeNodeArray_Get(Clay__LayoutElementTreeNodeArray *array, int index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__LAYOUT_ELEMENT_TREE_NODE_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

typedef struct
{
    Clay_LayoutElement *layoutElement;
    uint32_t parentId; // This can be zero in the case of the root layout tree
    uint32_t clipElementId; // This can be zero if there is no clip element
    uint32_t zIndex;
} Clay__LayoutElementTreeRoot;

Clay__LayoutElementTreeRoot CLAY__LAYOUT_ELEMENT_TREE_ROOT_DEFAULT = (Clay__LayoutElementTreeRoot) {};

// __GENERATED__ template array_define,array_add,array_get TYPE=Clay__LayoutElementTreeRoot NAME=Clay__LayoutElementTreeRootArray DEFAULT_VALUE=&CLAY__LAYOUT_ELEMENT_TREE_ROOT_DEFAULT
#pragma region generated
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay__LayoutElementTreeRoot *internalArray;
} Clay__LayoutElementTreeRootArray;

Clay__LayoutElementTreeRootArray Clay__LayoutElementTreeRootArray_Allocate_Arena(uint32_t capacity, Clay_Arena *arena) {
    return (Clay__LayoutElementTreeRootArray){.capacity = capacity, .length = 0, .internalArray = (Clay__LayoutElementTreeRoot *)Clay__Array_Allocate_Arena(capacity, sizeof(Clay__LayoutElementTreeRoot), CLAY__ALIGNMENT(Clay__LayoutElementTreeRoot), arena)};
}
Clay__LayoutElementTreeRoot *Clay__LayoutElementTreeRootArray_Add(Clay__LayoutElementTreeRootArray *array, Clay__LayoutElementTreeRoot item) {
	if (Clay__Array_IncrementCapacityCheck(array->length, array->capacity)) {
		array->internalArray[array->length++] = item;
		return &array->internalArray[array->length - 1];
	}
	return &CLAY__LAYOUT_ELEMENT_TREE_ROOT_DEFAULT;
}
Clay__LayoutElementTreeRoot *Clay__LayoutElementTreeRootArray_Get(Clay__LayoutElementTreeRootArray *array, int index) {
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &CLAY__LAYOUT_ELEMENT_TREE_ROOT_DEFAULT;
}
#pragma endregion
// __GENERATED__ template

Clay_Vector2 Clay__pointerPosition = (Clay_Vector2) { -1, -1 };
uint64_t Clay__arenaResetOffset = 0;
Clay_Arena Clay__internalArena;
// Layout Elements / Render Commands
Clay_LayoutElementArray Clay__layoutElements;
Clay_RenderCommandArray Clay__renderCommands;
Clay__LayoutElementPointerArray Clay__openLayoutElementStack;
Clay__LayoutElementPointerArray Clay__layoutElementChildren;
Clay__LayoutElementPointerArray Clay__layoutElementChildrenBuffer;
Clay__LayoutElementPointerArray Clay__textElementPointers;
Clay__LayoutElementPointerArray Clay__imageElementPointers;
Clay__LayoutElementPointerArray Clay__layoutElementReusableBuffer;
// Configs
Clay__LayoutConfigArray Clay__layoutConfigs;
Clay__RectangleElementConfigArray Clay__rectangleElementConfigs;
Clay__TextElementConfigArray Clay__textElementConfigs;
Clay__ImageElementConfigArray Clay__imageElementConfigs;
Clay__FloatingElementConfigArray Clay__floatingElementConfigs;
Clay__ScrollElementConfigArray Clay__scrollElementConfigs;
Clay__CustomElementConfigArray Clay__customElementConfigs;
Clay__BorderElementConfigArray Clay__borderElementConfigs;
// Misc Data Structures
Clay__LayoutElementTreeNodeArray Clay__layoutElementTreeNodeArray1;
Clay__LayoutElementTreeRootArray Clay__layoutElementTreeRoots;
Clay__LayoutElementHashMapItemArray Clay__layoutElementsHashMapInternal;
Clay__int32_tArray Clay__layoutElementsHashMap;
Clay__MeasureTextCacheItemArray Clay__measureTextHashMapInternal;
Clay__int32_tArray Clay__measureTextHashMap;
Clay__int32_tArray Clay__openClipElementStack;
Clay__int32_tArray Clay__pointerOverIds;
Clay__ScrollContainerDataInternalArray Clay__scrollContainerDatas;
Clay__BoolArray Clay__treeNodeVisited;

#if CLAY_WASM
    __attribute__((import_module("clay"), import_name("measureTextFunction"))) Clay_Dimensions Clay__MeasureText(Clay_String *text, Clay_TextElementConfig *config);
#else
    Clay_Dimensions (*Clay__MeasureText)(Clay_String *text, Clay_TextElementConfig *config);
#endif

Clay_String LAST_HASH;

uint32_t Clay__HashString(Clay_String key, const uint32_t offset) {
    uint32_t hash = 0;

    for (int i = 0; i < key.length; i++) {
        hash += key.chars[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += offset;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    #ifdef CLAY_DEBUG
    LAST_HASH = key;
    LAST_HASH.length = (int)offset;
    #endif
    return hash + 1; // Reserve the hash result of zero as "null id"
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
    union {
        float fontSize;
        uint32_t bits;
    } fontSizeBits = { .fontSize = config->fontSize };
    uint32_t hash = 0;
    uint64_t pointerAsNumber = (uint64_t)text->chars;

    hash += pointerAsNumber;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += text->length;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += config->fontId;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += fontSizeBits.bits;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash + 1; // Reserve the hash result of zero as "null id"
}

Clay_Dimensions Clay__MeasureTextCached(Clay_String *text, Clay_TextElementConfig *config) {
    if (text->length < 50) {
        return Clay__MeasureText(text, config);
    }
    uint32_t id = Clay__HashTextWithConfig(text, config);
    uint32_t hashBucket = id % Clay__measureTextHashMap.capacity;
    int32_t elementIndexPrevious = 0;
    int32_t elementIndex = Clay__measureTextHashMap.internalArray[hashBucket];
    while (elementIndex != 0) {
        Clay__MeasureTextCacheItem *hashEntry = Clay__MeasureTextCacheItemArray_Get(&Clay__measureTextHashMapInternal, elementIndex);
        if (hashEntry->id == id) {
            return hashEntry->dimensions;
        }
        elementIndexPrevious = elementIndex;
        elementIndex = hashEntry->nextIndex;
    }
    Clay_Dimensions measured = Clay__MeasureText(text, config);
    if (elementIndexPrevious != 0) {
        Clay__MeasureTextCacheItemArray_Get(&Clay__measureTextHashMapInternal, elementIndexPrevious)->nextIndex = (int32_t)Clay__measureTextHashMapInternal.length - 1;
    } else {
        Clay__measureTextHashMap.internalArray[hashBucket] = (int32_t)Clay__measureTextHashMapInternal.length - 1;
    }
    return measured;
}

bool Clay__PointIsInsideRect(Clay_Vector2 point, Clay_BoundingBox rect) {
    return point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height;
}

Clay_LayoutElementHashMapItem* Clay__AddHashMapItem(uint32_t id, Clay_LayoutElement* layoutElement) {
    Clay_LayoutElementHashMapItem item = (Clay_LayoutElementHashMapItem) { .id = id, .layoutElement = layoutElement, .nextIndex = -1 };
    uint32_t hashBucket = id % Clay__layoutElementsHashMap.capacity;
    int32_t hashItemPrevious = -1;
    int32_t hashItemIndex = Clay__layoutElementsHashMap.internalArray[hashBucket];
    while (hashItemIndex != -1) { // Just replace collision, not a big deal - leave it up to the end user
        Clay_LayoutElementHashMapItem *hashItem = Clay__LayoutElementHashMapItemArray_Get(&Clay__layoutElementsHashMapInternal, hashItemIndex);
        if (hashItem->id == id) { // Collision - swap out linked list item
            item.nextIndex = hashItem->nextIndex;
            Clay__LayoutElementHashMapItemArray_Set(&Clay__layoutElementsHashMapInternal, hashItemIndex, item);
            return Clay__LayoutElementHashMapItemArray_Get(&Clay__layoutElementsHashMapInternal, hashItemIndex);
        }
        hashItemPrevious = hashItemIndex;
        hashItemIndex = hashItem->nextIndex;
    }
    if (hashItemPrevious != -1) {
        Clay__LayoutElementHashMapItemArray_Get(&Clay__layoutElementsHashMapInternal, hashItemPrevious)->nextIndex = (int32_t)Clay__layoutElementsHashMapInternal.length;
    }
    Clay_LayoutElementHashMapItem *hashItem = Clay__LayoutElementHashMapItemArray_Add(&Clay__layoutElementsHashMapInternal, item);
    Clay__layoutElementsHashMap.internalArray[hashBucket] = (int32_t)Clay__layoutElementsHashMapInternal.length - 1;
    return hashItem;
}

Clay_LayoutElementHashMapItem *Clay__GetHashMapItem(uint32_t id) {
    uint32_t hashBucket = id % Clay__layoutElementsHashMap.capacity;
    int32_t elementIndex = Clay__layoutElementsHashMap.internalArray[hashBucket];
    while (elementIndex != -1) {
    Clay_LayoutElementHashMapItem *hashEntry = Clay__LayoutElementHashMapItemArray_Get(&Clay__layoutElementsHashMapInternal, elementIndex);
        if (hashEntry->id == id) {
            return hashEntry;
        }
        elementIndex = hashEntry->nextIndex;
    }
    return CLAY__NULL;
}

Clay_LayoutElement *Clay__OpenElementWithParent(uint32_t id, Clay__LayoutElementType commandType, Clay_LayoutConfig* layoutConfig, Clay_ElementConfigUnion elementConfig) {
    Clay_LayoutElement layoutElement = (Clay_LayoutElement) {
        #ifdef CLAY_DEBUG
        .name = LAST_HASH,
        #endif
        .id = id,
        .elementType = commandType,
        .minDimensions = (Clay_Dimensions) { (float)layoutConfig->padding.x * 2, (float)layoutConfig->padding.y * 2 },
        .children = (Clay__LayoutElementChildren) { .length = 0 },
        .layoutConfig = layoutConfig,
        .elementConfig = elementConfig,
    };

    if (layoutConfig->sizing.width.type != CLAY__SIZING_TYPE_PERCENT) {
        layoutElement.dimensions.width = (float)layoutConfig->padding.x * 2;
        layoutElement.minDimensions.width = CLAY__MAX(layoutElement.minDimensions.width, layoutConfig->sizing.width.sizeMinMax.min);
        if (layoutConfig->sizing.width.sizeMinMax.max <= 0) { // Set the max size if the user didn't specify, makes calculations easier
            layoutConfig->sizing.width.sizeMinMax.max = CLAY__MAXFLOAT;
        }
    }
    if (layoutConfig->sizing.height.type != CLAY__SIZING_TYPE_PERCENT) {
        layoutElement.dimensions.height = (float)layoutConfig->padding.y * 2;
        layoutElement.minDimensions.height = CLAY__MAX(layoutElement.minDimensions.height, layoutConfig->sizing.height.sizeMinMax.min);
        if (layoutConfig->sizing.height.sizeMinMax.max <= 0) { // Set the max size if the user didn't specify, makes calculations easier
            layoutConfig->sizing.height.sizeMinMax.max = CLAY__MAXFLOAT;
        }
    }

    Clay__openLayoutElement = Clay_LayoutElementArray_Add(&Clay__layoutElements, layoutElement);
    Clay__LayoutElementPointerArray_Add(&Clay__openLayoutElementStack, Clay__openLayoutElement);
    Clay__AddHashMapItem(id, Clay__openLayoutElement);
    return Clay__openLayoutElement;
}

Clay_LayoutElement *Clay__OpenElement(uint32_t id, Clay__LayoutElementType commandType, Clay_LayoutConfig *layoutConfig, Clay_ElementConfigUnion elementConfig) {
    Clay__openLayoutElement->children.length++;
    Clay_LayoutElement *element = Clay__OpenElementWithParent(id, commandType, layoutConfig, elementConfig);
    Clay__LayoutElementPointerArray_Add(&Clay__layoutElementChildrenBuffer, element);
    return element;
}

void Clay__OpenContainerElement(int id, Clay_LayoutConfig *layoutConfig) {
    Clay__OpenElement(id, CLAY__LAYOUT_ELEMENT_TYPE_CONTAINER, layoutConfig, (Clay_ElementConfigUnion){ CLAY__NULL });
}

void Clay__OpenRectangleElement(int id, Clay_LayoutConfig *layoutConfig, Clay_RectangleElementConfig *rectangleConfig) {
    Clay__OpenElement(id, CLAY__LAYOUT_ELEMENT_TYPE_RECTANGLE, layoutConfig, (Clay_ElementConfigUnion) { .rectangleElementConfig = rectangleConfig });
}

void Clay__OpenImageElement(int id, Clay_LayoutConfig *layoutConfig, Clay_ImageElementConfig *imageConfig) {
    Clay__OpenElement(id, CLAY__LAYOUT_ELEMENT_TYPE_IMAGE, layoutConfig, (Clay_ElementConfigUnion) { .imageElementConfig = imageConfig });
    Clay__LayoutElementPointerArray_Add(&Clay__imageElementPointers, Clay__openLayoutElement);
}

void Clay__OpenBorderElement(int id, Clay_LayoutConfig *layoutConfig, Clay_BorderElementConfig *borderConfig) {
    Clay__OpenElement(id, CLAY__LAYOUT_ELEMENT_TYPE_BORDER_CONTAINER, layoutConfig, (Clay_ElementConfigUnion){ .borderElementConfig = borderConfig });
}

void Clay__OpenCustomElement(uint32_t id, Clay_LayoutConfig *layoutConfig, Clay_CustomElementConfig *customElementConfig) {
    Clay__OpenElement(id, CLAY__LAYOUT_ELEMENT_TYPE_CUSTOM, layoutConfig, (Clay_ElementConfigUnion) { .customElementConfig = customElementConfig });
}

Clay_LayoutElement *Clay__OpenScrollElement(uint32_t id, Clay_LayoutConfig *layoutConfig, Clay_ScrollElementConfig *scrollConfig) {
    Clay_LayoutElement *scrollElement = Clay__OpenElement(id, CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER, layoutConfig, (Clay_ElementConfigUnion){ .scrollElementConfig = scrollConfig });
    Clay__int32_tArray_Add(&Clay__openClipElementStack, (int)scrollElement->id);
    Clay__ScrollContainerDataInternal *scrollOffset = CLAY__NULL;
    for (int i = 0; i < Clay__scrollContainerDatas.length; i++) {
        Clay__ScrollContainerDataInternal *mapping = Clay__ScrollContainerDataInternalArray_Get(&Clay__scrollContainerDatas, i);
        if (id == mapping->elementId) {
            scrollOffset = mapping;
            scrollOffset->layoutElement = scrollElement;
            scrollOffset->openThisFrame = true;
        }
    }
    if (!scrollOffset) {
        Clay__ScrollContainerDataInternalArray_Add(&Clay__scrollContainerDatas, (Clay__ScrollContainerDataInternal){.elementId = id, .layoutElement = scrollElement, .scrollOrigin = {-1,-1}, .openThisFrame = true});
    }
    return scrollElement;
}

Clay_LayoutElement *Clay__OpenFloatingElement(uint32_t id, Clay_LayoutConfig *layoutConfig, Clay_FloatingElementConfig *floatingElementConfig) {
    Clay_LayoutElement *parent = Clay__openLayoutElement;
    if (floatingElementConfig->parentId > 0) {
        Clay_LayoutElementHashMapItem *parentItem = Clay__GetHashMapItem(floatingElementConfig->parentId);
        if (!parentItem) {
            Clay__StringArray_Add(&Clay_warnings, CLAY_STRING("Clay Warning: Couldn't find parent container to attach floating container to."));
        } else {
            parent = parentItem->layoutElement;
        }
    }
    Clay__OpenElementWithParent(id, CLAY__LAYOUT_ELEMENT_TYPE_FLOATING_CONTAINER, layoutConfig, (Clay_ElementConfigUnion) { .floatingElementConfig = floatingElementConfig });
    Clay__LayoutElementTreeRootArray_Add(&Clay__layoutElementTreeRoots, (Clay__LayoutElementTreeRoot) {
        .layoutElement = Clay__openLayoutElement,
        .parentId = parent->id,
        .zIndex = floatingElementConfig->zIndex,
        .clipElementId = Clay__openClipElementStack.length > 0 ? (uint32_t)*Clay__int32_tArray_Get(&Clay__openClipElementStack, (int)Clay__openClipElementStack.length - 1) : 0,
    });
    return Clay__openLayoutElement;
}

void Clay__AttachContainerChildren() {
    Clay_LayoutConfig *layoutConfig = Clay__openLayoutElement->layoutConfig;
    Clay__openLayoutElement->children.elements = &Clay__layoutElementChildren.internalArray[Clay__layoutElementChildren.length];

    if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
        for (int i = 0; i < Clay__openLayoutElement->children.length; i++) {
            Clay_LayoutElement *child = *Clay__LayoutElementPointerArray_Get(&Clay__layoutElementChildrenBuffer, (int)Clay__layoutElementChildrenBuffer.length - Clay__openLayoutElement->children.length + i);
            Clay__openLayoutElement->dimensions.width += child->dimensions.width;
            Clay__openLayoutElement->dimensions.height = CLAY__MAX(Clay__openLayoutElement->dimensions.height, child->dimensions.height + layoutConfig->padding.y * 2);
            // Minimum size of child elements doesn't matter to scroll containers as they can shrink and hide their contents
            if (Clay__openLayoutElement->elementType != CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER || !Clay__openLayoutElement->elementConfig.scrollElementConfig->horizontal) {
                Clay__openLayoutElement->minDimensions.width += child->minDimensions.width;
            }
            if (Clay__openLayoutElement->elementType != CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER || !Clay__openLayoutElement->elementConfig.scrollElementConfig->vertical) {
                Clay__openLayoutElement->minDimensions.height = CLAY__MAX(Clay__openLayoutElement->minDimensions.height, child->minDimensions.height + layoutConfig->padding.y * 2);
            }
            Clay__LayoutElementPointerArray_Add(&Clay__layoutElementChildren, child);
        }
        float childGap = (float)(CLAY__MAX(Clay__openLayoutElement->children.length - 1, 0) * layoutConfig->childGap);
        Clay__openLayoutElement->dimensions.width += childGap;
        Clay__openLayoutElement->minDimensions.width += childGap;
    }
    else if (layoutConfig->layoutDirection == CLAY_TOP_TO_BOTTOM) {
        for (int i = 0; i < Clay__openLayoutElement->children.length; i++) {
            Clay_LayoutElement *child = *Clay__LayoutElementPointerArray_Get(&Clay__layoutElementChildrenBuffer, (int)Clay__layoutElementChildrenBuffer.length - Clay__openLayoutElement->children.length + i);
            Clay__openLayoutElement->dimensions.height += child->dimensions.height;
            Clay__openLayoutElement->dimensions.width = CLAY__MAX(Clay__openLayoutElement->dimensions.width, child->dimensions.width + layoutConfig->padding.x * 2);
            // Minimum size of child elements doesn't matter to scroll containers as they can shrink and hide their contents
            if (Clay__openLayoutElement->elementType != CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER || !Clay__openLayoutElement->elementConfig.scrollElementConfig->vertical) {
                Clay__openLayoutElement->minDimensions.height += child->minDimensions.height;
            }
            if (Clay__openLayoutElement->elementType != CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER || !Clay__openLayoutElement->elementConfig.scrollElementConfig->horizontal) {
                Clay__openLayoutElement->minDimensions.width = CLAY__MAX(Clay__openLayoutElement->minDimensions.width, child->minDimensions.width + layoutConfig->padding.x * 2);
            }
            Clay__LayoutElementPointerArray_Add(&Clay__layoutElementChildren, child);
        }
        float childGap = (float)(CLAY__MAX(Clay__openLayoutElement->children.length - 1, 0) * layoutConfig->childGap);
        Clay__openLayoutElement->dimensions.height += childGap;
        Clay__openLayoutElement->minDimensions.height += childGap;
    }

    Clay__layoutElementChildrenBuffer.length -= Clay__openLayoutElement->children.length;
}

void Clay__CloseElement() {
    Clay_LayoutConfig *layoutConfig = Clay__openLayoutElement->layoutConfig;

    if (layoutConfig->sizing.width.type != CLAY__SIZING_TYPE_PERCENT) {
    // TODO I think minsize has already been applied by this point so no need to do it again
        Clay__openLayoutElement->dimensions.width = CLAY__MIN(CLAY__MAX(Clay__openLayoutElement->dimensions.width, layoutConfig->sizing.width.sizeMinMax.min),  layoutConfig->sizing.width.sizeMinMax.max);
    } else {
        Clay__openLayoutElement->dimensions.width = 0;
    }
    if (layoutConfig->sizing.height.type != CLAY__SIZING_TYPE_PERCENT) {
        Clay__openLayoutElement->dimensions.height = CLAY__MIN(CLAY__MAX(Clay__openLayoutElement->dimensions.height, layoutConfig->sizing.height.sizeMinMax.min), layoutConfig->sizing.height.sizeMinMax.max);
    } else {
        Clay__openLayoutElement->dimensions.height = 0;
    }

    Clay__LayoutElementPointerArray_RemoveSwapback(&Clay__openLayoutElementStack, (int)Clay__openLayoutElementStack.length - 1);
    Clay__openLayoutElement = *Clay__LayoutElementPointerArray_Get(&Clay__openLayoutElementStack, (int)Clay__openLayoutElementStack.length - 1);
}

void Clay__OpenTextElement(int id, Clay_String text, Clay_TextElementConfig *textConfig) {
    Clay_LayoutElement *internalElement = Clay__OpenElement(id, CLAY__LAYOUT_ELEMENT_TYPE_TEXT, &CLAY_LAYOUT_DEFAULT, (Clay_ElementConfigUnion) { .textElementConfig = textConfig });
    Clay_Dimensions textMeasured = Clay__MeasureTextCached(&text, textConfig);
    internalElement->dimensions.width = textMeasured.width;
    internalElement->dimensions.height = textMeasured.height;
    internalElement->text = text;
    internalElement->minDimensions = (Clay_Dimensions) { .width = textMeasured.height, .height = textMeasured.height }; // TODO not sure this is the best way to decide min width for text
    Clay__LayoutElementPointerArray_Add(&Clay__textElementPointers, internalElement);
    Clay__CloseElement();
}

void Clay__CloseElementWithChildren() {
    Clay__AttachContainerChildren();
    Clay__CloseElement();
}

void Clay__CloseScrollElement() {
    Clay__openClipElementStack.length--;
    Clay__CloseElementWithChildren();
}

void Clay__CloseFloatingElement() {
    Clay__AttachContainerChildren();
    Clay__CloseElement();
}

void Clay__InitializeEphemeralMemory(Clay_Arena *arena) {
    // Ephemeral Memory - reset every frame
    Clay__internalArena.nextAllocation = Clay__arenaResetOffset;

    Clay__layoutElementChildrenBuffer = Clay__LayoutElementPointerArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__layoutElements = Clay_LayoutElementArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay_warnings = Clay__StringArray_Allocate_Arena(100, arena);

    Clay__layoutConfigs = Clay__LayoutConfigArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__rectangleElementConfigs = Clay__RectangleElementConfigArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__textElementConfigs = Clay__TextElementConfigArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__imageElementConfigs = Clay__ImageElementConfigArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__floatingElementConfigs = Clay__FloatingElementConfigArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__scrollElementConfigs = Clay__ScrollElementConfigArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__customElementConfigs = Clay__CustomElementConfigArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__borderElementConfigs = Clay__BorderElementConfigArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);

    Clay__layoutElementTreeNodeArray1 = Clay__LayoutElementTreeNodeArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__layoutElementTreeRoots = Clay__LayoutElementTreeRootArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__layoutElementChildren = Clay__LayoutElementPointerArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__openLayoutElementStack = Clay__LayoutElementPointerArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__textElementPointers = Clay__LayoutElementPointerArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__imageElementPointers = Clay__LayoutElementPointerArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__layoutElementReusableBuffer = Clay__LayoutElementPointerArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__renderCommands = Clay_RenderCommandArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__treeNodeVisited = Clay__BoolArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__treeNodeVisited.length = Clay__treeNodeVisited.capacity; // This array is accessed directly rather than behaving as a list
    Clay__openClipElementStack = Clay__int32_tArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
}

void Clay__InitializePersistentMemory(Clay_Arena *arena) {
    Clay__scrollContainerDatas = Clay__ScrollContainerDataInternalArray_Allocate_Arena(10, arena);
    Clay__layoutElementsHashMapInternal = Clay__LayoutElementHashMapItemArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__layoutElementsHashMap = Clay__int32_tArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__measureTextHashMapInternal = Clay__MeasureTextCacheItemArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__measureTextHashMap = Clay__int32_tArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__pointerOverIds = Clay__int32_tArray_Allocate_Arena(CLAY_MAX_ELEMENT_COUNT, arena);
    Clay__arenaResetOffset = arena->nextAllocation;
}


typedef enum
{
    CLAY__SIZE_DISTRIBUTION_TYPE_SCROLL_CONTAINER,
    CLAY__SIZE_DISTRIBUTION_TYPE_RESIZEABLE_CONTAINER,
    CLAY__SIZE_DISTRIBUTION_TYPE_GROW_CONTAINER,
} Clay__SizeDistributionType;

// Because of the max and min sizing properties, we can't predict ahead of time how (or if) all the excess width
// will actually be distributed. So we keep looping until either all the excess width is distributed or
// we have exhausted all our containers that can change size along this axis
float Clay__DistributeSizeAmongChildren(bool xAxis, float sizeToDistribute, Clay__LayoutElementPointerArray resizableContainerBuffer, Clay__SizeDistributionType distributionType) {
    Clay__LayoutElementPointerArray backBuffer = Clay__layoutElementReusableBuffer;
    backBuffer.length = 0;

    Clay__LayoutElementPointerArray remainingElements = resizableContainerBuffer;
    float totalDistributedSize;
    while (sizeToDistribute != 0 && remainingElements.length > 0) {
        totalDistributedSize = 0;
        for (int childOffset = 0; childOffset < remainingElements.length; childOffset++) {
            Clay_LayoutElement *childElement = *Clay__LayoutElementPointerArray_Get(&remainingElements, childOffset);
            Clay_SizingAxis childSizing = xAxis ? childElement->layoutConfig->sizing.width : childElement->layoutConfig->sizing.height;
            float *childSize = xAxis ? &childElement->dimensions.width : &childElement->dimensions.height;
            float childMinSize = xAxis ? childElement->minDimensions.width : childElement->minDimensions.height;

            if ((sizeToDistribute < 0 && *childSize == childSizing.sizeMinMax.min) || (sizeToDistribute > 0 && *childSize == childSizing.sizeMinMax.max)) {
                continue;
            }

            if (!xAxis && childElement->elementType == CLAY__LAYOUT_ELEMENT_TYPE_IMAGE) {
                continue; // Currently, we don't support squishing aspect ratio images on their Y axis as it would break ratio
            }

            switch (distributionType) {
                case CLAY__SIZE_DISTRIBUTION_TYPE_RESIZEABLE_CONTAINER: break;
                case CLAY__SIZE_DISTRIBUTION_TYPE_GROW_CONTAINER: if (childSizing.type != CLAY__SIZING_TYPE_GROW) { continue; } break;
                case CLAY__SIZE_DISTRIBUTION_TYPE_SCROLL_CONTAINER: if ((childElement->elementType != CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER || (xAxis && !childElement->elementConfig.scrollElementConfig->horizontal) || (!xAxis && !childElement->elementConfig.scrollElementConfig->vertical))) { continue; } break;
            }

            float dividedSize = sizeToDistribute / (float)(remainingElements.length - childOffset);
            float oldChildSize = *childSize;
            *childSize = CLAY__MAX(CLAY__MAX(CLAY__MIN(childSizing.sizeMinMax.max, *childSize + dividedSize), childSizing.sizeMinMax.min), childMinSize);
            float diff = *childSize - oldChildSize;
            if (diff != 0) {
                Clay__LayoutElementPointerArray_Add(&backBuffer, childElement);
            }
            sizeToDistribute -= diff;
            totalDistributedSize += diff;
        }
        if (totalDistributedSize == 0) {
            break;
        }
        // Flip the buffers
        Clay__LayoutElementPointerArray temp = remainingElements;
        remainingElements = backBuffer;
        backBuffer = temp;
    }
    return sizeToDistribute;
}

void Clay__SizeContainersAlongAxis(bool xAxis) {
    Clay__LayoutElementPointerArray bfsBuffer = Clay__layoutElementChildrenBuffer;
    Clay__LayoutElementPointerArray resizableContainerBuffer = Clay__openLayoutElementStack;
    for (int rootIndex = 0; rootIndex < Clay__layoutElementTreeRoots.length; ++rootIndex) {
        bfsBuffer.length = 0;
        Clay__LayoutElementTreeRoot *root = Clay__LayoutElementTreeRootArray_Get(&Clay__layoutElementTreeRoots, rootIndex);
        Clay_LayoutElement *rootElement = root->layoutElement;
        Clay__LayoutElementPointerArray_Add(&bfsBuffer, root->layoutElement);

        // Size floating containers to their parents
        if (rootElement->elementType == CLAY__LAYOUT_ELEMENT_TYPE_FLOATING_CONTAINER) {
            Clay_LayoutElementHashMapItem *parentItem = Clay__GetHashMapItem(rootElement->elementConfig.floatingElementConfig->parentId);
            if (parentItem) {
                Clay_LayoutElement *parentLayoutElement = parentItem->layoutElement;
                if (rootElement->layoutConfig->sizing.width.type == CLAY__SIZING_TYPE_GROW) {
                    rootElement->dimensions.width = parentLayoutElement->dimensions.width - (float)parentLayoutElement->layoutConfig->padding.x * 2;
                }
                if (rootElement->layoutConfig->sizing.height.type == CLAY__SIZING_TYPE_GROW) {
                    rootElement->dimensions.height = parentLayoutElement->dimensions.height - (float)parentLayoutElement->layoutConfig->padding.x * 2;
                }
            }
        }

        rootElement->dimensions.width = CLAY__MIN(CLAY__MAX(rootElement->dimensions.width, rootElement->layoutConfig->sizing.width.sizeMinMax.min), rootElement->layoutConfig->sizing.width.sizeMinMax.max);
        rootElement->dimensions.height = CLAY__MIN(CLAY__MAX(rootElement->dimensions.height, rootElement->layoutConfig->sizing.height.sizeMinMax.min), rootElement->layoutConfig->sizing.height.sizeMinMax.max);

        for (int i = 0; i < bfsBuffer.length; ++i) {
            Clay_LayoutElement *parent = *Clay__LayoutElementPointerArray_Get(&bfsBuffer, i);
            Clay_LayoutConfig *parentStyleConfig = parent->layoutConfig;
            float parentSize = xAxis ? parent->dimensions.width : parent->dimensions.height;
            float parentPadding = (float)(xAxis ? parent->layoutConfig->padding.x : parent->layoutConfig->padding.y);
            float innerContentSize = 0, totalPaddingAndChildGaps = parentPadding * 2;
            bool sizingAlongAxis = (xAxis && parentStyleConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) || (!xAxis && parentStyleConfig->layoutDirection == CLAY_TOP_TO_BOTTOM);
            resizableContainerBuffer.length = 0;
            float parentChildGap = parentStyleConfig->childGap;

            for (int childOffset = 0; childOffset < parent->children.length; childOffset++) {
                Clay_LayoutElement *childElement = parent->children.elements[childOffset];
                Clay_SizingAxis childSizing = xAxis ? childElement->layoutConfig->sizing.width : childElement->layoutConfig->sizing.height;
                float childSize = xAxis ? childElement->dimensions.width : childElement->dimensions.height;

                if (childElement->elementType != CLAY__LAYOUT_ELEMENT_TYPE_TEXT && childElement->children.length > 0) {
                    Clay__LayoutElementPointerArray_Add(&bfsBuffer, childElement);
                }

                if (childSizing.type != CLAY__SIZING_TYPE_PERCENT) {
                    Clay__LayoutElementPointerArray_Add(&resizableContainerBuffer, childElement);
                }

                if (sizingAlongAxis) {
                    innerContentSize += (childSizing.type == CLAY__SIZING_TYPE_PERCENT ? 0 : childSize);
                    if (childOffset > 0) {
                        innerContentSize += parentChildGap; // For children after index 0, the childAxisOffset is the gap from the previous child
                        totalPaddingAndChildGaps += parentChildGap;
                    }
                } else {
                    innerContentSize = CLAY__MAX(childSize, innerContentSize);
                }
            }

            // Expand percentage containers to size
            for (int childOffset = 0; childOffset < parent->children.length; childOffset++) {
                Clay_LayoutElement *childElement = parent->children.elements[childOffset];
                Clay_SizingAxis childSizing = xAxis ? childElement->layoutConfig->sizing.width : childElement->layoutConfig->sizing.height;
                float *childSize = xAxis ? &childElement->dimensions.width : &childElement->dimensions.height;
                if (childSizing.type == CLAY__SIZING_TYPE_PERCENT) {
                    *childSize = (parentSize - totalPaddingAndChildGaps) * childSizing.sizePercent;
                    if (sizingAlongAxis) {
                        innerContentSize += *childSize;
                        if (childOffset > 0) {
                            innerContentSize += parentChildGap; // For children after index 0, the childAxisOffset is the gap from the previous child
                            totalPaddingAndChildGaps += parentChildGap;
                        }
                    } else {
                        innerContentSize = CLAY__MAX(*childSize, innerContentSize);
                    }
                }
            }

            if (sizingAlongAxis) {
                float sizeToDistribute = parentSize - parentPadding * 2 - innerContentSize;
                // If the content is too large, compress the children as much as possible
                if (sizeToDistribute < 0) {
                    // If the parent can scroll in the axis direction in this direction, just leave the children alone
                    if (parent->elementType == CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER) {
                        if (((xAxis && parent->elementConfig.scrollElementConfig->horizontal) || (!xAxis && parent->elementConfig.scrollElementConfig->vertical))) {
                            continue;
                        }
                    }
                    // Scrolling containers preferentially compress before others
                    sizeToDistribute = Clay__DistributeSizeAmongChildren(xAxis, sizeToDistribute, resizableContainerBuffer, CLAY__SIZE_DISTRIBUTION_TYPE_SCROLL_CONTAINER);

                    // If there is still height to make up, remove it from all containers that haven't hit their minimum size
                    if (sizeToDistribute < 0) {
                        Clay__DistributeSizeAmongChildren(xAxis, sizeToDistribute, resizableContainerBuffer, CLAY__SIZE_DISTRIBUTION_TYPE_RESIZEABLE_CONTAINER);
                    }
                // The content is too small, allow SIZING_GROW containers to expand
                } else {
                    Clay__DistributeSizeAmongChildren(xAxis, sizeToDistribute, resizableContainerBuffer, CLAY__SIZE_DISTRIBUTION_TYPE_GROW_CONTAINER);
                }
            // Sizing along the non layout axis ("off axis")
            } else {
                for (int childOffset = 0; childOffset < resizableContainerBuffer.length; childOffset++) {
                    Clay_LayoutElement *childElement = *Clay__LayoutElementPointerArray_Get(&resizableContainerBuffer, childOffset);
                    Clay_SizingAxis childSizing = xAxis ? childElement->layoutConfig->sizing.width : childElement->layoutConfig->sizing.height;
                    float *childSize = xAxis ? &childElement->dimensions.width : &childElement->dimensions.height;

                    if (!xAxis && childElement->elementType == CLAY__LAYOUT_ELEMENT_TYPE_IMAGE) {
                        continue; // Currently we don't support resizing aspect ratio images on the Y axis because it would break the ratio
                    }

                    // If we're laying out the children of a scroll panel, grow containers expand to the height of the inner content, not the outer container
                    float maxSize = parentSize - parentPadding * 2;
                    if (parent->elementType == CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER && ((xAxis && parent->elementConfig.scrollElementConfig->horizontal) || (!xAxis && parent->elementConfig.scrollElementConfig->vertical))) {
                        maxSize = CLAY__MAX(maxSize, innerContentSize);
                    }
                    if (childSizing.type == CLAY__SIZING_TYPE_FIT) {
                        *childSize = CLAY__MAX(childSizing.sizeMinMax.min, CLAY__MIN(*childSize, maxSize));
                    } else if (childSizing.type == CLAY__SIZING_TYPE_GROW) {
                        *childSize = CLAY__MIN(maxSize, childSizing.sizeMinMax.max);
                    }
                }
            }
        }
    }
}

void Clay__CalculateFinalLayout(int screenWidth, int screenHeight) {
    // Calculate sizing along the X axis
    Clay__SizeContainersAlongAxis(true);

    // Wrap text
    uint32_t originalTextLayoutElementDataLength = Clay__textElementPointers.length;
    for (int i = 0; i < originalTextLayoutElementDataLength; ++i) {
        Clay_LayoutElement *containerElement = *Clay__LayoutElementPointerArray_Get(&Clay__textElementPointers, i);
        Clay_String text = containerElement->text;
        Clay_TextElementConfig *textConfig = containerElement->elementConfig.textElementConfig;
        containerElement->elementType = CLAY__LAYOUT_ELEMENT_TYPE_CONTAINER;
        // Clone the style config to prevent pollution of other elements that share this config
        containerElement->layoutConfig = Clay__LayoutConfigArray_Add(&Clay__layoutConfigs, *containerElement->layoutConfig);
        containerElement->layoutConfig->layoutDirection = CLAY_TOP_TO_BOTTOM;
        containerElement->layoutConfig->childGap = textConfig->lineSpacing;
        containerElement->dimensions.height = 0;
        float fontSize = containerElement->elementConfig.textElementConfig->fontSize;
        int lineStartIndex = 0;
        int wordStartIndex = 0;
        int wordEndIndex = 0;
        containerElement->children = (Clay__LayoutElementChildren) { // Note: this overwrites the text property
            .length = 0,
            .elements = &Clay__layoutElementChildren.internalArray[Clay__layoutElementChildren.length]
        };
        Clay_Dimensions lineDimensions = (Clay_Dimensions){};
        float spaceWidth = Clay__MeasureText(&CLAY__SPACECHAR, textConfig).width; // todo may as well cache it somewhere
        while (wordStartIndex < text.length) {
            if (text.chars[wordEndIndex] == ' ' || text.chars[wordEndIndex] == '\n' || wordEndIndex == text.length) {
                Clay_String stringToRender = (Clay_String) { .length = wordEndIndex - lineStartIndex, .chars = text.chars + lineStartIndex };
                Clay_String wordToMeasure = (Clay_String) { .length = wordEndIndex - wordStartIndex, .chars = text.chars + wordStartIndex };
                // Clip off trailing spaces and newline characters
                Clay_Dimensions wordDimensions = Clay__MeasureTextCached(&wordToMeasure, textConfig);
                lineDimensions.width = lineDimensions.width + wordDimensions.width + spaceWidth;
                lineDimensions.height = wordDimensions.height;
                bool isOverlappingBoundaries = (lineDimensions.width - spaceWidth) > containerElement->dimensions.width + 0.01f; // Epsilon for floating point inaccuracy of adding components
                // Need to wrap
                if (isOverlappingBoundaries) {
                    lineDimensions.width -= spaceWidth;
                    // We can wrap at the most recent word start
                    if (wordStartIndex != lineStartIndex) {
                        stringToRender = (Clay_String) { .length = wordStartIndex - lineStartIndex - 1, .chars = text.chars + lineStartIndex };
                        lineDimensions.width -= (wordDimensions.width + spaceWidth);
                        lineStartIndex = wordStartIndex;
                        wordStartIndex = lineStartIndex;
                        wordEndIndex = lineStartIndex;
                        containerElement->dimensions.width = CLAY__MAX(containerElement->dimensions.width, lineDimensions.width);
                    // The single word is larger than the entire container - just render it in place
                    } else {
                        lineStartIndex = wordEndIndex + 1;
                        wordStartIndex = lineStartIndex;
                        wordEndIndex = lineStartIndex;
                        containerElement->dimensions.width = CLAY__MAX(containerElement->dimensions.width, lineDimensions.width);
                    }
                // If we're at a space character and the current phrase fits, just keep going
                } else if (text.chars[wordEndIndex] == ' ') {
                    wordStartIndex = wordEndIndex + 1;
                    wordEndIndex = wordStartIndex;
                    continue;
                // Newline or end of string
                } else {
                    lineStartIndex = wordEndIndex + 1;
                    wordStartIndex = lineStartIndex;
                    wordEndIndex = lineStartIndex;
                }
                Clay_LayoutElement *newTextLayoutElement = Clay_LayoutElementArray_Add(&Clay__layoutElements, (Clay_LayoutElement) {
                    .id = Clay__RehashWithNumber(containerElement->id, containerElement->children.length),
                    .elementType = CLAY__LAYOUT_ELEMENT_TYPE_TEXT,
                    .text = stringToRender,
                    .layoutConfig = &CLAY_LAYOUT_DEFAULT,
                    .elementConfig.textElementConfig = containerElement->elementConfig.textElementConfig,
                    .dimensions = { lineDimensions.width, lineDimensions.height },
                });
                containerElement->dimensions.height += lineDimensions.height + (float)(containerElement->children.length > 0 ? textConfig->lineSpacing : 0);
                containerElement->children.length++;
                lineDimensions = (Clay_Dimensions) {};
                Clay__LayoutElementPointerArray_Add(&Clay__layoutElementChildren, newTextLayoutElement);
            } else {
                // In the middle of a word
                wordEndIndex++;
            }
        }
    }

    // Scale vertical image heights according to aspect ratio
    for (int i = 0; i < Clay__imageElementPointers.length; ++i) {
        Clay_LayoutElement* imageElement = *Clay__LayoutElementPointerArray_Get(&Clay__imageElementPointers, i);
        Clay_ImageElementConfig *config = imageElement->elementConfig.imageElementConfig;
        imageElement->dimensions.height = (config->sourceDimensions.height / CLAY__MAX(config->sourceDimensions.width, 1)) * imageElement->dimensions.width;
    }

    // Propagate effect of text wrapping, image aspect scaling etc. on height of parents
    Clay__LayoutElementTreeNodeArray dfsBuffer = Clay__layoutElementTreeNodeArray1;
    dfsBuffer.length = 0;
    for (int i = 0; i < Clay__layoutElementTreeRoots.length; ++i) {
        Clay__LayoutElementTreeRoot *root = Clay__LayoutElementTreeRootArray_Get(&Clay__layoutElementTreeRoots, i);
        Clay__LayoutElementTreeNodeArray_Add(&dfsBuffer, (Clay__LayoutElementTreeNode) { .layoutElement = root->layoutElement });
    }
    Clay__treeNodeVisited.internalArray[0] = false;
    while (dfsBuffer.length > 0) {
        Clay__LayoutElementTreeNode *currentElementTreeNode = Clay__LayoutElementTreeNodeArray_Get(&dfsBuffer, (int)dfsBuffer.length - 1);
        Clay_LayoutElement *currentElement = currentElementTreeNode->layoutElement;
        if (!Clay__treeNodeVisited.internalArray[dfsBuffer.length - 1]) {
            Clay__treeNodeVisited.internalArray[dfsBuffer.length - 1] = true;
            // If the element has no children or is the container for a text element, don't bother inspecting it
            if (currentElement->children.length == 0 || (currentElement->children.length > 0 && currentElement->children.elements[0]->elementType == CLAY__LAYOUT_ELEMENT_TYPE_TEXT)) {
                dfsBuffer.length--;
                continue;
            }
            // Add the children to the DFS buffer (needs to be pushed in reverse so that stack traversal is in correct layout order)
            for (int i = 0; i < currentElement->children.length; i++) {
                Clay__treeNodeVisited.internalArray[dfsBuffer.length] = false;
                Clay__LayoutElementTreeNodeArray_Add(&dfsBuffer, (Clay__LayoutElementTreeNode) { .layoutElement = currentElement->children.elements[i] });
            }
            continue;
        }
        dfsBuffer.length--;

        // DFS node has been visited, this is on the way back up to the root
        Clay_LayoutConfig *layoutConfig = currentElement->layoutConfig;
        if (layoutConfig->sizing.height.type == CLAY__SIZING_TYPE_PERCENT) {
            continue;
        }
        if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
            // Resize any parent containers that have grown in height along their non layout axis
            for (int j = 0; j < currentElement->children.length; ++j) {
                Clay_LayoutElement *childElement = currentElement->children.elements[j];
                float childHeightWithPadding = CLAY__MAX(childElement->dimensions.height + layoutConfig->padding.y * 2, currentElement->dimensions.height);
                currentElement->dimensions.height = CLAY__MIN(CLAY__MAX(childHeightWithPadding, layoutConfig->sizing.height.sizeMinMax.min), layoutConfig->sizing.height.sizeMinMax.max);
            }
        } else if (layoutConfig->layoutDirection == CLAY_TOP_TO_BOTTOM) {
            // Resizing along the layout axis
            float contentHeight = (float)layoutConfig->padding.y * 2;
            for (int j = 0; j < currentElement->children.length; ++j) {
                Clay_LayoutElement *childElement = currentElement->children.elements[j];
                contentHeight += childElement->dimensions.height;
            }
            contentHeight += (float)(CLAY__MAX(currentElement->children.length - 1, 0) * layoutConfig->childGap);
            currentElement->dimensions.height = CLAY__MIN(CLAY__MAX(contentHeight, layoutConfig->sizing.height.sizeMinMax.min), layoutConfig->sizing.height.sizeMinMax.max);
        }
    }

    // Calculate sizing along the Y axis
    Clay__SizeContainersAlongAxis(false);

    // layoutElementsHashMap has non-linear access pattern so just resetting .length won't zero out the data.
    // Need to zero it all out here
    for (int i = 0; i < Clay__layoutElementsHashMap.capacity; ++i) {
        Clay__layoutElementsHashMap.internalArray[i] = -1;
    }
    Clay__layoutElementsHashMapInternal.length = 0;

    // Calculate final positions and generate render commands
    Clay__renderCommands.length = 0;
    dfsBuffer.length = 0;
    for (int rootIndex = 0; rootIndex < Clay__layoutElementTreeRoots.length; ++rootIndex) {
        dfsBuffer.length = 0;
        Clay__LayoutElementTreeRoot *root = Clay__LayoutElementTreeRootArray_Get(&Clay__layoutElementTreeRoots, rootIndex);
        Clay_Vector2 rootPosition = (Clay_Vector2) {};
        Clay_LayoutElementHashMapItem *parentHashMapItem = Clay__GetHashMapItem(root->parentId);
        // Position root floating containers
        if (parentHashMapItem) {
            Clay_FloatingElementConfig *config = root->layoutElement->elementConfig.floatingElementConfig;
            Clay_Dimensions rootDimensions = root->layoutElement->dimensions;
            Clay_BoundingBox parentBoundingBox = parentHashMapItem->boundingBox;
            // Set X position
            Clay_Vector2 targetAttachPosition = (Clay_Vector2){};
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
                Clay_RenderCommandArray_Add(&Clay__renderCommands, (Clay_RenderCommand) {
                    .id = Clay__RehashWithNumber(root->layoutElement->id, 10), // TODO need a better strategy for managing derived ids
                    .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_START,
                    .boundingBox = clipHashMapItem->boundingBox,
                });
            }
        }
        Clay__LayoutElementTreeNodeArray_Add(&dfsBuffer, (Clay__LayoutElementTreeNode) { .layoutElement = root->layoutElement, .position = rootPosition, .nextChildOffset = (Clay_Vector2) { .x = (float)root->layoutElement->layoutConfig->padding.x, .y = (float)root->layoutElement->layoutConfig->padding.y } });

        Clay__treeNodeVisited.internalArray[0] = false;
        while (dfsBuffer.length > 0) {
            Clay__LayoutElementTreeNode *currentElementTreeNode = Clay__LayoutElementTreeNodeArray_Get(&dfsBuffer, (int)dfsBuffer.length - 1);
            Clay_LayoutElement *currentElement = currentElementTreeNode->layoutElement;
            Clay_LayoutConfig *layoutConfig = currentElement->layoutConfig;
            Clay_Vector2 scrollOffset = {0};

            // This will only be run a single time for each element in downwards DFS order
            if (!Clay__treeNodeVisited.internalArray[dfsBuffer.length - 1]) {
                Clay__treeNodeVisited.internalArray[dfsBuffer.length - 1] = true;

                Clay_BoundingBox currentElementBoundingBox = (Clay_BoundingBox) { currentElementTreeNode->position.x, currentElementTreeNode->position.y, currentElement->dimensions.width, currentElement->dimensions.height };
                if (currentElement->elementType == CLAY__LAYOUT_ELEMENT_TYPE_FLOATING_CONTAINER) {
                    Clay_FloatingElementConfig *floatingElementConfig = currentElement->elementConfig.floatingElementConfig;
                    Clay_Dimensions expand = floatingElementConfig->expand;
                    currentElementBoundingBox.x -= expand.width;
                    currentElementBoundingBox.width += expand.width * 2;
                    currentElementBoundingBox.y -= expand.height;
                    currentElementBoundingBox.height += expand.height * 2;
                }

                Clay__ScrollContainerDataInternal *scrollContainerData = CLAY__NULL;
                // Apply scroll offsets to container
                if (currentElement->elementType == CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER) {
                    Clay_RenderCommandArray_Add(&Clay__renderCommands, (Clay_RenderCommand) {
                        .id = Clay__RehashWithNumber(currentElement->id, 10),
                       .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_START,
                       .boundingBox = currentElementBoundingBox,
                    });

                    // This linear scan could theoretically be slow under very strange conditions, but I can't imagine a real UI with more than a few 10's of scroll containers
                    for (int i = 0; i < Clay__scrollContainerDatas.length; i++) {
                        Clay__ScrollContainerDataInternal *mapping = Clay__ScrollContainerDataInternalArray_Get(&Clay__scrollContainerDatas, i);
                        if (mapping->layoutElement == currentElement) {
                            scrollContainerData = mapping;
                            mapping->boundingBox = currentElementBoundingBox;
                            Clay_ScrollElementConfig *config = mapping->layoutElement->elementConfig.scrollElementConfig;
                            if (config->horizontal) {
                                scrollOffset.x = mapping->scrollPosition.x;
                            }
                            if (config->vertical) {
                                scrollOffset.y = mapping->scrollPosition.y;
                            }
                            break;
                        }
                    }
                }

                // Create the render command for this element
                Clay_RenderCommand renderCommand = (Clay_RenderCommand) {
                    .id = currentElement->id,
                    .commandType = Clay__LayoutElementTypeToRenderCommandType[currentElement->elementType],
                    .boundingBox = currentElementBoundingBox,
                    .config = currentElement->elementConfig
                };

                Clay_LayoutElementHashMapItem *hashMapItem = Clay__AddHashMapItem(currentElement->id, currentElement);
                if (hashMapItem) {
                    hashMapItem->boundingBox = renderCommand.boundingBox;
                }

                // Culling - Don't bother to generate render commands for rectangles entirely outside the screen - this won't stop their children from being rendered if they overflow
                bool offscreen = currentElementBoundingBox.x > (float)screenWidth || currentElementBoundingBox.y > (float)screenHeight || currentElementBoundingBox.x + currentElementBoundingBox.width < 0 || currentElementBoundingBox.y + currentElementBoundingBox.height < 0;
                bool shouldRender = !offscreen;
                switch (renderCommand.commandType) {
                    case CLAY_RENDER_COMMAND_TYPE_NONE: {
                        shouldRender = false;
                        break;
                    }
                    case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                        renderCommand.text = currentElement->text;
                        break;
                    }
                    case CLAY_RENDER_COMMAND_TYPE_BORDER: { // We render borders on close because they need to render above children
                        shouldRender = false;
                        break;
                    }
                    default: break;
                }
                if (shouldRender) {
                    Clay_RenderCommandArray_Add(&Clay__renderCommands, renderCommand);
                }
                if (offscreen) {
                    // NOTE: You may be tempted to try an early return / continue if an element is off screen. Why bother calculating layout for its children, right?
                    // Unfortunately, a FLOATING_CONTAINER may be defined that attaches to a child or grandchild of this element, which is large enough to still
                    // be on screen, even if this element isn't. That depends on this element and it's children being laid out correctly (even if they are entirely off screen)
                }

                // Handle child alignment along the layout axis
                if (currentElementTreeNode->layoutElement->elementType != CLAY__LAYOUT_ELEMENT_TYPE_TEXT) {
                    dfsBuffer.length += currentElement->children.length;

                    Clay_Dimensions contentSize = (Clay_Dimensions) {0,0};
                    if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                        for (int i = 0; i < currentElement->children.length; ++i) {
                            Clay_LayoutElement *childElement = currentElement->children.elements[i];
                            contentSize.width += childElement->dimensions.width;
                            contentSize.height = CLAY__MAX(contentSize.height, childElement->dimensions.height);
                        }
                        contentSize.width += (float)(CLAY__MAX(currentElement->children.length - 1, 0) * layoutConfig->childGap);
                        float extraSpace = currentElement->dimensions.width - (float)layoutConfig->padding.x * 2 - contentSize.width;
                        switch (layoutConfig->childAlignment.x) {
                            case CLAY_ALIGN_X_LEFT: extraSpace = 0; break;
                            case CLAY_ALIGN_X_CENTER: extraSpace /= 2; break;
                            default: break;
                        }
                        currentElementTreeNode->nextChildOffset.x += extraSpace;
                    } else {
                        for (int i = 0; i < currentElement->children.length; ++i) {
                            Clay_LayoutElement *childElement = currentElement->children.elements[i];
                            contentSize.width = CLAY__MAX(contentSize.width, childElement->dimensions.width);
                            contentSize.height += childElement->dimensions.height;
                        }
                        contentSize.height += (float)(CLAY__MAX(currentElement->children.length - 1, 0) * layoutConfig->childGap);
                        float extraSpace = currentElement->dimensions.height - (float)layoutConfig->padding.y * 2 - contentSize.height;
                        switch (layoutConfig->childAlignment.y) {
                            case CLAY_ALIGN_Y_TOP: extraSpace = 0; break;
                            case CLAY_ALIGN_Y_CENTER: extraSpace /= 2; break;
                            default: break;
                        }
                        currentElementTreeNode->nextChildOffset.y += extraSpace;
                    }

                    if (scrollContainerData) {
                        scrollContainerData->contentSize = contentSize;
                    }
                }
            } else {
                // DFS is returning upwards backwards
                if (currentElement->elementType == CLAY__LAYOUT_ELEMENT_TYPE_SCROLL_CONTAINER) {
                    Clay_RenderCommandArray_Add(&Clay__renderCommands, (Clay_RenderCommand) {
                        .id = Clay__RehashWithNumber(currentElement->id, 11),
                       .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_END,
                    });
                // Borders between elements are expressed as additional rectangle render commands
                } else if (currentElement->elementType == CLAY__LAYOUT_ELEMENT_TYPE_BORDER_CONTAINER) {
                    Clay_BoundingBox currentElementBoundingBox = (Clay_BoundingBox) { currentElementTreeNode->position.x, currentElementTreeNode->position.y, currentElement->dimensions.width, currentElement->dimensions.height };
                    bool offscreen = currentElementBoundingBox.x > (float)screenWidth || currentElementBoundingBox.y > (float)screenHeight || currentElementBoundingBox.x + currentElementBoundingBox.width < 0 || currentElementBoundingBox.y + currentElementBoundingBox.height < 0;
                    if (offscreen) {
                        dfsBuffer.length--;
                        continue;
                    }
                    Clay_BorderElementConfig *borderConfig = currentElement->elementConfig.borderElementConfig;

                    Clay_RenderCommandArray_Add(&Clay__renderCommands, (Clay_RenderCommand) {
                        .id = currentElement->id,
                        .commandType = CLAY_RENDER_COMMAND_TYPE_BORDER,
                        .boundingBox = currentElementBoundingBox,
                        .config = currentElement->elementConfig
                    });

                    // Render border elements between children
                    if (borderConfig->betweenChildren.width > 0 && borderConfig->betweenChildren.color.a > 0) {
                        Clay_Vector2 borderOffset = { (float)layoutConfig->padding.x, (float)layoutConfig->padding.y };
                        if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                            for (int i = 0; i < currentElement->children.length; ++i) {
                                Clay_LayoutElement *childElement = currentElement->children.elements[i];
                                if (i > 0) {
                                    Clay_RenderCommandArray_Add(&Clay__renderCommands, (Clay_RenderCommand) {
                                        .id = Clay__RehashWithNumber(currentElement->id, 5 + i),
                                        .commandType = CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
                                        .boundingBox = { currentElementBoundingBox.x + borderOffset.x, currentElementBoundingBox.y, (float)borderConfig->betweenChildren.width, currentElement->dimensions.height },
                                        .config = CLAY_RECTANGLE_CONFIG(.color = borderConfig->betweenChildren.color)
                                    });
                                }
                                borderOffset.x += (childElement->dimensions.width + (float)layoutConfig->childGap / 2);
                            }
                        } else {
                            for (int i = 0; i < currentElement->children.length; ++i) {
                                Clay_LayoutElement *childElement = currentElement->children.elements[i];
                                if (i > 0) {
                                    Clay_RenderCommandArray_Add(&Clay__renderCommands, (Clay_RenderCommand) {
                                        .id = Clay__RehashWithNumber(currentElement->id, 5 + i),
                                        .commandType = CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
                                        .boundingBox = { currentElementBoundingBox.x, currentElementBoundingBox.y + borderOffset.y, currentElement->dimensions.width, (float)borderConfig->betweenChildren.width },
                                        .config = CLAY_RECTANGLE_CONFIG(.color = borderConfig->betweenChildren.color)
                                    });
                                }
                                borderOffset.y += (childElement->dimensions.height + (float)layoutConfig->childGap / 2);
                            }
                        }
                    }
                }
                dfsBuffer.length--;
                continue;
            }

            // Add children to the DFS buffer
            if (currentElement->elementType != CLAY__LAYOUT_ELEMENT_TYPE_TEXT) {
                for (int i = 0; i < currentElement->children.length; ++i) {
                    Clay_LayoutElement *childElement = currentElement->children.elements[i];
                    // Alignment along non layout axis
                    if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                        currentElementTreeNode->nextChildOffset.y = currentElement->layoutConfig->padding.y;
                        float whiteSpaceAroundChild = currentElement->dimensions.height - (float)currentElement->layoutConfig->padding.y * 2 - childElement->dimensions.height;
                        switch (layoutConfig->childAlignment.y) {
                            case CLAY_ALIGN_Y_TOP: break;
                            case CLAY_ALIGN_Y_CENTER: currentElementTreeNode->nextChildOffset.y += whiteSpaceAroundChild / 2; break;
                            case CLAY_ALIGN_Y_BOTTOM: currentElementTreeNode->nextChildOffset.y += whiteSpaceAroundChild; break;
                        }
                    } else {
                        currentElementTreeNode->nextChildOffset.x = currentElement->layoutConfig->padding.x;
                        float whiteSpaceAroundChild = currentElement->dimensions.width - (float)currentElement->layoutConfig->padding.x * 2 - childElement->dimensions.width;
                        switch (layoutConfig->childAlignment.x) {
                            case CLAY_ALIGN_X_LEFT: break;
                            case CLAY_ALIGN_X_CENTER: currentElementTreeNode->nextChildOffset.x += whiteSpaceAroundChild / 2; break;
                            case CLAY_ALIGN_X_RIGHT: currentElementTreeNode->nextChildOffset.x += whiteSpaceAroundChild; break;
                        }
                    }

                    Clay_Vector2 childPosition = (Clay_Vector2) {
                        currentElementTreeNode->position.x + currentElementTreeNode->nextChildOffset.x + scrollOffset.x,
                        currentElementTreeNode->position.y + currentElementTreeNode->nextChildOffset.y + scrollOffset.y,
                    };

                    // DFS buffer elements need to be added in reverse because stack traversal happens backwards
                    uint32_t newNodeIndex = dfsBuffer.length - 1 - i;
                    dfsBuffer.internalArray[newNodeIndex] = (Clay__LayoutElementTreeNode) {
                        .layoutElement = childElement,
                        .position = (Clay_Vector2) { childPosition.x, childPosition.y },
                        .nextChildOffset = (Clay_Vector2) { .x = (float)childElement->layoutConfig->padding.x, .y = (float)childElement->layoutConfig->padding.y },
                    };
                    Clay__treeNodeVisited.internalArray[newNodeIndex] = false;

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
            Clay_RenderCommandArray_Add(&Clay__renderCommands, (Clay_RenderCommand) { .id = Clay__RehashWithNumber(root->layoutElement->id, 11), .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_END });
        }
    }
}

// PUBLIC API FROM HERE ---------------------------------------

CLAY_WASM_EXPORT("Clay_MinMemorySize")
uint32_t Clay_MinMemorySize() {
    Clay_Arena fakeArena = (Clay_Arena) { .capacity = INT64_MAX };
    Clay__InitializePersistentMemory(&fakeArena);
    Clay__InitializeEphemeralMemory(&fakeArena);
    return fakeArena.nextAllocation;
}

CLAY_WASM_EXPORT("Clay_CreateArenaWithCapacityAndMemory")
Clay_Arena Clay_CreateArenaWithCapacityAndMemory(uint32_t capacity, void *offset) {
    Clay_Arena arena = (Clay_Arena) {
        .capacity = capacity,
        .memory = (char *)offset
    };
    return arena;
}

#ifndef CLAY_WASM
void Clay_SetMeasureTextFunction(Clay_Dimensions (*measureTextFunction)(Clay_String *text, Clay_TextElementConfig *config)) {
    Clay__MeasureText = measureTextFunction;
}
#endif

CLAY_WASM_EXPORT("Clay_SetPointerPosition")
void Clay_SetPointerPosition(Clay_Vector2 position) {
    Clay__pointerPosition = position;
    Clay__pointerOverIds.length = 0;
    Clay__LayoutElementPointerArray dfsBuffer = Clay__layoutElementChildrenBuffer;
    for (int rootIndex = 0; rootIndex < Clay__layoutElementTreeRoots.length; ++rootIndex) {
        dfsBuffer.length = 0;
        Clay__LayoutElementTreeRoot *root = Clay__LayoutElementTreeRootArray_Get(&Clay__layoutElementTreeRoots, rootIndex);
        Clay__LayoutElementPointerArray_Add(&dfsBuffer, root->layoutElement);
        Clay__treeNodeVisited.internalArray[0] = false;
        while (dfsBuffer.length > 0) {
            if (Clay__treeNodeVisited.internalArray[dfsBuffer.length - 1]) {
                dfsBuffer.length--;
                continue;
            }
            Clay__treeNodeVisited.internalArray[dfsBuffer.length - 1] = true;
            Clay_LayoutElement *currentElement = *Clay__LayoutElementPointerArray_Get(&dfsBuffer, (int)dfsBuffer.length - 1);
            Clay_LayoutElementHashMapItem *mapItem = Clay__GetHashMapItem(currentElement->id); // TODO I wish there was a way around this, maybe the fact that it's essentially a binary tree limits the cost, have to measure
            if ((mapItem && Clay__PointIsInsideRect(position, mapItem->boundingBox)) || (!mapItem && Clay__PointIsInsideRect(position, (Clay_BoundingBox) {0,0, currentElement->dimensions.width, currentElement->dimensions.height}))) {
                Clay__int32_tArray_Add(&Clay__pointerOverIds, (int)currentElement->id);
                if (currentElement->elementType == CLAY__LAYOUT_ELEMENT_TYPE_TEXT) {
                    dfsBuffer.length--;
                    continue;
                }
                for (int i = currentElement->children.length - 1; i >= 0; --i) {
                    Clay__LayoutElementPointerArray_Add(&dfsBuffer, currentElement->children.elements[i]);
                    Clay__treeNodeVisited.internalArray[dfsBuffer.length - 1] = false; // TODO needs to be ranged checked
                }
            } else {
                dfsBuffer.length--;
            }
        }
    }
}

CLAY_WASM_EXPORT("Clay_Initialize")
void Clay_Initialize(Clay_Arena arena) {
    Clay__internalArena = arena;
    Clay__InitializePersistentMemory(&Clay__internalArena);
    Clay__InitializeEphemeralMemory(&Clay__internalArena);
    for (int i = 0; i < Clay__layoutElementsHashMap.capacity; ++i) {
        Clay__layoutElementsHashMap.internalArray[i] = -1;
    }
    Clay__measureTextHashMapInternal.length = 1; // Reserve the 0 value to mean "no next element"
}

CLAY_WASM_EXPORT("Clay_UpdateScrollContainers")
void Clay_UpdateScrollContainers(bool isPointerActive, Clay_Vector2 scrollDelta, float deltaTime) {
    // Don't apply scroll events to ancestors of the inner element
    int32_t highestPriorityElementIndex = -1;
    Clay__ScrollContainerDataInternal *highestPriorityScrollData = CLAY__NULL;
    for (int i = 0; i < Clay__scrollContainerDatas.length; i++) {
        Clay__ScrollContainerDataInternal *scrollData = Clay__ScrollContainerDataInternalArray_Get(&Clay__scrollContainerDatas, i);
        if (!scrollData->openThisFrame) {
            Clay__ScrollContainerDataInternalArray_RemoveSwapback(&Clay__scrollContainerDatas, i);
            continue;
        }
        scrollData->openThisFrame = false;
        Clay_LayoutElementHashMapItem *hashMapItem = Clay__GetHashMapItem(scrollData->elementId);
        // Element isn't rendered this frame but scroll offset has been retained
        if (!hashMapItem) {
            Clay__ScrollContainerDataInternalArray_RemoveSwapback(&Clay__scrollContainerDatas, i);
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

            scrollData->pointerOrigin = (Clay_Vector2){0,0};
            scrollData->scrollOrigin = (Clay_Vector2){0,0};
            scrollData->momentumTime = 0;
        }

        // Apply existing momentum
        scrollData->scrollPosition.x += scrollData->scrollMomentum.x;
        scrollData->scrollMomentum.x *= 0.95f;
        bool scrollOccurred = scrollDelta.x != 0 || scrollDelta.y != 0;
        if ((scrollData->scrollMomentum.x > -0.1f && scrollData->scrollMomentum.x < 0.1f) || scrollOccurred) {
            scrollData->scrollMomentum.x = 0;
        }
        scrollData->scrollPosition.x = CLAY__MAX(CLAY__MIN(scrollData->scrollPosition.x, 0), -(scrollData->contentSize.width - scrollData->layoutElement->dimensions.width));

        scrollData->scrollPosition.y += scrollData->scrollMomentum.y;
        scrollData->scrollMomentum.y *= 0.95f;
        if ((scrollData->scrollMomentum.y > -0.1f && scrollData->scrollMomentum.y < 0.1f) || scrollOccurred) {
            scrollData->scrollMomentum.y = 0;
        }
        scrollData->scrollPosition.y = CLAY__MAX(CLAY__MIN(scrollData->scrollPosition.y, 0), -(scrollData->contentSize.height - scrollData->layoutElement->dimensions.height));

        for (int j = 0; j < Clay__pointerOverIds.length; ++j) { // TODO n & m are small here but this being n*m gives me the creeps
            if (scrollData->layoutElement->id == *Clay__int32_tArray_Get(&Clay__pointerOverIds, j)) {
                highestPriorityElementIndex = j;
                highestPriorityScrollData = scrollData;
            }
        }
    }

    if (highestPriorityElementIndex > -1 && highestPriorityScrollData) {
        Clay_LayoutElement *scrollElement = highestPriorityScrollData->layoutElement;
        bool canScrollVertically = scrollElement->elementConfig.scrollElementConfig->vertical && highestPriorityScrollData->contentSize.height > scrollElement->dimensions.height;
        bool canScrollHorizontally = scrollElement->elementConfig.scrollElementConfig->horizontal && highestPriorityScrollData->contentSize.width > scrollElement->dimensions.width;
        // Handle wheel scroll
        if (canScrollVertically) {
            highestPriorityScrollData->scrollPosition.y = highestPriorityScrollData->scrollPosition.y + scrollDelta.y * 10;
        }
        if (canScrollHorizontally) {
            highestPriorityScrollData->scrollPosition.x = highestPriorityScrollData->scrollPosition.x + scrollDelta.x * 10;
        }
        // Handle click / touch scroll
        if (isPointerActive) {
            highestPriorityScrollData->scrollMomentum = (Clay_Vector2){0};
            if (!highestPriorityScrollData->pointerScrollActive) {
                highestPriorityScrollData->pointerOrigin = Clay__pointerPosition;
                highestPriorityScrollData->scrollOrigin = highestPriorityScrollData->scrollPosition;
                highestPriorityScrollData->pointerScrollActive = true;
            } else {
                float scrollDeltaX = 0, scrollDeltaY = 0;
                if (canScrollHorizontally) {
                    float oldXScrollPosition = highestPriorityScrollData->scrollPosition.x;
                    highestPriorityScrollData->scrollPosition.x = highestPriorityScrollData->scrollOrigin.x + (Clay__pointerPosition.x - highestPriorityScrollData->pointerOrigin.x);
                    highestPriorityScrollData->scrollPosition.x = CLAY__MAX(CLAY__MIN(highestPriorityScrollData->scrollPosition.x, 0), -(highestPriorityScrollData->contentSize.width - highestPriorityScrollData->boundingBox.width));
                    scrollDeltaX = highestPriorityScrollData->scrollPosition.x - oldXScrollPosition;
                }
                if (canScrollVertically) {
                    float oldYScrollPosition = highestPriorityScrollData->scrollPosition.y;
                    highestPriorityScrollData->scrollPosition.y = highestPriorityScrollData->scrollOrigin.y + (Clay__pointerPosition.y - highestPriorityScrollData->pointerOrigin.y);
                    highestPriorityScrollData->scrollPosition.y = CLAY__MAX(CLAY__MIN(highestPriorityScrollData->scrollPosition.y, 0), -(highestPriorityScrollData->contentSize.height - highestPriorityScrollData->boundingBox.height));
                    scrollDeltaY = highestPriorityScrollData->scrollPosition.y - oldYScrollPosition;
                }
                if (scrollDeltaX > -0.1f && scrollDeltaX < 0.1f && scrollDeltaY > -0.1f && scrollDeltaY < 0.1f && highestPriorityScrollData->momentumTime > 0.15f) {
                    highestPriorityScrollData->momentumTime = 0;
                    highestPriorityScrollData->pointerOrigin = Clay__pointerPosition;
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
void Clay_BeginLayout(int screenWidth, int screenHeight) {
    Clay__InitializeEphemeralMemory(&Clay__internalArena);
    // Set up the root container that covers the entire window
    Clay_LayoutElement rootLayoutElement = (Clay_LayoutElement){.layoutConfig = CLAY_LAYOUT(.sizing = {CLAY_SIZING_FIXED((float)screenWidth), CLAY_SIZING_FIXED((float)screenHeight)})};
    Clay__openLayoutElement = Clay_LayoutElementArray_Add(&Clay__layoutElements, rootLayoutElement);
    Clay__LayoutElementPointerArray_Add(&Clay__openLayoutElementStack, Clay__openLayoutElement);
    Clay__LayoutElementTreeRootArray_Add(&Clay__layoutElementTreeRoots, (Clay__LayoutElementTreeRoot) { .layoutElement = Clay__openLayoutElement });
}

CLAY_WASM_EXPORT("Clay_EndLayout")
Clay_RenderCommandArray Clay_EndLayout(int screenWidth, int screenHeight)
{
    Clay__AttachContainerChildren();
    Clay__CalculateFinalLayout(screenWidth, screenHeight);
    return Clay__renderCommands;
}

CLAY_WASM_EXPORT("Clay_PointerOver")
bool Clay_PointerOver(uint32_t id) { // TODO return priority for separating multiple results
    for (int i = 0; i < Clay__pointerOverIds.length; ++i) {
        if (*Clay__int32_tArray_Get(&Clay__pointerOverIds, i) == id) {
            return true;
        }
    }
    return false;
}

typedef struct
{
    // Note: This is a pointer to the real internal scroll position, mutating it may cause a change in final layout.
    // Intended for use with external functionality that modifies scroll position, such as scroll bars or auto scrolling.
    Clay_Vector2 *scrollPosition;
    Clay_Dimensions scrollContainerDimensions;
    Clay_Dimensions contentDimensions;
    Clay_ScrollElementConfig config;
    // Indicates whether an actual scroll container matched the provided ID or if the default struct was returned.
    bool found;
} Clay_ScrollContainerData;

CLAY_WASM_EXPORT("Clay_GetScrollContainerData")
Clay_ScrollContainerData Clay_GetScrollContainerData(uint32_t id) {
    for (int i = 0; i < Clay__scrollContainerDatas.length; ++i) {
        Clay__ScrollContainerDataInternal *scrollContainerData = Clay__ScrollContainerDataInternalArray_Get(&Clay__scrollContainerDatas, i);
        if (scrollContainerData->elementId == id) {
            return (Clay_ScrollContainerData) {
                .scrollPosition = &scrollContainerData->scrollPosition,
                .scrollContainerDimensions = (Clay_Dimensions) { scrollContainerData->boundingBox.width, scrollContainerData->boundingBox.height },
                .contentDimensions = scrollContainerData->contentSize,
                .config = *scrollContainerData->layoutElement->elementConfig.scrollElementConfig,
                .found = true
            };
        }
    }
    return (Clay_ScrollContainerData){};
}

#endif //CLAY_IMPLEMENTATION

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