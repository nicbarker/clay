// VERSION: 0.14

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

// SIMD includes on supported platforms
#if !defined(CLAY_DISABLE_SIMD) && (defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64))
#include <emmintrin.h>
#elif !defined(CLAY_DISABLE_SIMD) && defined(__aarch64__)
#include <arm_neon.h>
#endif
#if __CLION_IDE__
#define CLAY_IMPLEMENTATION
#endif

// -----------------------------------------
// HEADER DECLARATIONS ---------------------
// -----------------------------------------

#ifndef CLAY_HEADER
#define CLAY_HEADER

#if !( \
    (defined(__cplusplus) && __cplusplus >= 202002L) || \
    (defined(__STDC__) && __STDC__ == 1 && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
    defined(_MSC_VER) || \
    defined(__OBJC__) \
)
#error "Clay requires C99, C++20, or MSVC"
#endif

#ifdef CLAY_WASM
#define CLAY_WASM_EXPORT(name) __attribute__((export_name(name)))
#else
#define CLAY_WASM_EXPORT(null)
#endif

#ifdef CLAY_DLL
#define CLAY_DLL_EXPORT __declspec(dllexport) __stdcall
#else
#define CLAY_DLL_EXPORT
#endif

// Public Macro API ------------------------

#define CLAY__MAX(x, y) (((x) > (y)) ? (x) : (y))
#define CLAY__MIN(x, y) (((x) < (y)) ? (x) : (y))

#define CLAY_TEXT_CONFIG(...) __VA_ARGS__

#define CLAY_BORDER_OUTSIDE(widthValue) {widthValue, widthValue, widthValue, widthValue, 0}

#define CLAY_BORDER_ALL(widthValue) {widthValue, widthValue, widthValue, widthValue, widthValue}

#define CLAY_CORNER_RADIUS(radius) (CLAY__INIT(Clay_CornerRadius) { radius, radius, radius, radius })

#define CLAY_PADDING_ALL(padding) CLAY__CONFIG_WRAPPER(Clay_Padding, { padding, padding, padding, padding })

#define CLAY_SIZING_FIT(...) (CLAY__INIT(Clay_SizingAxis) { .size = { .minMax = { __VA_ARGS__ } }, .type = CLAY__SIZING_TYPE_FIT })

#define CLAY_SIZING_GROW(...) (CLAY__INIT(Clay_SizingAxis) { .size = { .minMax = { __VA_ARGS__ } }, .type = CLAY__SIZING_TYPE_GROW })

#define CLAY_SIZING_FIXED(fixedSize) (CLAY__INIT(Clay_SizingAxis) { .size = { .minMax = { fixedSize, fixedSize } }, .type = CLAY__SIZING_TYPE_FIXED })

#define CLAY_SIZING_PERCENT(percentOfParent) (CLAY__INIT(Clay_SizingAxis) { .size = { .percent = (percentOfParent) }, .type = CLAY__SIZING_TYPE_PERCENT })

// Note: If a compile error led you here, you might be trying to use CLAY_ID with something other than a string literal. To construct an ID with a dynamic string, use CLAY_SID instead.
#define CLAY_ID(label) CLAY_SID(CLAY_STRING(label))

#define CLAY_SID(label) Clay__HashString(label, 0)

// Note: If a compile error led you here, you might be trying to use CLAY_IDI with something other than a string literal. To construct an ID with a dynamic string, use CLAY_SIDI instead.
#define CLAY_IDI(label, index) CLAY_SIDI(CLAY_STRING(label), index)

#define CLAY_SIDI(label, index) Clay__HashStringWithOffset(label, index, 0)

// Note: If a compile error led you here, you might be trying to use CLAY_ID_LOCAL with something other than a string literal. To construct an ID with a dynamic string, use CLAY_SID_LOCAL instead.
#define CLAY_ID_LOCAL(label) CLAY_SID_LOCAL(CLAY_STRING(label))

#define CLAY_SID_LOCAL(label) Clay__HashString(label, Clay_GetOpenElementId())

// Note: If a compile error led you here, you might be trying to use CLAY_IDI_LOCAL with something other than a string literal. To construct an ID with a dynamic string, use CLAY_SIDI_LOCAL instead.
#define CLAY_IDI_LOCAL(label, index) CLAY_SIDI_LOCAL(CLAY_STRING(label), index)

#define CLAY_SIDI_LOCAL(label, index) Clay__HashStringWithOffset(label, index, Clay_GetOpenElementId())

#define CLAY__STRING_LENGTH(s) ((sizeof(s) / sizeof((s)[0])) - sizeof((s)[0]))

#define CLAY__ENSURE_STRING_LITERAL(x) ("" x "")

// Note: If an error led you here, it's because CLAY_STRING can only be used with string literals, i.e. CLAY_STRING("SomeString") and not CLAY_STRING(yourString)
#define CLAY_STRING(string) (CLAY__INIT(Clay_String) { .isStaticallyAllocated = true, .length = CLAY__STRING_LENGTH(CLAY__ENSURE_STRING_LITERAL(string)), .chars = (string) })

#define CLAY_STRING_CONST(string) { .isStaticallyAllocated = true, .length = CLAY__STRING_LENGTH(CLAY__ENSURE_STRING_LITERAL(string)), .chars = (string) }

static uint8_t CLAY__ELEMENT_DEFINITION_LATCH;

// GCC marks the above CLAY__ELEMENT_DEFINITION_LATCH as an unused variable for files that include clay.h but don't declare any layout
// This is to suppress that warning
static inline void Clay__SuppressUnusedLatchDefinitionVariableWarning(void) { (void) CLAY__ELEMENT_DEFINITION_LATCH; }

// Publicly visible layout element macros -----------------------------------------------------

/* This macro looks scary on the surface, but is actually quite simple.
  It turns a macro call like this:

  CLAY({
    .id = CLAY_ID("Container"),
    .backgroundColor = { 255, 200, 200, 255 }
  }) {
      ...children declared here
  }

  Into calls like this:

  Clay__OpenElement();
  Clay__ConfigureOpenElement((Clay_ElementDeclaration) {
    .id = CLAY_ID("Container"),
    .backgroundColor = { 255, 200, 200, 255 }
  });
  ...children declared here
  Clay__CloseElement();

  The for loop will only ever run a single iteration, putting Clay__CloseElement() in the increment of the loop
  means that it will run after the body - where the children are declared. It just exists to make sure you don't forget
  to call Clay_CloseElement().
*/
#define CLAY_AUTO_ID(...)                                                                                                                                   \
    for (                                                                                                                                                   \
        CLAY__ELEMENT_DEFINITION_LATCH = (Clay__OpenElement(), Clay__ConfigureOpenElement(CLAY__CONFIG_WRAPPER(Clay_ElementDeclaration, __VA_ARGS__)), 0);  \
        CLAY__ELEMENT_DEFINITION_LATCH < 1;                                                                                                                 \
        CLAY__ELEMENT_DEFINITION_LATCH=1, Clay__CloseElement()                                                                                              \
    )

#define CLAY(id, ...)                                                                                                                                               \
    for (                                                                                                                                                           \
        CLAY__ELEMENT_DEFINITION_LATCH = (Clay__OpenElementWithId(id), Clay__ConfigureOpenElement(CLAY__CONFIG_WRAPPER(Clay_ElementDeclaration, __VA_ARGS__)), 0);  \
        CLAY__ELEMENT_DEFINITION_LATCH < 1;                                                                                                                         \
        CLAY__ELEMENT_DEFINITION_LATCH=1, Clay__CloseElement()                                                                                                      \
    )

// These macros exist to allow the CLAY() macro to be called both with an inline struct definition, such as
// CLAY({ .id = something... });
// As well as by passing a predefined declaration struct
// Clay_ElementDeclaration declarationStruct = ...
// CLAY(declarationStruct);
#define CLAY__WRAPPER_TYPE(type) Clay__##type##Wrapper
#define CLAY__WRAPPER_STRUCT(type) typedef struct { type wrapped; } CLAY__WRAPPER_TYPE(type)
#define CLAY__CONFIG_WRAPPER(type, ...) (CLAY__INIT(CLAY__WRAPPER_TYPE(type)) { __VA_ARGS__ }).wrapped

#define CLAY_TEXT(text, ...) Clay__OpenTextElement(text, CLAY__CONFIG_WRAPPER(Clay_TextElementConfig, __VA_ARGS__))

#ifdef __cplusplus

#define CLAY__INIT(type) type

#define CLAY_PACKED_ENUM enum : uint8_t

#define CLAY__DEFAULT_STRUCT {}

#else

#define CLAY__INIT(type) (type)

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
typedef struct Clay_String {
    // Set this boolean to true if the char* data underlying this string will live for the entire lifetime of the program.
    // This will automatically be set for strings created with CLAY_STRING, as the macro requires a string literal.
    bool isStaticallyAllocated;
    int32_t length;
    // The underlying character memory. Note: this will not be copied and will not extend the lifetime of the underlying memory.
    const char *chars;
} Clay_String;

// Clay_StringSlice is used to represent non owning string slices, and includes
// a baseChars field which points to the string this slice is derived from.
typedef struct Clay_StringSlice {
    int32_t length;
    const char *chars;
    const char *baseChars; // The source string / char* that this slice was derived from
} Clay_StringSlice;

typedef struct Clay_Context Clay_Context;

// Clay_Arena is a memory arena structure that is used by clay to manage its internal allocations.
// Rather than creating it by hand, it's easier to use Clay_CreateArenaWithCapacityAndMemory()
typedef struct Clay_Arena {
    uintptr_t nextAllocation;
    size_t capacity;
    char *memory;
} Clay_Arena;

typedef struct Clay_Dimensions {
    float width, height;
} Clay_Dimensions;

typedef struct Clay_Vector2 {
    float x, y;
} Clay_Vector2;

// Internally clay conventionally represents colors as 0-255, but interpretation is up to the renderer.
typedef struct Clay_Color {
    float r, g, b, a;
} Clay_Color;

typedef struct Clay_BoundingBox {
    float x, y, width, height;
} Clay_BoundingBox;

// Primarily created via the CLAY_ID(), CLAY_IDI(), CLAY_ID_LOCAL() and CLAY_IDI_LOCAL() macros.
// Represents a hashed string ID used for identifying and finding specific clay UI elements, required
// by functions such as Clay_PointerOver() and Clay_GetElementData().
typedef struct Clay_ElementId {
    uint32_t id; // The resulting hash generated from the other fields.
    uint32_t offset; // A numerical offset applied after computing the hash from stringId.
    uint32_t baseId; // A base hash value to start from, for example the parent element ID is used when calculating CLAY_ID_LOCAL().
    Clay_String stringId; // The string id to hash.
} Clay_ElementId;

// A sized array of Clay_ElementId.
typedef struct
{
    int32_t capacity;
    int32_t length;
    Clay_ElementId *internalArray;
} Clay_ElementIdArray;

// Controls the "radius", or corner rounding of elements, including rectangles, borders and images.
// The rounding is determined by drawing a circle inset into the element corner by (radius, radius) pixels.
typedef struct Clay_CornerRadius {
    float topLeft;
    float topRight;
    float bottomLeft;
    float bottomRight;
} Clay_CornerRadius;

// Element Configs ---------------------------

// Controls the direction in which child elements will be automatically laid out.
typedef CLAY_PACKED_ENUM {
    // (Default) Lays out child elements from left to right with increasing x.
    CLAY_LEFT_TO_RIGHT,
    // Lays out child elements from top to bottom with increasing y.
    CLAY_TOP_TO_BOTTOM,
} Clay_LayoutDirection;

// Controls the alignment along the x axis (horizontal) of child elements.
typedef CLAY_PACKED_ENUM {
    // (Default) Aligns child elements to the left hand side of this element, offset by padding.width.left
    CLAY_ALIGN_X_LEFT,
    // Aligns child elements to the right hand side of this element, offset by padding.width.right
    CLAY_ALIGN_X_RIGHT,
    // Aligns child elements horizontally to the center of this element
    CLAY_ALIGN_X_CENTER,
} Clay_LayoutAlignmentX;

// Controls the alignment along the y axis (vertical) of child elements.
typedef CLAY_PACKED_ENUM {
    // (Default) Aligns child elements to the top of this element, offset by padding.width.top
    CLAY_ALIGN_Y_TOP,
    // Aligns child elements to the bottom of this element, offset by padding.width.bottom
    CLAY_ALIGN_Y_BOTTOM,
    // Aligns child elements vertically to the center of this element
    CLAY_ALIGN_Y_CENTER,
} Clay_LayoutAlignmentY;

// Controls how the element takes up space inside its parent container.
typedef CLAY_PACKED_ENUM {
    // (default) Wraps tightly to the size of the element's contents.
    CLAY__SIZING_TYPE_FIT,
    // Expands along this axis to fill available space in the parent element, sharing it with other GROW elements.
    CLAY__SIZING_TYPE_GROW,
    // Expects 0-1 range. Clamps the axis size to a percent of the parent container's axis size minus padding and child gaps.
    CLAY__SIZING_TYPE_PERCENT,
    // Clamps the axis size to an exact size in pixels.
    CLAY__SIZING_TYPE_FIXED,
} Clay__SizingType;

// Controls how child elements are aligned on each axis.
typedef struct Clay_ChildAlignment {
    Clay_LayoutAlignmentX x; // Controls alignment of children along the x axis.
    Clay_LayoutAlignmentY y; // Controls alignment of children along the y axis.
} Clay_ChildAlignment;

// Controls the minimum and maximum size in pixels that this element is allowed to grow or shrink to,
// overriding sizing types such as FIT or GROW.
typedef struct Clay_SizingMinMax {
    float min; // The smallest final size of the element on this axis will be this value in pixels.
    float max; // The largest final size of the element on this axis will be this value in pixels.
} Clay_SizingMinMax;

// Controls the sizing of this element along one axis inside its parent container.
typedef struct Clay_SizingAxis {
    union {
        Clay_SizingMinMax minMax; // Controls the minimum and maximum size in pixels that this element is allowed to grow or shrink to, overriding sizing types such as FIT or GROW.
        float percent; // Expects 0-1 range. Clamps the axis size to a percent of the parent container's axis size minus padding and child gaps.
    } size;
    Clay__SizingType type; // Controls how the element takes up space inside its parent container.
} Clay_SizingAxis;

// Controls the sizing of this element along one axis inside its parent container.
typedef struct Clay_Sizing {
    Clay_SizingAxis width; // Controls the width sizing of the element, along the x axis.
    Clay_SizingAxis height;  // Controls the height sizing of the element, along the y axis.
} Clay_Sizing;

// Controls "padding" in pixels, which is a gap between the bounding box of this element and where its children
// will be placed.
typedef struct Clay_Padding {
    uint16_t left;
    uint16_t right;
    uint16_t top;
    uint16_t bottom;
} Clay_Padding;

CLAY__WRAPPER_STRUCT(Clay_Padding);

// Controls various settings that affect the size and position of an element, as well as the sizes and positions
// of any child elements.
typedef struct Clay_LayoutConfig {
    Clay_Sizing sizing; // Controls the sizing of this element inside it's parent container, including FIT, GROW, PERCENT and FIXED sizing.
    Clay_Padding padding; // Controls "padding" in pixels, which is a gap between the bounding box of this element and where its children will be placed.
    uint16_t childGap; // Controls the gap in pixels between child elements along the layout axis (horizontal gap for LEFT_TO_RIGHT, vertical gap for TOP_TO_BOTTOM).
    Clay_ChildAlignment childAlignment; // Controls how child elements are aligned on each axis.
    Clay_LayoutDirection layoutDirection; // Controls the direction in which child elements will be automatically laid out.
} Clay_LayoutConfig;

CLAY__WRAPPER_STRUCT(Clay_LayoutConfig);

extern Clay_LayoutConfig CLAY_LAYOUT_DEFAULT;

// Controls how text "wraps", that is how it is broken into multiple lines when there is insufficient horizontal space.
typedef CLAY_PACKED_ENUM {
    // (default) breaks on whitespace characters.
    CLAY_TEXT_WRAP_WORDS,
    // Don't break on space characters, only on newlines.
    CLAY_TEXT_WRAP_NEWLINES,
    // Disable text wrapping entirely.
    CLAY_TEXT_WRAP_NONE,
} Clay_TextElementConfigWrapMode;

// Controls how wrapped lines of text are horizontally aligned within the outer text bounding box.
typedef CLAY_PACKED_ENUM {
    // (default) Horizontally aligns wrapped lines of text to the left hand side of their bounding box.
    CLAY_TEXT_ALIGN_LEFT,
    // Horizontally aligns wrapped lines of text to the center of their bounding box.
    CLAY_TEXT_ALIGN_CENTER,
    // Horizontally aligns wrapped lines of text to the right hand side of their bounding box.
    CLAY_TEXT_ALIGN_RIGHT,
} Clay_TextAlignment;

// Controls various functionality related to text elements.
typedef struct Clay_TextElementConfig {
    // A pointer that will be transparently passed through to the resulting render command.
    void *userData;
    // The RGBA color of the font to render, conventionally specified as 0-255.
    Clay_Color textColor;
    // An integer transparently passed to Clay_MeasureText to identify the font to use.
    // The debug view will pass fontId = 0 for its internal text.
    uint16_t fontId;
    // Controls the size of the font. Handled by the function provided to Clay_MeasureText.
    uint16_t fontSize;
    // Controls extra horizontal spacing between characters. Handled by the function provided to Clay_MeasureText.
    uint16_t letterSpacing;
    // Controls additional vertical space between wrapped lines of text.
    uint16_t lineHeight;
    // Controls how text "wraps", that is how it is broken into multiple lines when there is insufficient horizontal space.
    // CLAY_TEXT_WRAP_WORDS (default) breaks on whitespace characters.
    // CLAY_TEXT_WRAP_NEWLINES doesn't break on space characters, only on newlines.
    // CLAY_TEXT_WRAP_NONE disables wrapping entirely.
    Clay_TextElementConfigWrapMode wrapMode;
    // Controls how wrapped lines of text are horizontally aligned within the outer text bounding box.
    // CLAY_TEXT_ALIGN_LEFT (default) - Horizontally aligns wrapped lines of text to the left hand side of their bounding box.
    // CLAY_TEXT_ALIGN_CENTER - Horizontally aligns wrapped lines of text to the center of their bounding box.
    // CLAY_TEXT_ALIGN_RIGHT - Horizontally aligns wrapped lines of text to the right hand side of their bounding box.
    Clay_TextAlignment textAlignment;
} Clay_TextElementConfig;

CLAY__WRAPPER_STRUCT(Clay_TextElementConfig);

// Aspect Ratio --------------------------------

// Controls various settings related to aspect ratio scaling element.
typedef struct Clay_AspectRatioElementConfig {
    float aspectRatio; // A float representing the target "Aspect ratio" for an element, which is its final width divided by its final height.
} Clay_AspectRatioElementConfig;

CLAY__WRAPPER_STRUCT(Clay_AspectRatioElementConfig);

// Image --------------------------------

// Controls various settings related to image elements.
typedef struct Clay_ImageElementConfig {
    void* imageData; // A transparent pointer used to pass image data through to the renderer.
} Clay_ImageElementConfig;

CLAY__WRAPPER_STRUCT(Clay_ImageElementConfig);

// Floating -----------------------------

// Controls where a floating element is offset relative to its parent element.
// Note: see https://github.com/user-attachments/assets/b8c6dfaa-c1b1-41a4-be55-013473e4a6ce for a visual explanation.
typedef CLAY_PACKED_ENUM {
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

// Controls where a floating element is offset relative to its parent element.
typedef struct Clay_FloatingAttachPoints {
    Clay_FloatingAttachPointType element; // Controls the origin point on a floating element that attaches to its parent.
    Clay_FloatingAttachPointType parent; // Controls the origin point on the parent element that the floating element attaches to.
} Clay_FloatingAttachPoints;

// Controls how mouse pointer events like hover and click are captured or passed through to elements underneath a floating element.
typedef CLAY_PACKED_ENUM {
    // (default) "Capture" the pointer event and don't allow events like hover and click to pass through to elements underneath.
    CLAY_POINTER_CAPTURE_MODE_CAPTURE,
    //    CLAY_POINTER_CAPTURE_MODE_PARENT, TODO pass pointer through to attached parent
    // Transparently pass through pointer events like hover and click to elements underneath the floating element.
    CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
} Clay_PointerCaptureMode;

// Controls which element a floating element is "attached" to (i.e. relative offset from).
typedef CLAY_PACKED_ENUM {
    // (default) Disables floating for this element.
    CLAY_ATTACH_TO_NONE,
    // Attaches this floating element to its parent, positioned based on the .attachPoints and .offset fields.
    CLAY_ATTACH_TO_PARENT,
    // Attaches this floating element to an element with a specific ID, specified with the .parentId field. positioned based on the .attachPoints and .offset fields.
    CLAY_ATTACH_TO_ELEMENT_WITH_ID,
    // Attaches this floating element to the root of the layout, which combined with the .offset field provides functionality similar to "absolute positioning".
    CLAY_ATTACH_TO_ROOT,
} Clay_FloatingAttachToElement;

// Controls whether or not a floating element is clipped to the same clipping rectangle as the element it's attached to.
typedef CLAY_PACKED_ENUM {
    // (default) - The floating element does not inherit clipping.
    CLAY_CLIP_TO_NONE,
    // The floating element is clipped to the same clipping rectangle as the element it's attached to.
    CLAY_CLIP_TO_ATTACHED_PARENT
} Clay_FloatingClipToElement;

// Controls various settings related to "floating" elements, which are elements that "float" above other elements, potentially overlapping their boundaries,
// and not affecting the layout of sibling or parent elements.
typedef struct Clay_FloatingElementConfig {
    // Offsets this floating element by the provided x,y coordinates from its attachPoints.
    Clay_Vector2 offset;
    // Expands the boundaries of the outer floating element without affecting its children.
    Clay_Dimensions expand;
    // When used in conjunction with .attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID, attaches this floating element to the element in the hierarchy with the provided ID.
    // Hint: attach the ID to the other element with .id = CLAY_ID("yourId"), and specify the id the same way, with .parentId = CLAY_ID("yourId").id
    uint32_t parentId;
    // Controls the z index of this floating element and all its children. Floating elements are sorted in ascending z order before output.
    // zIndex is also passed to the renderer for all elements contained within this floating element.
    int16_t zIndex;
    // Controls how mouse pointer events like hover and click are captured or passed through to elements underneath / behind a floating element.
    // Enum is of the form CLAY_ATTACH_POINT_foo_bar. See Clay_FloatingAttachPoints for more details.
    // Note: see <img src="https://github.com/user-attachments/assets/b8c6dfaa-c1b1-41a4-be55-013473e4a6ce />
    // and <img src="https://github.com/user-attachments/assets/ebe75e0d-1904-46b0-982d-418f929d1516 /> for a visual explanation.
    Clay_FloatingAttachPoints attachPoints;
    // Controls how mouse pointer events like hover and click are captured or passed through to elements underneath a floating element.
    // CLAY_POINTER_CAPTURE_MODE_CAPTURE (default) - "Capture" the pointer event and don't allow events like hover and click to pass through to elements underneath.
    // CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH - Transparently pass through pointer events like hover and click to elements underneath the floating element.
    Clay_PointerCaptureMode pointerCaptureMode;
    // Controls which element a floating element is "attached" to (i.e. relative offset from).
    // CLAY_ATTACH_TO_NONE (default) - Disables floating for this element.
    // CLAY_ATTACH_TO_PARENT - Attaches this floating element to its parent, positioned based on the .attachPoints and .offset fields.
    // CLAY_ATTACH_TO_ELEMENT_WITH_ID - Attaches this floating element to an element with a specific ID, specified with the .parentId field. positioned based on the .attachPoints and .offset fields.
    // CLAY_ATTACH_TO_ROOT - Attaches this floating element to the root of the layout, which combined with the .offset field provides functionality similar to "absolute positioning".
    Clay_FloatingAttachToElement attachTo;
    // Controls whether or not a floating element is clipped to the same clipping rectangle as the element it's attached to.
    // CLAY_CLIP_TO_NONE (default) - The floating element does not inherit clipping.
    // CLAY_CLIP_TO_ATTACHED_PARENT - The floating element is clipped to the same clipping rectangle as the element it's attached to.
    Clay_FloatingClipToElement clipTo;
} Clay_FloatingElementConfig;

CLAY__WRAPPER_STRUCT(Clay_FloatingElementConfig);

// Custom -----------------------------

// Controls various settings related to custom elements.
typedef struct Clay_CustomElementConfig {
    // A transparent pointer through which you can pass custom data to the renderer.
    // Generates CUSTOM render commands.
    void* customData;
} Clay_CustomElementConfig;

CLAY__WRAPPER_STRUCT(Clay_CustomElementConfig);

// Scroll -----------------------------

// Controls the axis on which an element switches to "scrolling", which clips the contents and allows scrolling in that direction.
typedef struct Clay_ClipElementConfig {
    bool horizontal; // Clip overflowing elements on the X axis.
    bool vertical; // Clip overflowing elements on the Y axis.
    Clay_Vector2 childOffset; // Offsets the x,y positions of all child elements. Used primarily for scrolling containers.
} Clay_ClipElementConfig;

CLAY__WRAPPER_STRUCT(Clay_ClipElementConfig);

// Border -----------------------------

// Controls the widths of individual element borders.
typedef struct Clay_BorderWidth {
    uint16_t left;
    uint16_t right;
    uint16_t top;
    uint16_t bottom;
    // Creates borders between each child element, depending on the .layoutDirection.
    // e.g. for LEFT_TO_RIGHT, borders will be vertical lines, and for TOP_TO_BOTTOM borders will be horizontal lines.
    // .betweenChildren borders will result in individual RECTANGLE render commands being generated.
    uint16_t betweenChildren;
} Clay_BorderWidth;

// Controls settings related to element borders.
typedef struct Clay_BorderElementConfig {
    Clay_Color color; // Controls the color of all borders with width > 0. Conventionally represented as 0-255, but interpretation is up to the renderer.
    Clay_BorderWidth width; // Controls the widths of individual borders. At least one of these should be > 0 for a BORDER render command to be generated.
} Clay_BorderElementConfig;

CLAY__WRAPPER_STRUCT(Clay_BorderElementConfig);

typedef struct {
    Clay_BoundingBox boundingBox;
    Clay_Color backgroundColor;
    Clay_Color overlayColor;
    Clay_Color borderColor;
    Clay_BorderWidth borderWidth;
} Clay_TransitionData;

typedef enum {
    CLAY_TRANSITION_STATE_IDLE,
    CLAY_TRANSITION_STATE_ENTERING,
    CLAY_TRANSITION_STATE_TRANSITIONING,
    CLAY_TRANSITION_STATE_EXITING,
} Clay_TransitionState;

typedef enum {
    CLAY_TRANSITION_PROPERTY_NONE = 0,
    CLAY_TRANSITION_PROPERTY_X = 1,
    CLAY_TRANSITION_PROPERTY_Y = 2,
    CLAY_TRANSITION_PROPERTY_POSITION = CLAY_TRANSITION_PROPERTY_X | CLAY_TRANSITION_PROPERTY_Y,
    CLAY_TRANSITION_PROPERTY_WIDTH = 4,
    CLAY_TRANSITION_PROPERTY_HEIGHT = 8,
    CLAY_TRANSITION_PROPERTY_DIMENSIONS = CLAY_TRANSITION_PROPERTY_WIDTH | CLAY_TRANSITION_PROPERTY_HEIGHT,
    CLAY_TRANSITION_PROPERTY_BOUNDING_BOX = CLAY_TRANSITION_PROPERTY_POSITION | CLAY_TRANSITION_PROPERTY_DIMENSIONS,
    CLAY_TRANSITION_PROPERTY_BACKGROUND_COLOR = 16,
    CLAY_TRANSITION_PROPERTY_OVERLAY_COLOR = 32,
    CLAY_TRANSITION_PROPERTY_CORNER_RADIUS = 64,
    CLAY_TRANSITION_PROPERTY_BORDER_COLOR = 128,
    CLAY_TRANSITION_PROPERTY_BORDER_WIDTH = 256,
    CLAY_TRANSITION_PROPERTY_BORDER = CLAY_TRANSITION_PROPERTY_BORDER_COLOR | CLAY_TRANSITION_PROPERTY_BORDER_WIDTH
} Clay_TransitionProperty;

typedef struct {
    Clay_TransitionState transitionState;
    Clay_TransitionData initial;
    Clay_TransitionData *current;
    Clay_TransitionData target;
    float elapsedTime;
    float duration;
    Clay_TransitionProperty properties;
} Clay_TransitionCallbackArguments;

typedef CLAY_PACKED_ENUM {
    CLAY_TRANSITION_ENTER_SKIP_ON_FIRST_PARENT_FRAME,
    CLAY_TRANSITION_ENTER_TRIGGER_ON_FIRST_PARENT_FRAME,
} Clay_TransitionEnterTriggerType;

typedef CLAY_PACKED_ENUM {
    CLAY_TRANSITION_EXIT_SKIP_WHEN_PARENT_EXITS,
    CLAY_TRANSITION_EXIT_TRIGGER_WHEN_PARENT_EXITS,
} Clay_TransitionExitTriggerType;

typedef CLAY_PACKED_ENUM {
    CLAY_TRANSITION_DISABLE_INTERACTIONS_WHILE_TRANSITIONING_POSITION,
    CLAY_TRANSITION_ALLOW_INTERACTIONS_WHILE_TRANSITIONING_POSITION,
} Clay_TransitionInteractionHandlingType;

typedef CLAY_PACKED_ENUM {
    CLAY_EXIT_TRANSITION_ORDERING_UNDERNEATH_SIBLINGS,
    CLAY_EXIT_TRANSITION_ORDERING_NATURAL_ORDER,
    CLAY_EXIT_TRANSITION_ORDERING_ABOVE_SIBLINGS,
} Clay_ExitTransitionSiblingOrdering;

// Controls settings related to transitions
typedef struct Clay_TransitionElementConfig {
    bool (*handler)(Clay_TransitionCallbackArguments arguments);
    float duration;
    Clay_TransitionProperty properties;
    Clay_TransitionInteractionHandlingType interactionHandling;
    struct {
        Clay_TransitionData (*setInitialState)(Clay_TransitionData targetState, Clay_TransitionProperty properties);
        Clay_TransitionEnterTriggerType trigger;
    } enter;
    struct {
        Clay_TransitionData (*setFinalState)(Clay_TransitionData initialState, Clay_TransitionProperty properties);
        Clay_TransitionExitTriggerType trigger;
        Clay_ExitTransitionSiblingOrdering siblingOrdering;
    } exit;
} Clay_TransitionElementConfig;

CLAY__WRAPPER_STRUCT(Clay_TransitionElementConfig);

// Render Command Data -----------------------------

// Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_TEXT
typedef struct Clay_TextRenderData {
    // A string slice containing the text to be rendered.
    // Note: this is not guaranteed to be null terminated.
    Clay_StringSlice stringContents;
    // Conventionally represented as 0-255 for each channel, but interpretation is up to the renderer.
    Clay_Color textColor;
    // An integer representing the font to use to render this text, transparently passed through from the text declaration.
    uint16_t fontId;
    uint16_t fontSize;
    // Specifies the extra whitespace gap in pixels between each character.
    uint16_t letterSpacing;
    // The height of the bounding box for this line of text.
    uint16_t lineHeight;
} Clay_TextRenderData;

// Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_RECTANGLE
typedef struct Clay_RectangleRenderData {
    // The solid background color to fill this rectangle with. Conventionally represented as 0-255 for each channel, but interpretation is up to the renderer.
    Clay_Color backgroundColor;
    // Controls the "radius", or corner rounding of elements, including rectangles, borders and images.
    // The rounding is determined by drawing a circle inset into the element corner by (radius, radius) pixels.
    Clay_CornerRadius cornerRadius;
} Clay_RectangleRenderData;

// Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_IMAGE
typedef struct Clay_ImageRenderData {
    // The tint color for this image. Note that the default value is 0,0,0,0 and should likely be interpreted
    // as "untinted".
    // Conventionally represented as 0-255 for each channel, but interpretation is up to the renderer.
    Clay_Color backgroundColor;
    // Controls the "radius", or corner rounding of this image.
    // The rounding is determined by drawing a circle inset into the element corner by (radius, radius) pixels.
    Clay_CornerRadius cornerRadius;
    // A pointer transparently passed through from the original element definition, typically used to represent image data.
    void* imageData;
} Clay_ImageRenderData;

// Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_CUSTOM
typedef struct Clay_CustomRenderData {
    // Passed through from .backgroundColor in the original element declaration.
    // Conventionally represented as 0-255 for each channel, but interpretation is up to the renderer.
    Clay_Color backgroundColor;
    // Controls the "radius", or corner rounding of this custom element.
    // The rounding is determined by drawing a circle inset into the element corner by (radius, radius) pixels.
    Clay_CornerRadius cornerRadius;
    // A pointer transparently passed through from the original element definition.
    void* customData;
} Clay_CustomRenderData;

// Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_SCISSOR_START || commandType == CLAY_RENDER_COMMAND_TYPE_SCISSOR_END
typedef struct Clay_ClipRenderData {
    bool horizontal;
    bool vertical;
} Clay_ClipRenderData;

// Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_OVERLAY_COLOR_START || commandType == CLAY_RENDER_COMMAND_TYPE_OVERLAY_COLOR_END
typedef struct Clay_OverlayColorRenderData {
    Clay_Color color;
} Clay_OverlayColorRenderData;

// Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_BORDER
typedef struct Clay_BorderRenderData {
    // Controls a shared color for all this element's borders.
    // Conventionally represented as 0-255 for each channel, but interpretation is up to the renderer.
    Clay_Color color;
    // Specifies the "radius", or corner rounding of this border element.
    // The rounding is determined by drawing a circle inset into the element corner by (radius, radius) pixels.
    Clay_CornerRadius cornerRadius;
    // Controls individual border side widths.
    Clay_BorderWidth width;
} Clay_BorderRenderData;

// A struct union containing data specific to this command's .commandType
typedef union Clay_RenderData {
    // Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_RECTANGLE
    Clay_RectangleRenderData rectangle;
    // Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_TEXT
    Clay_TextRenderData text;
    // Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_IMAGE
    Clay_ImageRenderData image;
    // Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_CUSTOM
    Clay_CustomRenderData custom;
    // Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_BORDER
    Clay_BorderRenderData border;
    // Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_SCISSOR_START|END
    Clay_ClipRenderData clip;
    // Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_OVERLAY_COLOR_START|END
    Clay_OverlayColorRenderData overlayColor;
} Clay_RenderData;

// Miscellaneous Structs & Enums ---------------------------------

// Data representing the current internal state of a scrolling element.
typedef struct Clay_ScrollContainerData {
    // Note: This is a pointer to the real internal scroll position, mutating it may cause a change in final layout.
    // Intended for use with external functionality that modifies scroll position, such as scroll bars or auto scrolling.
    Clay_Vector2 *scrollPosition;
    // The bounding box of the scroll element.
    Clay_Dimensions scrollContainerDimensions;
    // The outer dimensions of the inner scroll container content, including the padding of the parent scroll container.
    Clay_Dimensions contentDimensions;
    // The config that was originally passed to the clip element.
    Clay_ClipElementConfig config;
    // Indicates whether an actual scroll container matched the provided ID or if the default struct was returned.
    bool found;
} Clay_ScrollContainerData;

// Bounding box and other data for a specific UI element.
typedef struct Clay_ElementData {
    // The rectangle that encloses this UI element, with the position relative to the root of the layout.
    Clay_BoundingBox boundingBox;
    // Indicates whether an actual Element matched the provided ID or if the default struct was returned.
    bool found;
} Clay_ElementData;

// Used by renderers to determine specific handling for each render command.
typedef CLAY_PACKED_ENUM {
    // This command type should be skipped.
    CLAY_RENDER_COMMAND_TYPE_NONE,
    // The renderer should draw a solid color rectangle.
    CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
    // The renderer should draw a colored border inset into the bounding box.
    CLAY_RENDER_COMMAND_TYPE_BORDER,
    // The renderer should draw text.
    CLAY_RENDER_COMMAND_TYPE_TEXT,
    // The renderer should draw an image.
    CLAY_RENDER_COMMAND_TYPE_IMAGE,
    // The renderer should begin clipping all future draw commands, only rendering content that falls within the provided boundingBox.
    CLAY_RENDER_COMMAND_TYPE_SCISSOR_START,
    // The renderer should finish any previously active clipping, and begin rendering elements in full again.
    CLAY_RENDER_COMMAND_TYPE_SCISSOR_END,
    // The renderer should begin performing a "color overlay" on all subsequent render commands until disabled again.
    CLAY_RENDER_COMMAND_TYPE_OVERLAY_COLOR_START,
    // The renderer should disable any previously active "color overlay" and render elements with their standard colors again.
    CLAY_RENDER_COMMAND_TYPE_OVERLAY_COLOR_END,
    // The renderer should provide a custom implementation for handling this render command based on its .customData
    CLAY_RENDER_COMMAND_TYPE_CUSTOM,
} Clay_RenderCommandType;

typedef struct Clay_RenderCommand {
    // A rectangular box that fully encloses this UI element, with the position relative to the root of the layout.
    Clay_BoundingBox boundingBox;
    // A struct union containing data specific to this command's commandType.
    Clay_RenderData renderData;
    // A pointer transparently passed through from the original element declaration.
    void *userData;
    // The id of this element, transparently passed through from the original element declaration.
    uint32_t id;
    // The z order required for drawing this command correctly.
    // Note: the render command array is already sorted in ascending order, and will produce correct results if drawn in naive order.
    // This field is intended for use in batching renderers for improved performance.
    int16_t zIndex;
    // Specifies how to handle rendering of this command.
    // CLAY_RENDER_COMMAND_TYPE_RECTANGLE - The renderer should draw a solid color rectangle.
    // CLAY_RENDER_COMMAND_TYPE_BORDER - The renderer should draw a colored border inset into the bounding box.
    // CLAY_RENDER_COMMAND_TYPE_TEXT - The renderer should draw text.
    // CLAY_RENDER_COMMAND_TYPE_IMAGE - The renderer should draw an image.
    // CLAY_RENDER_COMMAND_TYPE_SCISSOR_START - The renderer should begin clipping all future draw commands, only rendering content that falls within the provided boundingBox.
    // CLAY_RENDER_COMMAND_TYPE_SCISSOR_END - The renderer should finish any previously active clipping, and begin rendering elements in full again.
    // CLAY_RENDER_COMMAND_TYPE_CUSTOM - The renderer should provide a custom implementation for handling this render command based on its .customData
    Clay_RenderCommandType commandType;
} Clay_RenderCommand;

// A sized array of render commands.
typedef struct Clay_RenderCommandArray {
    // The underlying max capacity of the array, not necessarily all initialized.
    int32_t capacity;
    // The number of initialized elements in this array. Used for loops and iteration.
    int32_t length;
    // A pointer to the first element in the internal array.
    Clay_RenderCommand* internalArray;
} Clay_RenderCommandArray;

// Represents the current state of interaction with clay this frame.
typedef CLAY_PACKED_ENUM {
    // A left mouse click, or touch occurred this frame.
    CLAY_POINTER_DATA_PRESSED_THIS_FRAME,
    // The left mouse button click or touch happened at some point in the past, and is still currently held down this frame.
    CLAY_POINTER_DATA_PRESSED,
    // The left mouse button click or touch was released this frame.
    CLAY_POINTER_DATA_RELEASED_THIS_FRAME,
    // The left mouse button click or touch is not currently down / was released at some point in the past.
    CLAY_POINTER_DATA_RELEASED,
} Clay_PointerDataInteractionState;

// Information on the current state of pointer interactions this frame.
typedef struct Clay_PointerData {
    // The position of the mouse / touch / pointer relative to the root of the layout.
    Clay_Vector2 position;
    // Represents the current state of interaction with clay this frame.
    // CLAY_POINTER_DATA_PRESSED_THIS_FRAME - A left mouse click, or touch occurred this frame.
    // CLAY_POINTER_DATA_PRESSED - The left mouse button click or touch happened at some point in the past, and is still currently held down this frame.
    // CLAY_POINTER_DATA_RELEASED_THIS_FRAME - The left mouse button click or touch was released this frame.
    // CLAY_POINTER_DATA_RELEASED - The left mouse button click or touch is not currently down / was released at some point in the past.
    Clay_PointerDataInteractionState state;
} Clay_PointerData;

typedef struct Clay_ElementDeclaration {
    // Controls various settings that affect the size and position of an element, as well as the sizes and positions of any child elements.
    Clay_LayoutConfig layout;
    // Controls the background color of the resulting element.
    // By convention specified as 0-255, but interpretation is up to the renderer.
    // If no other config is specified, .backgroundColor will generate a RECTANGLE render command, otherwise it will be passed as a property to IMAGE or CUSTOM render commands.
    Clay_Color backgroundColor;
    // Perform an image editing style "Color Overlay" on this element and all its children, equivalent to
    // glsl mix(elementColor, overlayColor.rgb, overlayColor.a)
    Clay_Color overlayColor;
    // Controls the "radius", or corner rounding of elements, including rectangles, borders and images.
    Clay_CornerRadius cornerRadius;
    // Controls settings related to aspect ratio scaling.
    Clay_AspectRatioElementConfig aspectRatio;
    // Controls settings related to image elements.
    Clay_ImageElementConfig image;
    // Controls whether and how an element "floats", which means it layers over the top of other elements in z order, and doesn't affect the position and size of siblings or parent elements.
    // Note: in order to activate floating, .floating.attachTo must be set to something other than the default value.
    Clay_FloatingElementConfig floating;
    // Used to create CUSTOM render commands, usually to render element types not supported by Clay.
    Clay_CustomElementConfig custom;
    // Controls whether an element should clip its contents, as well as providing child x,y offset configuration for scrolling.
    Clay_ClipElementConfig clip;
    // Controls settings related to element borders, and will generate BORDER render commands.
    Clay_BorderElementConfig border;
    Clay_TransitionElementConfig transition;
    // A pointer that will be transparently passed through to resulting render commands.
    void *userData;
} Clay_ElementDeclaration;

CLAY__WRAPPER_STRUCT(Clay_ElementDeclaration);

// Represents the type of error clay encountered while computing layout.
typedef CLAY_PACKED_ENUM {
    // A text measurement function wasn't provided using Clay_SetMeasureTextFunction(), or the provided function was null.
    CLAY_ERROR_TYPE_TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED,
    // Clay attempted to allocate its internal data structures but ran out of space.
    // The arena passed to Clay_Initialize was created with a capacity smaller than that required by Clay_MinMemorySize().
    CLAY_ERROR_TYPE_ARENA_CAPACITY_EXCEEDED,
    // Clay ran out of capacity in its internal array for storing elements. This limit can be increased with Clay_SetMaxElementCount().
    CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED,
    // Clay ran out of capacity in its internal array for storing elements. This limit can be increased with Clay_SetMaxMeasureTextCacheWordCount().
    CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED,
    // Two elements were declared with exactly the same ID within one layout.
    CLAY_ERROR_TYPE_DUPLICATE_ID,
    // A floating element was declared using CLAY_ATTACH_TO_ELEMENT_ID and either an invalid .parentId was provided or no element with the provided .parentId was found.
    CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND,
    // An element was declared that using CLAY_SIZING_PERCENT but the percentage value was over 1. Percentage values are expected to be in the 0-1 range.
    CLAY_ERROR_TYPE_PERCENTAGE_OVER_1,
    // Clay encountered an internal error. It would be wonderful if you could report this so we can fix it!
    CLAY_ERROR_TYPE_INTERNAL_ERROR,
    // Clay__OpenElement was called more times than Clay__CloseElement, so there were still remaining open elements when the layout ended.
    CLAY_ERROR_TYPE_UNBALANCED_OPEN_CLOSE,
    CLAY_ERROR_TYPE_HASH_MAP_CAPACITY_EXCEEDED
} Clay_ErrorType;

// Data to identify the error that clay has encountered.
typedef struct Clay_ErrorData {
    // Represents the type of error clay encountered while computing layout.
    // CLAY_ERROR_TYPE_TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED - A text measurement function wasn't provided using Clay_SetMeasureTextFunction(), or the provided function was null.
    // CLAY_ERROR_TYPE_ARENA_CAPACITY_EXCEEDED - Clay attempted to allocate its internal data structures but ran out of space. The arena passed to Clay_Initialize was created with a capacity smaller than that required by Clay_MinMemorySize().
    // CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED - Clay ran out of capacity in its internal array for storing elements. This limit can be increased with Clay_SetMaxElementCount().
    // CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED - Clay ran out of capacity in its internal array for storing elements. This limit can be increased with Clay_SetMaxMeasureTextCacheWordCount().
    // CLAY_ERROR_TYPE_DUPLICATE_ID - Two elements were declared with exactly the same ID within one layout.
    // CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND - A floating element was declared using CLAY_ATTACH_TO_ELEMENT_ID and either an invalid .parentId was provided or no element with the provided .parentId was found.
    // CLAY_ERROR_TYPE_PERCENTAGE_OVER_1 - An element was declared that using CLAY_SIZING_PERCENT but the percentage value was over 1. Percentage values are expected to be in the 0-1 range.
    // CLAY_ERROR_TYPE_INTERNAL_ERROR - Clay encountered an internal error. It would be wonderful if you could report this so we can fix it!
    // CLAY_ERROR_TYPE_UNBALANCED_OPEN_CLOSE - Clay__OpenElement was called more times than Clay__CloseElement, so there were still remaining open elements when the layout ended.
    // CLAY_ERROR_TYPE_HASH_MAP_CAPACITY_EXCEEDED - Clay ran out of capacity in its internal hash map for storing element IDs -> elements. This limit can be increased with Clay_SetMaxElementCount().
    Clay_ErrorType errorType;
    // A string containing human-readable error text that explains the error in more detail.
    Clay_String errorText;
    // A transparent pointer passed through from when the error handler was first provided.
    void *userData;
} Clay_ErrorData;

// A wrapper struct around Clay's error handler function.
typedef struct {
    // A user provided function to call when Clay encounters an error during layout.
    void (*errorHandlerFunction)(Clay_ErrorData errorText);
    // A pointer that will be transparently passed through to the error handler when it is called.
    void *userData;
} Clay_ErrorHandler;

// Function Forward Declarations ---------------------------------

// Public API functions ------------------------------------------

// Returns the size, in bytes, of the minimum amount of memory Clay requires to operate at its current settings.
CLAY_DLL_EXPORT uint32_t Clay_MinMemorySize(void);
// Creates an arena for clay to use for its internal allocations, given a certain capacity in bytes and a pointer to an allocation of at least that size.
// Intended to be used with Clay_MinMemorySize in the following way:
// uint32_t minMemoryRequired = Clay_MinMemorySize();
// Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(minMemoryRequired, malloc(minMemoryRequired));
CLAY_DLL_EXPORT Clay_Arena Clay_CreateArenaWithCapacityAndMemory(size_t capacity, void *memory);
// Sets the state of the "pointer" (i.e. the mouse or touch) in Clay's internal data. Used for detecting and responding to mouse events in the debug view,
// as well as for Clay_Hovered() and scroll element handling.
CLAY_DLL_EXPORT void Clay_SetPointerState(Clay_Vector2 position, bool pointerDown);
// Returns the state of the "pointer" (i.e. the mouse or touch) which was set via Clay_SetPointerState().
CLAY_DLL_EXPORT Clay_PointerData Clay_GetPointerState(void);
// Initialize Clay's internal arena and setup required data before layout can begin. Only needs to be called once.
// - arena can be created using Clay_CreateArenaWithCapacityAndMemory()
// - layoutDimensions are the initial bounding dimensions of the layout (i.e. the screen width and height for a full screen layout)
// - errorHandler is used by Clay to inform you if something has gone wrong in configuration or layout.
CLAY_DLL_EXPORT Clay_Context* Clay_Initialize(Clay_Arena arena, Clay_Dimensions layoutDimensions, Clay_ErrorHandler errorHandler);
// Returns the Context that clay is currently using. Used when using multiple instances of clay simultaneously.
CLAY_DLL_EXPORT Clay_Context* Clay_GetCurrentContext(void);
// Sets the context that clay will use to compute the layout.
// Used to restore a context saved from Clay_GetCurrentContext when using multiple instances of clay simultaneously.
CLAY_DLL_EXPORT void Clay_SetCurrentContext(Clay_Context* context);
// Updates the state of Clay's internal scroll data, updating scroll content positions if scrollDelta is non zero, and progressing momentum scrolling.
// - enableDragScrolling when set to true will enable mobile device like "touch drag" scroll of scroll containers, including momentum scrolling after the touch has ended.
// - scrollDelta is the amount to scroll this frame on each axis in pixels.
// - deltaTime is the time in seconds since the last "frame" (scroll update)
CLAY_DLL_EXPORT void Clay_UpdateScrollContainers(bool enableDragScrolling, Clay_Vector2 scrollDelta, float deltaTime);
// Returns the internally stored scroll offset for the currently open element.
// Generally intended for use with clip elements to create scrolling containers.
CLAY_DLL_EXPORT Clay_Vector2 Clay_GetScrollOffset(void);
// Updates the layout dimensions in response to the window or outer container being resized.
CLAY_DLL_EXPORT void Clay_SetLayoutDimensions(Clay_Dimensions dimensions);
// Returns the current dimensions set by Clay_SetLayoutDimensions.
CLAY_DLL_EXPORT Clay_Dimensions Clay_GetLayoutDimensions(void);
// Called before starting any layout declarations.
CLAY_DLL_EXPORT void Clay_BeginLayout(void);
// Called when all layout declarations are finished.
// Computes the layout and generates and returns the array of render commands to draw.
CLAY_DLL_EXPORT Clay_RenderCommandArray Clay_EndLayout(float deltaTime);
// Gets the ID of the currently open element, useful for retrieving IDs generated by CLAY_AUTO_ID()
CLAY_DLL_EXPORT uint32_t Clay_GetOpenElementId(void);
// Calculates a hash ID from the given idString.
// Generally only used for dynamic strings when CLAY_ID("stringLiteral") can't be used.
CLAY_DLL_EXPORT Clay_ElementId Clay_GetElementId(Clay_String idString);
// Calculates a hash ID from the given idString and index.
// - index is used to avoid constructing dynamic ID strings in loops.
// Generally only used for dynamic strings when CLAY_IDI("stringLiteral", index) can't be used.
CLAY_DLL_EXPORT Clay_ElementId Clay_GetElementIdWithIndex(Clay_String idString, uint32_t index);
// Returns layout data such as the final calculated bounding box for an element with a given ID.
// The returned Clay_ElementData contains a `found` bool that will be true if an element with the provided ID was found.
// This ID can be calculated either with CLAY_ID() for string literal IDs, or Clay_GetElementId for dynamic strings.
CLAY_DLL_EXPORT Clay_ElementData Clay_GetElementData(Clay_ElementId id);
// Returns true if the pointer position provided by Clay_SetPointerState is within the current element's bounding box.
// Works during element declaration, e.g. CLAY({ .backgroundColor = Clay_Hovered() ? BLUE : RED });
CLAY_DLL_EXPORT bool Clay_Hovered(void);
// Bind a callback that will be called when the pointer position provided by Clay_SetPointerState is within the current element's bounding box.
// - onHoverFunction is a function pointer to a user defined function.
// - userData is a pointer that will be transparently passed through when the onHoverFunction is called.
CLAY_DLL_EXPORT void Clay_OnHover(void (*onHoverFunction)(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData), void *userData);
// An imperative function that returns true if the pointer position provided by Clay_SetPointerState is within the element with the provided ID's bounding box.
// This ID can be calculated either with CLAY_ID() for string literal IDs, or Clay_GetElementId for dynamic strings.
CLAY_DLL_EXPORT bool Clay_PointerOver(Clay_ElementId elementId);
// Returns the array of element IDs that the pointer is currently over.
CLAY_DLL_EXPORT Clay_ElementIdArray Clay_GetPointerOverIds(void);
// Returns data representing the state of the scrolling element with the provided ID.
// The returned Clay_ScrollContainerData contains a `found` bool that will be true if a scroll element was found with the provided ID.
// An imperative function that returns true if the pointer position provided by Clay_SetPointerState is within the element with the provided ID's bounding box.
// This ID can be calculated either with CLAY_ID() for string literal IDs, or Clay_GetElementId for dynamic strings.
CLAY_DLL_EXPORT Clay_ScrollContainerData Clay_GetScrollContainerData(Clay_ElementId id);
// Binds a callback function that Clay will call to determine the dimensions of a given string slice.
// - measureTextFunction is a user provided function that adheres to the interface Clay_Dimensions (Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);
// - userData is a pointer that will be transparently passed through when the measureTextFunction is called.
CLAY_DLL_EXPORT void Clay_SetMeasureTextFunction(Clay_Dimensions (*measureTextFunction)(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData), void *userData);
// Experimental - Used in cases where Clay needs to integrate with a system that manages its own scrolling containers externally.
// Please reach out if you plan to use this function, as it may be subject to change.
CLAY_DLL_EXPORT void Clay_SetQueryScrollOffsetFunction(Clay_Vector2 (*queryScrollOffsetFunction)(uint32_t elementId, void *userData), void *userData);
// A bounds-checked "get" function for the Clay_RenderCommandArray returned from Clay_EndLayout().
CLAY_DLL_EXPORT Clay_RenderCommand * Clay_RenderCommandArray_Get(Clay_RenderCommandArray* array, int32_t index);
// Enables and disables Clay's internal debug tools.
// This state is retained and does not need to be set each frame.
CLAY_DLL_EXPORT void Clay_SetDebugModeEnabled(bool enabled);
// Returns true if Clay's internal debug tools are currently enabled.
CLAY_DLL_EXPORT bool Clay_IsDebugModeEnabled(void);
// Enables and disables visibility culling. By default, Clay will not generate render commands for elements whose bounding box is entirely outside the screen.
CLAY_DLL_EXPORT void Clay_SetCullingEnabled(bool enabled);
// Returns the maximum number of UI elements supported by Clay's current configuration.
CLAY_DLL_EXPORT int32_t Clay_GetMaxElementCount(void);
// Modifies the maximum number of UI elements supported by Clay's current configuration.
// This may require reallocating additional memory, and re-calling Clay_Initialize();
CLAY_DLL_EXPORT void Clay_SetMaxElementCount(int32_t maxElementCount);
// Returns the maximum number of measured "words" (whitespace seperated runs of characters) that Clay can store in its internal text measurement cache.
CLAY_DLL_EXPORT int32_t Clay_GetMaxMeasureTextCacheWordCount(void);
// Modifies the maximum number of measured "words" (whitespace seperated runs of characters) that Clay can store in its internal text measurement cache.
// This may require reallocating additional memory, and re-calling Clay_Initialize();
CLAY_DLL_EXPORT void Clay_SetMaxMeasureTextCacheWordCount(int32_t maxMeasureTextCacheWordCount);
// Resets Clay's internal text measurement cache. Useful if font mappings have changed or fonts have been reloaded.
CLAY_DLL_EXPORT void Clay_ResetMeasureTextCache(void);
// A built in transition function that uses the "Ease Out" curve
CLAY_DLL_EXPORT bool Clay_EaseOut(Clay_TransitionCallbackArguments arguments);

// Internal API functions required by macros ----------------------

CLAY_DLL_EXPORT void Clay__OpenElement(void);
CLAY_DLL_EXPORT void Clay__OpenElementWithId(Clay_ElementId elementId);
CLAY_DLL_EXPORT void Clay__ConfigureOpenElement(const Clay_ElementDeclaration config);
CLAY_DLL_EXPORT void Clay__ConfigureOpenElementPtr(const Clay_ElementDeclaration *config);
CLAY_DLL_EXPORT void Clay__CloseElement(void);
CLAY_DLL_EXPORT Clay_ElementId Clay__HashString(Clay_String key, uint32_t seed);
CLAY_DLL_EXPORT Clay_ElementId Clay__HashStringWithOffset(Clay_String key, uint32_t offset, uint32_t seed);
CLAY_DLL_EXPORT void Clay__OpenTextElement(Clay_String text, Clay_TextElementConfig textConfig);

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

Clay_LayoutConfig CLAY_LAYOUT_DEFAULT = CLAY__DEFAULT_STRUCT;

Clay_Color Clay__Color_DEFAULT = CLAY__DEFAULT_STRUCT;
Clay_CornerRadius Clay__CornerRadius_DEFAULT = CLAY__DEFAULT_STRUCT;
Clay_BorderWidth Clay__BorderWidth_DEFAULT = CLAY__DEFAULT_STRUCT;

// The below functions define array bounds checking and convenience functions for a provided type.
#define CLAY__ARRAY_DEFINE_FUNCTIONS(typeName, arrayName)                                                       \
                                                                                                                \
typedef struct                                                                                                  \
{                                                                                                               \
    int32_t length;                                                                                             \
    typeName *internalArray;                                                                                    \
} arrayName##Slice;                                                                                             \
                                                                                                                \
typeName typeName##_DEFAULT = CLAY__DEFAULT_STRUCT;                                                             \
                                                                                                                \
arrayName arrayName##_Allocate_Arena(int32_t capacity, Clay_Arena *arena) {                                     \
    return CLAY__INIT(arrayName){.capacity = capacity, .length = 0,                                             \
        .internalArray = (typeName *)Clay__Array_Allocate_Arena(capacity, sizeof(typeName), arena)};            \
}                                                                                                               \
                                                                                                                \
typeName *arrayName##_Get(arrayName *array, int32_t index) {                                                    \
    return Clay__Array_RangeCheck(index, array->length) ? &array->internalArray[index] : &typeName##_DEFAULT;   \
}                                                                                                               \
                                                                                                                \
typeName arrayName##_GetValue(arrayName *array, int32_t index) {                                                \
    return Clay__Array_RangeCheck(index, array->length) ? array->internalArray[index] : typeName##_DEFAULT;     \
}                                                                                                               \
                                                                                                                \
typeName *arrayName##_GetCheckCapacity(arrayName *array, int32_t index) {                                                    \
    return Clay__Array_RangeCheck(index, array->capacity) ? &array->internalArray[index] : &typeName##_DEFAULT;   \
}                                                                                                               \
                                                                                                                \
typeName *arrayName##_Add(arrayName *array, typeName item) {                                                    \
    if (Clay__Array_AddCapacityCheck(array->length, array->capacity)) {                                         \
        array->internalArray[array->length++] = item;                                                           \
        return &array->internalArray[array->length - 1];                                                        \
    }                                                                                                           \
    return &typeName##_DEFAULT;                                                                                 \
}                                                                                                               \
                                                                                                                \
typeName *arrayName##Slice_Get(arrayName##Slice *slice, int32_t index) {                                        \
    return Clay__Array_RangeCheck(index, slice->length) ? &slice->internalArray[index] : &typeName##_DEFAULT;   \
}                                                                                                               \
                                                                                                                \
typeName arrayName##_RemoveSwapback(arrayName *array, int32_t index) {                                          \
	if (Clay__Array_RangeCheck(index, array->length)) {                                                         \
		array->length--;                                                                                        \
		typeName removed = array->internalArray[index];                                                         \
		array->internalArray[index] = array->internalArray[array->length];                                      \
		return removed;                                                                                         \
	}                                                                                                           \
	return typeName##_DEFAULT;                                                                                  \
}                                                                                                               \
                                                                                                                \
typeName* arrayName##_Set(arrayName *array, int32_t index, typeName value) {                                    \
	if (Clay__Array_RangeCheck(index, array->capacity)) {                                                       \
		array->internalArray[index] = value;                                                                    \
		array->length = index < array->length ? array->length : index + 1;                                      \
        return &array->internalArray[index];\
	}                                                                                                           \
    return NULL;\
}                                                                                                               \
                                                                                                                \
typeName* arrayName##_Set_DontTouchLength(arrayName *array, int32_t index, typeName value) {                                    \
	if (Clay__Array_RangeCheck(index, array->capacity)) {                                                       \
		array->internalArray[index] = value;                                                                    \
        return &array->internalArray[index];\
	}                                                                                                           \
    return NULL;\
}  \

#define CLAY__ARRAY_DEFINE(typeName, arrayName)     \
typedef struct                                      \
{                                                   \
    int32_t capacity;                               \
    int32_t length;                                 \
    typeName *internalArray;                        \
} arrayName;                                        \
                                                    \
CLAY__ARRAY_DEFINE_FUNCTIONS(typeName, arrayName)   \

Clay_Context *Clay__currentContext;
int32_t Clay__defaultMaxElementCount = 8192;
int32_t Clay__defaultMaxMeasureTextWordCacheCount = 16384;

void Clay__ErrorHandlerFunctionDefault(Clay_ErrorData errorText) {
    (void) errorText;
}

Clay_String CLAY__SPACECHAR = { .length = 1, .chars = " " };
Clay_String CLAY__STRING_DEFAULT = { .length = 0, .chars = NULL };

typedef struct {
    bool maxElementsExceeded;
    bool maxRenderCommandsExceeded;
    bool maxTextMeasureCacheExceeded;
    bool textMeasurementFunctionNotSet;
    bool hashMapCapacityExceeded;
} Clay_BooleanWarnings;

typedef struct {
    Clay_String baseMessage;
    Clay_String dynamicMessage;
} Clay__Warning;

Clay__Warning CLAY__WARNING_DEFAULT = CLAY__DEFAULT_STRUCT;

typedef struct {
    int32_t capacity;
    int32_t length;
    Clay__Warning *internalArray;
} Clay__WarningArray;

Clay__WarningArray Clay__WarningArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena);
Clay__Warning *Clay__WarningArray_Add(Clay__WarningArray *array, Clay__Warning item);
void* Clay__Array_Allocate_Arena(int32_t capacity, uint32_t itemSize, Clay_Arena *arena);
bool Clay__Array_RangeCheck(int32_t index, int32_t length);
bool Clay__Array_AddCapacityCheck(int32_t length, int32_t capacity);

CLAY__ARRAY_DEFINE(bool, Clay__boolArray)
CLAY__ARRAY_DEFINE(int32_t, Clay__int32_tArray)
CLAY__ARRAY_DEFINE(char, Clay__charArray)
CLAY__ARRAY_DEFINE_FUNCTIONS(Clay_ElementId, Clay_ElementIdArray)
CLAY__ARRAY_DEFINE(Clay_String, Clay__StringArray)
CLAY__ARRAY_DEFINE_FUNCTIONS(Clay_RenderCommand, Clay_RenderCommandArray)

typedef struct {
    Clay_Dimensions dimensions;
    Clay_String line;
} Clay__WrappedTextLine;

CLAY__ARRAY_DEFINE(Clay__WrappedTextLine, Clay__WrappedTextLineArray)

typedef struct {
    Clay_String text;
    Clay_Dimensions preferredDimensions;
    Clay__WrappedTextLineArraySlice wrappedLines;
} Clay__TextElementData;

typedef struct {
    int32_t *elements;
    uint16_t length;
} Clay__LayoutElementChildren;

typedef struct Clay_LayoutElement {
    Clay__LayoutElementChildren children;
    Clay_Dimensions dimensions;
    Clay_Dimensions minDimensions;
    union {
        Clay_ElementDeclaration config;
        struct {
            Clay_TextElementConfig textConfig;
            Clay__TextElementData textElementData;
        };
    };
    uint32_t id;
    uint16_t floatingChildrenCount;
    bool isTextElement;
    // True if the element is currently in an exit transition, and is "synthetic"
    // i.e. data was retained from previous frames
    bool exiting;
} Clay_LayoutElement;

CLAY__ARRAY_DEFINE(Clay_LayoutElement, Clay_LayoutElementArray)

typedef struct {
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

CLAY__ARRAY_DEFINE(Clay__ScrollContainerDataInternal, Clay__ScrollContainerDataInternalArray)

// Data representing the current internal state of a transition element.
typedef struct Clay__TransitionDataInternal {
    Clay_TransitionData initialState;
    Clay_TransitionData currentState;
    Clay_TransitionData targetState;
    Clay_LayoutElement* elementThisFrame;
    Clay_Vector2 oldParentRelativePosition;
    uint32_t elementId;
    uint32_t parentId;
    uint32_t siblingIndex;
    float elapsedTime;
    Clay_TransitionState state;
    bool transitionOut;
    bool reparented;
    Clay_TransitionProperty activeProperties;
} Clay__TransitionDataInternal;

CLAY__ARRAY_DEFINE(Clay__TransitionDataInternal, Clay__TransitionDataInternalArray)

typedef struct { // todo get this struct into a single cache line
    Clay_BoundingBox boundingBox;
    Clay_ElementId elementId;
    Clay_LayoutElement* layoutElement;
    void (*onHoverFunction)(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData);
    void *hoverFunctionUserData;
    int32_t nextIndex;
    uint32_t generation;
    bool appearedThisFrame;
    struct {
        bool collision;
        bool collapsed;
    } debugData;
} Clay_LayoutElementHashMapItem;

CLAY__ARRAY_DEFINE(Clay_LayoutElementHashMapItem, Clay__LayoutElementHashMapItemArray)

typedef struct {
    int32_t startOffset;
    int32_t length;
    float width;
    int32_t next;
} Clay__MeasuredWord;

CLAY__ARRAY_DEFINE(Clay__MeasuredWord, Clay__MeasuredWordArray)

typedef struct {
    Clay_Dimensions unwrappedDimensions;
    int32_t measuredWordsStartIndex;
    float minWidth;
    bool containsNewlines;
    // Hash map data
    uint32_t id;
    int32_t nextIndex;
    uint32_t generation;
} Clay__MeasureTextCacheItem;

CLAY__ARRAY_DEFINE(Clay__MeasureTextCacheItem, Clay__MeasureTextCacheItemArray)

typedef struct {
    Clay_LayoutElement *layoutElement;
    Clay_Vector2 position;
    Clay_Vector2 nextChildOffset;
    bool parentMovedThisFramed; // Used to relativise transitions
} Clay__LayoutElementTreeNode;

CLAY__ARRAY_DEFINE(Clay__LayoutElementTreeNode, Clay__LayoutElementTreeNodeArray)

typedef struct {
    int32_t layoutElementIndex;
    uint32_t parentId; // This can be zero in the case of the root layout tree
    uint32_t clipElementId; // This can be zero if there is no clip element
    int16_t zIndex;
    Clay_Vector2 pointerOffset; // Only used when scroll containers are managed externally
} Clay__LayoutElementTreeRoot;

CLAY__ARRAY_DEFINE(Clay__LayoutElementTreeRoot, Clay__LayoutElementTreeRootArray)

struct Clay_Context {
    int32_t maxElementCount;
    int32_t maxMeasureTextCacheWordCount;
    int32_t exitingElementsLength;
    int32_t exitingElementsChildrenLength;
    bool warningsEnabled;
    bool rootResizedLastFrame;
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
    void *measureTextUserData;
    void *queryScrollOffsetUserData;
    Clay_Arena internalArena;
    // Layout Elements / Render Commands
    Clay_LayoutElementArray layoutElements;
    Clay_RenderCommandArray renderCommands;
    Clay__int32_tArray openLayoutElementStack;
    Clay__int32_tArray layoutElementChildren;
    Clay__int32_tArray layoutElementChildrenBuffer;
    Clay__int32_tArray reusableElementIndexBuffer;
    Clay__int32_tArray layoutElementClipElementIds;
    // Misc Data Structures
    Clay__StringArray layoutElementIdStrings;
    Clay__WrappedTextLineArray wrappedTextLines;
    Clay__LayoutElementTreeNodeArray layoutElementTreeNodeArray1;
    Clay__LayoutElementTreeRootArray layoutElementTreeRoots;
    Clay__LayoutElementHashMapItemArray layoutElementsHashMapInternal;
    Clay__int32_tArray layoutElementsHashMap;
    Clay__int32_tArray layoutElementsHashMapFreeList;
    Clay__MeasureTextCacheItemArray measureTextHashMapInternal;
    Clay__int32_tArray measureTextHashMapInternalFreeList;
    Clay__int32_tArray measureTextHashMap;
    Clay__MeasuredWordArray measuredWords;
    Clay__int32_tArray measuredWordsFreeList;
    Clay__int32_tArray openClipElementStack;
    Clay_ElementIdArray pointerOverIds;
    Clay__ScrollContainerDataInternalArray scrollContainerDatas;
    Clay__TransitionDataInternalArray transitionDatas;
    Clay__boolArray treeNodeVisited;
    Clay__charArray dynamicStringData;
};

Clay_Context* Clay__Context_Allocate_Arena(Clay_Arena *arena) {
    size_t totalSizeBytes = sizeof(Clay_Context);
    if (totalSizeBytes > arena->capacity)
    {
        return NULL;
    }
    arena->nextAllocation += totalSizeBytes;
    return (Clay_Context*)(arena->memory);
}

Clay_String Clay__WriteStringToCharBuffer(Clay__charArray *buffer, Clay_String string) {
    for (int32_t i = 0; i < string.length; i++) {
        buffer->internalArray[buffer->length + i] = string.chars[i];
    }
    buffer->length += string.length;
    return CLAY__INIT(Clay_String) { .length = string.length, .chars = (const char *)(buffer->internalArray + buffer->length - string.length) };
}

#ifdef CLAY_WASM
    __attribute__((import_module("clay"), import_name("measureTextFunction"))) Clay_Dimensions Clay__MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);
    __attribute__((import_module("clay"), import_name("queryScrollOffsetFunction"))) Clay_Vector2 Clay__QueryScrollOffset(uint32_t elementId, void *userData);
#else
    Clay_Dimensions (*Clay__MeasureText)(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);
    Clay_Vector2 (*Clay__QueryScrollOffset)(uint32_t elementId, void *userData);
#endif

Clay_LayoutElement* Clay__GetOpenLayoutElement(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    return Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&context->openLayoutElementStack, context->openLayoutElementStack.length - 1));
}

Clay_LayoutElement* Clay__GetParentElement(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    return Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&context->openLayoutElementStack, context->openLayoutElementStack.length - 2));
}

uint32_t Clay__GetParentElementId(void) {
    return Clay__GetParentElement()->id;
}

bool Clay__BorderHasAnyWidth(Clay_BorderElementConfig* borderConfig) {
    return borderConfig->width.betweenChildren > 0 || borderConfig->width.left > 0 || borderConfig->width.right > 0 || borderConfig->width.top > 0 || borderConfig->width.bottom > 0;
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

Clay_ElementId Clay__HashString(Clay_String key, const uint32_t seed) {
    uint32_t hash = seed;

    for (int32_t i = 0; i < key.length; i++) {
        hash += key.chars[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return CLAY__INIT(Clay_ElementId) { .id = hash + 1, .offset = 0, .baseId = hash + 1, .stringId = key }; // Reserve the hash result of zero as "null id"
}

Clay_ElementId Clay__HashStringWithOffset(Clay_String key, const uint32_t offset, const uint32_t seed) {
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

#if !defined(CLAY_DISABLE_SIMD) && (defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64))
static inline __m128i Clay__SIMDRotateLeft(__m128i x, int r) {
    return _mm_or_si128(_mm_slli_epi64(x, r), _mm_srli_epi64(x, 64 - r));
}

static inline void Clay__SIMDARXMix(__m128i* a, __m128i* b) {
    *a = _mm_add_epi64(*a, *b);
    *b = _mm_xor_si128(Clay__SIMDRotateLeft(*b, 17), *a);
}

uint64_t Clay__HashData(const uint8_t* data, size_t length) {
    // Pinched these constants from the BLAKE implementation
    __m128i v0 = _mm_set1_epi64x(0x6a09e667f3bcc908ULL);
    __m128i v1 = _mm_set1_epi64x(0xbb67ae8584caa73bULL);
    __m128i v2 = _mm_set1_epi64x(0x3c6ef372fe94f82bULL);
    __m128i v3 = _mm_set1_epi64x(0xa54ff53a5f1d36f1ULL);

    uint8_t overflowBuffer[16] = { 0 };  // Temporary buffer for small inputs

    while (length > 0) {
        __m128i msg;
        if (length >= 16) {
            msg = _mm_loadu_si128((const __m128i*)data);
            data += 16;
            length -= 16;
        }
        else {
            for (size_t i = 0; i < length; i++) {
                overflowBuffer[i] = data[i];
            }
            msg = _mm_loadu_si128((const __m128i*)overflowBuffer);
            length = 0;
        }

        v0 = _mm_xor_si128(v0, msg);
        Clay__SIMDARXMix(&v0, &v1);
        Clay__SIMDARXMix(&v2, &v3);

        v0 = _mm_add_epi64(v0, v2);
        v1 = _mm_add_epi64(v1, v3);
    }

    Clay__SIMDARXMix(&v0, &v1);
    Clay__SIMDARXMix(&v2, &v3);
    v0 = _mm_add_epi64(v0, v2);
    v1 = _mm_add_epi64(v1, v3);
    v0 = _mm_add_epi64(v0, v1);

    uint64_t result[2];
    _mm_storeu_si128((__m128i*)result, v0);

    return result[0] ^ result[1];
}
#elif !defined(CLAY_DISABLE_SIMD) && defined(__aarch64__)
static inline void Clay__SIMDARXMix(uint64x2_t* a, uint64x2_t* b) {
    *a = vaddq_u64(*a, *b);
    *b = veorq_u64(vorrq_u64(vshlq_n_u64(*b, 17), vshrq_n_u64(*b, 64 - 17)), *a);
}

uint64_t Clay__HashData(const uint8_t* data, size_t length) {
    // Pinched these constants from the BLAKE implementation
    uint64x2_t v0 = vdupq_n_u64(0x6a09e667f3bcc908ULL);
    uint64x2_t v1 = vdupq_n_u64(0xbb67ae8584caa73bULL);
    uint64x2_t v2 = vdupq_n_u64(0x3c6ef372fe94f82bULL);
    uint64x2_t v3 = vdupq_n_u64(0xa54ff53a5f1d36f1ULL);

    uint8_t overflowBuffer[8] = { 0 };

    while (length > 0) {
        uint64x2_t msg;
        if (length > 16) {
            msg = vld1q_u64((const uint64_t*)data);
            data += 16;
            length -= 16;
        }
        else if (length > 8) {
            msg = vcombine_u64(vld1_u64((const uint64_t*)data), vdup_n_u64(0));
            data += 8;
            length -= 8;
        }
        else {
            for (size_t i = 0; i < length; i++) {
                overflowBuffer[i] = data[i];
            }
            uint8x8_t lower = vld1_u8(overflowBuffer);
            msg = vreinterpretq_u64_u8(vcombine_u8(lower, vdup_n_u8(0)));
            length = 0;
        }
        v0 = veorq_u64(v0, msg);
        Clay__SIMDARXMix(&v0, &v1);
        Clay__SIMDARXMix(&v2, &v3);

        v0 = vaddq_u64(v0, v2);
        v1 = vaddq_u64(v1, v3);
    }

    Clay__SIMDARXMix(&v0, &v1);
    Clay__SIMDARXMix(&v2, &v3);
    v0 = vaddq_u64(v0, v2);
    v1 = vaddq_u64(v1, v3);
    v0 = vaddq_u64(v0, v1);

    uint64_t result[2];
    vst1q_u64(result, v0);

    return result[0] ^ result[1];
}
#else
uint64_t Clay__HashData(const uint8_t* data, size_t length) {
    uint64_t hash = 0;

    for (size_t i = 0; i < length; i++) {
        hash += data[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    return hash;
}
#endif

uint32_t Clay__HashStringContentsWithConfig(Clay_String *text, Clay_TextElementConfig *config) {
    uint32_t hash = 0;
    if (text->isStaticallyAllocated) {
        hash += (uintptr_t)text->chars;
        hash += (hash << 10);
        hash ^= (hash >> 6);
        hash += text->length;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    } else {
        hash = Clay__HashData((const uint8_t *)text->chars, text->length) % UINT32_MAX;
    }

    hash += config->fontId;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += config->fontSize;
    hash += (hash << 10);
    hash ^= (hash >> 6);

    hash += config->letterSpacing;
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
        uint32_t newItemIndex = Clay__int32_tArray_GetValue(&context->measuredWordsFreeList, (int)context->measuredWordsFreeList.length - 1);
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
        return &Clay__MeasureTextCacheItem_DEFAULT;
    }
    #endif
    uint32_t id = Clay__HashStringContentsWithConfig(text, config);
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
        newItemIndex = Clay__int32_tArray_GetValue(&context->measureTextHashMapInternalFreeList, context->measureTextHashMapInternalFreeList.length - 1);
        context->measureTextHashMapInternalFreeList.length--;
        Clay__MeasureTextCacheItemArray_Set(&context->measureTextHashMapInternal, newItemIndex, newCacheItem);
        measured = Clay__MeasureTextCacheItemArray_Get(&context->measureTextHashMapInternal, newItemIndex);
    } else {
        if (context->measureTextHashMapInternal.length == context->measureTextHashMapInternal.capacity - 1) {
            if (!context->booleanWarnings.maxTextMeasureCacheExceeded) {
                context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                        .errorType = CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED,
                        .errorText = CLAY_STRING("Clay ran out of capacity while attempting to measure text elements. Try using Clay_SetMaxElementCount() with a higher value."),
                        .userData = context->errorHandler.userData });
                context->booleanWarnings.maxTextMeasureCacheExceeded = true;
            }
            return &Clay__MeasureTextCacheItem_DEFAULT;
        }
        measured = Clay__MeasureTextCacheItemArray_Add(&context->measureTextHashMapInternal, newCacheItem);
        newItemIndex = context->measureTextHashMapInternal.length - 1;
    }

    int32_t start = 0;
    int32_t end = 0;
    float lineWidth = 0;
    float measuredWidth = 0;
    float measuredHeight = 0;
    float spaceWidth = Clay__MeasureText(CLAY__INIT(Clay_StringSlice) { .length = 1, .chars = CLAY__SPACECHAR.chars, .baseChars = CLAY__SPACECHAR.chars }, config, context->measureTextUserData).width;
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
            return &Clay__MeasureTextCacheItem_DEFAULT;
        }
        char current = text->chars[end];
        if (current == ' ' || current == '\n') {
            int32_t length = end - start;
            Clay_Dimensions dimensions = CLAY__DEFAULT_STRUCT;
            if (length > 0) {
                dimensions = Clay__MeasureText(CLAY__INIT(Clay_StringSlice) {.length = length, .chars = &text->chars[start], .baseChars = text->chars}, config, context->measureTextUserData);
            }
            measured->minWidth = CLAY__MAX(dimensions.width, measured->minWidth);
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
        Clay_Dimensions dimensions = Clay__MeasureText(CLAY__INIT(Clay_StringSlice) { .length = end - start, .chars = &text->chars[start], .baseChars = text->chars }, config, context->measureTextUserData);
        Clay__AddMeasuredWord(CLAY__INIT(Clay__MeasuredWord) { .startOffset = start, .length = end - start, .width = dimensions.width, .next = -1 }, previousWord);
        lineWidth += dimensions.width;
        measuredHeight = CLAY__MAX(measuredHeight, dimensions.height);
        measured->minWidth = CLAY__MAX(dimensions.width, measured->minWidth);
    }
    measuredWidth = CLAY__MAX(lineWidth, measuredWidth) - config->letterSpacing;

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
        if (!context->booleanWarnings.hashMapCapacityExceeded) {
            context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                .errorType = CLAY_ERROR_TYPE_HASH_MAP_CAPACITY_EXCEEDED,
                .errorText = CLAY_STRING("Clay has run out of space in it's internal element ID hashmap.  Try using Clay_SetMaxElementCount() with a higher value."),
                .userData = context->errorHandler.userData });
            context->booleanWarnings.hashMapCapacityExceeded = true;
        }
        return NULL;
    }
    Clay_LayoutElementHashMapItem item = { .elementId = elementId, .layoutElement = layoutElement, .nextIndex = -1, .generation = context->generation + 1, .appearedThisFrame = true };
    uint32_t hashBucket = elementId.id % context->layoutElementsHashMap.capacity;
    int32_t hashItemPrevious = -1;
    int32_t hashItemIndex = context->layoutElementsHashMap.internalArray[hashBucket];
    while (hashItemIndex != -1) { // Just replace collision, not a big deal - leave it up to the end user
        Clay_LayoutElementHashMapItem *hashItem = Clay__LayoutElementHashMapItemArray_Get(&context->layoutElementsHashMapInternal, hashItemIndex);
        if (hashItem->elementId.id == elementId.id) { // Collision - resolve based on generation
            item.nextIndex = hashItem->nextIndex;
            if (hashItem->generation <= context->generation) { // First collision - assume this is the "same" element
                hashItem->appearedThisFrame = hashItem->generation < context->generation;
                hashItem->elementId = elementId; // Make sure to copy this across. If the stringId reference has changed, we should update the hash item to use the new one.
                hashItem->generation = context->generation + 1;
                hashItem->layoutElement = layoutElement;
                hashItem->debugData.collision = false;
                hashItem->onHoverFunction = NULL;
                hashItem->hoverFunctionUserData = 0;
            } else { // Multiple collisions this frame - two elements have the same ID
                context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                    .errorType = CLAY_ERROR_TYPE_DUPLICATE_ID,
                    .errorText = CLAY_STRING("An element with this ID was already previously declared during this layout."),
                    .userData = context->errorHandler.userData });
                if (context->debugModeEnabled) {
                    hashItem->debugData.collision = true;
                }
            }
            return hashItem;
        }
        hashItemPrevious = hashItemIndex;
        hashItemIndex = hashItem->nextIndex;
    }

    int32_t indexToUse = 0;
    if (context->layoutElementsHashMapFreeList.length > 0) {
        indexToUse = Clay__int32_tArray_GetValue(&context->layoutElementsHashMapFreeList, context->layoutElementsHashMapFreeList.length - 1);
        context->layoutElementsHashMapFreeList.length--;
    } else {
        indexToUse = context->layoutElementsHashMapInternal.length;
    }
    Clay_LayoutElementHashMapItem *hashItem = Clay__LayoutElementHashMapItemArray_Set(&context->layoutElementsHashMapInternal, indexToUse, item);
    if (hashItemPrevious != -1) {
        Clay__LayoutElementHashMapItemArray_Get(&context->layoutElementsHashMapInternal, hashItemPrevious)->nextIndex = (int32_t)indexToUse;
    } else {
        context->layoutElementsHashMap.internalArray[hashBucket] = (int32_t)indexToUse;
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
    return &Clay_LayoutElementHashMapItem_DEFAULT;
}

void Clay__UpdateAspectRatioBox(Clay_LayoutElement *layoutElement) {
    if (layoutElement->config.aspectRatio.aspectRatio != 0) {
        if (layoutElement->dimensions.width == 0 && layoutElement->dimensions.height != 0) {
            layoutElement->dimensions.width = layoutElement->dimensions.height * layoutElement->config.aspectRatio.aspectRatio;
        } else if (layoutElement->dimensions.width != 0 && layoutElement->dimensions.height == 0) {
            layoutElement->dimensions.height = layoutElement->dimensions.width * (1 / layoutElement->config.aspectRatio.aspectRatio);
        }
    }
}

void Clay__CloseElement(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    Clay_LayoutConfig *layoutConfig = &openLayoutElement->config.layout;
    bool elementHasClipHorizontal = openLayoutElement->config.clip.horizontal;
    bool elementHasClipVertical = openLayoutElement->config.clip.vertical;
    if (elementHasClipHorizontal || elementHasClipVertical || openLayoutElement->config.floating.attachTo != CLAY_ATTACH_TO_NONE) {
        context->openClipElementStack.length--;
    }

    float leftRightPadding = (float)(layoutConfig->padding.left + layoutConfig->padding.right);
    float topBottomPadding = (float)(layoutConfig->padding.top + layoutConfig->padding.bottom);

    // Attach children to the current open element
    openLayoutElement->children.elements = &context->layoutElementChildren.internalArray[context->layoutElementChildren.length];
    if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
        openLayoutElement->dimensions.width = leftRightPadding;
        openLayoutElement->minDimensions.width = leftRightPadding;
        for (int32_t i = 0; i < openLayoutElement->children.length; i++) {
            int32_t childIndex = Clay__int32_tArray_GetValue(&context->layoutElementChildrenBuffer, (int)context->layoutElementChildrenBuffer.length - openLayoutElement->children.length + i);
            Clay_LayoutElement *child = Clay_LayoutElementArray_Get(&context->layoutElements, childIndex);
            openLayoutElement->dimensions.width += child->dimensions.width;
            openLayoutElement->dimensions.height = CLAY__MAX(openLayoutElement->dimensions.height, child->dimensions.height + topBottomPadding);
            // Minimum size of child elements doesn't matter to clip containers as they can shrink and hide their contents
            if (!elementHasClipHorizontal) {
                openLayoutElement->minDimensions.width += child->minDimensions.width;
            }
            if (!elementHasClipVertical) {
                openLayoutElement->minDimensions.height = CLAY__MAX(openLayoutElement->minDimensions.height, child->minDimensions.height + topBottomPadding);
            }
            Clay__int32_tArray_Add(&context->layoutElementChildren, childIndex);
        }
        float childGap = (float)(CLAY__MAX(openLayoutElement->children.length - 1, 0) * layoutConfig->childGap);
        openLayoutElement->dimensions.width += childGap;
        if (!elementHasClipHorizontal) {
            openLayoutElement->minDimensions.width += childGap;
        }
    }
    else if (layoutConfig->layoutDirection == CLAY_TOP_TO_BOTTOM) {
        openLayoutElement->dimensions.height = topBottomPadding;
        openLayoutElement->minDimensions.height = topBottomPadding;
        for (int32_t i = 0; i < openLayoutElement->children.length; i++) {
            int32_t childIndex = Clay__int32_tArray_GetValue(&context->layoutElementChildrenBuffer, (int)context->layoutElementChildrenBuffer.length - openLayoutElement->children.length + i);
            Clay_LayoutElement *child = Clay_LayoutElementArray_Get(&context->layoutElements, childIndex);
            openLayoutElement->dimensions.height += child->dimensions.height;
            openLayoutElement->dimensions.width = CLAY__MAX(openLayoutElement->dimensions.width, child->dimensions.width + leftRightPadding);
            // Minimum size of child elements doesn't matter to clip containers as they can shrink and hide their contents
            if (!elementHasClipVertical) {
                openLayoutElement->minDimensions.height += child->minDimensions.height;
            }
            if (!elementHasClipHorizontal) {
                openLayoutElement->minDimensions.width = CLAY__MAX(openLayoutElement->minDimensions.width, child->minDimensions.width + leftRightPadding);
            }
            Clay__int32_tArray_Add(&context->layoutElementChildren, childIndex);
        }
        float childGap = (float)(CLAY__MAX(openLayoutElement->children.length - 1, 0) * layoutConfig->childGap);
        openLayoutElement->dimensions.height += childGap;
        if (!elementHasClipVertical) {
            openLayoutElement->minDimensions.height += childGap;
        }
    }

    context->layoutElementChildrenBuffer.length -= openLayoutElement->children.length;

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

    Clay__UpdateAspectRatioBox(openLayoutElement);

    bool elementIsFloating = openLayoutElement->config.floating.attachTo != CLAY_ATTACH_TO_NONE;

    // Close the currently open element
    int32_t closingElementIndex = Clay__int32_tArray_RemoveSwapback(&context->openLayoutElementStack, (int)context->openLayoutElementStack.length - 1);

    // Get the currently open parent
    openLayoutElement = Clay__GetOpenLayoutElement();

    if (context->openLayoutElementStack.length > 1) {
        if(elementIsFloating) {
            openLayoutElement->floatingChildrenCount++;
            return;
        }
        openLayoutElement->children.length++;
        Clay__int32_tArray_Add(&context->layoutElementChildrenBuffer, closingElementIndex);
    }
}

bool Clay__MemCmp(const char *s1, const char *s2, int32_t length);
#if !defined(CLAY_DISABLE_SIMD) && (defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64))
    bool Clay__MemCmp(const char *s1, const char *s2, int32_t length) {
        while (length >= 16) {
            __m128i v1 = _mm_loadu_si128((const __m128i *)s1);
            __m128i v2 = _mm_loadu_si128((const __m128i *)s2);

            if (_mm_movemask_epi8(_mm_cmpeq_epi8(v1, v2)) != 0xFFFF) { // If any byte differs
                return false;
            }

            s1 += 16;
            s2 += 16;
            length -= 16;
        }

        // Handle remaining bytes
        while (length--) {
            if (*s1 != *s2) {
                return false;
            }
            s1++;
            s2++;
        }

        return true;
    }
#elif !defined(CLAY_DISABLE_SIMD) && defined(__aarch64__)
    bool Clay__MemCmp(const char *s1, const char *s2, int32_t length) {
        while (length >= 16) {
            uint8x16_t v1 = vld1q_u8((const uint8_t *)s1);
            uint8x16_t v2 = vld1q_u8((const uint8_t *)s2);

            // Compare vectors
            if (vminvq_u32(vreinterpretq_u32_u8(vceqq_u8(v1, v2))) != 0xFFFFFFFF) { // If there's a difference
                return false;
            }

            s1 += 16;
            s2 += 16;
            length -= 16;
        }

        // Handle remaining bytes
        while (length--) {
            if (*s1 != *s2) {
                return false;
            }
            s1++;
            s2++;
        }

        return true;
    }
#else
    bool Clay__MemCmp(const char *s1, const char *s2, int32_t length) {
        for (int32_t i = 0; i < length; i++) {
            if (s1[i] != s2[i]) {
                return false;
            }
        }
        return true;
    }
#endif

void Clay__OpenElement(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->layoutElements.length == context->layoutElements.capacity - 1 || context->booleanWarnings.maxElementsExceeded) {
        context->booleanWarnings.maxElementsExceeded = true;
        return;
    }
    Clay_LayoutElement layoutElement = CLAY__DEFAULT_STRUCT;
    Clay_LayoutElement* openLayoutElement = Clay_LayoutElementArray_Add(&context->layoutElements, layoutElement);
    Clay__int32_tArray_Add(&context->openLayoutElementStack, context->layoutElements.length - 1);
    // Generate an ID
    Clay_LayoutElement *parentElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&context->openLayoutElementStack, context->openLayoutElementStack.length - 2));
    uint32_t offset = parentElement->children.length + parentElement->floatingChildrenCount;
    Clay_ElementId elementId = Clay__HashNumber(offset, parentElement->id);
    openLayoutElement->id = elementId.id;
    Clay__AddHashMapItem(elementId, openLayoutElement);
    Clay__StringArray_Add(&context->layoutElementIdStrings, elementId.stringId);
    if (context->openClipElementStack.length > 0) {
        Clay__int32_tArray_Set(&context->layoutElementClipElementIds, context->layoutElements.length - 1, Clay__int32_tArray_GetValue(&context->openClipElementStack, (int)context->openClipElementStack.length - 1));
    } else {
        Clay__int32_tArray_Set(&context->layoutElementClipElementIds, context->layoutElements.length - 1, 0);
    }
}

void Clay__OpenElementWithId(Clay_ElementId elementId) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->layoutElements.length == context->layoutElements.capacity - 1 || context->booleanWarnings.maxElementsExceeded) {
        context->booleanWarnings.maxElementsExceeded = true;
        return;
    }
    Clay_LayoutElement layoutElement = CLAY__DEFAULT_STRUCT;
    layoutElement.id = elementId.id;
    Clay_LayoutElement * openLayoutElement = Clay_LayoutElementArray_Add(&context->layoutElements, layoutElement);
    Clay__int32_tArray_Add(&context->openLayoutElementStack, context->layoutElements.length - 1);
    Clay__AddHashMapItem(elementId, openLayoutElement);
    Clay__StringArray_Add(&context->layoutElementIdStrings, elementId.stringId);
    if (context->openClipElementStack.length > 0) {
        Clay__int32_tArray_Set(&context->layoutElementClipElementIds, context->layoutElements.length - 1, Clay__int32_tArray_GetValue(&context->openClipElementStack, (int)context->openClipElementStack.length - 1));
    } else {
        Clay__int32_tArray_Set(&context->layoutElementClipElementIds, context->layoutElements.length - 1, 0);
    }
}

void Clay__OpenTextElement(Clay_String text, Clay_TextElementConfig textConfig) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->layoutElements.length == context->layoutElements.capacity - 1 || context->booleanWarnings.maxElementsExceeded) {
        context->booleanWarnings.maxElementsExceeded = true;
        return;
    }
    Clay_LayoutElement *parentElement = Clay__GetOpenLayoutElement();

    Clay_LayoutElement layoutElement = { .textConfig = textConfig, .isTextElement = true };
    Clay_LayoutElement *textElement = Clay_LayoutElementArray_Add(&context->layoutElements, layoutElement);
    if (context->openClipElementStack.length > 0) {
        Clay__int32_tArray_Set(&context->layoutElementClipElementIds, context->layoutElements.length - 1, Clay__int32_tArray_GetValue(&context->openClipElementStack, (int)context->openClipElementStack.length - 1));
    } else {
        Clay__int32_tArray_Set(&context->layoutElementClipElementIds, context->layoutElements.length - 1, 0);
    }

    Clay__int32_tArray_Add(&context->layoutElementChildrenBuffer, context->layoutElements.length - 1);
    Clay__MeasureTextCacheItem *textMeasured = Clay__MeasureTextCached(&text, &textConfig);
    Clay_ElementId elementId = Clay__HashNumber(parentElement->children.length + parentElement->floatingChildrenCount, parentElement->id);
    textElement->id = elementId.id;
    Clay__AddHashMapItem(elementId, textElement);
    Clay__StringArray_Add(&context->layoutElementIdStrings, elementId.stringId);
    Clay_Dimensions textDimensions = { .width = textMeasured->unwrappedDimensions.width, .height = textConfig.lineHeight > 0 ? (float)textConfig.lineHeight : textMeasured->unwrappedDimensions.height };
    textElement->dimensions = textDimensions;
    textElement->minDimensions = CLAY__INIT(Clay_Dimensions) { .width = textMeasured->minWidth, .height = textDimensions.height };
    textElement->textElementData = CLAY__INIT(Clay__TextElementData) { .text = text, .preferredDimensions = textMeasured->unwrappedDimensions };
    parentElement->children.length++;
}

void Clay__ConfigureOpenElementPtr(const Clay_ElementDeclaration *declaration) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    openLayoutElement->config = *declaration;
    if ((declaration->layout.sizing.width.type == CLAY__SIZING_TYPE_PERCENT && declaration->layout.sizing.width.size.percent > 1) || (declaration->layout.sizing.height.type == CLAY__SIZING_TYPE_PERCENT && declaration->layout.sizing.height.size.percent > 1)) {
        context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                .errorType = CLAY_ERROR_TYPE_PERCENTAGE_OVER_1,
                .errorText = CLAY_STRING("An element was configured with CLAY_SIZING_PERCENT, but the provided percentage value was over 1.0. Clay expects a value between 0 and 1, i.e. 20% is 0.2."),
                .userData = context->errorHandler.userData });
    }

    if (declaration->floating.attachTo != CLAY_ATTACH_TO_NONE) {
        Clay_FloatingElementConfig* floatingConfig = &openLayoutElement->config.floating;
        // This looks dodgy but because of the auto generated root element the depth of the tree will always be at least 2 here
        Clay_LayoutElement *hierarchicalParent = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&context->openLayoutElementStack, context->openLayoutElementStack.length - 2));
        if (hierarchicalParent) {
            uint32_t clipElementId = 0;
            if (declaration->floating.attachTo == CLAY_ATTACH_TO_PARENT) {
                // Attach to the element's direct hierarchical parent
                floatingConfig->parentId = hierarchicalParent->id;
                if (context->openClipElementStack.length > 0) {
                    clipElementId = Clay__int32_tArray_GetValue(&context->openClipElementStack, (int)context->openClipElementStack.length - 1);
                }
            } else if (declaration->floating.attachTo == CLAY_ATTACH_TO_ELEMENT_WITH_ID) {
                Clay_LayoutElementHashMapItem *parentItem = Clay__GetHashMapItem(floatingConfig->parentId);
                if (parentItem == &Clay_LayoutElementHashMapItem_DEFAULT) {
                    context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                            .errorType = CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND,
                            .errorText = CLAY_STRING("A floating element was declared with a parentId, but no element with that ID was found."),
                            .userData = context->errorHandler.userData });
                } else {
                    clipElementId = Clay__int32_tArray_GetValue(&context->layoutElementClipElementIds, (int32_t)(parentItem->layoutElement - context->layoutElements.internalArray));
                }
            } else if (declaration->floating.attachTo == CLAY_ATTACH_TO_ROOT) {
                floatingConfig->parentId = Clay__HashString(CLAY_STRING("Clay__RootContainer"), 0).id;
            }
            if (declaration->floating.clipTo == CLAY_CLIP_TO_NONE) {
                clipElementId = 0;
            }
            int32_t currentElementIndex = Clay__int32_tArray_GetValue(&context->openLayoutElementStack, context->openLayoutElementStack.length - 1);
            Clay__int32_tArray_Set(&context->layoutElementClipElementIds, currentElementIndex, clipElementId);
            Clay__int32_tArray_Add(&context->openClipElementStack, clipElementId);
            Clay__LayoutElementTreeRootArray_Add(&context->layoutElementTreeRoots, CLAY__INIT(Clay__LayoutElementTreeRoot) {
                .layoutElementIndex = Clay__int32_tArray_GetValue(&context->openLayoutElementStack, context->openLayoutElementStack.length - 1),
                .parentId = floatingConfig->parentId,
                .clipElementId = clipElementId,
                .zIndex = floatingConfig->zIndex,
            });
        }
    }

    if (declaration->clip.horizontal || declaration->clip.vertical) {
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
    }
    // Setup data to track transitions across frames
    if (declaration->transition.handler) {
        Clay__TransitionDataInternal *transitionData = CLAY__NULL;
        Clay_LayoutElement* parentElement = Clay__GetParentElement();
        for (int32_t i = 0; i < context->transitionDatas.length; i++) {
            Clay__TransitionDataInternal *existingData = Clay__TransitionDataInternalArray_Get(&context->transitionDatas, i);
            if (openLayoutElement->id == existingData->elementId) {
                if (existingData->state == CLAY_TRANSITION_STATE_EXITING) {
                    existingData->state = CLAY_TRANSITION_STATE_IDLE;
                    Clay_LayoutElementHashMapItem* hashMapItem = Clay__GetHashMapItem(openLayoutElement->id);
                    hashMapItem->appearedThisFrame = false;
                }
                transitionData = existingData;
                transitionData->elementThisFrame = openLayoutElement;
                if (transitionData->parentId != parentElement->id) {
                    transitionData->reparented = true;
                }
                transitionData->parentId = parentElement->id;
                transitionData->siblingIndex = parentElement->children.length;
                transitionData->transitionOut = !!declaration->transition.exit.setFinalState;
            }
        }
        if (!transitionData) {
            transitionData = Clay__TransitionDataInternalArray_Add(&context->transitionDatas, CLAY__INIT(Clay__TransitionDataInternal){
                .elementThisFrame = openLayoutElement,
                .elementId = openLayoutElement->id,
                .parentId = parentElement->id,
                .siblingIndex = parentElement->children.length,
                .transitionOut = !!declaration->transition.exit.setFinalState
            });
        }
    }
}

void Clay__ConfigureOpenElement(const Clay_ElementDeclaration declaration) {
    Clay__ConfigureOpenElementPtr(&declaration);
}

void Clay__InitializeEphemeralMemory(Clay_Context* context) {
    int32_t maxElementCount = context->maxElementCount;
    // Ephemeral Memory - reset every frame
    Clay_Arena *arena = &context->internalArena;
    arena->nextAllocation = context->arenaResetOffset;

    context->layoutElementChildrenBuffer = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElements = Clay_LayoutElementArray_Allocate_Arena(maxElementCount, arena);
    context->warnings = Clay__WarningArray_Allocate_Arena(100, arena);

    context->layoutElementIdStrings = Clay__StringArray_Allocate_Arena(maxElementCount, arena);
    context->wrappedTextLines = Clay__WrappedTextLineArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementTreeNodeArray1 = Clay__LayoutElementTreeNodeArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementTreeRoots = Clay__LayoutElementTreeRootArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementChildren = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->openLayoutElementStack = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->renderCommands = Clay_RenderCommandArray_Allocate_Arena(maxElementCount, arena);
    context->treeNodeVisited = Clay__boolArray_Allocate_Arena(maxElementCount, arena);
    context->treeNodeVisited.length = context->treeNodeVisited.capacity; // This array is accessed directly rather than behaving as a list
    context->openClipElementStack = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->reusableElementIndexBuffer = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementClipElementIds = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->dynamicStringData = Clay__charArray_Allocate_Arena(maxElementCount, arena);
}

void Clay__InitializePersistentMemory(Clay_Context* context) {
    // Persistent memory - initialized once and not reset
    int32_t maxElementCount = context->maxElementCount;
    int32_t maxMeasureTextCacheWordCount = context->maxMeasureTextCacheWordCount;
    Clay_Arena *arena = &context->internalArena;

    context->scrollContainerDatas = Clay__ScrollContainerDataInternalArray_Allocate_Arena(100, arena);
    context->transitionDatas = Clay__TransitionDataInternalArray_Allocate_Arena(200, arena);
    context->layoutElementsHashMapInternal = Clay__LayoutElementHashMapItemArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementsHashMap = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementsHashMapFreeList = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->measureTextHashMapInternal = Clay__MeasureTextCacheItemArray_Allocate_Arena(maxElementCount, arena);
    context->measureTextHashMapInternalFreeList = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->measuredWordsFreeList = Clay__int32_tArray_Allocate_Arena(maxMeasureTextCacheWordCount, arena);
    context->measureTextHashMap = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->measuredWords = Clay__MeasuredWordArray_Allocate_Arena(maxMeasureTextCacheWordCount, arena);
    context->pointerOverIds = Clay_ElementIdArray_Allocate_Arena(maxElementCount, arena);
    context->arenaResetOffset = arena->nextAllocation;
}

const float CLAY__EPSILON = 0.01;

bool Clay__FloatEqual(float left, float right) {
    float subtracted = left - right;
    return subtracted < CLAY__EPSILON && subtracted > -CLAY__EPSILON;
}

Clay_SizingAxis Clay__GetElementSizing(Clay_LayoutElement* element, bool xAxis) {
    if (element->isTextElement) {
        return CLAY__INIT(Clay_SizingAxis) {};
    } else {
        return xAxis ? element->config.layout.sizing.width : element->config.layout.sizing.height;
    }
}

// Writes out the location of text elements to layout elements buffer 1
void Clay__SizeContainersAlongAxis(bool xAxis, float deltaTime, Clay__int32_tArray* textElementsOut, Clay__int32_tArray* aspectRatioElementsOut) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__int32_tArray bfsBuffer = context->layoutElementChildrenBuffer;
    Clay__int32_tArray resizableContainerBuffer = context->openLayoutElementStack;
    for (int32_t rootIndex = 0; rootIndex < context->layoutElementTreeRoots.length; ++rootIndex) {
        bfsBuffer.length = 0;
        Clay__LayoutElementTreeRoot *root = Clay__LayoutElementTreeRootArray_Get(&context->layoutElementTreeRoots, rootIndex);
        Clay_LayoutElement *rootElement = Clay_LayoutElementArray_Get(&context->layoutElements, (int)root->layoutElementIndex);
        Clay__int32_tArray_Add(&bfsBuffer, (int32_t)root->layoutElementIndex);

        // Size floating containers to their parents
        if (rootElement->config.floating.attachTo != CLAY_ATTACH_TO_NONE) {
            Clay_FloatingElementConfig *floatingElementConfig = &rootElement->config.floating;
            Clay_LayoutElementHashMapItem *parentItem = Clay__GetHashMapItem(floatingElementConfig->parentId);
            if (parentItem && parentItem != &Clay_LayoutElementHashMapItem_DEFAULT) {
                Clay_LayoutElement *parentLayoutElement = parentItem->layoutElement;
                switch (rootElement->config.layout.sizing.width.type) {
                    case CLAY__SIZING_TYPE_GROW: {
                        rootElement->dimensions.width = parentLayoutElement->dimensions.width;
                        break;
                    }
                    case CLAY__SIZING_TYPE_PERCENT: {
                        rootElement->dimensions.width = parentLayoutElement->dimensions.width * rootElement->config.layout.sizing.width.size.percent;
                        break;
                    }
                    default: break;
                }
                switch (rootElement->config.layout.sizing.height.type) {
                    case CLAY__SIZING_TYPE_GROW: {
                        rootElement->dimensions.height = parentLayoutElement->dimensions.height;
                        break;
                    }
                    case CLAY__SIZING_TYPE_PERCENT: {
                        rootElement->dimensions.height = parentLayoutElement->dimensions.height * rootElement->config.layout.sizing.height.size.percent;
                        break;
                    }
                    default: break;
                }
            }
        }

        if (rootElement->config.layout.sizing.width.type != CLAY__SIZING_TYPE_PERCENT) {
            rootElement->dimensions.width = CLAY__MIN(CLAY__MAX(rootElement->dimensions.width, rootElement->config.layout.sizing.width.size.minMax.min), rootElement->config.layout.sizing.width.size.minMax.max);
        }
        if (rootElement->config.layout.sizing.height.type != CLAY__SIZING_TYPE_PERCENT) {
            rootElement->dimensions.height = CLAY__MIN(CLAY__MAX(rootElement->dimensions.height, rootElement->config.layout.sizing.height.size.minMax.min), rootElement->config.layout.sizing.height.size.minMax.max);
        }


        for (int32_t i = 0; i < bfsBuffer.length; ++i) {
            int32_t parentIndex = Clay__int32_tArray_GetValue(&bfsBuffer, i);
            Clay_LayoutElement *parent = Clay_LayoutElementArray_Get(&context->layoutElements, parentIndex);
            Clay_LayoutConfig *parentLayoutConfig = &parent->config.layout;
            int32_t growContainerCount = 0;
            float parentSize = xAxis ? parent->dimensions.width : parent->dimensions.height;
            float parentPadding = (float)(xAxis ? (parentLayoutConfig->padding.left + parentLayoutConfig->padding.right) : (parentLayoutConfig->padding.top + parentLayoutConfig->padding.bottom));
            float innerContentSize = 0, totalPaddingAndChildGaps = parentPadding;
            bool sizingAlongAxis = (xAxis && parentLayoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) || (!xAxis && parentLayoutConfig->layoutDirection == CLAY_TOP_TO_BOTTOM);
            resizableContainerBuffer.length = 0;
            float parentChildGap = parentLayoutConfig->childGap;
            bool isFirstChild = true;

            for (int32_t childOffset = 0; childOffset < parent->children.length; childOffset++) {
                int32_t childElementIndex = parent->children.elements[childOffset];
                Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, childElementIndex);
                Clay_SizingAxis childSizing = Clay__GetElementSizing(childElement, xAxis);
                float childSize = xAxis ? childElement->dimensions.width : childElement->dimensions.height;

                if (textElementsOut && childElement->isTextElement) {
                    Clay__int32_tArray_Add(textElementsOut, childElementIndex);
                } else if (childElement->children.length > 0) {
                    Clay__int32_tArray_Add(&bfsBuffer, childElementIndex);
                }

                if (!childElement->isTextElement && aspectRatioElementsOut && childElement->config.aspectRatio.aspectRatio != 0) {
                    Clay__int32_tArray_Add(aspectRatioElementsOut, childElementIndex);
                }

                // Note: setting isFirstChild = false is skipped here
                if (childElement->exiting) {
                    continue;
                }

                if (childSizing.type != CLAY__SIZING_TYPE_PERCENT
                    && childSizing.type != CLAY__SIZING_TYPE_FIXED
                    && (!childElement->isTextElement || childElement->textConfig.wrapMode == CLAY_TEXT_WRAP_WORDS)
//                    && (xAxis || !Clay__ElementHasConfig(childElement, CLAY__ELEMENT_CONFIG_TYPE_ASPECT))
                ) {
                    Clay__int32_tArray_Add(&resizableContainerBuffer, childElementIndex);
                }

                if (sizingAlongAxis) {
                    innerContentSize += (childSizing.type == CLAY__SIZING_TYPE_PERCENT ? 0 : childSize);
                    if (childSizing.type == CLAY__SIZING_TYPE_GROW) {
                        growContainerCount++;
                    }
                    if (!isFirstChild) {
                        innerContentSize += parentChildGap; // For children after index 0, the childAxisOffset is the gap from the previous child
                        totalPaddingAndChildGaps += parentChildGap;
                    }
                } else {
                    innerContentSize = CLAY__MAX(childSize, innerContentSize);
                }
                isFirstChild = false;
            }

            // Expand percentage containers to size
            for (int32_t childOffset = 0; childOffset < parent->children.length; childOffset++) {
                int32_t childElementIndex = parent->children.elements[childOffset];
                Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, childElementIndex);
                Clay_SizingAxis childSizing = Clay__GetElementSizing(childElement, xAxis);
                float *childSize = xAxis ? &childElement->dimensions.width : &childElement->dimensions.height;
                if (childSizing.type == CLAY__SIZING_TYPE_PERCENT) {
                    *childSize = (parentSize - totalPaddingAndChildGaps) * childSizing.size.percent;
                    if (sizingAlongAxis) {
                        innerContentSize += *childSize;
                    }
                    Clay__UpdateAspectRatioBox(childElement);
                }
            }

            if (sizingAlongAxis) {
                float sizeToDistribute = parentSize - parentPadding - innerContentSize;
                // The content is too large, compress the children as much as possible
                if (sizeToDistribute < 0) {
                    // If the parent clips content in this axis direction, don't compress children, just leave them alone
                    if (((xAxis && parent->config.clip.horizontal) || (!xAxis && parent->config.clip.vertical))) {
                        continue;
                    }
                    // Scrolling containers preferentially compress before others
                    while (sizeToDistribute < -CLAY__EPSILON && resizableContainerBuffer.length > 0) {
                        float largest = 0;
                        float secondLargest = 0;
                        float widthToAdd = sizeToDistribute;
                        for (int childIndex = 0; childIndex < resizableContainerBuffer.length; childIndex++) {
                            Clay_LayoutElement *child = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&resizableContainerBuffer, childIndex));
                            float childSize = xAxis ? child->dimensions.width : child->dimensions.height;
                            if (Clay__FloatEqual(childSize, largest)) { continue; }
                            if (childSize > largest) {
                                secondLargest = largest;
                                largest = childSize;
                            }
                            if (childSize < largest) {
                                secondLargest = CLAY__MAX(secondLargest, childSize);
                                widthToAdd = secondLargest - largest;
                            }
                        }

                        widthToAdd = CLAY__MAX(widthToAdd, sizeToDistribute / resizableContainerBuffer.length);

                        for (int childIndex = 0; childIndex < resizableContainerBuffer.length; childIndex++) {
                            Clay_LayoutElement *child = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&resizableContainerBuffer, childIndex));
                            float *childSize = xAxis ? &child->dimensions.width : &child->dimensions.height;
                            float minSize = xAxis ? child->minDimensions.width : child->minDimensions.height;
                            float previousWidth = *childSize;
                            if (Clay__FloatEqual(*childSize, largest)) {
                                *childSize += widthToAdd;
                                if (*childSize <= minSize) {
                                    *childSize = minSize;
                                    Clay__int32_tArray_RemoveSwapback(&resizableContainerBuffer, childIndex--);
                                }
                                sizeToDistribute -= (*childSize - previousWidth);
                            }
                        }
                    }
                // The content is too small, allow SIZING_GROW containers to expand
                } else if (sizeToDistribute > 0 && growContainerCount > 0) {
                    for (int childIndex = 0; childIndex < resizableContainerBuffer.length; childIndex++) {
                        Clay_LayoutElement *child = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&resizableContainerBuffer, childIndex));
                        Clay__SizingType childSizing = Clay__GetElementSizing(child, xAxis).type;
                        if (childSizing != CLAY__SIZING_TYPE_GROW) {
                            Clay__int32_tArray_RemoveSwapback(&resizableContainerBuffer, childIndex--);
                        }
                    }
                    while (sizeToDistribute > CLAY__EPSILON && resizableContainerBuffer.length > 0) {
                        float smallest = CLAY__MAXFLOAT;
                        float secondSmallest = CLAY__MAXFLOAT;
                        float widthToAdd = sizeToDistribute;
                        for (int childIndex = 0; childIndex < resizableContainerBuffer.length; childIndex++) {
                            Clay_LayoutElement *child = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&resizableContainerBuffer, childIndex));
                            float childSize = xAxis ? child->dimensions.width : child->dimensions.height;
                            if (Clay__FloatEqual(childSize, smallest)) { continue; }
                            if (childSize < smallest) {
                                secondSmallest = smallest;
                                smallest = childSize;
                            }
                            if (childSize > smallest) {
                                secondSmallest = CLAY__MIN(secondSmallest, childSize);
                                widthToAdd = secondSmallest - smallest;
                            }
                        }

                        widthToAdd = CLAY__MIN(widthToAdd, sizeToDistribute / resizableContainerBuffer.length);

                        for (int childIndex = 0; childIndex < resizableContainerBuffer.length; childIndex++) {
                            Clay_LayoutElement *child = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&resizableContainerBuffer, childIndex));
                            float *childSize = xAxis ? &child->dimensions.width : &child->dimensions.height;
                            Clay_SizingAxis childSizing = Clay__GetElementSizing(child, xAxis);
                            float maxSize = childSizing.size.minMax.max;
                            float previousWidth = *childSize;
                            if (Clay__FloatEqual(*childSize, smallest)) {
                                *childSize += widthToAdd;
                                if (*childSize >= maxSize) {
                                    *childSize = maxSize;
                                    Clay__int32_tArray_RemoveSwapback(&resizableContainerBuffer, childIndex--);
                                }
                                sizeToDistribute -= (*childSize - previousWidth);
                            }
                        }
                    }
                }
            // Sizing along the non layout axis ("off axis")
            } else {
                for (int32_t childOffset = 0; childOffset < resizableContainerBuffer.length; childOffset++) {
                    Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&resizableContainerBuffer, childOffset));
                    Clay_SizingAxis childSizing = Clay__GetElementSizing(childElement, xAxis);
                    float minSize = xAxis ? childElement->minDimensions.width : childElement->minDimensions.height;
                    float *childSize = xAxis ? &childElement->dimensions.width : &childElement->dimensions.height;

                    float maxSize = parentSize - parentPadding;
                    // If we're laying out the children of a scroll panel, grow containers expand to the size of the inner content, not the outer container
                    if (((xAxis && parent->config.clip.horizontal) || (!xAxis && parent->config.clip.vertical))) {
                        maxSize = CLAY__MAX(maxSize, innerContentSize);
                    }
                    if (childSizing.type == CLAY__SIZING_TYPE_GROW) {
                        *childSize = CLAY__MIN(maxSize, childSizing.size.minMax.max);
                    }
                    *childSize = CLAY__MAX(minSize, CLAY__MIN(*childSize, maxSize));
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

void Clay__CalculateFinalLayout(float deltaTime, bool useStoredBoundingBoxes, bool generateRenderCommands) {
    Clay_Context* context = Clay_GetCurrentContext();

    // Calculate sizing along the X axis
    Clay__int32_tArray textElements = context->openClipElementStack;
    textElements.length = 0;
    Clay__int32_tArray aspectRatioElements = context->reusableElementIndexBuffer;
    aspectRatioElements.length = 0;
    Clay__SizeContainersAlongAxis(true, deltaTime, &textElements, &aspectRatioElements);

    // Wrap text
    for (int32_t textElementIndex = 0; textElementIndex < textElements.length; ++textElementIndex) {
        Clay_LayoutElement *element = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&textElements, textElementIndex));
        Clay__TextElementData *textElementData = &element->textElementData;
        textElementData->wrappedLines = CLAY__INIT(Clay__WrappedTextLineArraySlice) { .length = 0, .internalArray = &context->wrappedTextLines.internalArray[context->wrappedTextLines.length] };
        Clay_LayoutElement *containerElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&textElements, textElementIndex));
        Clay__MeasureTextCacheItem *measureTextCacheItem = Clay__MeasureTextCached(&textElementData->text, &containerElement->textConfig);
        float lineWidth = 0;
        float lineHeight = containerElement->textConfig.lineHeight > 0 ? (float)containerElement->textConfig.lineHeight : textElementData->preferredDimensions.height;
        int32_t lineLengthChars = 0;
        int32_t lineStartOffset = 0;
        if (!measureTextCacheItem->containsNewlines && textElementData->preferredDimensions.width <= containerElement->dimensions.width) {
            Clay__WrappedTextLineArray_Add(&context->wrappedTextLines, CLAY__INIT(Clay__WrappedTextLine) { containerElement->dimensions,  textElementData->text });
            textElementData->wrappedLines.length++;
            continue;
        }
        float spaceWidth = Clay__MeasureText(CLAY__INIT(Clay_StringSlice) { .length = 1, .chars = CLAY__SPACECHAR.chars, .baseChars = CLAY__SPACECHAR.chars }, &containerElement->textConfig, context->measureTextUserData).width;
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
                bool finalCharIsSpace = textElementData->text.chars[CLAY__MAX(lineStartOffset + lineLengthChars - 1, 0)] == ' ';
                Clay__WrappedTextLineArray_Add(&context->wrappedTextLines, CLAY__INIT(Clay__WrappedTextLine) { { lineWidth + (finalCharIsSpace ? -spaceWidth : 0), lineHeight }, { .length = lineLengthChars + (finalCharIsSpace ? -1 : 0), .chars = &textElementData->text.chars[lineStartOffset] } });
                textElementData->wrappedLines.length++;
                if (lineLengthChars == 0 || measuredWord->length == 0) {
                    wordIndex = measuredWord->next;
                }
                lineWidth = 0;
                lineLengthChars = 0;
                lineStartOffset = measuredWord->startOffset;
            } else {
                lineWidth += measuredWord->width + containerElement->textConfig.letterSpacing;
                lineLengthChars += measuredWord->length;
                wordIndex = measuredWord->next;
            }
        }
        if (lineLengthChars > 0) {
            Clay__WrappedTextLineArray_Add(&context->wrappedTextLines, CLAY__INIT(Clay__WrappedTextLine) { { lineWidth - containerElement->textConfig.letterSpacing, lineHeight }, {.length = lineLengthChars, .chars = &textElementData->text.chars[lineStartOffset] } });
            textElementData->wrappedLines.length++;
        }
        containerElement->dimensions.height = lineHeight * (float)textElementData->wrappedLines.length;
    }

    // Scale vertical heights according to aspect ratio
    for (int32_t i = 0; i < aspectRatioElements.length; ++i) {
        Clay_LayoutElement* aspectElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&aspectRatioElements, i));
        aspectElement->dimensions.height = (1 / aspectElement->config.aspectRatio.aspectRatio) * aspectElement->dimensions.width;
        aspectElement->config.layout.sizing.height.size.minMax.max = aspectElement->dimensions.height;
    }

    // Propagate effect of text wrapping, aspect scaling etc. on height of parents
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
            if (currentElement->isTextElement || currentElement->children.length == 0) {
                dfsBuffer.length--;
                continue;
            }
            // Add the children to the DFS buffer (needs to be pushed in reverse so that stack traversal is in correct layout order)
            for (int32_t i = 0; i < currentElement->children.length; i++) {
                context->treeNodeVisited.internalArray[dfsBuffer.length] = false;
                Clay__LayoutElementTreeNodeArray_Add(&dfsBuffer, CLAY__INIT(Clay__LayoutElementTreeNode) { .layoutElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->children.elements[i]) });
            }
            continue;
        }
        dfsBuffer.length--;

        // DFS node has been visited, this is on the way back up to the root
        Clay_LayoutConfig *layoutConfig = &currentElement->config.layout;
        if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
            // Resize any parent containers that have grown in height along their non layout axis
            for (int32_t j = 0; j < currentElement->children.length; ++j) {
                Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->children.elements[j]);
                float childHeightWithPadding = CLAY__MAX(childElement->dimensions.height + layoutConfig->padding.top + layoutConfig->padding.bottom, currentElement->dimensions.height);
                currentElement->dimensions.height = CLAY__MIN(CLAY__MAX(childHeightWithPadding, layoutConfig->sizing.height.size.minMax.min), layoutConfig->sizing.height.size.minMax.max);
            }
        } else if (layoutConfig->layoutDirection == CLAY_TOP_TO_BOTTOM) {
            // Resizing along the layout axis
            float contentHeight = (float)(layoutConfig->padding.top + layoutConfig->padding.bottom);
            for (int32_t j = 0; j < currentElement->children.length; ++j) {
                Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->children.elements[j]);
                contentHeight += childElement->dimensions.height;
            }
            contentHeight += (float)(CLAY__MAX(currentElement->children.length - 1, 0) * layoutConfig->childGap);
            currentElement->dimensions.height = CLAY__MIN(CLAY__MAX(contentHeight, layoutConfig->sizing.height.size.minMax.min), layoutConfig->sizing.height.size.minMax.max);
        }
    }

    // Calculate sizing along the Y axis
    Clay__SizeContainersAlongAxis(false, deltaTime, NULL, NULL);

    // Scale horizontal widths according to aspect ratio
    for (int32_t i = 0; i < aspectRatioElements.length; ++i) {
        Clay_LayoutElement* aspectElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&aspectRatioElements, i));
        aspectElement->dimensions.width = aspectElement->config.aspectRatio.aspectRatio * aspectElement->dimensions.height;
    }

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
        if (rootElement->config.floating.attachTo != CLAY_ATTACH_TO_NONE && parentHashMapItem) {
            Clay_FloatingElementConfig *config = &rootElement->config.floating;
            Clay_Dimensions rootDimensions = rootElement->dimensions;
            Clay_BoundingBox parentBoundingBox = parentHashMapItem->boundingBox;
            // Set X position
            Clay_Vector2 targetAttachPosition = CLAY__DEFAULT_STRUCT;
            switch (config->attachPoints.parent) {
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
            switch (config->attachPoints.element) {
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
            switch (config->attachPoints.parent) { // I know I could merge the x and y switch statements, but this is easier to read
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
            switch (config->attachPoints.element) {
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
            if (clipHashMapItem && !Clay__ElementIsOffscreen(&clipHashMapItem->boundingBox)) {
                // Floating elements that are attached to scrolling contents won't be correctly positioned if external scroll handling is enabled, fix here
                if (context->externalScrollHandlingEnabled) {
                    if (clipHashMapItem->layoutElement->config.clip.horizontal) {
                        rootPosition.x += clipHashMapItem->layoutElement->config.clip.childOffset.x;
                    }
                    if (clipHashMapItem->layoutElement->config.clip.vertical) {
                        rootPosition.y += clipHashMapItem->layoutElement->config.clip.childOffset.y;
                    }
                }
                if (generateRenderCommands) {
                    Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                        .boundingBox = clipHashMapItem->boundingBox,
                        .userData = 0,
                        .id = Clay__HashNumber(rootElement->id, rootElement->children.length + 10).id, // TODO need a better strategy for managing derived ids
                        .zIndex = root->zIndex,
                        .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_START,
                    });
                }
            }
        }
        Clay__LayoutElementTreeNodeArray_Add(&dfsBuffer, CLAY__INIT(Clay__LayoutElementTreeNode) { .layoutElement = rootElement, .position = rootPosition, .nextChildOffset = { .x = (float)rootElement->config.layout.padding.left, .y = (float)rootElement->config.layout.padding.top } });

        context->treeNodeVisited.internalArray[0] = false;
        while (dfsBuffer.length > 0) {
            Clay__LayoutElementTreeNode *currentElementTreeNode = Clay__LayoutElementTreeNodeArray_Get(&dfsBuffer, (int)dfsBuffer.length - 1);
            Clay_LayoutElement *currentElement = currentElementTreeNode->layoutElement;
            Clay_LayoutConfig *layoutConfig = currentElement->isTextElement ? &CLAY_LAYOUT_DEFAULT : &currentElement->config.layout;
            Clay_Vector2 scrollOffset = CLAY__DEFAULT_STRUCT;

            // DFS is returning back upwards
            if (context->treeNodeVisited.internalArray[dfsBuffer.length - 1]) {
                if (currentElement->isTextElement) {
                    dfsBuffer.length--;
                    continue;
                }
                Clay_LayoutElementHashMapItem *currentElementData = Clay__GetHashMapItem(currentElement->id);
                if (generateRenderCommands && !Clay__ElementIsOffscreen(&currentElementData->boundingBox)) {
                    // DFS is returning upwards backwards
                    bool closeClipElement = false;
                    if (currentElement->config.clip.horizontal || currentElement->config.clip.vertical) {
                        closeClipElement = true;
                        for (int32_t i = 0; i < context->scrollContainerDatas.length; i++) {
                            Clay__ScrollContainerDataInternal *mapping = Clay__ScrollContainerDataInternalArray_Get(&context->scrollContainerDatas, i);
                            if (mapping->layoutElement == currentElement) {
                                scrollOffset = currentElement->config.clip.childOffset;
                                if (context->externalScrollHandlingEnabled) {
                                    scrollOffset = CLAY__INIT(Clay_Vector2) CLAY__DEFAULT_STRUCT;
                                }
                                break;
                            }
                        }
                    }

                    if (Clay__BorderHasAnyWidth(&currentElement->config.border)) {
                        Clay_BoundingBox currentElementBoundingBox = currentElementData->boundingBox;
                        Clay_BorderElementConfig *borderConfig = &currentElement->config.border;
                        Clay_RenderCommand renderCommand = {
                            .boundingBox = currentElementBoundingBox,
                            .renderData = { .border = {
                                .color = borderConfig->color,
                                .cornerRadius = currentElement->config.cornerRadius,
                                .width = borderConfig->width
                            }},
                            .userData = currentElement->config.userData,
                            .id = Clay__HashNumber(currentElement->id, currentElement->children.length).id,
                            .commandType = CLAY_RENDER_COMMAND_TYPE_BORDER,
                        };
                        Clay__AddRenderCommand(renderCommand);
                        if (borderConfig->width.betweenChildren > 0 && borderConfig->color.a > 0) {
                            float halfGap = layoutConfig->childGap / 2;
                            float halfWidth = borderConfig->width.betweenChildren / 2;
                            Clay_Vector2 borderOffset = { (float)layoutConfig->padding.left - halfGap, (float)layoutConfig->padding.top - halfGap };
                            if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                                for (int32_t i = 0; i < currentElement->children.length; ++i) {
                                    Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->children.elements[i]);
                                    if (i > 0) {
                                        Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                                                .boundingBox = { currentElementBoundingBox.x + borderOffset.x + scrollOffset.x - halfWidth, currentElementBoundingBox.y + scrollOffset.y, (float)borderConfig->width.betweenChildren, currentElement->dimensions.height },
                                                .renderData = { .rectangle = {
                                                        .backgroundColor = borderConfig->color,
                                                } },
                                                .userData = currentElement->config.userData,
                                                .id = Clay__HashNumber(currentElement->id, currentElement->children.length + 1 + i).id,
                                                .commandType = CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
                                        });
                                    }
                                    borderOffset.x += (childElement->dimensions.width + (float)layoutConfig->childGap);
                                }
                            } else {
                                for (int32_t i = 0; i < currentElement->children.length; ++i) {
                                    Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->children.elements[i]);
                                    if (i > 0) {
                                        Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                                                .boundingBox = { currentElementBoundingBox.x + scrollOffset.x, currentElementBoundingBox.y + borderOffset.y + scrollOffset.y - halfWidth, currentElement->dimensions.width, (float)borderConfig->width.betweenChildren },
                                                .renderData = { .rectangle = {
                                                        .backgroundColor = borderConfig->color,
                                                } },
                                                .userData = currentElement->config.userData,
                                                .id = Clay__HashNumber(currentElement->id, currentElement->children.length + 1 + i).id,
                                                .commandType = CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
                                        });
                                    }
                                    borderOffset.y += (childElement->dimensions.height + (float)layoutConfig->childGap);
                                }
                            }
                        }
                    }
                    if (currentElement->config.overlayColor.a > 0) {
                        Clay_RenderCommand renderCommand = {
                                .userData = currentElement->config.userData,
                                .id = currentElement->id,
                                .zIndex = root->zIndex,
                                .commandType = CLAY_RENDER_COMMAND_TYPE_OVERLAY_COLOR_END,
                        };
                        Clay__AddRenderCommand(renderCommand);
                    }
                    // This exists because the scissor needs to end _after_ borders between elements
                    if (closeClipElement) {
                        Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                                .id = Clay__HashNumber(currentElement->id, rootElement->children.length + 11).id,
                                .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_END,
                        });
                    }
                }

                dfsBuffer.length--;
                continue;
            }

            // This will only be run a single time for each element in downwards DFS order
            context->treeNodeVisited.internalArray[dfsBuffer.length - 1] = true;
            Clay_BoundingBox currentElementBoundingBox = { currentElementTreeNode->position.x, currentElementTreeNode->position.y, currentElement->dimensions.width, currentElement->dimensions.height };
            Clay__ScrollContainerDataInternal *scrollContainerData = CLAY__NULL;
            if (!currentElement->isTextElement) {
                if (useStoredBoundingBoxes && currentElement->config.transition.handler) {
                    bool found = false;
                    for (int j = 0; j < context->transitionDatas.length; ++j) {
                        Clay__TransitionDataInternal* transitionData = Clay__TransitionDataInternalArray_Get(&context->transitionDatas, j);
                        if (transitionData->elementId == currentElement->id) {
                            found = true;
                            if (transitionData->state != CLAY_TRANSITION_STATE_IDLE) {
                                if ((transitionData->activeProperties & CLAY_TRANSITION_PROPERTY_X) != 0) currentElementBoundingBox.x = transitionData->currentState.boundingBox.x;
                                if ((transitionData->activeProperties & CLAY_TRANSITION_PROPERTY_Y) != 0) currentElementBoundingBox.y = transitionData->currentState.boundingBox.y;
                                if ((transitionData->activeProperties & CLAY_TRANSITION_PROPERTY_WIDTH) != 0) currentElementBoundingBox.width = transitionData->currentState.boundingBox.width;
                                if ((transitionData->activeProperties & CLAY_TRANSITION_PROPERTY_HEIGHT) != 0) currentElementBoundingBox.height = transitionData->currentState.boundingBox.height;
                            }
                            break;
                        }
                    }
                    // An exiting element that completed its transition this frame - skip tree
                    if (!found && currentElement->config.transition.exit.setFinalState) {
                        dfsBuffer.length--;
                        continue;
                    }
                }
                if (currentElement->config.floating.attachTo != CLAY_ATTACH_TO_NONE) {
                    Clay_FloatingElementConfig *floatingElementConfig = &currentElement->config.floating;
                    Clay_Dimensions expand = floatingElementConfig->expand;
                    currentElementBoundingBox.x -= expand.width;
                    currentElementBoundingBox.width += expand.width * 2;
                    currentElementBoundingBox.y -= expand.height;
                    currentElementBoundingBox.height += expand.height * 2;
                }

                // Apply scroll offsets to container
                if (currentElement->config.clip.horizontal || currentElement->config.clip.vertical) {
                    // This linear scan could theoretically be slow under very strange conditions, but I can't imagine a real UI with more than a few 10's of scroll containers
                    for (int32_t i = 0; i < context->scrollContainerDatas.length; i++) {
                        Clay__ScrollContainerDataInternal *mapping = Clay__ScrollContainerDataInternalArray_Get(&context->scrollContainerDatas, i);
                        if (mapping->layoutElement == currentElement) {
                            scrollContainerData = mapping;
                            mapping->boundingBox = currentElementBoundingBox;
                            scrollOffset = currentElement->config.clip.childOffset;
                            if (context->externalScrollHandlingEnabled) {
                                scrollOffset = CLAY__INIT(Clay_Vector2) CLAY__DEFAULT_STRUCT;
                            }
                            break;
                        }
                    }
                }
            }

            bool offscreen = Clay__ElementIsOffscreen(&currentElementBoundingBox);

            // Generate render commands for current element
            if (generateRenderCommands && !offscreen) {
                if (currentElement->isTextElement) {
                    Clay_TextElementConfig *textElementConfig = &currentElement->textConfig;
                    float naturalLineHeight = currentElement->textElementData.preferredDimensions.height;
                    float finalLineHeight = textElementConfig->lineHeight > 0 ? (float)textElementConfig->lineHeight : naturalLineHeight;
                    float lineHeightOffset = (finalLineHeight - naturalLineHeight) / 2;
                    float yPosition = lineHeightOffset;
                    for (int32_t lineIndex = 0; lineIndex < currentElement->textElementData.wrappedLines.length; ++lineIndex) {
                        Clay__WrappedTextLine *wrappedLine = Clay__WrappedTextLineArraySlice_Get(&currentElement->textElementData.wrappedLines, lineIndex);
                        if (wrappedLine->line.length == 0) {
                            yPosition += finalLineHeight;
                            continue;
                        }
                        float offset = (currentElementBoundingBox.width - wrappedLine->dimensions.width);
                        if (textElementConfig->textAlignment == CLAY_TEXT_ALIGN_LEFT) {
                            offset = 0;
                        }
                        if (textElementConfig->textAlignment == CLAY_TEXT_ALIGN_CENTER) {
                            offset /= 2;
                        }
                        Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                            .boundingBox = { currentElementBoundingBox.x + offset, currentElementBoundingBox.y + yPosition, wrappedLine->dimensions.width, wrappedLine->dimensions.height },
                            .renderData = { .text = {
                                .stringContents = CLAY__INIT(Clay_StringSlice) { .length = wrappedLine->line.length, .chars = wrappedLine->line.chars, .baseChars = currentElement->textElementData.text.chars },
                                .textColor = textElementConfig->textColor,
                                .fontId = textElementConfig->fontId,
                                .fontSize = textElementConfig->fontSize,
                                .letterSpacing = textElementConfig->letterSpacing,
                                .lineHeight = textElementConfig->lineHeight,
                            }},
                            .userData = textElementConfig->userData,
                            .id = Clay__HashNumber(lineIndex, currentElement->id).id,
                            .zIndex = root->zIndex,
                            .commandType = CLAY_RENDER_COMMAND_TYPE_TEXT,
                        });
                        yPosition += finalLineHeight;

                        if (!context->disableCulling && (currentElementBoundingBox.y + yPosition > context->layoutDimensions.height)) {
                            break;
                        }
                    }
                } else {
                    if (currentElement->config.overlayColor.a > 0) {
                        Clay_RenderCommand renderCommand = {
                            .renderData = {
                                .overlayColor = { .color = currentElement->config.overlayColor }
                            },
                            .userData = currentElement->config.userData,
                            .id = currentElement->id,
                            .zIndex = root->zIndex,
                            .commandType = CLAY_RENDER_COMMAND_TYPE_OVERLAY_COLOR_START,
                        };
                        Clay__AddRenderCommand(renderCommand);
                    }
                    if (currentElement->config.image.imageData) {
                        Clay_RenderCommand renderCommand = {
                            .boundingBox = currentElementBoundingBox,
                            .renderData = {
                                .image = {
                                    .backgroundColor = currentElement->config.backgroundColor,
                                    .cornerRadius = currentElement->config.cornerRadius,
                                    .imageData = currentElement->config.image.imageData,
                                }
                            },
                            .userData = currentElement->config.userData,
                            .id = currentElement->id,
                            .zIndex = root->zIndex,
                                .commandType = CLAY_RENDER_COMMAND_TYPE_IMAGE,
                        };
                        Clay__AddRenderCommand(renderCommand);
                    }
                    if (currentElement->config.custom.customData) {
                        Clay_RenderCommand renderCommand = {
                            .boundingBox = currentElementBoundingBox,
                            .renderData = {
                                .custom = {
                                    .backgroundColor = currentElement->config.backgroundColor,
                                    .cornerRadius = currentElement->config.cornerRadius,
                                    .customData = currentElement->config.custom.customData,
                                }
                            },
                            .userData = currentElement->config.userData,
                            .id = currentElement->id,
                            .zIndex = root->zIndex,
                            .commandType = CLAY_RENDER_COMMAND_TYPE_CUSTOM,
                        };
                        Clay__AddRenderCommand(renderCommand);
                    }
                    if (currentElement->config.clip.horizontal || currentElement->config.clip.vertical) {
                        Clay_RenderCommand renderCommand = {
                            .boundingBox = currentElementBoundingBox,
                            .renderData = {
                                .clip = {
                                    .horizontal = currentElement->config.clip.horizontal,
                                    .vertical = currentElement->config.clip.vertical,
                                }
                            },
                            .userData = currentElement->config.userData,
                            .id = currentElement->id,
                            .zIndex = root->zIndex,
                            .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_START,
                        };
                        Clay__AddRenderCommand(renderCommand);
                    }
                    if (currentElement->config.backgroundColor.a > 0) {
                        Clay_RenderCommand renderCommand = {
                            .boundingBox = currentElementBoundingBox,
                            .renderData = { .rectangle = {
                                .backgroundColor = currentElement->config.backgroundColor,
                                .cornerRadius = currentElement->config.cornerRadius,
                            } },
                            .userData = currentElement->config.userData,
                            .id = currentElement->id,
                            .zIndex = root->zIndex,
                            .commandType = CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
                        };
                        Clay__AddRenderCommand(renderCommand);
                    }
                }
            }

            Clay_LayoutElementHashMapItem *hashMapItem = Clay__GetHashMapItem(currentElement->id);
            hashMapItem->boundingBox = currentElementBoundingBox;

            if (currentElement->isTextElement) continue;

            // Setup positions for child elements and add to DFS buffer ----------

            // On-axis alignment
            Clay_Dimensions contentSizeCurrent = {};
            if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                for (int32_t i = 0; i < currentElement->children.length; ++i) {
                    Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->children.elements[i]);
                    if (childElement->exiting) continue;
                    contentSizeCurrent.width += childElement->dimensions.width;
                    contentSizeCurrent.height = CLAY__MAX(contentSizeCurrent.height, childElement->dimensions.height);
                }
                contentSizeCurrent.width += (float)(CLAY__MAX(currentElement->children.length - 1, 0) * layoutConfig->childGap);
                float extraSpace = currentElement->dimensions.width - (float)(layoutConfig->padding.left + layoutConfig->padding.right) - contentSizeCurrent.width;
                switch (layoutConfig->childAlignment.x) {
                    case CLAY_ALIGN_X_LEFT: extraSpace = 0; break;
                    case CLAY_ALIGN_X_CENTER: extraSpace /= 2; break;
                    default: break;
                }
                extraSpace = CLAY__MAX(0, extraSpace);
                currentElementTreeNode->nextChildOffset.x += extraSpace;
            } else if (layoutConfig->layoutDirection == CLAY_TOP_TO_BOTTOM) {
                for (int32_t i = 0; i < currentElement->children.length; ++i) {
                    Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->children.elements[i]);
                    if (childElement->exiting) continue;
                    contentSizeCurrent.width = CLAY__MAX(contentSizeCurrent.width, childElement->dimensions.width);
                    contentSizeCurrent.height += childElement->dimensions.height;
                }
                contentSizeCurrent.height += (float)(CLAY__MAX(currentElement->children.length - 1, 0) * layoutConfig->childGap);
                float extraSpace = currentElement->dimensions.height - (float)(layoutConfig->padding.top + layoutConfig->padding.bottom) - contentSizeCurrent.height;
                switch (layoutConfig->childAlignment.y) {
                    case CLAY_ALIGN_Y_TOP: extraSpace = 0; break;
                    case CLAY_ALIGN_Y_CENTER: extraSpace /= 2; break;
                    default: break;
                }
                extraSpace = CLAY__MAX(0, extraSpace);
                currentElementTreeNode->nextChildOffset.y += extraSpace;
            }

            if (scrollContainerData) {
                scrollContainerData->contentSize = CLAY__INIT(Clay_Dimensions) {contentSizeCurrent.width + (float)(layoutConfig->padding.left + layoutConfig->padding.right), contentSizeCurrent.height + (float)(layoutConfig->padding.top + layoutConfig->padding.bottom) };
            }

            // Add children to the DFS buffer
            dfsBuffer.length += currentElement->children.length;
            for (int32_t i = 0; i < currentElement->children.length; ++i) {
                Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->children.elements[i]);
                Clay_LayoutElementHashMapItem* childMapItem = Clay__GetHashMapItem(childElement->id);
                // Alignment along non layout axis
                if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                    currentElementTreeNode->nextChildOffset.y = currentElement->config.layout.padding.top;
                    float whiteSpaceAroundChild = currentElement->dimensions.height - (float)(layoutConfig->padding.top + layoutConfig->padding.bottom) - childElement->dimensions.height;
                    switch (layoutConfig->childAlignment.y) {
                        case CLAY_ALIGN_Y_TOP: break;
                        case CLAY_ALIGN_Y_CENTER: currentElementTreeNode->nextChildOffset.y += whiteSpaceAroundChild / 2; break;
                        case CLAY_ALIGN_Y_BOTTOM: currentElementTreeNode->nextChildOffset.y += whiteSpaceAroundChild; break;
                    }
                } else {
                    currentElementTreeNode->nextChildOffset.x = currentElement->config.layout.padding.left;
                    float whiteSpaceAroundChild = currentElement->dimensions.width - (float)(layoutConfig->padding.left + layoutConfig->padding.right) - childElement->dimensions.width;
                    switch (layoutConfig->childAlignment.x) {
                        case CLAY_ALIGN_X_LEFT: break;
                        case CLAY_ALIGN_X_CENTER: currentElementTreeNode->nextChildOffset.x += whiteSpaceAroundChild / 2; break;
                        case CLAY_ALIGN_X_RIGHT: currentElementTreeNode->nextChildOffset.x += whiteSpaceAroundChild; break;
                    }
                }

                Clay_Vector2 childPosition = {
                    currentElementBoundingBox.x + currentElementTreeNode->nextChildOffset.x + scrollOffset.x,
                    currentElementBoundingBox.y + currentElementTreeNode->nextChildOffset.y + scrollOffset.y,
                };

                // DFS buffer elements need to be added in reverse because stack traversal happens backwards
                uint32_t newNodeIndex = dfsBuffer.length - 1 - i;
                dfsBuffer.internalArray[newNodeIndex] = CLAY__INIT(Clay__LayoutElementTreeNode) {
                    .layoutElement = childElement,
                    .position = CLAY__INIT(Clay_Vector2) { childPosition.x, childPosition.y },
                    .nextChildOffset = { .x = (float)childElement->config.layout.padding.left, .y = (float)childElement->config.layout.padding.top },
                };
                context->treeNodeVisited.internalArray[newNodeIndex] = false;

                // Update parent offsets
                if (!childElement->exiting) {
                    if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                        currentElementTreeNode->nextChildOffset.x += childElement->dimensions.width + (float)layoutConfig->childGap;
                    } else {
                        currentElementTreeNode->nextChildOffset.y += childElement->dimensions.height + (float)layoutConfig->childGap;
                    }
                }
            }
        }

        if (root->clipElementId) {
            Clay_LayoutElementHashMapItem *clipHashMapItem = Clay__GetHashMapItem(root->clipElementId);
            if (clipHashMapItem && !Clay__ElementIsOffscreen(&clipHashMapItem->boundingBox)) {
                Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) { .id = Clay__HashNumber(rootElement->id, rootElement->children.length + 11).id, .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_END });
            }
        }
    }
}

CLAY_WASM_EXPORT("Clay_GetPointerOverIds")
CLAY_DLL_EXPORT Clay_ElementIdArray Clay_GetPointerOverIds(void) {
    return Clay_GetCurrentContext()->pointerOverIds;
}

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

typedef struct {
    Clay_String label;
    Clay_Color color;
} Clay__DebugElementConfigTypeLabelConfig;

typedef enum {
    CLAY__ELEMENT_CONFIG_TYPE_BACKGROUND_COLOR,
    CLAY__ELEMENT_CONFIG_TYPE_OVERLAY_COLOR,
    CLAY__ELEMENT_CONFIG_TYPE_CORNER_RADIUS,
    CLAY__ELEMENT_CONFIG_TYPE_TEXT,
    CLAY__ELEMENT_CONFIG_TYPE_ASPECT,
    CLAY__ELEMENT_CONFIG_TYPE_IMAGE,
    CLAY__ELEMENT_CONFIG_TYPE_FLOATING,
    CLAY__ELEMENT_CONFIG_TYPE_CLIP,
    CLAY__ELEMENT_CONFIG_TYPE_BORDER,
    CLAY__ELEMENT_CONFIG_TYPE_CUSTOM,
} Clay__DebugElementConfigType;

Clay__DebugElementConfigTypeLabelConfig Clay__DebugGetElementConfigTypeLabel(Clay__DebugElementConfigType type) {
    switch (type) {
        case CLAY__ELEMENT_CONFIG_TYPE_BACKGROUND_COLOR: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Background"), {243,134,48,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_OVERLAY_COLOR: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Overlay"), { 142,129,206, 255} };
        case CLAY__ELEMENT_CONFIG_TYPE_CORNER_RADIUS: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) {CLAY_STRING("Radius"), {239,148,157, 255 } };
        case CLAY__ELEMENT_CONFIG_TYPE_TEXT: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Text"), {105,210,231,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_ASPECT: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Aspect"), {101,149,194,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_IMAGE: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Image"), {121,189,154,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_FLOATING: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Floating"), {250,105,0,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_CLIP: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) {CLAY_STRING("Scroll"), {242, 196, 90, 255} };
        case CLAY__ELEMENT_CONFIG_TYPE_BORDER: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) {CLAY_STRING("Border"), {108, 91, 123, 255} };
        case CLAY__ELEMENT_CONFIG_TYPE_CUSTOM: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Custom"), {11,72,107,255} };
        default: break;
    }
    return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Error"), {0,0,0,255} };
}

void Clay__RenderElementConfigTypeLabel(Clay_String label, Clay_Color color, bool offscreen) {
    Clay_Color backgroundColor = color;
    backgroundColor.a = 90;
    CLAY_AUTO_ID({ .layout = { .padding = { 8, 8, 2, 2 } }, .backgroundColor = backgroundColor, .cornerRadius = CLAY_CORNER_RADIUS(4), .border = { .color = color, .width = { 1, 1, 1, 1, 0 } } }) {
        CLAY_TEXT(label, CLAY_TEXT_CONFIG({ .textColor = offscreen ? CLAY__DEBUGVIEW_COLOR_3 : CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
    }
}

typedef struct {
    int32_t rowCount;
    int32_t selectedElementRowIndex;
} Clay__RenderDebugLayoutData;

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
            CLAY(CLAY_IDI("Clay__DebugView_EmptyRowOuter", rootIndex), { .layout = { .sizing = {.width = CLAY_SIZING_GROW(0)}, .padding = {CLAY__DEBUGVIEW_INDENT_WIDTH / 2, 0, 0, 0} } }) {
                CLAY(CLAY_IDI("Clay__DebugView_EmptyRow", rootIndex), { .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED((float)CLAY__DEBUGVIEW_ROW_HEIGHT) }}, .border = { .color = CLAY__DEBUGVIEW_COLOR_3, .width = { .top = 1 } } }) {}
            }
            layoutData.rowCount++;
        }
        while (dfsBuffer.length > 0) {
            int32_t currentElementIndex = Clay__int32_tArray_GetValue(&dfsBuffer, (int)dfsBuffer.length - 1);
            Clay_LayoutElement *currentElement = Clay_LayoutElementArray_Get(&context->layoutElements, (int)currentElementIndex);
            if (context->treeNodeVisited.internalArray[dfsBuffer.length - 1]) {
                if (!currentElement->isTextElement && currentElement->children.length > 0) {
                    Clay__CloseElement();
                    Clay__CloseElement();
                    Clay__CloseElement();
                }
                dfsBuffer.length--;
                continue;
            }

            if (currentElement->exiting) { // TODO there is a duplicate ID problem with exiting elements
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
            CLAY(CLAY_IDI("Clay__DebugView_ElementOuter", currentElement->id), { .layout = Clay__DebugView_ScrollViewItemLayoutConfig }) {
                // Collapse icon / button
                if (!(currentElement->isTextElement || currentElement->children.length == 0)) {
                    CLAY(CLAY_IDI("Clay__DebugView_CollapseElement", currentElement->id), {
                        .layout = { .sizing = {CLAY_SIZING_FIXED(16), CLAY_SIZING_FIXED(16)}, .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER} },
                        .cornerRadius = CLAY_CORNER_RADIUS(4),
                        .border = { .color = CLAY__DEBUGVIEW_COLOR_3, .width = {1, 1, 1, 1, 0} },
                    }) {
                        CLAY_TEXT((currentElementData && currentElementData->debugData.collapsed) ? CLAY_STRING("+") : CLAY_STRING("-"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
                    }
                } else { // Square dot for empty containers
                    CLAY_AUTO_ID({ .layout = { .sizing = {CLAY_SIZING_FIXED(16), CLAY_SIZING_FIXED(16)}, .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER } } }) {
                        CLAY_AUTO_ID({ .layout = { .sizing = {CLAY_SIZING_FIXED(8), CLAY_SIZING_FIXED(8)} }, .backgroundColor = CLAY__DEBUGVIEW_COLOR_3, .cornerRadius = CLAY_CORNER_RADIUS(2) }) {}
                    }
                }
                // Collisions and offscreen info
                if (currentElementData) {
                    if (currentElementData->debugData.collision) {
                        CLAY_AUTO_ID({ .layout = { .padding = { 8, 8, 2, 2 }}, .border = { .color = {177, 147, 8, 255}, .width = {1, 1, 1, 1, 0} } }) {
                            CLAY_TEXT(CLAY_STRING("Duplicate ID"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16 }));
                        }
                    }
                    if (offscreen) {
                        CLAY_AUTO_ID({ .layout = { .padding = { 8, 8, 2, 2 } }, .border = {  .color = CLAY__DEBUGVIEW_COLOR_3, .width = { 1, 1, 1, 1, 0} } }) {
                            CLAY_TEXT(CLAY_STRING("Offscreen"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16 }));
                        }
                    }
                }
                if (currentElementData->elementId.stringId.length > 0) {
                    CLAY_AUTO_ID() {
                        Clay_TextElementConfig textConfig = offscreen ? CLAY__INIT(Clay_TextElementConfig) { .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16 } : Clay__DebugView_TextNameConfig;
                        CLAY_TEXT(currentElementData->elementId.stringId, textConfig);
                        if (currentElementData->elementId.offset != 0) {
                            CLAY_TEXT(CLAY_STRING(" ("), textConfig);
                            CLAY_TEXT(Clay__IntToString(currentElementData->elementId.offset), textConfig);
                            CLAY_TEXT(CLAY_STRING(")"), textConfig);
                        }
                    }
                }
                if (currentElement->isTextElement) {
                    Clay__RenderElementConfigTypeLabel(CLAY_STRING("Text"), CLAY__INIT(Clay_Color) { 105,210,231,255 }, offscreen);
                } else {
                    if (currentElement->config.backgroundColor.a > 0) {
                        Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(CLAY__ELEMENT_CONFIG_TYPE_BACKGROUND_COLOR);
                        Clay__RenderElementConfigTypeLabel(config.label, config.color, offscreen);
                    }
                    if (currentElement->config.overlayColor.a > 0) {
                        Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(CLAY__ELEMENT_CONFIG_TYPE_OVERLAY_COLOR);
                        Clay__RenderElementConfigTypeLabel(config.label, config.color, offscreen);
                    }
                    if (!Clay__MemCmp((const char*)&currentElement->config.cornerRadius, (const char*)&Clay__CornerRadius_DEFAULT, sizeof(Clay_CornerRadius))) {
                        Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(CLAY__ELEMENT_CONFIG_TYPE_CORNER_RADIUS);
                        Clay__RenderElementConfigTypeLabel(config.label, config.color, offscreen);
                    }
                    if (currentElement->config.aspectRatio.aspectRatio != 0) {
                        Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(CLAY__ELEMENT_CONFIG_TYPE_ASPECT);
                        Clay__RenderElementConfigTypeLabel(config.label, config.color, offscreen);
                    }
                    if (currentElement->config.image.imageData) {
                        Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(CLAY__ELEMENT_CONFIG_TYPE_IMAGE);
                        Clay__RenderElementConfigTypeLabel(config.label, config.color, offscreen);
                    }
                    if (currentElement->config.floating.attachTo != CLAY_ATTACH_TO_NONE) {
                        Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(CLAY__ELEMENT_CONFIG_TYPE_FLOATING);
                        Clay__RenderElementConfigTypeLabel(config.label, config.color, offscreen);
                    }
                    if (currentElement->config.clip.horizontal || currentElement->config.clip.vertical) {
                        Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(CLAY__ELEMENT_CONFIG_TYPE_CLIP);
                        Clay__RenderElementConfigTypeLabel(config.label, config.color, offscreen);
                    }
                    if (Clay__BorderHasAnyWidth(&currentElement->config.border)) {
                        Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(CLAY__ELEMENT_CONFIG_TYPE_BORDER);
                        Clay__RenderElementConfigTypeLabel(config.label, config.color, offscreen);
                    }
                    if (currentElement->config.custom.customData) {
                        Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(CLAY__ELEMENT_CONFIG_TYPE_CUSTOM);
                        Clay__RenderElementConfigTypeLabel(config.label, config.color, offscreen);
                    }
                }
            }

            // Render the text contents below the element as a non-interactive row
            if (currentElement->isTextElement) {
                layoutData.rowCount++;
                Clay__TextElementData *textElementData = &currentElement->textElementData;
                Clay_TextElementConfig rawTextConfig = offscreen ? CLAY__INIT(Clay_TextElementConfig) { .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16 } : Clay__DebugView_TextNameConfig;
                CLAY_AUTO_ID({ .layout = { .sizing = { .height = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } } }) {
                    CLAY_AUTO_ID({ .layout = { .sizing = {.width = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_INDENT_WIDTH + 16) } } }) {}
                    CLAY_TEXT(CLAY_STRING("\""), rawTextConfig);
                    CLAY_TEXT(textElementData->text.length > 40 ? (CLAY__INIT(Clay_String) { .length = 40, .chars = textElementData->text.chars }) : textElementData->text, rawTextConfig);
                    if (textElementData->text.length > 40) {
                        CLAY_TEXT(CLAY_STRING("..."), rawTextConfig);
                    }
                    CLAY_TEXT(CLAY_STRING("\""), rawTextConfig);
                }
            } else if (currentElement->children.length > 0) {
                Clay__OpenElement();
                Clay__ConfigureOpenElement(CLAY__INIT(Clay_ElementDeclaration) { .layout = { .padding = { .left = 8 } } });
                Clay__OpenElement();
                Clay__ConfigureOpenElement(CLAY__INIT(Clay_ElementDeclaration) { .layout = { .padding = { .left = CLAY__DEBUGVIEW_INDENT_WIDTH }}, .border = { .color = CLAY__DEBUGVIEW_COLOR_3, .width = { .left = 1 } }});
                Clay__OpenElement();
                Clay__ConfigureOpenElement(CLAY__INIT(Clay_ElementDeclaration) { .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM } });
            }

            layoutData.rowCount++;
            if (!(currentElement->isTextElement || (currentElementData && currentElementData->debugData.collapsed))) {
                for (int32_t i = currentElement->children.length - 1; i >= 0; --i) {
                    Clay__int32_tArray_Add(&dfsBuffer, currentElement->children.elements[i]);
                    context->treeNodeVisited.internalArray[dfsBuffer.length - 1] = false; // TODO needs to be ranged checked
                }
            }
        }
    }

    if (context->pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        Clay_ElementId collapseButtonId = Clay__HashString(CLAY_STRING("Clay__DebugView_CollapseElement"), 0);
        for (int32_t i = (int)context->pointerOverIds.length - 1; i >= 0; i--) {
            Clay_ElementId *elementId = Clay_ElementIdArray_Get(&context->pointerOverIds, i);
            if (elementId->baseId == collapseButtonId.baseId) {
                Clay_LayoutElementHashMapItem *highlightedItem = Clay__GetHashMapItem(elementId->offset);
                highlightedItem->debugData.collapsed = !highlightedItem->debugData.collapsed;
                break;
            }
        }
    }

    if (highlightedElementId) {
        CLAY(CLAY_ID("Clay__DebugView_ElementHighlight"), { .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }, .floating = { .parentId = highlightedElementId, .zIndex = 32767, .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH, .attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID } }) {
            CLAY(CLAY_ID("Clay__DebugView_ElementHighlightRectangle"), { .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }, .backgroundColor = Clay__debugViewHighlightColor }) {}
        }
    }
    return layoutData;
}

void Clay__RenderDebugLayoutSizing(Clay_SizingAxis sizing, Clay_TextElementConfig infoTextConfig) {
    Clay_String sizingLabel = CLAY_STRING("GROW");
    if (sizing.type == CLAY__SIZING_TYPE_FIT) {
        sizingLabel = CLAY_STRING("FIT");
    } else if (sizing.type == CLAY__SIZING_TYPE_PERCENT) {
        sizingLabel = CLAY_STRING("PERCENT");
    } else if (sizing.type == CLAY__SIZING_TYPE_FIXED) {
        sizingLabel = CLAY_STRING("FIXED");
    }
    CLAY_TEXT(sizingLabel, infoTextConfig);
    if (sizing.type == CLAY__SIZING_TYPE_GROW || sizing.type == CLAY__SIZING_TYPE_FIT || sizing.type == CLAY__SIZING_TYPE_FIXED) {
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
    } else if (sizing.type == CLAY__SIZING_TYPE_PERCENT) {
        CLAY_TEXT(CLAY_STRING("("), infoTextConfig);
        CLAY_TEXT(Clay__IntToString(sizing.size.percent * 100), infoTextConfig);
        CLAY_TEXT(CLAY_STRING("%)"), infoTextConfig);
    }
}

void Clay__DebugViewRenderElementConfigHeader(Clay_String elementId, Clay__DebugElementConfigType type) {
    Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(type);
    Clay_Color backgroundColor = config.color;
    backgroundColor.a = 90;
    CLAY_AUTO_ID({ .layout = { .padding = { 8, 8, 2, 2 } }, .backgroundColor = backgroundColor, .cornerRadius = CLAY_CORNER_RADIUS(4), .border = { .color = config.color, .width = { 1, 1, 1, 1, 0 } } }) {
        CLAY_TEXT(config.label, CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
    }
}

void Clay__RenderDebugViewColor(Clay_Color color, Clay_TextElementConfig textConfig) {
    CLAY_AUTO_ID({ .layout = { .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} } }) {
        CLAY_TEXT(CLAY_STRING("{ r: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.r), textConfig);
        CLAY_TEXT(CLAY_STRING(", g: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.g), textConfig);
        CLAY_TEXT(CLAY_STRING(", b: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.b), textConfig);
        CLAY_TEXT(CLAY_STRING(", a: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.a), textConfig);
        CLAY_TEXT(CLAY_STRING(" }"), textConfig);
        CLAY_AUTO_ID({ .layout = { .sizing = { .width = CLAY_SIZING_FIXED(10) } } }) {}
        CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 8), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 8)} }, .backgroundColor = color, .cornerRadius = CLAY_CORNER_RADIUS(4), .border = { .color = CLAY__DEBUGVIEW_COLOR_4, .width = { 1, 1, 1, 1, 0 } } }) {}
    }
}

void Clay__RenderDebugViewCornerRadius(Clay_CornerRadius cornerRadius, Clay_TextElementConfig textConfig) {
    CLAY_AUTO_ID({ .layout = { .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} } }) {
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

void HandleDebugViewCloseButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    Clay_Context* context = Clay_GetCurrentContext();
    (void) elementId; (void) pointerInfo; (void) userData;
    if (pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        context->debugModeEnabled = false;
    }
}

void Clay__RenderDebugView(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay_ElementId closeButtonId = Clay__HashString(CLAY_STRING("Clay__DebugViewTopHeaderCloseButtonOuter"), 0);
    if (context->pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        for (int32_t i = 0; i < context->pointerOverIds.length; ++i) {
            Clay_ElementId *elementId = Clay_ElementIdArray_Get(&context->pointerOverIds, i);
            if (elementId->id == closeButtonId.id) {
                context->debugModeEnabled = false;
                return;
            }
        }
    }

    uint32_t initialRootsLength = context->layoutElementTreeRoots.length;
    uint32_t initialElementsLength = context->layoutElements.length;
    Clay_TextElementConfig infoTextConfig = CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16, .wrapMode = CLAY_TEXT_WRAP_NONE });
    Clay_TextElementConfig infoTitleConfig = CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16, .wrapMode = CLAY_TEXT_WRAP_NONE });
    Clay_ElementId scrollId = Clay__HashString(CLAY_STRING("Clay__DebugViewOuterScrollPane"), 0);
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
    CLAY(CLAY_ID("Clay__DebugView"), {
         .layout = { .sizing = { CLAY_SIZING_FIXED((float)Clay__debugViewWidth) , CLAY_SIZING_FIXED(context->layoutDimensions.height) }, .layoutDirection = CLAY_TOP_TO_BOTTOM },
        .floating = { .zIndex = 32765, .attachPoints = { .element = CLAY_ATTACH_POINT_LEFT_CENTER, .parent = CLAY_ATTACH_POINT_RIGHT_CENTER }, .attachTo = CLAY_ATTACH_TO_ROOT, .clipTo = CLAY_CLIP_TO_ATTACHED_PARENT },
        .border = { .color = CLAY__DEBUGVIEW_COLOR_3, .width = { .bottom = 1 } }
    }) {
        CLAY_AUTO_ID({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING, 0, 0 }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} }, .backgroundColor = CLAY__DEBUGVIEW_COLOR_2 }) {
            CLAY_TEXT(CLAY_STRING("Clay Debug Tools"), infoTextConfig);
            CLAY_AUTO_ID({ .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) } } }) {}
            // Close button
            CLAY_AUTO_ID({
                .layout = { .sizing = {CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 10), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 10)}, .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER} },
                .backgroundColor = {217,91,67,80},
                .cornerRadius = CLAY_CORNER_RADIUS(4),
                .border = { .color = { 217,91,67,255 }, .width = { 1, 1, 1, 1, 0 } },
            }) {
                Clay_OnHover(HandleDebugViewCloseButtonInteraction, 0);
                CLAY_TEXT(CLAY_STRING("x"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
            }
        }
        CLAY_AUTO_ID({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1)} }, .backgroundColor = CLAY__DEBUGVIEW_COLOR_3 } ) {}
        CLAY(scrollId, { .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }, .clip = { .horizontal = true, .vertical = true, .childOffset = Clay_GetScrollOffset() } }) {
            CLAY_AUTO_ID({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .layoutDirection = CLAY_TOP_TO_BOTTOM }, .backgroundColor = ((initialElementsLength + initialRootsLength) & 1) == 0 ? CLAY__DEBUGVIEW_COLOR_2 : CLAY__DEBUGVIEW_COLOR_1 }) {
                Clay_ElementId panelContentsId = Clay__HashString(CLAY_STRING("Clay__DebugViewPaneOuter"), 0);
                // Element list
                CLAY(panelContentsId, { .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }, .floating = { .zIndex = 32766, .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH, .attachTo = CLAY_ATTACH_TO_PARENT, .clipTo = CLAY_CLIP_TO_ATTACHED_PARENT } }) {
                    CLAY_AUTO_ID({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = { CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING, 0, 0 }, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                        layoutData = Clay__RenderDebugLayoutElementsList((int32_t)initialRootsLength, highlightedRow);
                    }
                }
                float contentWidth = Clay__GetHashMapItem(panelContentsId.id)->layoutElement->dimensions.width;
                CLAY_AUTO_ID({ .layout = { .sizing = {.width = CLAY_SIZING_FIXED(contentWidth) }, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {}
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
                    CLAY_AUTO_ID({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .layoutDirection = CLAY_TOP_TO_BOTTOM }, .backgroundColor = rowColor } ) {}
                }
            }
        }
        CLAY_AUTO_ID({ .layout = { .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(1)} }, .backgroundColor = CLAY__DEBUGVIEW_COLOR_3 }) {}
        Clay_LayoutElementHashMapItem *selectedItem = Clay__GetHashMapItem(context->debugSelectedElementId);
        if (selectedItem->layoutElement) {
            CLAY_AUTO_ID({
                .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(300)}, .layoutDirection = CLAY_TOP_TO_BOTTOM },
                .backgroundColor = CLAY__DEBUGVIEW_COLOR_2 ,
                .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() },
                .border = { .color = CLAY__DEBUGVIEW_COLOR_3, .width = { .betweenChildren = 1 } }
            }) {
                CLAY_AUTO_ID({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT + 8)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING, 0, 0 }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} } }) {
                    CLAY_TEXT(CLAY_STRING("Element Configuration"), infoTextConfig);
                    CLAY_AUTO_ID({ .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) } } }) {}
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
                CLAY_AUTO_ID({ .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                    CLAY_AUTO_ID({ .layout = { .padding = { 8, 8, 2, 2 } }, .backgroundColor = { 200, 200, 200, 120 }, .cornerRadius = CLAY_CORNER_RADIUS(4), .border = { .color = { 200, 200, 200, 255 }, .width = { 1, 1, 1, 1, 0 } } }) {
                        CLAY_TEXT(CLAY_STRING("Layout"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
                    }
                    // .boundingBox
                    CLAY_TEXT(CLAY_STRING("Bounding Box"), infoTitleConfig);
                    CLAY_AUTO_ID({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
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
                    if (!selectedItem->layoutElement->isTextElement) {
                        // .layoutDirection
                        CLAY_TEXT(CLAY_STRING("Layout Direction"), infoTitleConfig);
                        Clay_LayoutConfig *layoutConfig = &selectedItem->layoutElement->config.layout;
                        CLAY_TEXT(layoutConfig->layoutDirection == CLAY_TOP_TO_BOTTOM ? CLAY_STRING("TOP_TO_BOTTOM") : CLAY_STRING("LEFT_TO_RIGHT"), infoTextConfig);
                        // .sizing
                        CLAY_TEXT(CLAY_STRING("Sizing"), infoTitleConfig);
                        CLAY_AUTO_ID({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
                            CLAY_TEXT(CLAY_STRING("width: "), infoTextConfig);
                            Clay__RenderDebugLayoutSizing(layoutConfig->sizing.width, infoTextConfig);
                        }
                        CLAY_AUTO_ID({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
                            CLAY_TEXT(CLAY_STRING("height: "), infoTextConfig);
                            Clay__RenderDebugLayoutSizing(layoutConfig->sizing.height, infoTextConfig);
                        }
                        // .padding
                        CLAY_TEXT(CLAY_STRING("Padding"), infoTitleConfig);
                        CLAY(CLAY_ID("Clay__DebugViewElementInfoPadding"), { }) {
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
                        CLAY_AUTO_ID({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
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
                }
                if (selectedItem->layoutElement->isTextElement) {
                    Clay_TextElementConfig *textConfig = &selectedItem->layoutElement->textConfig;
                    CLAY_AUTO_ID({ .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                        Clay__DebugViewRenderElementConfigHeader(selectedItem->elementId.stringId, CLAY__ELEMENT_CONFIG_TYPE_TEXT);
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
                        // .wrapMode
                        CLAY_TEXT(CLAY_STRING("Wrap Mode"), infoTitleConfig);
                        Clay_String wrapMode = CLAY_STRING("WORDS");
                        if (textConfig->wrapMode == CLAY_TEXT_WRAP_NONE) {
                            wrapMode = CLAY_STRING("NONE");
                        } else if (textConfig->wrapMode == CLAY_TEXT_WRAP_NEWLINES) {
                            wrapMode = CLAY_STRING("NEWLINES");
                        }
                        CLAY_TEXT(wrapMode, infoTextConfig);
                        // .textAlignment
                        CLAY_TEXT(CLAY_STRING("Text Alignment"), infoTitleConfig);
                        Clay_String textAlignment = CLAY_STRING("LEFT");
                        if (textConfig->textAlignment == CLAY_TEXT_ALIGN_CENTER) {
                            textAlignment = CLAY_STRING("CENTER");
                        } else if (textConfig->textAlignment == CLAY_TEXT_ALIGN_RIGHT) {
                            textAlignment = CLAY_STRING("RIGHT");
                        }
                        CLAY_TEXT(textAlignment, infoTextConfig);
                        // .textColor
                        CLAY_TEXT(CLAY_STRING("Text Color"), infoTitleConfig);
                        Clay__RenderDebugViewColor(textConfig->textColor, infoTextConfig);
                    }
                } else {
                    CLAY(CLAY_ID("Clay__DebugViewElementInfoSharedBody"), { .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                        Clay__DebugElementConfigTypeLabelConfig labelConfig = Clay__DebugGetElementConfigTypeLabel(CLAY__ELEMENT_CONFIG_TYPE_BACKGROUND_COLOR);
                        Clay_Color backgroundColor = labelConfig.color;
                        backgroundColor.a = 90;
                        CLAY_AUTO_ID({ .layout = { .padding = { 8, 8, 2, 2 } }, .backgroundColor = backgroundColor, .cornerRadius = CLAY_CORNER_RADIUS(4), .border = { .color = labelConfig.color, .width = { 1, 1, 1, 1, 0 } } }) {
                            CLAY_TEXT(CLAY_STRING("Color & Radius"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
                        }
                        // .backgroundColor
                        if (selectedItem->layoutElement->config.backgroundColor.a > 0) {
                            CLAY_TEXT(CLAY_STRING("Background Color"), infoTitleConfig);
                            Clay__RenderDebugViewColor(selectedItem->layoutElement->config.backgroundColor, infoTextConfig);
                        }
                        // .cornerRadius
                        if (!Clay__MemCmp((const char*)&selectedItem->layoutElement->config.cornerRadius, (const char*)&Clay__CornerRadius_DEFAULT, sizeof(Clay_CornerRadius))) {
                            CLAY_TEXT(CLAY_STRING("Corner Radius"), infoTitleConfig);
                            Clay__RenderDebugViewCornerRadius(selectedItem->layoutElement->config.cornerRadius, infoTextConfig);
                        }
                        // .overlayColor
                        if (selectedItem->layoutElement->config.overlayColor.a > 0) {
                            CLAY_TEXT(CLAY_STRING("Overlay Color"), infoTitleConfig);
                            Clay__RenderDebugViewColor(selectedItem->layoutElement->config.overlayColor, infoTextConfig);
                        }
                    }
                    if (selectedItem->layoutElement->config.aspectRatio.aspectRatio > 0) {
                        Clay_AspectRatioElementConfig *aspectRatioConfig = &selectedItem->layoutElement->config.aspectRatio;
                        CLAY(CLAY_ID("Clay__DebugViewElementInfoAspectRatioBody"), { .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                            Clay__DebugViewRenderElementConfigHeader(selectedItem->elementId.stringId, CLAY__ELEMENT_CONFIG_TYPE_ASPECT);
                            CLAY_TEXT(CLAY_STRING("Aspect Ratio"), infoTitleConfig);
                            // Aspect Ratio
                            CLAY(CLAY_ID("Clay__DebugViewElementInfoAspectRatio"), { }) {
                                CLAY_TEXT(Clay__IntToString(aspectRatioConfig->aspectRatio), infoTextConfig);
                                CLAY_TEXT(CLAY_STRING("."), infoTextConfig);
                                float frac = aspectRatioConfig->aspectRatio - (int)(aspectRatioConfig->aspectRatio);
                                frac *= 100;
                                if ((int)frac < 10) {
                                    CLAY_TEXT(CLAY_STRING("0"), infoTextConfig);
                                }
                                CLAY_TEXT(Clay__IntToString(frac), infoTextConfig);
                            }
                        }
                    }
                    if (selectedItem->layoutElement->config.image.imageData) {
                        Clay_ImageElementConfig *imageConfig = &selectedItem->layoutElement->config.image;
                        Clay_AspectRatioElementConfig aspectConfig = { 1 };
                        if (selectedItem->layoutElement->config.aspectRatio.aspectRatio > 0) {
                            aspectConfig = selectedItem->layoutElement->config.aspectRatio;
                        }
                        CLAY(CLAY_ID("Clay__DebugViewElementInfoImageBody"), { .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                            Clay__DebugViewRenderElementConfigHeader(selectedItem->elementId.stringId, CLAY__ELEMENT_CONFIG_TYPE_IMAGE);
                            // Image Preview
                            CLAY_TEXT(CLAY_STRING("Preview"), infoTitleConfig);
                            CLAY_AUTO_ID({ .layout = { .sizing = { .width = CLAY_SIZING_GROW(64, 128), .height = CLAY_SIZING_GROW(64, 128) }}, .aspectRatio = aspectConfig, .image = *imageConfig }) {}
                        }
                    }
                    if (selectedItem->layoutElement->config.floating.attachTo != CLAY_ATTACH_TO_NONE) {
                        Clay_FloatingElementConfig* floatingConfig = &selectedItem->layoutElement->config.floating;
                        CLAY_AUTO_ID({ .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                            Clay__DebugViewRenderElementConfigHeader(selectedItem->elementId.stringId, CLAY__ELEMENT_CONFIG_TYPE_FLOATING);
                            // .offset
                            CLAY_TEXT(CLAY_STRING("Offset"), infoTitleConfig);
                            CLAY_AUTO_ID({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
                                CLAY_TEXT(CLAY_STRING("{ x: "), infoTextConfig);
                                CLAY_TEXT(Clay__IntToString(floatingConfig->offset.x), infoTextConfig);
                                CLAY_TEXT(CLAY_STRING(", y: "), infoTextConfig);
                                CLAY_TEXT(Clay__IntToString(floatingConfig->offset.y), infoTextConfig);
                                CLAY_TEXT(CLAY_STRING(" }"), infoTextConfig);
                            }
                            // .expand
                            CLAY_TEXT(CLAY_STRING("Expand"), infoTitleConfig);
                            CLAY_AUTO_ID({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
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
                            // .attachPoints
                            CLAY_TEXT(CLAY_STRING("Attach Points"), infoTitleConfig);
                            CLAY_AUTO_ID({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
                                CLAY_TEXT(CLAY_STRING("{ element: "), infoTextConfig);
                                Clay_String attachPointElement = CLAY_STRING("LEFT_TOP");
                                if (floatingConfig->attachPoints.element == CLAY_ATTACH_POINT_LEFT_CENTER) {
                                    attachPointElement = CLAY_STRING("LEFT_CENTER");
                                } else if (floatingConfig->attachPoints.element == CLAY_ATTACH_POINT_LEFT_BOTTOM) {
                                    attachPointElement = CLAY_STRING("LEFT_BOTTOM");
                                } else if (floatingConfig->attachPoints.element == CLAY_ATTACH_POINT_CENTER_TOP) {
                                    attachPointElement = CLAY_STRING("CENTER_TOP");
                                } else if (floatingConfig->attachPoints.element == CLAY_ATTACH_POINT_CENTER_CENTER) {
                                    attachPointElement = CLAY_STRING("CENTER_CENTER");
                                } else if (floatingConfig->attachPoints.element == CLAY_ATTACH_POINT_CENTER_BOTTOM) {
                                    attachPointElement = CLAY_STRING("CENTER_BOTTOM");
                                } else if (floatingConfig->attachPoints.element == CLAY_ATTACH_POINT_RIGHT_TOP) {
                                    attachPointElement = CLAY_STRING("RIGHT_TOP");
                                } else if (floatingConfig->attachPoints.element == CLAY_ATTACH_POINT_RIGHT_CENTER) {
                                    attachPointElement = CLAY_STRING("RIGHT_CENTER");
                                } else if (floatingConfig->attachPoints.element == CLAY_ATTACH_POINT_RIGHT_BOTTOM) {
                                    attachPointElement = CLAY_STRING("RIGHT_BOTTOM");
                                }
                                CLAY_TEXT(attachPointElement, infoTextConfig);
                                Clay_String attachPointParent = CLAY_STRING("LEFT_TOP");
                                if (floatingConfig->attachPoints.parent == CLAY_ATTACH_POINT_LEFT_CENTER) {
                                    attachPointParent = CLAY_STRING("LEFT_CENTER");
                                } else if (floatingConfig->attachPoints.parent == CLAY_ATTACH_POINT_LEFT_BOTTOM) {
                                    attachPointParent = CLAY_STRING("LEFT_BOTTOM");
                                } else if (floatingConfig->attachPoints.parent == CLAY_ATTACH_POINT_CENTER_TOP) {
                                    attachPointParent = CLAY_STRING("CENTER_TOP");
                                } else if (floatingConfig->attachPoints.parent == CLAY_ATTACH_POINT_CENTER_CENTER) {
                                    attachPointParent = CLAY_STRING("CENTER_CENTER");
                                } else if (floatingConfig->attachPoints.parent == CLAY_ATTACH_POINT_CENTER_BOTTOM) {
                                    attachPointParent = CLAY_STRING("CENTER_BOTTOM");
                                } else if (floatingConfig->attachPoints.parent == CLAY_ATTACH_POINT_RIGHT_TOP) {
                                    attachPointParent = CLAY_STRING("RIGHT_TOP");
                                } else if (floatingConfig->attachPoints.parent == CLAY_ATTACH_POINT_RIGHT_CENTER) {
                                    attachPointParent = CLAY_STRING("RIGHT_CENTER");
                                } else if (floatingConfig->attachPoints.parent == CLAY_ATTACH_POINT_RIGHT_BOTTOM) {
                                    attachPointParent = CLAY_STRING("RIGHT_BOTTOM");
                                }
                                CLAY_TEXT(CLAY_STRING(", parent: "), infoTextConfig);
                                CLAY_TEXT(attachPointParent, infoTextConfig);
                                CLAY_TEXT(CLAY_STRING(" }"), infoTextConfig);
                            }
                            // .pointerCaptureMode
                            CLAY_TEXT(CLAY_STRING("Pointer Capture Mode"), infoTitleConfig);
                            Clay_String pointerCaptureMode = CLAY_STRING("NONE");
                            if (floatingConfig->pointerCaptureMode == CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH) {
                                pointerCaptureMode = CLAY_STRING("PASSTHROUGH");
                            }
                            CLAY_TEXT(pointerCaptureMode, infoTextConfig);
                            // .attachTo
                            CLAY_TEXT(CLAY_STRING("Attach To"), infoTitleConfig);
                            Clay_String attachTo = CLAY_STRING("NONE");
                            if (floatingConfig->attachTo == CLAY_ATTACH_TO_PARENT) {
                                attachTo = CLAY_STRING("PARENT");
                            } else if (floatingConfig->attachTo == CLAY_ATTACH_TO_ELEMENT_WITH_ID) {
                                attachTo = CLAY_STRING("ELEMENT_WITH_ID");
                            } else if (floatingConfig->attachTo == CLAY_ATTACH_TO_ROOT) {
                                attachTo = CLAY_STRING("ROOT");
                            }
                            CLAY_TEXT(attachTo, infoTextConfig);
                            // .clipTo
                            CLAY_TEXT(CLAY_STRING("Clip To"), infoTitleConfig);
                            Clay_String clipTo = CLAY_STRING("ATTACHED_PARENT");
                            if (floatingConfig->clipTo == CLAY_CLIP_TO_NONE) {
                                clipTo = CLAY_STRING("NONE");
                            }
                            CLAY_TEXT(clipTo, infoTextConfig);
                        }
                    }
                    Clay_ClipElementConfig *clipConfig = &selectedItem->layoutElement->config.clip;
                    if (clipConfig->horizontal || clipConfig->vertical) {
                        CLAY_AUTO_ID({ .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                            Clay__DebugViewRenderElementConfigHeader(selectedItem->elementId.stringId, CLAY__ELEMENT_CONFIG_TYPE_CLIP);
                            // .vertical
                            CLAY_TEXT(CLAY_STRING("Vertical"), infoTitleConfig);
                            CLAY_TEXT(clipConfig->vertical ? CLAY_STRING("true") : CLAY_STRING("false") , infoTextConfig);
                            // .horizontal
                            CLAY_TEXT(CLAY_STRING("Horizontal"), infoTitleConfig);
                            CLAY_TEXT(clipConfig->horizontal ? CLAY_STRING("true") : CLAY_STRING("false") , infoTextConfig);
                        }
                    }
                    Clay_BorderElementConfig *borderConfig = &selectedItem->layoutElement->config.border;
                    if (Clay__BorderHasAnyWidth(borderConfig)) {
                        CLAY(CLAY_ID("Clay__DebugViewElementInfoBorderBody"), { .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                            Clay__DebugViewRenderElementConfigHeader(selectedItem->elementId.stringId, CLAY__ELEMENT_CONFIG_TYPE_BORDER);
                            CLAY_TEXT(CLAY_STRING("Border Widths"), infoTitleConfig);
                            CLAY_AUTO_ID({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
                                CLAY_TEXT(CLAY_STRING("{ left: "), infoTextConfig);
                                CLAY_TEXT(Clay__IntToString(borderConfig->width.left), infoTextConfig);
                                CLAY_TEXT(CLAY_STRING(", right: "), infoTextConfig);
                                CLAY_TEXT(Clay__IntToString(borderConfig->width.right), infoTextConfig);
                                CLAY_TEXT(CLAY_STRING(", top: "), infoTextConfig);
                                CLAY_TEXT(Clay__IntToString(borderConfig->width.top), infoTextConfig);
                                CLAY_TEXT(CLAY_STRING(", bottom: "), infoTextConfig);
                                CLAY_TEXT(Clay__IntToString(borderConfig->width.bottom), infoTextConfig);
                                CLAY_TEXT(CLAY_STRING(" }"), infoTextConfig);
                            }
                            // .textColor
                            CLAY_TEXT(CLAY_STRING("Border Color"), infoTitleConfig);
                            Clay__RenderDebugViewColor(borderConfig->color, infoTextConfig);
                        }
                    }
                }
            }
        } else {
            CLAY(CLAY_ID("Clay__DebugViewWarningsScrollPane"), { .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(300)}, .childGap = 6, .layoutDirection = CLAY_TOP_TO_BOTTOM }, .backgroundColor = CLAY__DEBUGVIEW_COLOR_2, .clip = { .horizontal = true, .vertical = true, .childOffset = Clay_GetScrollOffset() } }) {
                Clay_TextElementConfig warningConfig = CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16, .wrapMode = CLAY_TEXT_WRAP_NONE });
                CLAY(CLAY_ID("Clay__DebugViewWarningItemHeader"), { .layout = { .sizing = {.height = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING, 0, 0 }, .childGap = 8, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} } }) {
                    CLAY_TEXT(CLAY_STRING("Warnings"), warningConfig);
                }
                CLAY(CLAY_ID("Clay__DebugViewWarningsTopBorder"), { .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(1)} }, .backgroundColor = {200, 200, 200, 255} }) {}
                int32_t previousWarningsLength = context->warnings.length;
                for (int32_t i = 0; i < previousWarningsLength; i++) {
                    Clay__Warning warning = context->warnings.internalArray[i];
                    CLAY(CLAY_IDI("Clay__DebugViewWarningItem", i), { .layout = { .sizing = {.height = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING, 0, 0 }, .childGap = 8, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} } }) {
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
    uintptr_t nextAllocOffset = arena->nextAllocation + (64 - (arena->nextAllocation % 64));
    if (nextAllocOffset + totalSizeBytes <= arena->capacity) {
        array.internalArray = (Clay__Warning*)((uintptr_t)arena->memory + (uintptr_t)nextAllocOffset);
        arena->nextAllocation = nextAllocOffset + totalSizeBytes;
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

void* Clay__Array_Allocate_Arena(int32_t capacity, uint32_t itemSize, Clay_Arena *arena)
{
    size_t totalSizeBytes = capacity * itemSize;
    uintptr_t nextAllocOffset = arena->nextAllocation + ((64 - (arena->nextAllocation % 64)) & 63);
    if (nextAllocOffset + totalSizeBytes <= arena->capacity) {
        arena->nextAllocation = nextAllocOffset + totalSizeBytes;
        return (void*)((uintptr_t)arena->memory + (uintptr_t)nextAllocOffset);
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
        fakeContext.maxMeasureTextCacheWordCount = currentContext->maxMeasureTextCacheWordCount;
    }
    // Reserve space in the arena for the context, important for calculating min memory size correctly
    Clay__Context_Allocate_Arena(&fakeContext.internalArena);
    Clay__InitializePersistentMemory(&fakeContext);
    Clay__InitializeEphemeralMemory(&fakeContext);
    return (uint32_t)fakeContext.internalArena.nextAllocation + 128;
}

CLAY_WASM_EXPORT("Clay_CreateArenaWithCapacityAndMemory")
Clay_Arena Clay_CreateArenaWithCapacityAndMemory(size_t capacity, void *memory) {
    Clay_Arena arena = {
        .capacity = capacity,
        .memory = (char *)memory
    };
    return arena;
}

#ifndef CLAY_WASM
void Clay_SetMeasureTextFunction(Clay_Dimensions (*measureTextFunction)(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData), void *userData) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__MeasureText = measureTextFunction;
    context->measureTextUserData = userData;
}
void Clay_SetQueryScrollOffsetFunction(Clay_Vector2 (*queryScrollOffsetFunction)(uint32_t elementId, void *userData), void *userData) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__QueryScrollOffset = queryScrollOffsetFunction;
    context->queryScrollOffsetUserData = userData;
}
#endif

CLAY_WASM_EXPORT("Clay_SetLayoutDimensions")
void Clay_SetLayoutDimensions(Clay_Dimensions dimensions) {
    Clay_Context* context = Clay_GetCurrentContext();
    context->rootResizedLastFrame = !Clay__FloatEqual(context->layoutDimensions.width, dimensions.width) || !Clay__FloatEqual(context->layoutDimensions.height, dimensions.height);
    context->layoutDimensions = dimensions;
}

CLAY_WASM_EXPORT("Clay_SetLayoutDimensions")
Clay_Dimensions Clay_GetLayoutDimensions() {
    Clay_Context* context = Clay_GetCurrentContext();
    return context->layoutDimensions;
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
        bool skipTree = false;
        while (dfsBuffer.length > 0) {
            if (context->treeNodeVisited.internalArray[dfsBuffer.length - 1]) {
                dfsBuffer.length--;
                continue;
            }
            context->treeNodeVisited.internalArray[dfsBuffer.length - 1] = true;
            Clay_LayoutElement *currentElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&dfsBuffer, (int)dfsBuffer.length - 1));

            Clay_LayoutElementHashMapItem *mapItem = Clay__GetHashMapItem(currentElement->id); // TODO think of a way around this, maybe the fact that it's essentially a binary tree limits the cost, but the worst case is not great
            int32_t clipElementId = Clay__int32_tArray_GetValue(&context->layoutElementClipElementIds, (int32_t)(currentElement - context->layoutElements.internalArray));
            Clay_LayoutElementHashMapItem *clipItem = Clay__GetHashMapItem(clipElementId);
            // This check skips mouse interactions for elements that are currently "exit transitioning"
            if (mapItem && mapItem->generation > context->generation) {
                // Conditionally skip mouse interactions on non-exit transitions, based on user config
                if (!currentElement->isTextElement && currentElement->config.transition.handler) {
                    for (int I = 0; I < context->transitionDatas.length; ++I) {
                        Clay__TransitionDataInternal* data = Clay__TransitionDataInternalArray_Get(&context->transitionDatas, I);
                        if (data->elementId == currentElement->id) {
                            if (currentElement->config.transition.interactionHandling == CLAY_TRANSITION_DISABLE_INTERACTIONS_WHILE_TRANSITIONING_POSITION) {
                                if (data->state == CLAY_TRANSITION_STATE_EXITING || data->state == CLAY_TRANSITION_STATE_ENTERING || ((data->activeProperties & CLAY_TRANSITION_PROPERTY_POSITION) && data->state == CLAY_TRANSITION_STATE_TRANSITIONING)) {
                                    skipTree = true;
                                }
                            } else if (currentElement->config.transition.interactionHandling == CLAY_TRANSITION_ALLOW_INTERACTIONS_WHILE_TRANSITIONING_POSITION) {
                                if (data->state == CLAY_TRANSITION_STATE_EXITING) {
                                    skipTree = true;
                                }
                            }
                        }
                    }
                }

                if (skipTree) {
                    dfsBuffer.length--;
                    continue;
                }

                Clay_BoundingBox elementBox = mapItem->boundingBox;
                elementBox.x -= root->pointerOffset.x;
                elementBox.y -= root->pointerOffset.y;
                if ((Clay__PointIsInsideRect(position, elementBox)) && (clipElementId == 0 || (Clay__PointIsInsideRect(position, clipItem->boundingBox)) || context->externalScrollHandlingEnabled)) {
                    if (!skipTree) {
                        if (mapItem->onHoverFunction) {
                            mapItem->onHoverFunction(mapItem->elementId, context->pointerInfo, mapItem->hoverFunctionUserData);
                        }
                        Clay_ElementIdArray_Add(&context->pointerOverIds, mapItem->elementId);
                    }
                    found = true;
                }

                for (int32_t i = currentElement->children.length - 1; i >= 0; --i) {
                    Clay__int32_tArray_Add(&dfsBuffer, currentElement->children.elements[i]);
                    context->treeNodeVisited.internalArray[dfsBuffer.length - 1] = false; // TODO needs to be ranged checked
                }
            } else {
                dfsBuffer.length--;
            }
        }

        Clay_LayoutElement *rootElement = Clay_LayoutElementArray_Get(&context->layoutElements, root->layoutElementIndex);
        if (found && rootElement->config.floating.attachTo != CLAY_ATTACH_TO_NONE && rootElement->config.floating.pointerCaptureMode == CLAY_POINTER_CAPTURE_MODE_CAPTURE) {
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

CLAY_WASM_EXPORT("Clay_GetPointerState")
CLAY_DLL_EXPORT Clay_PointerData Clay_GetPointerState(void) {
    return Clay_GetCurrentContext()->pointerInfo;
}

CLAY_WASM_EXPORT("Clay_Initialize")
Clay_Context* Clay_Initialize(Clay_Arena arena, Clay_Dimensions layoutDimensions, Clay_ErrorHandler errorHandler) {
    // Cacheline align memory passed in
    uintptr_t baseOffset = 64 - ((uintptr_t)arena.memory % 64);
    baseOffset = baseOffset == 64 ? 0 : baseOffset;
    arena.memory += baseOffset;
    Clay_Context *context = Clay__Context_Allocate_Arena(&arena);
    if (context == NULL) return NULL;
    // DEFAULTS
    Clay_Context *oldContext = Clay_GetCurrentContext();
    *context = CLAY__INIT(Clay_Context) {
        .maxElementCount = oldContext ? oldContext->maxElementCount : Clay__defaultMaxElementCount,
        .maxMeasureTextCacheWordCount = oldContext ? oldContext->maxMeasureTextCacheWordCount : Clay__defaultMaxMeasureTextWordCacheCount,
        .errorHandler = errorHandler.errorHandlerFunction ? errorHandler : CLAY__INIT(Clay_ErrorHandler) { Clay__ErrorHandlerFunctionDefault, 0 },
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

CLAY_WASM_EXPORT("Clay_GetScrollOffset")
Clay_Vector2 Clay_GetScrollOffset(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return CLAY__INIT(Clay_Vector2) CLAY__DEFAULT_STRUCT;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    for (int32_t i = 0; i < context->scrollContainerDatas.length; i++) {
        Clay__ScrollContainerDataInternal *mapping = Clay__ScrollContainerDataInternalArray_Get(&context->scrollContainerDatas, i);
        if (mapping->layoutElement == openLayoutElement) {
            return mapping->scrollPosition;
        }
    }
    return CLAY__INIT(Clay_Vector2) CLAY__DEFAULT_STRUCT;
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
            if (scrollData->layoutElement->id == Clay_ElementIdArray_Get(&context->pointerOverIds, j)->id) {
                highestPriorityElementIndex = j;
                highestPriorityScrollData = scrollData;
            }
        }
    }

    if (highestPriorityElementIndex > -1 && highestPriorityScrollData) {
        Clay_LayoutElement *scrollElement = highestPriorityScrollData->layoutElement;
        Clay_ClipElementConfig *clipConfig = &scrollElement->config.clip;
        bool canScrollVertically = clipConfig->vertical && highestPriorityScrollData->contentSize.height > scrollElement->dimensions.height;
        bool canScrollHorizontally = clipConfig->horizontal && highestPriorityScrollData->contentSize.width > scrollElement->dimensions.width;
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
    Clay__OpenElementWithId(CLAY_ID("Clay__RootContainer"));
    Clay__ConfigureOpenElement(CLAY__INIT(Clay_ElementDeclaration) {
        .layout = { .sizing = {CLAY_SIZING_FIXED((rootDimensions.width)), CLAY_SIZING_FIXED(rootDimensions.height)} }
    });
    Clay__int32_tArray_Add(&context->openLayoutElementStack, 0);
    Clay__LayoutElementTreeRootArray_Add(&context->layoutElementTreeRoots, CLAY__INIT(Clay__LayoutElementTreeRoot) { .layoutElementIndex = 0 });
}

void Clay__CloneElementsWithExitTransition() {
    Clay_Context* context = Clay_GetCurrentContext();
    int32_t nextIndex = context->layoutElements.capacity - 1;
    int32_t nextChildIndex = context->layoutElementChildren.capacity - 1;

    for (int i = 0; i < context->transitionDatas.length; ++i) {
        Clay__TransitionDataInternal *data = Clay__TransitionDataInternalArray_Get(&context->transitionDatas, i);
        Clay_TransitionElementConfig* config = &data->elementThisFrame->config.transition;
        if (data->transitionOut) {
            Clay__int32_tArray bfsBuffer = context->openLayoutElementStack;
            bfsBuffer.length = 0;
            Clay_LayoutElement* newElement = Clay_LayoutElementArray_Set_DontTouchLength(&context->layoutElements, nextIndex, *data->elementThisFrame);
            Clay__StringArray_Set_DontTouchLength(&context->layoutElementIdStrings, nextIndex, *Clay__StringArray_GetCheckCapacity(&context->layoutElementIdStrings, data->elementThisFrame - context->layoutElements.internalArray));
            Clay__int32_tArray_Add(&bfsBuffer, nextIndex);
            data->elementThisFrame = newElement;
            nextIndex--;

            int32_t bufferIndex = 0;
            while(bufferIndex < bfsBuffer.length) {
                Clay_LayoutElement *layoutElement = Clay_LayoutElementArray_GetCheckCapacity(&context->layoutElements, Clay__int32_tArray_GetValue(&bfsBuffer, bufferIndex));
                bufferIndex++;
                for (int j = layoutElement->children.length - 1; j >= 0; --j) {
                    Clay_LayoutElement* childElement = Clay_LayoutElementArray_GetCheckCapacity(&context->layoutElements, layoutElement->children.elements[j]);
                    Clay__int32_tArray_Add(&bfsBuffer, nextIndex);
                    Clay_LayoutElement* newChildElement = Clay_LayoutElementArray_Set_DontTouchLength(&context->layoutElements, nextIndex, *childElement);
                    Clay__StringArray_Set_DontTouchLength(&context->layoutElementIdStrings, nextIndex, *Clay__StringArray_GetCheckCapacity(&context->layoutElementIdStrings, childElement - context->layoutElements.internalArray));
                    Clay__int32_tArray_Set_DontTouchLength(&context->layoutElementChildren, nextChildIndex, nextIndex);
                    nextIndex--;
                    nextChildIndex--;
                }
                layoutElement->children.elements = &context->layoutElementChildren.internalArray[nextChildIndex + 1];
            }
        }
    }
};

void Clay_ApplyTransitionedPropertiesToElement(Clay_LayoutElement* currentElement, Clay_TransitionProperty properties, Clay_TransitionData currentTransitionData, Clay_BoundingBox* boundingBox, bool reparented) {
    if (properties & CLAY_TRANSITION_PROPERTY_WIDTH) {
        if (!reparented) {
            currentElement->dimensions.width = currentTransitionData.boundingBox.width;
            currentElement->config.layout.sizing.width = CLAY_SIZING_FIXED(currentTransitionData.boundingBox.width);
        } else {
            boundingBox->width = currentTransitionData.boundingBox.width;
        }
    }
    if (properties & CLAY_TRANSITION_PROPERTY_HEIGHT) {
        if (!reparented) {
            currentElement->dimensions.height = currentTransitionData.boundingBox.height;
            currentElement->config.layout.sizing.height = CLAY_SIZING_FIXED(currentTransitionData.boundingBox.height);
        } else {
            boundingBox->height = currentTransitionData.boundingBox.height;
        }
    }
    if (properties & CLAY_TRANSITION_PROPERTY_X) {
        boundingBox->x = currentTransitionData.boundingBox.x;
    }
    if (properties & CLAY_TRANSITION_PROPERTY_Y) {
        boundingBox->y = currentTransitionData.boundingBox.y;
    }
    if (properties & CLAY_TRANSITION_PROPERTY_OVERLAY_COLOR) {
        currentElement->config.overlayColor = currentTransitionData.overlayColor;
    }
    if (properties & CLAY_TRANSITION_PROPERTY_BACKGROUND_COLOR) {
        currentElement->config.backgroundColor = currentTransitionData.backgroundColor;
    }
    if (properties & CLAY_TRANSITION_PROPERTY_BORDER_COLOR) {
        currentElement->config.border.color = currentTransitionData.borderColor;
    }
    if (properties & CLAY_TRANSITION_PROPERTY_BORDER_WIDTH) {
        currentElement->config.border.width = currentTransitionData.borderWidth;
    }
}

CLAY_WASM_EXPORT("Clay_EndLayout")
Clay_RenderCommandArray Clay_EndLayout(float deltaTime) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__CloseElement();

    for (int i = 0; i < context->transitionDatas.length; ++i) {
        Clay__TransitionDataInternal *data = Clay__TransitionDataInternalArray_Get(&context->transitionDatas, i);
        Clay_LayoutElementHashMapItem *hashMapItem = Clay__GetHashMapItem(data->elementId);
        // This might seems strange - can't we just look up the element itself, and check the config to see whether it has an exit transition defined?
        // That would work fine if the element actually had an exit transition in the first place. If it doesn't have an exit transition defined, the element
        // will have simply disappeared completely at this point, and there will be no element through which to access the config.
        if (data->transitionOut) {
            Clay_TransitionElementConfig* config = &data->elementThisFrame->config.transition;
            // Element wasn't found this frame - either delete transition data or transition out
            if (hashMapItem->generation <= context->generation) {
                Clay_LayoutElementHashMapItem *parentHashMapItem = Clay__GetHashMapItem(data->parentId);
                // Don't exit transition if the parent has also exited and SKIP_WHEN_PARENT_EXITS is used
                if (config->exit.trigger == CLAY_TRANSITION_EXIT_TRIGGER_WHEN_PARENT_EXITS || !parentHashMapItem || parentHashMapItem->generation > context->generation) {
                    if (data->state != CLAY_TRANSITION_STATE_EXITING) {
                        if (parentHashMapItem->generation <= context->generation) {
                            data->elementThisFrame->config.floating.attachTo = CLAY_ATTACH_TO_ROOT;
                            data->elementThisFrame->config.floating.offset = CLAY__INIT(Clay_Vector2) { hashMapItem->boundingBox.x, hashMapItem->boundingBox.y };
                        }
                        data->elementThisFrame->exiting = true;
                        data->elementThisFrame->config.layout.sizing.width = CLAY_SIZING_FIXED(data->elementThisFrame->dimensions.width);
                        data->elementThisFrame->config.layout.sizing.height = CLAY_SIZING_FIXED(data->elementThisFrame->dimensions.height);
                        data->state = CLAY_TRANSITION_STATE_EXITING;
                        data->activeProperties = config->properties;
                        data->elapsedTime = 0;
                        data->targetState = config->exit.setFinalState(data->targetState, config->properties);
                    }

                    // Clone the entire subtree back into the main UI layout tree
                    Clay__int32_tArray bfsBuffer = context->openLayoutElementStack;
                    bfsBuffer.length = 0;
                    data->elementThisFrame = Clay_LayoutElementArray_Add(&context->layoutElements, *data->elementThisFrame);
                    int32_t exitingElementIndex = data->elementThisFrame - context->layoutElements.internalArray;
                    Clay__StringArray_Add(&context->layoutElementIdStrings, *Clay__StringArray_GetCheckCapacity(&context->layoutElementIdStrings, exitingElementIndex));
                    Clay__int32_tArray_Add(&context->layoutElementClipElementIds, *Clay__int32_tArray_GetCheckCapacity(&context->layoutElementClipElementIds, exitingElementIndex));
                    Clay__int32_tArray_Add(&bfsBuffer, exitingElementIndex);
                    hashMapItem->layoutElement = data->elementThisFrame;
                    hashMapItem->generation = context->generation + 1;
                    int32_t bufferIndex = 0;
                    while (bufferIndex < bfsBuffer.length) {
                        Clay_LayoutElement *layoutElement = Clay_LayoutElementArray_GetCheckCapacity(&context->layoutElements, Clay__int32_tArray_GetValue(&bfsBuffer, bufferIndex));
                        bufferIndex++;
                        int32_t firstChildSlot = context->layoutElementChildren.length;
                        for (int j = 0; j < layoutElement->children.length; ++j) {
                            Clay_LayoutElement* childElement = Clay_LayoutElementArray_GetCheckCapacity(&context->layoutElements, layoutElement->children.elements[j]);
                            int32_t childElementIndex = childElement - context->layoutElements.internalArray;
                            Clay_LayoutElement* newChildElement = Clay_LayoutElementArray_Add(&context->layoutElements, *childElement);
                            Clay__StringArray_Add(&context->layoutElementIdStrings, *Clay__StringArray_GetCheckCapacity(&context->layoutElementIdStrings, childElementIndex));
                            Clay__int32_tArray_Add(&context->layoutElementClipElementIds, *Clay__int32_tArray_GetCheckCapacity(&context->layoutElementClipElementIds, childElementIndex));
                            Clay__int32_tArray_Add(&bfsBuffer, context->layoutElements.length - 1);
                            if (newChildElement->isTextElement) {
                                newChildElement->textElementData.wrappedLines.length = 0;
                            }
                            Clay__int32_tArray_Add(&context->layoutElementChildren, context->layoutElements.length - 1);
                        }
                        layoutElement->children.elements = &context->layoutElementChildren.internalArray[firstChildSlot];
                    }

                    // Reattach the inserted subtree to its previous parent if it still exists
                    if (parentHashMapItem->generation > context->generation) {
                        Clay_LayoutElement *parentElement = parentHashMapItem->layoutElement;
                        int32_t newChildrenStartIndex = context->layoutElementChildren.length;
                        bool found = false;
                        if (config->exit.siblingOrdering == CLAY_EXIT_TRANSITION_ORDERING_UNDERNEATH_SIBLINGS) {
                            Clay__int32_tArray_Add(&context->layoutElementChildren, exitingElementIndex);
                            found = true;
                        }
                        for (int j = 0; j < parentElement->children.length; ++j) {
                            if (config->exit.siblingOrdering == CLAY_EXIT_TRANSITION_ORDERING_NATURAL_ORDER && j == data->siblingIndex) {
                                Clay__int32_tArray_Add(&context->layoutElementChildren, exitingElementIndex);
                                found = true;
                            }
                            Clay__int32_tArray_Add(&context->layoutElementChildren, parentElement->children.elements[j]);
                        }
                        if (!found) {
                            Clay__int32_tArray_Add(&context->layoutElementChildren, exitingElementIndex);
                        }
                        parentElement->children.length++;
                        parentElement->children.elements = &context->layoutElementChildren.internalArray[newChildrenStartIndex];
                    // Otherwise, just attach to the root as a floating element
                    } else {
                        Clay__LayoutElementTreeRootArray_Add(&context->layoutElementTreeRoots, CLAY__INIT(Clay__LayoutElementTreeRoot) {
                            .layoutElementIndex = (int32_t)(data->elementThisFrame - context->layoutElements.internalArray),
                            .parentId = Clay__HashString(CLAY_STRING("Clay__RootContainer"), 0).id,
                            .zIndex = 1,
                        });
                    }
                // Parent exited, just delete child without exit transition
                } else {
                    Clay__TransitionDataInternalArray_RemoveSwapback(&context->transitionDatas, i);
                    i--;
                    continue;
                }
            }
        // Transition element exited and doesn't have an exit handler defined
        // Or, the user deleted the transition handler from one frame to the next
        } else if (hashMapItem->generation <= context->generation || !hashMapItem->layoutElement->config.transition.handler) {
            Clay__TransitionDataInternalArray_RemoveSwapback(&context->transitionDatas, i);
            i--;
            continue;
        }
    }

    if (context->booleanWarnings.maxElementsExceeded) {
        Clay_String message;
        message = CLAY_STRING("Clay Error: Layout elements exceeded Clay__maxElementCount");
        Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand ) {
            .boundingBox = { context->layoutDimensions.width / 2 - 59 * 4, context->layoutDimensions.height / 2, 0, 0 },
            .renderData = { .text = { .stringContents = CLAY__INIT(Clay_StringSlice) { .length = message.length, .chars = message.chars, .baseChars = message.chars }, .textColor = {255, 0, 0, 255}, .fontSize = 16 } },
            .commandType = CLAY_RENDER_COMMAND_TYPE_TEXT
        });
    } else {
        if (context->transitionDatas.length > 0) {
            Clay__CalculateFinalLayout(deltaTime, false, false);

            for (int i = 0; i < context->transitionDatas.length; ++i) {
                Clay__TransitionDataInternal* transitionData = Clay__TransitionDataInternalArray_Get(&context->transitionDatas, i);
                Clay_LayoutElement* currentElement = transitionData->elementThisFrame;
                Clay_LayoutElementHashMapItem* mapItem = Clay__GetHashMapItem(transitionData->elementId);
                Clay_LayoutElementHashMapItem* parentMapItem = Clay__GetHashMapItem(transitionData->parentId);
                Clay_TransitionData targetState = transitionData->targetState;
                if (transitionData->state != CLAY_TRANSITION_STATE_EXITING) {
                    targetState = CLAY__INIT(Clay_TransitionData) {
                            mapItem->boundingBox,
                            currentElement->config.backgroundColor,
                            currentElement->config.overlayColor,
                            currentElement->config.border.color,
                            currentElement->config.border.width,
                    };
                }
                Clay_TransitionData oldTargetState = transitionData->targetState;
                transitionData->targetState = targetState;
                if (mapItem->appearedThisFrame) {
                    if (currentElement->config.transition.enter.setInitialState && !(parentMapItem->appearedThisFrame && currentElement->config.transition.enter.trigger == CLAY_TRANSITION_ENTER_SKIP_ON_FIRST_PARENT_FRAME)) {
                        transitionData->state = CLAY_TRANSITION_STATE_ENTERING;
                        transitionData->initialState = currentElement->config.transition.enter.setInitialState(transitionData->targetState, currentElement->config.transition.properties);
                        transitionData->currentState = transitionData->initialState;
                        transitionData->activeProperties = currentElement->config.transition.properties;
                        Clay_ApplyTransitionedPropertiesToElement(currentElement, currentElement->config.transition.properties, transitionData->initialState, &mapItem->boundingBox, transitionData->reparented);
                    } else {
                        transitionData->initialState = targetState;
                        transitionData->currentState = targetState;
                        transitionData->activeProperties = CLAY_TRANSITION_PROPERTY_NONE;
                    }
                } else {
                    Clay_Vector2 parentScrollOffset = parentMapItem->layoutElement->config.clip.childOffset;
                    Clay_Vector2 newRelativePosition = {
                        mapItem->boundingBox.x - parentMapItem->boundingBox.x - parentScrollOffset.x,
                        mapItem->boundingBox.y - parentMapItem->boundingBox.y - parentScrollOffset.y,
                    };
                    Clay_Vector2 oldRelativePosition = transitionData->oldParentRelativePosition;
                    transitionData->oldParentRelativePosition = newRelativePosition;
                    Clay_TransitionProperty properties = currentElement->config.transition.properties;
                    int32_t newActiveProperties = CLAY_TRANSITION_PROPERTY_NONE;
                    if (properties & CLAY_TRANSITION_PROPERTY_X) {
                        // Don't trigger a transition if...
                        if (
                            // The element's absolute position didn't change
                            !Clay__FloatEqual(oldTargetState.boundingBox.x, targetState.boundingBox.x)
                            // The element is still in the same parent container, and it's parent-relative position didn't change (parent moved)
                            && (!(Clay__FloatEqual(oldRelativePosition.x, newRelativePosition.x)) || transitionData->reparented)
                            // The position changed was triggered by the outer window resizing
                            && !context->rootResizedLastFrame
                        ) {
                            newActiveProperties |= CLAY_TRANSITION_PROPERTY_X;
                        }
                    }
                    if (properties & CLAY_TRANSITION_PROPERTY_Y) {
                        // See extended comments above in PROPERTY_X for explanation
                        if (!Clay__FloatEqual(oldTargetState.boundingBox.y, targetState.boundingBox.y) && (!(Clay__FloatEqual(oldRelativePosition.y, newRelativePosition.y)) || transitionData->reparented) && !context->rootResizedLastFrame) {
                            newActiveProperties |= CLAY_TRANSITION_PROPERTY_Y;
                        }
                    }
                    if (properties & CLAY_TRANSITION_PROPERTY_WIDTH) {
                        if (!Clay__FloatEqual(oldTargetState.boundingBox.width, targetState.boundingBox.width) && !context->rootResizedLastFrame) {
                            newActiveProperties |= CLAY_TRANSITION_PROPERTY_WIDTH;
                        }
                    }
                    if (properties & CLAY_TRANSITION_PROPERTY_HEIGHT) {
                        if (!Clay__FloatEqual(oldTargetState.boundingBox.height, targetState.boundingBox.height) && !context->rootResizedLastFrame) {
                            newActiveProperties |= CLAY_TRANSITION_PROPERTY_HEIGHT;
                        }
                    }
                    if (properties & CLAY_TRANSITION_PROPERTY_BACKGROUND_COLOR) {
                        if (!Clay__MemCmp((char *) &oldTargetState.backgroundColor, (char *)&targetState.backgroundColor, sizeof(Clay_Color))) {
                            newActiveProperties |= CLAY_TRANSITION_PROPERTY_BACKGROUND_COLOR;
                        }
                    }
                    if (properties & CLAY_TRANSITION_PROPERTY_OVERLAY_COLOR) {
                        if (!Clay__MemCmp((char *) &oldTargetState.overlayColor, (char *)&targetState.overlayColor, sizeof(Clay_Color))) {
                            newActiveProperties |= CLAY_TRANSITION_PROPERTY_OVERLAY_COLOR;
                        }
                    }
                    if (properties & CLAY_TRANSITION_PROPERTY_BORDER_COLOR) {
                        if (!Clay__MemCmp((char *) &oldTargetState.borderColor, (char *)&targetState.borderColor, sizeof(Clay_Color))) {
                            newActiveProperties |= CLAY_TRANSITION_PROPERTY_BORDER_COLOR;
                        }
                    }
                    if (properties & CLAY_TRANSITION_PROPERTY_BORDER_WIDTH) {
                        if (!Clay__MemCmp((char *) &oldTargetState.borderWidth, (char *)&targetState.borderWidth, sizeof(Clay_BorderWidth))) {
                            newActiveProperties |= CLAY_TRANSITION_PROPERTY_BORDER_WIDTH;
                        }
                    }

                    if (newActiveProperties != 0 && transitionData->state != CLAY_TRANSITION_STATE_EXITING) {
                        transitionData->elapsedTime = 0;
                        transitionData->initialState = transitionData->currentState;
                        transitionData->state = CLAY_TRANSITION_STATE_TRANSITIONING;
                        transitionData->activeProperties = (Clay_TransitionProperty)(transitionData->activeProperties | newActiveProperties);
                    }

                    if (transitionData->state == CLAY_TRANSITION_STATE_IDLE) {
                        transitionData->initialState = targetState;
                        transitionData->currentState = targetState;
                        transitionData->targetState = targetState;
                        transitionData->activeProperties = CLAY_TRANSITION_PROPERTY_NONE;
                    } else {
                        bool transitionComplete = true;
                        transitionComplete = currentElement->config.transition.handler(CLAY__INIT(Clay_TransitionCallbackArguments) {
                            transitionData->state,
                            transitionData->initialState,
                            &transitionData->currentState,
                            targetState,
                            transitionData->elapsedTime,
                            currentElement->config.transition.duration,
                            transitionData->activeProperties
                        });

                        Clay_ApplyTransitionedPropertiesToElement(currentElement, transitionData->activeProperties, transitionData->currentState, &mapItem->boundingBox, transitionData->reparented);
                        transitionData->elapsedTime += deltaTime;

                        if (transitionComplete) {
                            if (transitionData->state == CLAY_TRANSITION_STATE_ENTERING || transitionData->state == CLAY_TRANSITION_STATE_TRANSITIONING) {transitionData->state = CLAY_TRANSITION_STATE_IDLE;
                                transitionData->elapsedTime = 0;
                                transitionData->reparented = false;
                                transitionData->activeProperties = CLAY_TRANSITION_PROPERTY_NONE;
                            } else if (transitionData->state == CLAY_TRANSITION_STATE_EXITING) {
                                Clay__TransitionDataInternalArray_RemoveSwapback(&context->transitionDatas, i);
                            }
                        }
                    }
                }
            }

            if (context->debugModeEnabled) {
                context->warningsEnabled = false;
                Clay__RenderDebugView();
                context->warningsEnabled = true;
            }

            if (context->booleanWarnings.maxElementsExceeded) {
                Clay_String message;
                message = CLAY_STRING("Clay Error: Debug view caused layout element count to exceed Clay__maxElementCount");
                Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand ) {
                        .boundingBox = { context->layoutDimensions.width / 2 - 59 * 4, context->layoutDimensions.height / 2, 0, 0 },
                        .renderData = { .text = { .stringContents = CLAY__INIT(Clay_StringSlice) { .length = message.length, .chars = message.chars, .baseChars = message.chars }, .textColor = {255, 0, 0, 255}, .fontSize = 16 } },
                        .commandType = CLAY_RENDER_COMMAND_TYPE_TEXT
                });
            } else {
                Clay__CalculateFinalLayout(deltaTime, true, true);
                Clay__CloneElementsWithExitTransition();
            }
        } else {
            if (context->debugModeEnabled) {
                context->warningsEnabled = false;
                Clay__RenderDebugView();
                context->warningsEnabled = true;
            }

            if (context->booleanWarnings.maxElementsExceeded) {
                Clay_String message;
                message = CLAY_STRING("Clay Error: Debug view caused layout element count to exceed Clay__maxElementCount");
                Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand ) {
                    .boundingBox = { context->layoutDimensions.width / 2 - 59 * 4, context->layoutDimensions.height / 2, 0, 0 },
                    .renderData = { .text = { .stringContents = CLAY__INIT(Clay_StringSlice) { .length = message.length, .chars = message.chars, .baseChars = message.chars }, .textColor = {255, 0, 0, 255}, .fontSize = 16 } },
                    .commandType = CLAY_RENDER_COMMAND_TYPE_TEXT
                });
            } else {
                Clay__CalculateFinalLayout(deltaTime, false, true);
            }
        }
    }
    if (context->openLayoutElementStack.length > 1) {
        context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                .errorType = CLAY_ERROR_TYPE_UNBALANCED_OPEN_CLOSE,
                .errorText = CLAY_STRING("There were still open layout elements when EndLayout was called. This results from an unequal number of calls to Clay__OpenElement and Clay__CloseElement."),
                .userData = context->errorHandler.userData });
    }

    for (int i = 0; i < context->layoutElementsHashMap.capacity; ++i) {
        int32_t currentElementIndex = context->layoutElementsHashMap.internalArray[i];
        int32_t previousElementIndex = -1;
        int32_t listDepth = 0;
        while (currentElementIndex != -1) {
            Clay_LayoutElementHashMapItem* currentItem = Clay__LayoutElementHashMapItemArray_Get(&context->layoutElementsHashMapInternal, currentElementIndex);
            int32_t nextIndex = currentItem->nextIndex;
            // Needs to be pruned
            if (currentItem->generation <= context->generation) {
                // If it's the very top of the bucket, rewrite the first bucket pointer
                if (listDepth == 0) {
                    Clay__int32_tArray_Set(&context->layoutElementsHashMap, i, nextIndex);
                    listDepth--;
                } else {
                    // Rewrite previous pointer
                    Clay_LayoutElementHashMapItem* previousItem = Clay__LayoutElementHashMapItemArray_Get(&context->layoutElementsHashMapInternal, previousElementIndex);
                    previousItem->nextIndex = nextIndex;
                }
                // Delete the underlying item and add it to the freelist
                Clay__LayoutElementHashMapItemArray_Set(&context->layoutElementsHashMapInternal, currentElementIndex, CLAY__INIT(Clay_LayoutElementHashMapItem) { .nextIndex = -1 });
                Clay__int32_tArray_Add(&context->layoutElementsHashMapFreeList, currentElementIndex);
            }

            previousElementIndex = currentElementIndex;
            currentElementIndex = nextIndex;
            listDepth++;
        }
    }

    return context->renderCommands;
}

CLAY_WASM_EXPORT("Clay_GetOpenElementId")
uint32_t Clay_GetOpenElementId(void) {
    return Clay__GetOpenLayoutElement()->id;
}

CLAY_WASM_EXPORT("Clay_GetElementId")
Clay_ElementId Clay_GetElementId(Clay_String idString) {
    return Clay__HashString(idString, 0);
}

CLAY_WASM_EXPORT("Clay_GetElementIdWithIndex")
Clay_ElementId Clay_GetElementIdWithIndex(Clay_String idString, uint32_t index) {
    return Clay__HashStringWithOffset(idString, index, 0);
}

bool Clay_Hovered(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return false;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    for (int32_t i = 0; i < context->pointerOverIds.length; ++i) {
        if (Clay_ElementIdArray_Get(&context->pointerOverIds, i)->id == openLayoutElement->id) {
            return true;
        }
    }
    return false;
}

void Clay_OnHover(void (*onHoverFunction)(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData), void *userData) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    Clay_LayoutElementHashMapItem *hashMapItem = Clay__GetHashMapItem(openLayoutElement->id);
    hashMapItem->onHoverFunction = onHoverFunction;
    hashMapItem->hoverFunctionUserData = userData;
}

CLAY_WASM_EXPORT("Clay_PointerOver")
bool Clay_PointerOver(Clay_ElementId elementId) { // TODO return priority for separating multiple results
    Clay_Context* context = Clay_GetCurrentContext();
    for (int32_t i = 0; i < context->pointerOverIds.length; ++i) {
        if (Clay_ElementIdArray_Get(&context->pointerOverIds, i)->id == elementId.id) {
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
            if (!scrollContainerData->layoutElement) { // This can happen on the first frame before a scroll container is declared
                return CLAY__INIT(Clay_ScrollContainerData) CLAY__DEFAULT_STRUCT;
            }
            return CLAY__INIT(Clay_ScrollContainerData) {
                .scrollPosition = &scrollContainerData->scrollPosition,
                .scrollContainerDimensions = { scrollContainerData->boundingBox.width, scrollContainerData->boundingBox.height },
                .contentDimensions = scrollContainerData->contentSize,
                .config = scrollContainerData->layoutElement->config.clip,
                .found = true
            };
        }
    }
    return CLAY__INIT(Clay_ScrollContainerData) CLAY__DEFAULT_STRUCT;
}

CLAY_WASM_EXPORT("Clay_GetElementData")
Clay_ElementData Clay_GetElementData(Clay_ElementId id){
    Clay_LayoutElementHashMapItem * item = Clay__GetHashMapItem(id.id);
    if(item == &Clay_LayoutElementHashMapItem_DEFAULT) {
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
        Clay__defaultMaxMeasureTextWordCacheCount = maxElementCount * 2;
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

#define CLAY__LERP(from, to, mix) (from + (to - from) * mix)

CLAY_DLL_EXPORT bool Clay_EaseOut(Clay_TransitionCallbackArguments arguments) {
    float ratio = 1;
    if (arguments.duration > 0) {
        ratio = CLAY__MIN(arguments.elapsedTime / arguments.duration, 1);
    }
    float inverse = 1.0f - ratio;
    float lerpAmount = 1.0f - (inverse * inverse * inverse);
    if (arguments.properties & CLAY_TRANSITION_PROPERTY_X) {
        arguments.current->boundingBox.x = CLAY__LERP(arguments.initial.boundingBox.x, arguments.target.boundingBox.x, lerpAmount);
    }
    if (arguments.properties & CLAY_TRANSITION_PROPERTY_Y) {
        arguments.current->boundingBox.y = CLAY__LERP(arguments.initial.boundingBox.y, arguments.target.boundingBox.y, lerpAmount);
    }
    if (arguments.properties & CLAY_TRANSITION_PROPERTY_WIDTH) {
        arguments.current->boundingBox.width = CLAY__LERP(arguments.initial.boundingBox.width, arguments.target.boundingBox.width, lerpAmount);
    }
    if (arguments.properties & CLAY_TRANSITION_PROPERTY_HEIGHT) {
        arguments.current->boundingBox.height = CLAY__LERP(arguments.initial.boundingBox.height, arguments.target.boundingBox.height, lerpAmount);
    }
    if (arguments.properties & CLAY_TRANSITION_PROPERTY_BACKGROUND_COLOR) {
        arguments.current->backgroundColor = CLAY__INIT(Clay_Color) {
            .r = CLAY__LERP(arguments.initial.backgroundColor.r, arguments.target.backgroundColor.r, lerpAmount),
            .g = CLAY__LERP(arguments.initial.backgroundColor.g, arguments.target.backgroundColor.g, lerpAmount),
            .b = CLAY__LERP(arguments.initial.backgroundColor.b, arguments.target.backgroundColor.b, lerpAmount),
            .a = CLAY__LERP(arguments.initial.backgroundColor.a, arguments.target.backgroundColor.a, lerpAmount),
        };
    }
    if (arguments.properties & CLAY_TRANSITION_PROPERTY_OVERLAY_COLOR) {
        arguments.current->overlayColor = CLAY__INIT(Clay_Color) {
            .r = CLAY__LERP(arguments.initial.overlayColor.r, arguments.target.overlayColor.r, lerpAmount),
            .g = CLAY__LERP(arguments.initial.overlayColor.g, arguments.target.overlayColor.g, lerpAmount),
            .b = CLAY__LERP(arguments.initial.overlayColor.b, arguments.target.overlayColor.b, lerpAmount),
            .a = CLAY__LERP(arguments.initial.overlayColor.a, arguments.target.overlayColor.a, lerpAmount),
        };
    }
    if (arguments.properties & CLAY_TRANSITION_PROPERTY_BORDER_COLOR) {
        arguments.current->borderColor = CLAY__INIT(Clay_Color) {
            .r = CLAY__LERP(arguments.initial.borderColor.r, arguments.target.borderColor.r, lerpAmount),
            .g = CLAY__LERP(arguments.initial.borderColor.g, arguments.target.borderColor.g, lerpAmount),
            .b = CLAY__LERP(arguments.initial.borderColor.b, arguments.target.borderColor.b, lerpAmount),
            .a = CLAY__LERP(arguments.initial.borderColor.a, arguments.target.borderColor.a, lerpAmount),
        };
    }
    if (arguments.properties & CLAY_TRANSITION_PROPERTY_BORDER_WIDTH) {
        arguments.current->borderWidth = CLAY__INIT(Clay_BorderWidth) {
            .left = (uint16_t)CLAY__LERP(arguments.initial.borderWidth.left, arguments.target.borderWidth.left, lerpAmount),
            .right = (uint16_t)CLAY__LERP(arguments.initial.borderWidth.right, arguments.target.borderWidth.right, lerpAmount),
            .top = (uint16_t)CLAY__LERP(arguments.initial.borderWidth.top, arguments.target.borderWidth.top, lerpAmount),
            .bottom = (uint16_t)CLAY__LERP(arguments.initial.borderWidth.bottom, arguments.target.borderWidth.bottom, lerpAmount),
            .betweenChildren = (uint16_t)CLAY__LERP(arguments.initial.borderWidth.betweenChildren, arguments.target.borderWidth.betweenChildren, lerpAmount),
        };
    }
    return ratio >= 1;
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
