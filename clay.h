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

// SIMD includes on supported platforms
#if !defined(CLAY_DISABLE_SIMD) && (defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64))
#include <emmintrin.h>
#elif !defined(CLAY_DISABLE_SIMD) && defined(__aarch64__)
#include <arm_neon.h>
#endif

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

#define CLAY__MAX(x, y) (((x) > (y)) ? (x) : (y))
#define CLAY__MIN(x, y) (((x) < (y)) ? (x) : (y))

#define CLAY_TEXT_CONFIG(...) Clay__StoreTextElementConfig(CLAY__CONFIG_WRAPPER(Clay_TextElementConfig, __VA_ARGS__))

#define CLAY_BORDER_OUTSIDE(widthValue) {widthValue, widthValue, widthValue, widthValue, 0}

#define CLAY_BORDER_ALL(widthValue) {widthValue, widthValue, widthValue, widthValue, widthValue}

#define CLAY_CORNER_RADIUS(radius) (CLAY__INIT(Clay_CornerRadius) { radius, radius, radius, radius })

#define CLAY_PADDING_ALL(padding) CLAY__CONFIG_WRAPPER(Clay_Padding, { padding, padding, padding, padding })

#define CLAY_SIZING_FIT(...) (CLAY__INIT(Clay_SizingAxis) { .size = { .minMax = { __VA_ARGS__ } }, .type = CLAY__SIZING_TYPE_FIT })

#define CLAY_SIZING_GROW(...) (CLAY__INIT(Clay_SizingAxis) { .size = { .minMax = { __VA_ARGS__ } }, .type = CLAY__SIZING_TYPE_GROW })

#define CLAY_SIZING_FIXED(fixedSize) (CLAY__INIT(Clay_SizingAxis) { .size = { .minMax = { fixedSize, fixedSize } }, .type = CLAY__SIZING_TYPE_FIXED })

#define CLAY_SIZING_PERCENT(percentOfParent) (CLAY__INIT(Clay_SizingAxis) { .size = { .percent = (percentOfParent) }, .type = CLAY__SIZING_TYPE_PERCENT })

#define CLAY_ID(label) CLAY_IDI(label, 0)

#define CLAY_IDI(label, index) Clay__HashString(CLAY_STRING(label), index, 0)

#define CLAY_ID_LOCAL(label) CLAY_IDI_LOCAL(label, 0)

#define CLAY_IDI_LOCAL(label, index) Clay__HashString(CLAY_STRING(label), index, Clay__GetParentElementId())

#define CLAY__STRING_LENGTH(s) ((sizeof(s) / sizeof((s)[0])) - sizeof((s)[0]))

#define CLAY__ENSURE_STRING_LITERAL(x) ("" x "")

// Note: If an error led you here, it's because CLAY_STRING can only be used with string literals, i.e. CLAY_STRING("SomeString") and not CLAY_STRING(yourString)
#define CLAY_STRING(string) (CLAY__INIT(Clay_String) { .length = CLAY__STRING_LENGTH(CLAY__ENSURE_STRING_LITERAL(string)), .chars = (string) })

#define CLAY_STRING_CONST(string) { .length = CLAY__STRING_LENGTH(CLAY__ENSURE_STRING_LITERAL(string)), .chars = (string) }

static uint8_t CLAY__ELEMENT_DEFINITION_LATCH;

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

  Clay_OpenElement();
  Clay_ConfigureOpenElement((Clay_ElementDeclaration) {
    .id = CLAY_ID("Container"),
    .backgroundColor = { 255, 200, 200, 255 }
  });
  ...children declared here
  Clay_CloseElement();

  The for loop will only ever run a single iteration, putting Clay__CloseElement() in the increment of the loop
  means that it will run after the body - where the children are declared. It just exists to make sure you don't forget
  to call Clay_CloseElement().
*/
#define CLAY(...)                                                                                                                                           \
    for (                                                                                                                                                   \
        CLAY__ELEMENT_DEFINITION_LATCH = (Clay__OpenElement(), Clay__ConfigureOpenElement(CLAY__CONFIG_WRAPPER(Clay_ElementDeclaration, __VA_ARGS__)), 0);  \
        CLAY__ELEMENT_DEFINITION_LATCH < 1;                                                                                                                 \
        ++CLAY__ELEMENT_DEFINITION_LATCH, Clay__CloseElement()                                                                                              \
    )

// These macros exist to allow the CLAY() macro to be called both with an inline struct definition, such as
// CLAY({ .id = something... });
// As well as by passing a predefined declaration struct
// Clay_ElementDeclaration declarationStruct = ...
// CLAY(declarationStruct);
#define CLAY__WRAPPER_TYPE(type) Clay__##type##Wrapper
#define CLAY__WRAPPER_STRUCT(type) typedef struct { type wrapped; } CLAY__WRAPPER_TYPE(type)
#define CLAY__CONFIG_WRAPPER(type, ...) (CLAY__INIT(CLAY__WRAPPER_TYPE(type)) { __VA_ARGS__ }).wrapped

#define CLAY_TEXT(text, textConfig) Clay__OpenTextElement(text, textConfig)

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
typedef struct {
    int32_t length;
    // The underlying character memory. Note: this will not be copied and will not extend the lifetime of the underlying memory.
    const char *chars;
} Clay_String;

// Clay_StringSlice is used to represent non owning string slices, and includes
// a baseChars field which points to the string this slice is derived from.
typedef struct {
    int32_t length;
    const char *chars;
    const char *baseChars; // The source string / char* that this slice was derived from
} Clay_StringSlice;

typedef struct Clay_Context Clay_Context;

// Clay_Arena is a memory arena structure that is used by clay to manage its internal allocations.
// Rather than creating it by hand, it's easier to use Clay_CreateArenaWithCapacityAndMemory()
typedef struct {
    uintptr_t nextAllocation;
    size_t capacity;
    char *memory;
} Clay_Arena;

typedef struct {
    float width, height;
} Clay_Dimensions;

typedef struct {
    float x, y;
} Clay_Vector2;

// Internally clay conventionally represents colors as 0-255, but interpretation is up to the renderer.
typedef struct {
    float r, g, b, a;
} Clay_Color;

typedef struct {
    float x, y, width, height;
} Clay_BoundingBox;

// Primarily created via the CLAY_ID(), CLAY_IDI(), CLAY_ID_LOCAL() and CLAY_IDI_LOCAL() macros.
// Represents a hashed string ID used for identifying and finding specific clay UI elements, required
// by functions such as Clay_PointerOver() and Clay_GetElementData().
typedef struct {
    uint32_t id; // The resulting hash generated from the other fields.
    uint32_t offset; // A numerical offset applied after computing the hash from stringId.
    uint32_t baseId; // A base hash value to start from, for example the parent element ID is used when calculating CLAY_ID_LOCAL().
    Clay_String stringId; // The string id to hash.
} Clay_ElementId;

// Controls the "radius", or corner rounding of elements, including rectangles, borders and images.
// The rounding is determined by drawing a circle inset into the element corner by (radius, radius) pixels.
typedef struct {
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
    // Aligns child elements vertiically to the center of this element
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
typedef struct {
    Clay_LayoutAlignmentX x; // Controls alignment of children along the x axis.
    Clay_LayoutAlignmentY y; // Controls alignment of children along the y axis.
} Clay_ChildAlignment;

// Controls the minimum and maximum size in pixels that this element is allowed to grow or shrink to,
// overriding sizing types such as FIT or GROW.
typedef struct {
    float min; // The smallest final size of the element on this axis will be this value in pixels.
    float max; // The largest final size of the element on this axis will be this value in pixels.
} Clay_SizingMinMax;

// Controls the sizing of this element along one axis inside its parent container.
typedef struct {
    union {
        Clay_SizingMinMax minMax; // Controls the minimum and maximum size in pixels that this element is allowed to grow or shrink to, overriding sizing types such as FIT or GROW.
        float percent; // Expects 0-1 range. Clamps the axis size to a percent of the parent container's axis size minus padding and child gaps.
    } size;
    Clay__SizingType type; // Controls how the element takes up space inside its parent container.
} Clay_SizingAxis;

// Controls the sizing of this element along one axis inside its parent container.
typedef struct {
    Clay_SizingAxis width; // Controls the width sizing of the element, along the x axis.
    Clay_SizingAxis height;  // Controls the height sizing of the element, along the y axis.
} Clay_Sizing;

// Controls "padding" in pixels, which is a gap between the bounding box of this element and where its children
// will be placed.
typedef struct {
    uint16_t left;
    uint16_t right;
    uint16_t top;
    uint16_t bottom;
} Clay_Padding;

CLAY__WRAPPER_STRUCT(Clay_Padding);

// Controls various settings that affect the size and position of an element, as well as the sizes and positions
// of any child elements.
typedef struct {
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
typedef struct {
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
    // When set to true, clay will hash the entire text contents of this string as an identifier for its internal
    // text measurement cache, rather than just the pointer and length. This will incur significant performance cost for
    // long bodies of text.
    bool hashStringContents;
} Clay_TextElementConfig;

CLAY__WRAPPER_STRUCT(Clay_TextElementConfig);

// Image --------------------------------

// Controls various settings related to image elements.
typedef struct {
    void* imageData; // A transparent pointer used to pass image data through to the renderer.
    Clay_Dimensions sourceDimensions; // The original dimensions of the source image, used to control aspect ratio.
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
typedef struct {
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

// Controls various settings related to "floating" elements, which are elements that "float" above other elements, potentially overlapping their boundaries,
// and not affecting the layout of sibling or parent elements.
typedef struct {
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
} Clay_FloatingElementConfig;

CLAY__WRAPPER_STRUCT(Clay_FloatingElementConfig);

// Custom -----------------------------

// Controls various settings related to custom elements.
typedef struct {
    // A transparent pointer through which you can pass custom data to the renderer.
    // Generates CUSTOM render commands.
    void* customData;
} Clay_CustomElementConfig;

CLAY__WRAPPER_STRUCT(Clay_CustomElementConfig);

// Scroll -----------------------------

// Controls the axis on which an element switches to "scrolling", which clips the contents and allows scrolling in that direction.
typedef struct {
    bool horizontal; // Clip overflowing elements on the X axis and allow scrolling left and right.
    bool vertical; // Clip overflowing elements on the YU axis and allow scrolling up and down.
} Clay_ScrollElementConfig;

CLAY__WRAPPER_STRUCT(Clay_ScrollElementConfig);

// Border -----------------------------

// Controls the widths of individual element borders.
typedef struct {
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
typedef struct {
    Clay_Color color; // Controls the color of all borders with width > 0. Conventionally represented as 0-255, but interpretation is up to the renderer.
    Clay_BorderWidth width; // Controls the widths of individual borders. At least one of these should be > 0 for a BORDER render command to be generated.
} Clay_BorderElementConfig;

CLAY__WRAPPER_STRUCT(Clay_BorderElementConfig);

// Render Command Data -----------------------------

// Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_TEXT
typedef struct {
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
typedef struct {
    // The solid background color to fill this rectangle with. Conventionally represented as 0-255 for each channel, but interpretation is up to the renderer.
    Clay_Color backgroundColor;
    // Controls the "radius", or corner rounding of elements, including rectangles, borders and images.
    // The rounding is determined by drawing a circle inset into the element corner by (radius, radius) pixels.
    Clay_CornerRadius cornerRadius;
} Clay_RectangleRenderData;

// Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_IMAGE
typedef struct {
    // The tint color for this image. Note that the default value is 0,0,0,0 and should likely be interpreted
    // as "untinted".
    // Conventionally represented as 0-255 for each channel, but interpretation is up to the renderer.
    Clay_Color backgroundColor;
    // Controls the "radius", or corner rounding of this image.
    // The rounding is determined by drawing a circle inset into the element corner by (radius, radius) pixels.
    Clay_CornerRadius cornerRadius;
    // The original dimensions of the source image, used to control aspect ratio.
    Clay_Dimensions sourceDimensions;
    // A pointer transparently passed through from the original element definition, typically used to represent image data.
    void* imageData;
} Clay_ImageRenderData;

// Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_CUSTOM
typedef struct {
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
typedef struct {
    bool horizontal;
    bool vertical;
} Clay_ScrollRenderData;

// Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_BORDER
typedef struct {
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
typedef union {
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
    // Render command data when commandType == CLAY_RENDER_COMMAND_TYPE_SCROLL
    Clay_ScrollRenderData scroll;
} Clay_RenderData;

// Miscellaneous Structs & Enums ---------------------------------

// Data representing the current internal state of a scrolling element.
typedef struct {
    // Note: This is a pointer to the real internal scroll position, mutating it may cause a change in final layout.
    // Intended for use with external functionality that modifies scroll position, such as scroll bars or auto scrolling.
    Clay_Vector2 *scrollPosition;
    // The bounding box of the scroll element.
    Clay_Dimensions scrollContainerDimensions;
    // The outer dimensions of the inner scroll container content, including the padding of the parent scroll container.
    Clay_Dimensions contentDimensions;
    // The config that was originally passed to the scroll element.
    Clay_ScrollElementConfig config;
    // Indicates whether an actual scroll container matched the provided ID or if the default struct was returned.
    bool found;
} Clay_ScrollContainerData;

// Bounding box and other data for a specific UI element.
typedef struct {
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
    // The renderer should provide a custom implementation for handling this render command based on its .customData
    CLAY_RENDER_COMMAND_TYPE_CUSTOM,
} Clay_RenderCommandType;

typedef struct {
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
typedef struct {
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
typedef struct {
    // The position of the mouse / touch / pointer relative to the root of the layout.
    Clay_Vector2 position;
    // Represents the current state of interaction with clay this frame.
    // CLAY_POINTER_DATA_PRESSED_THIS_FRAME - A left mouse click, or touch occurred this frame.
    // CLAY_POINTER_DATA_PRESSED - The left mouse button click or touch happened at some point in the past, and is still currently held down this frame.
    // CLAY_POINTER_DATA_RELEASED_THIS_FRAME - The left mouse button click or touch was released this frame.
    // CLAY_POINTER_DATA_RELEASED - The left mouse button click or touch is not currently down / was released at some point in the past.
    Clay_PointerDataInteractionState state;
} Clay_PointerData;

typedef struct {
    // Primarily created via the CLAY_ID(), CLAY_IDI(), CLAY_ID_LOCAL() and CLAY_IDI_LOCAL() macros.
    // Represents a hashed string ID used for identifying and finding specific clay UI elements, required by functions such as Clay_PointerOver() and Clay_GetElementData().
    Clay_ElementId id;
    // Controls various settings that affect the size and position of an element, as well as the sizes and positions of any child elements.
    Clay_LayoutConfig layout;
    // Controls the background color of the resulting element.
    // By convention specified as 0-255, but interpretation is up to the renderer.
    // If no other config is specified, .backgroundColor will generate a RECTANGLE render command, otherwise it will be passed as a property to IMAGE or CUSTOM render commands.
    Clay_Color backgroundColor;
    // Controls the "radius", or corner rounding of elements, including rectangles, borders and images.
    Clay_CornerRadius cornerRadius;
    // Controls settings related to image elements.
    Clay_ImageElementConfig image;
    // Controls whether and how an element "floats", which means it layers over the top of other elements in z order, and doesn't affect the position and size of siblings or parent elements.
    // Note: in order to activate floating, .floating.attachTo must be set to something other than the default value.
    Clay_FloatingElementConfig floating;
    // Used to create CUSTOM render commands, usually to render element types not supported by Clay.
    Clay_CustomElementConfig custom;
    // Controls whether an element should clip its contents and allow scrolling rather than expanding to contain them.
    Clay_ScrollElementConfig scroll;
    // Controls settings related to element borders, and will generate BORDER render commands.
    Clay_BorderElementConfig border;
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
} Clay_ErrorType;

// Data to identify the error that clay has encountered.
typedef struct {
    // Represents the type of error clay encountered while computing layout.
    // CLAY_ERROR_TYPE_TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED - A text measurement function wasn't provided using Clay_SetMeasureTextFunction(), or the provided function was null.
    // CLAY_ERROR_TYPE_ARENA_CAPACITY_EXCEEDED - Clay attempted to allocate its internal data structures but ran out of space. The arena passed to Clay_Initialize was created with a capacity smaller than that required by Clay_MinMemorySize().
    // CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED - Clay ran out of capacity in its internal array for storing elements. This limit can be increased with Clay_SetMaxElementCount().
    // CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED - Clay ran out of capacity in its internal array for storing elements. This limit can be increased with Clay_SetMaxMeasureTextCacheWordCount().
    // CLAY_ERROR_TYPE_DUPLICATE_ID - Two elements were declared with exactly the same ID within one layout.
    // CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND - A floating element was declared using CLAY_ATTACH_TO_ELEMENT_ID and either an invalid .parentId was provided or no element with the provided .parentId was found.
    // CLAY_ERROR_TYPE_PERCENTAGE_OVER_1 - An element was declared that using CLAY_SIZING_PERCENT but the percentage value was over 1. Percentage values are expected to be in the 0-1 range.
    // CLAY_ERROR_TYPE_INTERNAL_ERROR - Clay encountered an internal error. It would be wonderful if you could report this so we can fix it!
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
uint32_t Clay_MinMemorySize(void);
// Creates an arena for clay to use for its internal allocations, given a certain capacity in bytes and a pointer to an allocation of at least that size.
// Intended to be used with Clay_MinMemorySize in the following way:
// uint32_t minMemoryRequired = Clay_MinMemorySize();
// Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(minMemoryRequired, malloc(minMemoryRequired));
Clay_Arena Clay_CreateArenaWithCapacityAndMemory(uint32_t capacity, void *memory);
// Sets the state of the "pointer" (i.e. the mouse or touch) in Clay's internal data. Used for detecting and responding to mouse events in the debug view,
// as well as for Clay_Hovered() and scroll element handling.
void Clay_SetPointerState(Clay_Vector2 position, bool pointerDown);
// Initialize Clay's internal arena and setup required data before layout can begin. Only needs to be called once.
// - arena can be created using Clay_CreateArenaWithCapacityAndMemory()
// - layoutDimensions are the initial bounding dimensions of the layout (i.e. the screen width and height for a full screen layout)
// - errorHandler is used by Clay to inform you if something has gone wrong in configuration or layout.
Clay_Context* Clay_Initialize(Clay_Arena arena, Clay_Dimensions layoutDimensions, Clay_ErrorHandler errorHandler);
// Returns the Context that clay is currently using. Used when using multiple instances of clay simultaneously.
Clay_Context* Clay_GetCurrentContext(void);
// Sets the context that clay will use to compute the layout.
// Used to restore a context saved from Clay_GetCurrentContext when using multiple instances of clay simultaneously.
void Clay_SetCurrentContext(Clay_Context* context);
// Updates the state of Clay's internal scroll data, updating scroll content positions if scrollDelta is non zero, and progressing momentum scrolling.
// - enableDragScrolling when set to true will enable mobile device like "touch drag" scroll of scroll containers, including momentum scrolling after the touch has ended.
// - scrollDelta is the amount to scroll this frame on each axis in pixels.
// - deltaTime is the time in seconds since the last "frame" (scroll update)
void Clay_UpdateScrollContainers(bool enableDragScrolling, Clay_Vector2 scrollDelta, float deltaTime);
// Updates the layout dimensions in response to the window or outer container being resized.
void Clay_SetLayoutDimensions(Clay_Dimensions dimensions);
// Called before starting any layout declarations.
void Clay_BeginLayout(void);
// Called when all layout declarations are finished.
// Computes the layout and generates and returns the array of render commands to draw.
Clay_RenderCommandArray Clay_EndLayout(void);
// Calculates a hash ID from the given idString.
// Generally only used for dynamic strings when CLAY_ID("stringLiteral") can't be used.
Clay_ElementId Clay_GetElementId(Clay_String idString);
// Calculates a hash ID from the given idString and index.
// - index is used to avoid constructing dynamic ID strings in loops.
// Generally only used for dynamic strings when CLAY_IDI("stringLiteral", index) can't be used.
Clay_ElementId Clay_GetElementIdWithIndex(Clay_String idString, uint32_t index);
// Returns layout data such as the final calculated bounding box for an element with a given ID.
// The returned Clay_ElementData contains a `found` bool that will be true if an element with the provided ID was found.
// This ID can be calculated either with CLAY_ID() for string literal IDs, or Clay_GetElementId for dynamic strings.
Clay_ElementData Clay_GetElementData(Clay_ElementId id);
// Returns true if the pointer position provided by Clay_SetPointerState is within the current element's bounding box.
// Works during element declaration, e.g. CLAY({ .backgroundColor = Clay_Hovered() ? BLUE : RED });
bool Clay_Hovered(void);
// Bind a callback that will be called when the pointer position provided by Clay_SetPointerState is within the current element's bounding box.
// - onHoverFunction is a function pointer to a user defined function.
// - userData is a pointer that will be transparently passed through when the onHoverFunction is called.
void Clay_OnHover(void (*onHoverFunction)(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData), intptr_t userData);
// An imperative function that returns true if the pointer position provided by Clay_SetPointerState is within the element with the provided ID's bounding box.
// This ID can be calculated either with CLAY_ID() for string literal IDs, or Clay_GetElementId for dynamic strings.
bool Clay_PointerOver(Clay_ElementId elementId);
// Returns data representing the state of the scrolling element with the provided ID.
// The returned Clay_ScrollContainerData contains a `found` bool that will be true if a scroll element was found with the provided ID.
// An imperative function that returns true if the pointer position provided by Clay_SetPointerState is within the element with the provided ID's bounding box.
// This ID can be calculated either with CLAY_ID() for string literal IDs, or Clay_GetElementId for dynamic strings.
Clay_ScrollContainerData Clay_GetScrollContainerData(Clay_ElementId id);
// Binds a callback function that Clay will call to determine the dimensions of a given string slice.
// - measureTextFunction is a user provided function that adheres to the interface Clay_Dimensions (Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);
// - userData is a pointer that will be transparently passed through when the measureTextFunction is called.
void Clay_SetMeasureTextFunction(Clay_Dimensions (*measureTextFunction)(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData), void *userData);
// Experimental - Used in cases where Clay needs to integrate with a system that manages its own scrolling containers externally.
// Please reach out if you plan to use this function, as it may be subject to change.
void Clay_SetQueryScrollOffsetFunction(Clay_Vector2 (*queryScrollOffsetFunction)(uint32_t elementId, void *userData), void *userData);
// A bounds-checked "get" function for the Clay_RenderCommandArray returned from Clay_EndLayout().
Clay_RenderCommand * Clay_RenderCommandArray_Get(Clay_RenderCommandArray* array, int32_t index);
// Enables and disables Clay's internal debug tools.
// This state is retained and does not need to be set each frame.
void Clay_SetDebugModeEnabled(bool enabled);
// Returns true if Clay's internal debug tools are currently enabled.
bool Clay_IsDebugModeEnabled(void);
// Enables and disables visibility culling. By default, Clay will not generate render commands for elements whose bounding box is entirely outside the screen.
void Clay_SetCullingEnabled(bool enabled);
// Returns the maximum number of UI elements supported by Clay's current configuration.
int32_t Clay_GetMaxElementCount(void);
// Modifies the maximum number of UI elements supported by Clay's current configuration.
// This may require reallocating additional memory, and re-calling Clay_Initialize();
void Clay_SetMaxElementCount(int32_t maxElementCount);
// Returns the maximum number of measured "words" (whitespace seperated runs of characters) that Clay can store in its internal text measurement cache.
int32_t Clay_GetMaxMeasureTextCacheWordCount(void);
// Modifies the maximum number of measured "words" (whitespace seperated runs of characters) that Clay can store in its internal text measurement cache.
// This may require reallocating additional memory, and re-calling Clay_Initialize();
void Clay_SetMaxMeasureTextCacheWordCount(int32_t maxMeasureTextCacheWordCount);
// Resets Clay's internal text measurement cache, useful if memory to represent strings is being re-used.
// Similar behaviour can be achieved on an individual text element level by using Clay_TextElementConfig.hashStringContents
void Clay_ResetMeasureTextCache(void);

// Internal API functions required by macros ----------------------

void Clay__OpenElement(void);
void Clay__ConfigureOpenElement(const Clay_ElementDeclaration config);
void Clay__CloseElement(void);
Clay_ElementId Clay__HashString(Clay_String key, uint32_t offset, uint32_t seed);
void Clay__OpenTextElement(Clay_String text, Clay_TextElementConfig *textConfig);
Clay_TextElementConfig *Clay__StoreTextElementConfig(Clay_TextElementConfig config);
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
void arrayName##_Set(arrayName *array, int32_t index, typeName value) {                                         \
	if (Clay__Array_RangeCheck(index, array->capacity)) {                                                       \
		array->internalArray[index] = value;                                                                    \
		array->length = index < array->length ? array->length : index + 1;                                      \
	}                                                                                                           \
}                                                                                                               \

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

typedef struct {
    Clay_Color backgroundColor;
    Clay_CornerRadius cornerRadius;
    void* userData;
} Clay_SharedElementConfig;

CLAY__WRAPPER_STRUCT(Clay_SharedElementConfig);

Clay__WarningArray Clay__WarningArray_Allocate_Arena(int32_t capacity, Clay_Arena *arena);
Clay__Warning *Clay__WarningArray_Add(Clay__WarningArray *array, Clay__Warning item);
void* Clay__Array_Allocate_Arena(int32_t capacity, uint32_t itemSize, Clay_Arena *arena);
bool Clay__Array_RangeCheck(int32_t index, int32_t length);
bool Clay__Array_AddCapacityCheck(int32_t length, int32_t capacity);

CLAY__ARRAY_DEFINE(bool, Clay__boolArray)
CLAY__ARRAY_DEFINE(int32_t, Clay__int32_tArray)
CLAY__ARRAY_DEFINE(char, Clay__charArray)
CLAY__ARRAY_DEFINE(Clay_ElementId, Clay__ElementIdArray)
CLAY__ARRAY_DEFINE(Clay_LayoutConfig, Clay__LayoutConfigArray)
CLAY__ARRAY_DEFINE(Clay_TextElementConfig, Clay__TextElementConfigArray)
CLAY__ARRAY_DEFINE(Clay_ImageElementConfig, Clay__ImageElementConfigArray)
CLAY__ARRAY_DEFINE(Clay_FloatingElementConfig, Clay__FloatingElementConfigArray)
CLAY__ARRAY_DEFINE(Clay_CustomElementConfig, Clay__CustomElementConfigArray)
CLAY__ARRAY_DEFINE(Clay_ScrollElementConfig, Clay__ScrollElementConfigArray)
CLAY__ARRAY_DEFINE(Clay_BorderElementConfig, Clay__BorderElementConfigArray)
CLAY__ARRAY_DEFINE(Clay_String, Clay__StringArray)
CLAY__ARRAY_DEFINE(Clay_SharedElementConfig, Clay__SharedElementConfigArray)
CLAY__ARRAY_DEFINE_FUNCTIONS(Clay_RenderCommand, Clay_RenderCommandArray)

typedef CLAY_PACKED_ENUM {
    CLAY__ELEMENT_CONFIG_TYPE_NONE,
    CLAY__ELEMENT_CONFIG_TYPE_BORDER,
    CLAY__ELEMENT_CONFIG_TYPE_FLOATING,
    CLAY__ELEMENT_CONFIG_TYPE_SCROLL,
    CLAY__ELEMENT_CONFIG_TYPE_IMAGE,
    CLAY__ELEMENT_CONFIG_TYPE_TEXT,
    CLAY__ELEMENT_CONFIG_TYPE_CUSTOM,
    CLAY__ELEMENT_CONFIG_TYPE_SHARED,
} Clay__ElementConfigType;

typedef union {
    Clay_TextElementConfig *textElementConfig;
    Clay_ImageElementConfig *imageElementConfig;
    Clay_FloatingElementConfig *floatingElementConfig;
    Clay_CustomElementConfig *customElementConfig;
    Clay_ScrollElementConfig *scrollElementConfig;
    Clay_BorderElementConfig *borderElementConfig;
    Clay_SharedElementConfig *sharedElementConfig;
} Clay_ElementConfigUnion;

typedef struct {
    Clay__ElementConfigType type;
    Clay_ElementConfigUnion config;
} Clay_ElementConfig;

CLAY__ARRAY_DEFINE(Clay_ElementConfig, Clay__ElementConfigArray)

typedef struct {
    Clay_Dimensions dimensions;
    Clay_String line;
} Clay__WrappedTextLine;

CLAY__ARRAY_DEFINE(Clay__WrappedTextLine, Clay__WrappedTextLineArray)

typedef struct {
    Clay_String text;
    Clay_Dimensions preferredDimensions;
    int32_t elementIndex;
    Clay__WrappedTextLineArraySlice wrappedLines;
} Clay__TextElementData;

CLAY__ARRAY_DEFINE(Clay__TextElementData, Clay__TextElementDataArray)

typedef struct {
    int32_t *elements;
    uint16_t length;
} Clay__LayoutElementChildren;

typedef struct {
    union {
        Clay__LayoutElementChildren children;
        Clay__TextElementData *textElementData;
    } childrenOrTextContent;
    Clay_Dimensions dimensions;
    Clay_Dimensions minDimensions;
    Clay_LayoutConfig *layoutConfig;
    Clay__ElementConfigArraySlice elementConfigs;
    uint32_t id;
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

typedef struct {
    bool collision;
    bool collapsed;
} Clay__DebugElementData;

CLAY__ARRAY_DEFINE(Clay__DebugElementData, Clay__DebugElementDataArray)

typedef struct { // todo get this struct into a single cache line
    Clay_BoundingBox boundingBox;
    Clay_ElementId elementId;
    Clay_LayoutElement* layoutElement;
    void (*onHoverFunction)(Clay_ElementId elementId, Clay_PointerData pointerInfo, intptr_t userData);
    intptr_t hoverFunctionUserData;
    int32_t nextIndex;
    uint32_t generation;
    uint32_t idAlias;
    Clay__DebugElementData *debugData;
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
    void *measureTextUserData;
    void *queryScrollOffsetUserData;
    Clay_Arena internalArena;
    // Layout Elements / Render Commands
    Clay_LayoutElementArray layoutElements;
    Clay_RenderCommandArray renderCommands;
    Clay__int32_tArray openLayoutElementStack;
    Clay__int32_tArray layoutElementChildren;
    Clay__int32_tArray layoutElementChildrenBuffer;
    Clay__TextElementDataArray textElementData;
    Clay__int32_tArray imageElementPointers;
    Clay__int32_tArray reusableElementIndexBuffer;
    Clay__int32_tArray layoutElementClipElementIds;
    // Configs
    Clay__LayoutConfigArray layoutConfigs;
    Clay__ElementConfigArray elementConfigs;
    Clay__TextElementConfigArray textElementConfigs;
    Clay__ImageElementConfigArray imageElementConfigs;
    Clay__FloatingElementConfigArray floatingElementConfigs;
    Clay__ScrollElementConfigArray scrollElementConfigs;
    Clay__CustomElementConfigArray customElementConfigs;
    Clay__BorderElementConfigArray borderElementConfigs;
    Clay__SharedElementConfigArray sharedElementConfigs;
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
    Clay__boolArray treeNodeVisited;
    Clay__charArray dynamicStringData;
    Clay__DebugElementDataArray debugElementData;
};

Clay_Context* Clay__Context_Allocate_Arena(Clay_Arena *arena) {
    size_t totalSizeBytes = sizeof(Clay_Context);
    uintptr_t memoryAddress = (uintptr_t)arena->memory;
    // Make sure the memory address passed in for clay to use is cache line aligned
    uintptr_t nextAllocOffset = (memoryAddress % 64);
    if (nextAllocOffset + totalSizeBytes > arena->capacity)
    {
        return NULL;
    }
    arena->nextAllocation = nextAllocOffset + totalSizeBytes;
    return (Clay_Context*)(memoryAddress + nextAllocOffset);
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

uint32_t Clay__GetParentElementId(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    return Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&context->openLayoutElementStack, context->openLayoutElementStack.length - 2))->id;
}

Clay_LayoutConfig * Clay__StoreLayoutConfig(Clay_LayoutConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &CLAY_LAYOUT_DEFAULT : Clay__LayoutConfigArray_Add(&Clay_GetCurrentContext()->layoutConfigs, config); }
Clay_TextElementConfig * Clay__StoreTextElementConfig(Clay_TextElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &Clay_TextElementConfig_DEFAULT : Clay__TextElementConfigArray_Add(&Clay_GetCurrentContext()->textElementConfigs, config); }
Clay_ImageElementConfig * Clay__StoreImageElementConfig(Clay_ImageElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &Clay_ImageElementConfig_DEFAULT : Clay__ImageElementConfigArray_Add(&Clay_GetCurrentContext()->imageElementConfigs, config); }
Clay_FloatingElementConfig * Clay__StoreFloatingElementConfig(Clay_FloatingElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &Clay_FloatingElementConfig_DEFAULT : Clay__FloatingElementConfigArray_Add(&Clay_GetCurrentContext()->floatingElementConfigs, config); }
Clay_CustomElementConfig * Clay__StoreCustomElementConfig(Clay_CustomElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &Clay_CustomElementConfig_DEFAULT : Clay__CustomElementConfigArray_Add(&Clay_GetCurrentContext()->customElementConfigs, config); }
Clay_ScrollElementConfig * Clay__StoreScrollElementConfig(Clay_ScrollElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &Clay_ScrollElementConfig_DEFAULT : Clay__ScrollElementConfigArray_Add(&Clay_GetCurrentContext()->scrollElementConfigs, config); }
Clay_BorderElementConfig * Clay__StoreBorderElementConfig(Clay_BorderElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &Clay_BorderElementConfig_DEFAULT : Clay__BorderElementConfigArray_Add(&Clay_GetCurrentContext()->borderElementConfigs, config); }
Clay_SharedElementConfig * Clay__StoreSharedElementConfig(Clay_SharedElementConfig config) {  return Clay_GetCurrentContext()->booleanWarnings.maxElementsExceeded ? &Clay_SharedElementConfig_DEFAULT : Clay__SharedElementConfigArray_Add(&Clay_GetCurrentContext()->sharedElementConfigs, config); }

Clay_ElementConfig Clay__AttachElementConfig(Clay_ElementConfigUnion config, Clay__ElementConfigType type) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return CLAY__INIT(Clay_ElementConfig) CLAY__DEFAULT_STRUCT;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    openLayoutElement->elementConfigs.length++;
    return *Clay__ElementConfigArray_Add(&context->elementConfigs, CLAY__INIT(Clay_ElementConfig) { .type = type, .config = config });
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

uint32_t Clay__HashTextWithConfig(Clay_String *text, Clay_TextElementConfig *config) {
    uint32_t hash = 0;
    uintptr_t pointerAsNumber = (uintptr_t)text->chars;

    if (config->hashStringContents) {
        uint32_t maxLengthToHash = CLAY__MIN(text->length, 256);
        for (uint32_t i = 0; i < maxLengthToHash; i++) {
            hash += text->chars[i];
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }
    } else {
        hash += pointerAsNumber;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

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
        newItemIndex = Clay__int32_tArray_GetValue(&context->measureTextHashMapInternalFreeList, context->measureTextHashMapInternalFreeList.length - 1);
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
            Clay_Dimensions dimensions = Clay__MeasureText(CLAY__INIT(Clay_StringSlice) { .length = length, .chars = &text->chars[start], .baseChars = text->chars }, config, context->measureTextUserData);
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

Clay_LayoutElementHashMapItem* Clay__AddHashMapItem(Clay_ElementId elementId, Clay_LayoutElement* layoutElement, uint32_t idAlias) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->layoutElementsHashMapInternal.length == context->layoutElementsHashMapInternal.capacity - 1) {
        return NULL;
    }
    Clay_LayoutElementHashMapItem item = { .elementId = elementId, .layoutElement = layoutElement, .nextIndex = -1, .generation = context->generation + 1, .idAlias = idAlias };
    uint32_t hashBucket = elementId.id % context->layoutElementsHashMap.capacity;
    int32_t hashItemPrevious = -1;
    int32_t hashItemIndex = context->layoutElementsHashMap.internalArray[hashBucket];
    while (hashItemIndex != -1) { // Just replace collision, not a big deal - leave it up to the end user
        Clay_LayoutElementHashMapItem *hashItem = Clay__LayoutElementHashMapItemArray_Get(&context->layoutElementsHashMapInternal, hashItemIndex);
        if (hashItem->elementId.id == elementId.id) { // Collision - resolve based on generation
            item.nextIndex = hashItem->nextIndex;
            if (hashItem->generation <= context->generation) { // First collision - assume this is the "same" element
                hashItem->elementId = elementId; // Make sure to copy this across. If the stringId reference has changed, we should update the hash item to use the new one.
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
    return &Clay_LayoutElementHashMapItem_DEFAULT;
}

Clay_ElementId Clay__GenerateIdForAnonymousElement(Clay_LayoutElement *openLayoutElement) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay_LayoutElement *parentElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&context->openLayoutElementStack, context->openLayoutElementStack.length - 2));
    Clay_ElementId elementId = Clay__HashNumber(parentElement->childrenOrTextContent.children.length, parentElement->id);
    openLayoutElement->id = elementId.id;
    Clay__AddHashMapItem(elementId, openLayoutElement, 0);
    Clay__StringArray_Add(&context->layoutElementIdStrings, elementId.stringId);
    return elementId;
}

bool Clay__ElementHasConfig(Clay_LayoutElement *layoutElement, Clay__ElementConfigType type) {
    for (int32_t i = 0; i < layoutElement->elementConfigs.length; i++) {
        if (Clay__ElementConfigArraySlice_Get(&layoutElement->elementConfigs, i)->type == type) {
            return true;
        }
    }
    return false;
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
    for (int32_t i = 0; i < openLayoutElement->elementConfigs.length; i++) {
        Clay_ElementConfig *config = Clay__ElementConfigArraySlice_Get(&openLayoutElement->elementConfigs, i);
        if (config->type == CLAY__ELEMENT_CONFIG_TYPE_SCROLL) {
            elementHasScrollHorizontal = config->config.scrollElementConfig->horizontal;
            elementHasScrollVertical = config->config.scrollElementConfig->vertical;
            context->openClipElementStack.length--;
            break;
        }
    }

    // Attach children to the current open element
    openLayoutElement->childrenOrTextContent.children.elements = &context->layoutElementChildren.internalArray[context->layoutElementChildren.length];
    if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
        openLayoutElement->dimensions.width = (float)(layoutConfig->padding.left + layoutConfig->padding.right);
        for (int32_t i = 0; i < openLayoutElement->childrenOrTextContent.children.length; i++) {
            int32_t childIndex = Clay__int32_tArray_GetValue(&context->layoutElementChildrenBuffer, (int)context->layoutElementChildrenBuffer.length - openLayoutElement->childrenOrTextContent.children.length + i);
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
            int32_t childIndex = Clay__int32_tArray_GetValue(&context->layoutElementChildrenBuffer, (int)context->layoutElementChildrenBuffer.length - openLayoutElement->childrenOrTextContent.children.length + i);
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

    bool elementIsFloating = Clay__ElementHasConfig(openLayoutElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING);

    // Close the currently open element
    int32_t closingElementIndex = Clay__int32_tArray_RemoveSwapback(&context->openLayoutElementStack, (int)context->openLayoutElementStack.length - 1);
    openLayoutElement = Clay__GetOpenLayoutElement();

    if (!elementIsFloating && context->openLayoutElementStack.length > 1) {
        openLayoutElement->childrenOrTextContent.children.length++;
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
            if (vminvq_u32(vceqq_u8(v1, v2)) != 0xFFFFFFFF) { // If there's a difference
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
    Clay_LayoutElementArray_Add(&context->layoutElements, layoutElement);
    Clay__int32_tArray_Add(&context->openLayoutElementStack, context->layoutElements.length - 1);
    if (context->openClipElementStack.length > 0) {
        Clay__int32_tArray_Set(&context->layoutElementClipElementIds, context->layoutElements.length - 1, Clay__int32_tArray_GetValue(&context->openClipElementStack, (int)context->openClipElementStack.length - 1));
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

    Clay_LayoutElement layoutElement = CLAY__DEFAULT_STRUCT;
    Clay_LayoutElement *textElement = Clay_LayoutElementArray_Add(&context->layoutElements, layoutElement);
    if (context->openClipElementStack.length > 0) {
        Clay__int32_tArray_Set(&context->layoutElementClipElementIds, context->layoutElements.length - 1, Clay__int32_tArray_GetValue(&context->openClipElementStack, (int)context->openClipElementStack.length - 1));
    } else {
        Clay__int32_tArray_Set(&context->layoutElementClipElementIds, context->layoutElements.length - 1, 0);
    }

    Clay__int32_tArray_Add(&context->layoutElementChildrenBuffer, context->layoutElements.length - 1);
    Clay__MeasureTextCacheItem *textMeasured = Clay__MeasureTextCached(&text, textConfig);
    Clay_ElementId elementId = Clay__HashNumber(parentElement->childrenOrTextContent.children.length, parentElement->id);
    textElement->id = elementId.id;
    Clay__AddHashMapItem(elementId, textElement, 0);
    Clay__StringArray_Add(&context->layoutElementIdStrings, elementId.stringId);
    Clay_Dimensions textDimensions = { .width = textMeasured->unwrappedDimensions.width, .height = textConfig->lineHeight > 0 ? (float)textConfig->lineHeight : textMeasured->unwrappedDimensions.height };
    textElement->dimensions = textDimensions;
    textElement->minDimensions = CLAY__INIT(Clay_Dimensions) { .width = textMeasured->unwrappedDimensions.height, .height = textDimensions.height }; // TODO not sure this is the best way to decide min width for text
    textElement->childrenOrTextContent.textElementData = Clay__TextElementDataArray_Add(&context->textElementData, CLAY__INIT(Clay__TextElementData) { .text = text, .preferredDimensions = textMeasured->unwrappedDimensions, .elementIndex = context->layoutElements.length - 1 });
    textElement->elementConfigs = CLAY__INIT(Clay__ElementConfigArraySlice) {
            .length = 1,
            .internalArray = Clay__ElementConfigArray_Add(&context->elementConfigs, CLAY__INIT(Clay_ElementConfig) { .type = CLAY__ELEMENT_CONFIG_TYPE_TEXT, .config = { .textElementConfig = textConfig }})
    };
    textElement->layoutConfig = &CLAY_LAYOUT_DEFAULT;
    parentElement->childrenOrTextContent.children.length++;
}

Clay_ElementId Clay__AttachId(Clay_ElementId elementId) {
    Clay_Context* context = Clay_GetCurrentContext();
    if (context->booleanWarnings.maxElementsExceeded) {
        return Clay_ElementId_DEFAULT;
    }
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    uint32_t idAlias = openLayoutElement->id;
    openLayoutElement->id = elementId.id;
    Clay__AddHashMapItem(elementId, openLayoutElement, idAlias);
    Clay__StringArray_Add(&context->layoutElementIdStrings, elementId.stringId);
    return elementId;
}

void Clay__ConfigureOpenElement(const Clay_ElementDeclaration declaration) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay_LayoutElement *openLayoutElement = Clay__GetOpenLayoutElement();
    openLayoutElement->layoutConfig = Clay__StoreLayoutConfig(declaration.layout);
    if ((declaration.layout.sizing.width.type == CLAY__SIZING_TYPE_PERCENT && declaration.layout.sizing.width.size.percent > 1) || (declaration.layout.sizing.height.type == CLAY__SIZING_TYPE_PERCENT && declaration.layout.sizing.height.size.percent > 1)) {
        context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                .errorType = CLAY_ERROR_TYPE_PERCENTAGE_OVER_1,
                .errorText = CLAY_STRING("An element was configured with CLAY_SIZING_PERCENT, but the provided percentage value was over 1.0. Clay expects a value between 0 and 1, i.e. 20% is 0.2."),
                .userData = context->errorHandler.userData });
    }

    Clay_ElementId openLayoutElementId = declaration.id;

    openLayoutElement->elementConfigs.internalArray = &context->elementConfigs.internalArray[context->elementConfigs.length];
    Clay_SharedElementConfig *sharedConfig = NULL;
    if (declaration.backgroundColor.a > 0) {
        sharedConfig = Clay__StoreSharedElementConfig(CLAY__INIT(Clay_SharedElementConfig) { .backgroundColor = declaration.backgroundColor });
        Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .sharedElementConfig = sharedConfig }, CLAY__ELEMENT_CONFIG_TYPE_SHARED);
    }
    if (!Clay__MemCmp((char *)(&declaration.cornerRadius), (char *)(&Clay__CornerRadius_DEFAULT), sizeof(Clay_CornerRadius))) {
        if (sharedConfig) {
            sharedConfig->cornerRadius = declaration.cornerRadius;
        } else {
            sharedConfig = Clay__StoreSharedElementConfig(CLAY__INIT(Clay_SharedElementConfig) { .cornerRadius = declaration.cornerRadius });
            Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .sharedElementConfig = sharedConfig }, CLAY__ELEMENT_CONFIG_TYPE_SHARED);
        }
    }
    if (declaration.userData != 0) {
        if (sharedConfig) {
            sharedConfig->userData = declaration.userData;
        } else {
            sharedConfig = Clay__StoreSharedElementConfig(CLAY__INIT(Clay_SharedElementConfig) { .userData = declaration.userData });
            Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .sharedElementConfig = sharedConfig }, CLAY__ELEMENT_CONFIG_TYPE_SHARED);
        }
    }
    if (declaration.image.imageData) {
        Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .imageElementConfig = Clay__StoreImageElementConfig(declaration.image) }, CLAY__ELEMENT_CONFIG_TYPE_IMAGE);
        Clay__int32_tArray_Add(&context->imageElementPointers, context->layoutElements.length - 1);
    }
    if (declaration.floating.attachTo != CLAY_ATTACH_TO_NONE) {
        Clay_FloatingElementConfig floatingConfig = declaration.floating;
        // This looks dodgy but because of the auto generated root element the depth of the tree will always be at least 2 here
        Clay_LayoutElement *hierarchicalParent = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&context->openLayoutElementStack, context->openLayoutElementStack.length - 2));
        if (hierarchicalParent) {
            uint32_t clipElementId = 0;
            if (declaration.floating.attachTo == CLAY_ATTACH_TO_PARENT) {
                // Attach to the element's direct hierarchical parent
                floatingConfig.parentId = hierarchicalParent->id;
                if (context->openClipElementStack.length > 0) {
                    clipElementId = Clay__int32_tArray_GetValue(&context->openClipElementStack, (int)context->openClipElementStack.length - 1);
                }
            } else if (declaration.floating.attachTo == CLAY_ATTACH_TO_ELEMENT_WITH_ID) {
                Clay_LayoutElementHashMapItem *parentItem = Clay__GetHashMapItem(floatingConfig.parentId);
                if (!parentItem) {
                    context->errorHandler.errorHandlerFunction(CLAY__INIT(Clay_ErrorData) {
                            .errorType = CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND,
                            .errorText = CLAY_STRING("A floating element was declared with a parentId, but no element with that ID was found."),
                            .userData = context->errorHandler.userData });
                } else {
                    clipElementId = Clay__int32_tArray_GetValue(&context->layoutElementClipElementIds, parentItem->layoutElement - context->layoutElements.internalArray);
                }
            } else if (declaration.floating.attachTo == CLAY_ATTACH_TO_ROOT) {
                floatingConfig.parentId = Clay__HashString(CLAY_STRING("Clay__RootContainer"), 0, 0).id;
            }
            if (!openLayoutElementId.id) {
                openLayoutElementId = Clay__HashString(CLAY_STRING("Clay__FloatingContainer"), context->layoutElementTreeRoots.length, 0);
            }
            Clay__LayoutElementTreeRootArray_Add(&context->layoutElementTreeRoots, CLAY__INIT(Clay__LayoutElementTreeRoot) {
                    .layoutElementIndex = Clay__int32_tArray_GetValue(&context->openLayoutElementStack, context->openLayoutElementStack.length - 1),
                    .parentId = floatingConfig.parentId,
                    .clipElementId = clipElementId,
                    .zIndex = floatingConfig.zIndex,
            });
            Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .floatingElementConfig = Clay__StoreFloatingElementConfig(declaration.floating) }, CLAY__ELEMENT_CONFIG_TYPE_FLOATING);
        }
    }
    if (declaration.custom.customData) {
        Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .customElementConfig = Clay__StoreCustomElementConfig(declaration.custom) }, CLAY__ELEMENT_CONFIG_TYPE_CUSTOM);
    }

    if (openLayoutElementId.id != 0) {
        Clay__AttachId(openLayoutElementId);
    } else if (openLayoutElement->id == 0) {
        openLayoutElementId = Clay__GenerateIdForAnonymousElement(openLayoutElement);
    }

    if (declaration.scroll.horizontal | declaration.scroll.vertical) {
        Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .scrollElementConfig = Clay__StoreScrollElementConfig(declaration.scroll) }, CLAY__ELEMENT_CONFIG_TYPE_SCROLL);
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
    if (!Clay__MemCmp((char *)(&declaration.border.width), (char *)(&Clay__BorderWidth_DEFAULT), sizeof(Clay_BorderWidth))) {
        Clay__AttachElementConfig(CLAY__INIT(Clay_ElementConfigUnion) { .borderElementConfig = Clay__StoreBorderElementConfig(declaration.border) }, CLAY__ELEMENT_CONFIG_TYPE_BORDER);
    }
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
    context->elementConfigs = Clay__ElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->textElementConfigs = Clay__TextElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->imageElementConfigs = Clay__ImageElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->floatingElementConfigs = Clay__FloatingElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->scrollElementConfigs = Clay__ScrollElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->customElementConfigs = Clay__CustomElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->borderElementConfigs = Clay__BorderElementConfigArray_Allocate_Arena(maxElementCount, arena);
    context->sharedElementConfigs = Clay__SharedElementConfigArray_Allocate_Arena(maxElementCount, arena);

    context->layoutElementIdStrings = Clay__StringArray_Allocate_Arena(maxElementCount, arena);
    context->wrappedTextLines = Clay__WrappedTextLineArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementTreeNodeArray1 = Clay__LayoutElementTreeNodeArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementTreeRoots = Clay__LayoutElementTreeRootArray_Allocate_Arena(maxElementCount, arena);
    context->layoutElementChildren = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->openLayoutElementStack = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
    context->textElementData = Clay__TextElementDataArray_Allocate_Arena(maxElementCount, arena);
    context->imageElementPointers = Clay__int32_tArray_Allocate_Arena(maxElementCount, arena);
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
            Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&resizableContainerBuffer, i));
            float childSize = xAxis ? childElement->dimensions.width : childElement->dimensions.height;
            if ((childSize - largestSize) < 0.1 && (childSize - largestSize) > -0.1) {
                Clay__int32_tArray_Add(&largestContainers, Clay__int32_tArray_GetValue(&resizableContainerBuffer, i));
            } else if (childSize > largestSize) {
                targetSize = largestSize;
                largestSize = childSize;
                largestContainers.length = 0;
                Clay__int32_tArray_Add(&largestContainers, Clay__int32_tArray_GetValue(&resizableContainerBuffer, i));
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
            int32_t childIndex = Clay__int32_tArray_GetValue(&largestContainers, childOffset);
            Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, childIndex);
            float *childSize = xAxis ? &childElement->dimensions.width : &childElement->dimensions.height;
            float childMinSize = xAxis ? childElement->minDimensions.width : childElement->minDimensions.height;
            float oldChildSize = *childSize;
            *childSize = CLAY__MAX(childMinSize, targetSize);
            totalSizeToDistribute -= (oldChildSize - *childSize);
            if (*childSize == childMinSize) {
                for (int32_t i = 0; i < resizableContainerBuffer.length; i++) {
                    if (Clay__int32_tArray_GetValue(&resizableContainerBuffer, i) == childIndex) {
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
        if (Clay__ElementHasConfig(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING)) {
            Clay_FloatingElementConfig *floatingElementConfig = Clay__FindElementConfigWithType(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING).floatingElementConfig;
            Clay_LayoutElementHashMapItem *parentItem = Clay__GetHashMapItem(floatingElementConfig->parentId);
            if (parentItem && parentItem != &Clay_LayoutElementHashMapItem_DEFAULT) {
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
            int32_t parentIndex = Clay__int32_tArray_GetValue(&bfsBuffer, i);
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

                if (childSizing.type != CLAY__SIZING_TYPE_PERCENT
                    && childSizing.type != CLAY__SIZING_TYPE_FIXED
                    && (!Clay__ElementHasConfig(childElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT) || (Clay__FindElementConfigWithType(childElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT).textElementConfig->wrapMode == CLAY_TEXT_WRAP_WORDS)) // todo too many loops
                    && (xAxis || !Clay__ElementHasConfig(childElement, CLAY__ELEMENT_CONFIG_TYPE_IMAGE))
                ) {
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
                    Clay_ScrollElementConfig *scrollElementConfig = Clay__FindElementConfigWithType(parent, CLAY__ELEMENT_CONFIG_TYPE_SCROLL).scrollElementConfig;
                    if (scrollElementConfig) {
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
                        Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&resizableContainerBuffer, childOffset));
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
                    Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&resizableContainerBuffer, childOffset));
                    Clay_SizingAxis childSizing = xAxis ? childElement->layoutConfig->sizing.width : childElement->layoutConfig->sizing.height;
                    float *childSize = xAxis ? &childElement->dimensions.width : &childElement->dimensions.height;

                    if (!xAxis && Clay__ElementHasConfig(childElement, CLAY__ELEMENT_CONFIG_TYPE_IMAGE)) {
                        continue; // Currently we don't support resizing aspect ratio images on the Y axis because it would break the ratio
                    }

                    // If we're laying out the children of a scroll panel, grow containers expand to the height of the inner content, not the outer container
                    float maxSize = parentSize - parentPadding;
                    if (Clay__ElementHasConfig(parent, CLAY__ELEMENT_CONFIG_TYPE_SCROLL)) {
                        Clay_ScrollElementConfig *scrollElementConfig = Clay__FindElementConfigWithType(parent, CLAY__ELEMENT_CONFIG_TYPE_SCROLL).scrollElementConfig;
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

void Clay__CalculateFinalLayout(void) {
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
        float spaceWidth = Clay__MeasureText(CLAY__INIT(Clay_StringSlice) { .length = 1, .chars = CLAY__SPACECHAR.chars, .baseChars = CLAY__SPACECHAR.chars }, textConfig, context->measureTextUserData).width;
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
                bool finalCharIsSpace = textElementData->text.chars[lineStartOffset + lineLengthChars - 1] == ' ';
                Clay__WrappedTextLineArray_Add(&context->wrappedTextLines, CLAY__INIT(Clay__WrappedTextLine) { { lineWidth + (finalCharIsSpace ? -spaceWidth : 0), lineHeight }, { .length = lineLengthChars + (finalCharIsSpace ? -1 : 0), .chars = &textElementData->text.chars[lineStartOffset] } });
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
        Clay_LayoutElement* imageElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&context->imageElementPointers, i));
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
        if (Clay__ElementHasConfig(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING) && parentHashMapItem) {
            Clay_FloatingElementConfig *config = Clay__FindElementConfigWithType(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING).floatingElementConfig;
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
            if (clipHashMapItem) {
                // Floating elements that are attached to scrolling contents won't be correctly positioned if external scroll handling is enabled, fix here
                if (context->externalScrollHandlingEnabled) {
                    Clay_ScrollElementConfig *scrollConfig = Clay__FindElementConfigWithType(clipHashMapItem->layoutElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL).scrollElementConfig;
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
                    .userData = 0,
                    .id = Clay__HashNumber(rootElement->id, rootElement->childrenOrTextContent.children.length + 10).id, // TODO need a better strategy for managing derived ids
                    .zIndex = root->zIndex,
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
                if (Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING)) {
                    Clay_FloatingElementConfig *floatingElementConfig = Clay__FindElementConfigWithType(currentElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING).floatingElementConfig;
                    Clay_Dimensions expand = floatingElementConfig->expand;
                    currentElementBoundingBox.x -= expand.width;
                    currentElementBoundingBox.width += expand.width * 2;
                    currentElementBoundingBox.y -= expand.height;
                    currentElementBoundingBox.height += expand.height * 2;
                }

                Clay__ScrollContainerDataInternal *scrollContainerData = CLAY__NULL;
                // Apply scroll offsets to container
                if (Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL)) {
                    Clay_ScrollElementConfig *scrollConfig = Clay__FindElementConfigWithType(currentElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL).scrollElementConfig;

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
                    if (hashMapItem->idAlias) {
                        Clay_LayoutElementHashMapItem *hashMapItemAlias = Clay__GetHashMapItem(hashMapItem->idAlias);
                        if (hashMapItemAlias) {
                            hashMapItemAlias->boundingBox = currentElementBoundingBox;
                        }
                    }
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
                        if (nextType == CLAY__ELEMENT_CONFIG_TYPE_SCROLL || currentType == CLAY__ELEMENT_CONFIG_TYPE_BORDER) {
                            sortedConfigIndexes[i] = next;
                            sortedConfigIndexes[i + 1] = current;
                        }
                    }
                    sortMax--;
                }

                bool emitRectangle = false;
                // Create the render commands for this element
                Clay_SharedElementConfig *sharedConfig = Clay__FindElementConfigWithType(currentElement, CLAY__ELEMENT_CONFIG_TYPE_SHARED).sharedElementConfig;
                if (sharedConfig && sharedConfig->backgroundColor.a > 0) {
                   emitRectangle = true;
                }
                else if (!sharedConfig) {
                    emitRectangle = false;
                    sharedConfig = &Clay_SharedElementConfig_DEFAULT;
                }
                for (int32_t elementConfigIndex = 0; elementConfigIndex < currentElement->elementConfigs.length; ++elementConfigIndex) {
                    Clay_ElementConfig *elementConfig = Clay__ElementConfigArraySlice_Get(&currentElement->elementConfigs, sortedConfigIndexes[elementConfigIndex]);
                    Clay_RenderCommand renderCommand = {
                        .boundingBox = currentElementBoundingBox,
                        .userData = sharedConfig->userData,
                        .id = currentElement->id,
                    };

                    bool offscreen = Clay__ElementIsOffscreen(&currentElementBoundingBox);
                    // Culling - Don't bother to generate render commands for rectangles entirely outside the screen - this won't stop their children from being rendered if they overflow
                    bool shouldRender = !offscreen;
                    switch (elementConfig->type) {
                        case CLAY__ELEMENT_CONFIG_TYPE_FLOATING:
                        case CLAY__ELEMENT_CONFIG_TYPE_SHARED:
                        case CLAY__ELEMENT_CONFIG_TYPE_BORDER: {
                            shouldRender = false;
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_SCROLL: {
                            renderCommand.commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_START;
                            renderCommand.renderData = CLAY__INIT(Clay_RenderData) {
                                .scroll = {
                                    .horizontal = elementConfig->config.scrollElementConfig->horizontal,
                                    .vertical = elementConfig->config.scrollElementConfig->vertical,
                                }
                            };
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_IMAGE: {
                            renderCommand.commandType = CLAY_RENDER_COMMAND_TYPE_IMAGE;
                            renderCommand.renderData = CLAY__INIT(Clay_RenderData) {
                                .image = {
                                    .backgroundColor = sharedConfig->backgroundColor,
                                    .cornerRadius = sharedConfig->cornerRadius,
                                    .sourceDimensions = elementConfig->config.imageElementConfig->sourceDimensions,
                                    .imageData = elementConfig->config.imageElementConfig->imageData,
                               }
                            };
                            emitRectangle = false;
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
                                Clay__WrappedTextLine *wrappedLine = Clay__WrappedTextLineArraySlice_Get(&currentElement->childrenOrTextContent.textElementData->wrappedLines, lineIndex);
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
                                        .stringContents = CLAY__INIT(Clay_StringSlice) { .length = wrappedLine->line.length, .chars = wrappedLine->line.chars, .baseChars = currentElement->childrenOrTextContent.textElementData->text.chars },
                                        .textColor = textElementConfig->textColor,
                                        .fontId = textElementConfig->fontId,
                                        .fontSize = textElementConfig->fontSize,
                                        .letterSpacing = textElementConfig->letterSpacing,
                                        .lineHeight = textElementConfig->lineHeight,
                                    }},
                                    .userData = sharedConfig->userData,
                                    .id = Clay__HashNumber(lineIndex, currentElement->id).id,
                                    .zIndex = root->zIndex,
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
                            renderCommand.renderData = CLAY__INIT(Clay_RenderData) {
                                .custom = {
                                    .backgroundColor = sharedConfig->backgroundColor,
                                    .cornerRadius = sharedConfig->cornerRadius,
                                    .customData = elementConfig->config.customElementConfig->customData,
                                }
                            };
                            emitRectangle = false;
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

                if (emitRectangle) {
                    Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                        .boundingBox = currentElementBoundingBox,
                        .renderData = { .rectangle = {
                                .backgroundColor = sharedConfig->backgroundColor,
                                .cornerRadius = sharedConfig->cornerRadius,
                        }},
                        .userData = sharedConfig->userData,
                        .id = currentElement->id,
                        .zIndex = root->zIndex,
                        .commandType = CLAY_RENDER_COMMAND_TYPE_RECTANGLE,
                    });
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
                Clay_ScrollElementConfig *scrollConfig = Clay__FindElementConfigWithType(currentElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL).scrollElementConfig;
                if (scrollConfig) {
                    closeScrollElement = true;
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

                if (Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_BORDER)) {
                    Clay_LayoutElementHashMapItem *currentElementData = Clay__GetHashMapItem(currentElement->id);
                    Clay_BoundingBox currentElementBoundingBox = currentElementData->boundingBox;

                    // Culling - Don't bother to generate render commands for rectangles entirely outside the screen - this won't stop their children from being rendered if they overflow
                    if (!Clay__ElementIsOffscreen(&currentElementBoundingBox)) {
                        Clay_SharedElementConfig *sharedConfig = Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_SHARED) ? Clay__FindElementConfigWithType(currentElement, CLAY__ELEMENT_CONFIG_TYPE_SHARED).sharedElementConfig : &Clay_SharedElementConfig_DEFAULT;
                        Clay_BorderElementConfig *borderConfig = Clay__FindElementConfigWithType(currentElement, CLAY__ELEMENT_CONFIG_TYPE_BORDER).borderElementConfig;
                        Clay_RenderCommand renderCommand = {
                                .boundingBox = currentElementBoundingBox,
                                .renderData = { .border = {
                                    .color = borderConfig->color,
                                    .cornerRadius = sharedConfig->cornerRadius,
                                    .width = borderConfig->width
                                }},
                                .userData = sharedConfig->userData,
                                .id = Clay__HashNumber(currentElement->id, currentElement->childrenOrTextContent.children.length).id,
                                .commandType = CLAY_RENDER_COMMAND_TYPE_BORDER,
                        };
                        Clay__AddRenderCommand(renderCommand);
                        if (borderConfig->width.betweenChildren > 0 && borderConfig->color.a > 0) {
                            float halfGap = layoutConfig->childGap / 2;
                            Clay_Vector2 borderOffset = { (float)layoutConfig->padding.left - halfGap, (float)layoutConfig->padding.top - halfGap };
                            if (layoutConfig->layoutDirection == CLAY_LEFT_TO_RIGHT) {
                                for (int32_t i = 0; i < currentElement->childrenOrTextContent.children.length; ++i) {
                                    Clay_LayoutElement *childElement = Clay_LayoutElementArray_Get(&context->layoutElements, currentElement->childrenOrTextContent.children.elements[i]);
                                    if (i > 0) {
                                        Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) {
                                            .boundingBox = { currentElementBoundingBox.x + borderOffset.x + scrollOffset.x, currentElementBoundingBox.y + scrollOffset.y, (float)borderConfig->width.betweenChildren, currentElement->dimensions.height },
                                            .renderData = { .rectangle = {
                                                .backgroundColor = borderConfig->color,
                                            } },
                                            .userData = sharedConfig->userData,
                                            .id = Clay__HashNumber(currentElement->id, currentElement->childrenOrTextContent.children.length + 1 + i).id,
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
                                            .boundingBox = { currentElementBoundingBox.x + scrollOffset.x, currentElementBoundingBox.y + borderOffset.y + scrollOffset.y, currentElement->dimensions.width, (float)borderConfig->width.betweenChildren },
                                            .renderData = { .rectangle = {
                                                    .backgroundColor = borderConfig->color,
                                            } },
                                            .userData = sharedConfig->userData,
                                            .id = Clay__HashNumber(currentElement->id, currentElement->childrenOrTextContent.children.length + 1 + i).id,
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
                        .id = Clay__HashNumber(currentElement->id, rootElement->childrenOrTextContent.children.length + 11).id,
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
            Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand) { .id = Clay__HashNumber(rootElement->id, rootElement->childrenOrTextContent.children.length + 11).id, .commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_END });
        }
    }
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

Clay__DebugElementConfigTypeLabelConfig Clay__DebugGetElementConfigTypeLabel(Clay__ElementConfigType type) {
    switch (type) {
        case CLAY__ELEMENT_CONFIG_TYPE_SHARED: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Shared"), {243,134,48,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_TEXT: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Text"), {105,210,231,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_IMAGE: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Image"), {121,189,154,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_FLOATING: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Floating"), {250,105,0,255} };
        case CLAY__ELEMENT_CONFIG_TYPE_SCROLL: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) {CLAY_STRING("Scroll"), {242, 196, 90, 255} };
        case CLAY__ELEMENT_CONFIG_TYPE_BORDER: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) {CLAY_STRING("Border"), {108, 91, 123, 255} };
        case CLAY__ELEMENT_CONFIG_TYPE_CUSTOM: return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Custom"), {11,72,107,255} };
        default: break;
    }
    return CLAY__INIT(Clay__DebugElementConfigTypeLabelConfig) { CLAY_STRING("Error"), {0,0,0,255} };
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
            CLAY({ .id = CLAY_IDI("Clay__DebugView_EmptyRowOuter", rootIndex), .layout = { .sizing = {.width = CLAY_SIZING_GROW(0)}, .padding = {CLAY__DEBUGVIEW_INDENT_WIDTH / 2, 0, 0, 0} } }) {
                CLAY({ .id = CLAY_IDI("Clay__DebugView_EmptyRow", rootIndex), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED((float)CLAY__DEBUGVIEW_ROW_HEIGHT) }}, .border = { .color = CLAY__DEBUGVIEW_COLOR_3, .width = { .top = 1 } } }) {}
            }
            layoutData.rowCount++;
        }
        while (dfsBuffer.length > 0) {
            int32_t currentElementIndex = Clay__int32_tArray_GetValue(&dfsBuffer, (int)dfsBuffer.length - 1);
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
            CLAY({ .id = CLAY_IDI("Clay__DebugView_ElementOuter", currentElement->id), .layout = Clay__DebugView_ScrollViewItemLayoutConfig }) {
                // Collapse icon / button
                if (!(Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT) || currentElement->childrenOrTextContent.children.length == 0)) {
                    CLAY({
                        .id = CLAY_IDI("Clay__DebugView_CollapseElement", currentElement->id),
                        .layout = { .sizing = {CLAY_SIZING_FIXED(16), CLAY_SIZING_FIXED(16)}, .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER} },
                        .cornerRadius = CLAY_CORNER_RADIUS(4),
                        .border = { .color = CLAY__DEBUGVIEW_COLOR_3, .width = {1, 1, 1, 1, 0} },
                    }) {
                        CLAY_TEXT((currentElementData && currentElementData->debugData->collapsed) ? CLAY_STRING("+") : CLAY_STRING("-"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
                    }
                } else { // Square dot for empty containers
                    CLAY({ .layout = { .sizing = {CLAY_SIZING_FIXED(16), CLAY_SIZING_FIXED(16)}, .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER } } }) {
                        CLAY({ .layout = { .sizing = {CLAY_SIZING_FIXED(8), CLAY_SIZING_FIXED(8)} }, .backgroundColor = CLAY__DEBUGVIEW_COLOR_3, .cornerRadius = CLAY_CORNER_RADIUS(2) }) {}
                    }
                }
                // Collisions and offscreen info
                if (currentElementData) {
                    if (currentElementData->debugData->collision) {
                        CLAY({ .layout = { .padding = { 8, 8, 2, 2 }}, .border = { .color = {177, 147, 8, 255}, .width = {1, 1, 1, 1, 0} } }) {
                            CLAY_TEXT(CLAY_STRING("Duplicate ID"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16 }));
                        }
                    }
                    if (offscreen) {
                        CLAY({ .layout = { .padding = { 8, 8, 2, 2 } }, .border = {  .color = CLAY__DEBUGVIEW_COLOR_3, .width = { 1, 1, 1, 1, 0} } }) {
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
                    if (elementConfig->type == CLAY__ELEMENT_CONFIG_TYPE_SHARED) {
                        Clay_Color labelColor = {243,134,48,90};
                        labelColor.a = 90;
                        Clay_Color backgroundColor = elementConfig->config.sharedElementConfig->backgroundColor;
                        Clay_CornerRadius radius = elementConfig->config.sharedElementConfig->cornerRadius;
                        if (backgroundColor.a > 0) {
                            CLAY({ .layout = { .padding = { 8, 8, 2, 2 } }, .backgroundColor = labelColor, .cornerRadius = CLAY_CORNER_RADIUS(4), .border = { .color = labelColor, .width = { 1, 1, 1, 1, 0} } }) {
                                CLAY_TEXT(CLAY_STRING("Color"), CLAY_TEXT_CONFIG({ .textColor = offscreen ? CLAY__DEBUGVIEW_COLOR_3 : CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
                            }
                        }
                        if (radius.bottomLeft > 0) {
                            CLAY({ .layout = { .padding = { 8, 8, 2, 2 } }, .backgroundColor = labelColor, .cornerRadius = CLAY_CORNER_RADIUS(4), .border = { .color = labelColor, .width = { 1, 1, 1, 1, 0 } } }) {
                                CLAY_TEXT(CLAY_STRING("Radius"), CLAY_TEXT_CONFIG({ .textColor = offscreen ? CLAY__DEBUGVIEW_COLOR_3 : CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
                            }
                        }
                        continue;
                    }
                    Clay__DebugElementConfigTypeLabelConfig config = Clay__DebugGetElementConfigTypeLabel(elementConfig->type);
                    Clay_Color backgroundColor = config.color;
                    backgroundColor.a = 90;
                    CLAY({ .layout = { .padding = { 8, 8, 2, 2 } }, .backgroundColor = backgroundColor, .cornerRadius = CLAY_CORNER_RADIUS(4), .border = { .color = config.color, .width = { 1, 1, 1, 1, 0 } } }) {
                        CLAY_TEXT(config.label, CLAY_TEXT_CONFIG({ .textColor = offscreen ? CLAY__DEBUGVIEW_COLOR_3 : CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
                    }
                }
            }

            // Render the text contents below the element as a non-interactive row
            if (Clay__ElementHasConfig(currentElement, CLAY__ELEMENT_CONFIG_TYPE_TEXT)) {
                layoutData.rowCount++;
                Clay__TextElementData *textElementData = currentElement->childrenOrTextContent.textElementData;
                Clay_TextElementConfig *rawTextConfig = offscreen ? CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16 }) : &Clay__DebugView_TextNameConfig;
                CLAY({ .layout = { .sizing = { .height = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } } }) {
                    CLAY({ .layout = { .sizing = {.width = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_INDENT_WIDTH + 16) } } }) {}
                    CLAY_TEXT(CLAY_STRING("\""), rawTextConfig);
                    CLAY_TEXT(textElementData->text.length > 40 ? (CLAY__INIT(Clay_String) { .length = 40, .chars = textElementData->text.chars }) : textElementData->text, rawTextConfig);
                    if (textElementData->text.length > 40) {
                        CLAY_TEXT(CLAY_STRING("..."), rawTextConfig);
                    }
                    CLAY_TEXT(CLAY_STRING("\""), rawTextConfig);
                }
            } else if (currentElement->childrenOrTextContent.children.length > 0) {
                Clay__OpenElement();
                Clay__ConfigureOpenElement(CLAY__INIT(Clay_ElementDeclaration) { .layout = { .padding = { .left = 8 } } });
                Clay__OpenElement();
                Clay__ConfigureOpenElement(CLAY__INIT(Clay_ElementDeclaration) { .layout = { .padding = { .left = CLAY__DEBUGVIEW_INDENT_WIDTH }}, .border = { .color = CLAY__DEBUGVIEW_COLOR_3, .width = { .left = 1 } }});
                Clay__OpenElement();
                Clay__ConfigureOpenElement(CLAY__INIT(Clay_ElementDeclaration) { .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM } });
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
        CLAY({ .id = CLAY_ID("Clay__DebugView_ElementHighlight"), .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }, .floating = { .parentId = highlightedElementId, .zIndex = 32767, .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH, .attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID } }) {
            CLAY({ .id = CLAY_ID("Clay__DebugView_ElementHighlightRectangle"), .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }, .backgroundColor = Clay__debugViewHighlightColor }) {}
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
    CLAY({ .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(CLAY__DEBUGVIEW_OUTER_PADDING), .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } } }) {
        CLAY({ .layout = { .padding = { 8, 8, 2, 2 } }, .backgroundColor = backgroundColor, .cornerRadius = CLAY_CORNER_RADIUS(4), .border = { .color = config.color, .width = { 1, 1, 1, 1, 0 } } }) {
            CLAY_TEXT(config.label, CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
        }
        CLAY({ .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) } } }) {}
        CLAY_TEXT(elementId, CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_3, .fontSize = 16, .wrapMode = CLAY_TEXT_WRAP_NONE }));
    }
}

void Clay__RenderDebugViewColor(Clay_Color color, Clay_TextElementConfig *textConfig) {
    CLAY({ .layout = { .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} } }) {
        CLAY_TEXT(CLAY_STRING("{ r: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.r), textConfig);
        CLAY_TEXT(CLAY_STRING(", g: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.g), textConfig);
        CLAY_TEXT(CLAY_STRING(", b: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.b), textConfig);
        CLAY_TEXT(CLAY_STRING(", a: "), textConfig);
        CLAY_TEXT(Clay__IntToString(color.a), textConfig);
        CLAY_TEXT(CLAY_STRING(" }"), textConfig);
        CLAY({ .layout = { .sizing = { .width = CLAY_SIZING_FIXED(10) } } }) {}
        CLAY({ .layout = { .sizing = { CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 8), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 8)} }, .backgroundColor = color, .cornerRadius = CLAY_CORNER_RADIUS(4), .border = { .color = CLAY__DEBUGVIEW_COLOR_4, .width = { 1, 1, 1, 1, 0 } } }) {}
    }
}

void Clay__RenderDebugViewCornerRadius(Clay_CornerRadius cornerRadius, Clay_TextElementConfig *textConfig) {
    CLAY({ .layout = { .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} } }) {
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

void HandleDebugViewCloseButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerInfo, intptr_t userData) {
    Clay_Context* context = Clay_GetCurrentContext();
    (void) elementId; (void) pointerInfo; (void) userData;
    if (pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        context->debugModeEnabled = false;
    }
}

void Clay__RenderDebugView(void) {
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
    CLAY({ .id = CLAY_ID("Clay__DebugView"),
         .layout = { .sizing = { CLAY_SIZING_FIXED((float)Clay__debugViewWidth) , CLAY_SIZING_FIXED(context->layoutDimensions.height) }, .layoutDirection = CLAY_TOP_TO_BOTTOM },
        .floating = { .zIndex = 32765, .attachPoints = { .element = CLAY_ATTACH_POINT_LEFT_CENTER, .parent = CLAY_ATTACH_POINT_RIGHT_CENTER }, .attachTo = CLAY_ATTACH_TO_ROOT },
        .border = { .color = CLAY__DEBUGVIEW_COLOR_3, .width = { .bottom = 1 } }
    }) {
        CLAY({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING, 0, 0 }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} }, .backgroundColor = CLAY__DEBUGVIEW_COLOR_2 }) {
            CLAY_TEXT(CLAY_STRING("Clay Debug Tools"), infoTextConfig);
            CLAY({ .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) } } }) {}
            // Close button
            CLAY({
                .layout = { .sizing = {CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 10), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT - 10)}, .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER} },
                .backgroundColor = {217,91,67,80},
                .cornerRadius = CLAY_CORNER_RADIUS(4),
                .border = { .color = { 217,91,67,255 }, .width = { 1, 1, 1, 1, 0 } },
            }) {
                Clay_OnHover(HandleDebugViewCloseButtonInteraction, 0);
                CLAY_TEXT(CLAY_STRING("x"), CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16 }));
            }
        }
        CLAY({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1)} }, .backgroundColor = CLAY__DEBUGVIEW_COLOR_3 } ) {}
        CLAY({ .id = scrollId, .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }, .scroll = { .horizontal = true, .vertical = true } }) {
            CLAY({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .layoutDirection = CLAY_TOP_TO_BOTTOM }, .backgroundColor = ((initialElementsLength + initialRootsLength) & 1) == 0 ? CLAY__DEBUGVIEW_COLOR_2 : CLAY__DEBUGVIEW_COLOR_1 }) {
                Clay_ElementId panelContentsId = Clay__HashString(CLAY_STRING("Clay__DebugViewPaneOuter"), 0, 0);
                // Element list
                CLAY({ .id = panelContentsId, .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} }, .floating = { .zIndex = 32766, .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH, .attachTo = CLAY_ATTACH_TO_PARENT } }) {
                    CLAY({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = { CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING, 0, 0 }, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                        layoutData = Clay__RenderDebugLayoutElementsList((int32_t)initialRootsLength, highlightedRow);
                    }
                }
                float contentWidth = Clay__GetHashMapItem(panelContentsId.id)->layoutElement->dimensions.width;
                CLAY({ .layout = { .sizing = {.width = CLAY_SIZING_FIXED(contentWidth) }, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {}
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
                    CLAY({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .layoutDirection = CLAY_TOP_TO_BOTTOM }, .backgroundColor = rowColor } ) {}
                }
            }
        }
        CLAY({ .layout = { .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(1)} }, .backgroundColor = CLAY__DEBUGVIEW_COLOR_3 }) {}
        if (context->debugSelectedElementId != 0) {
            Clay_LayoutElementHashMapItem *selectedItem = Clay__GetHashMapItem(context->debugSelectedElementId);
            CLAY({
                .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(300)}, .layoutDirection = CLAY_TOP_TO_BOTTOM },
                .backgroundColor = CLAY__DEBUGVIEW_COLOR_2 ,
                .scroll = { .vertical = true },
                .border = { .color = CLAY__DEBUGVIEW_COLOR_3, .width = { .betweenChildren = 1 } }
            }) {
                CLAY({ .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT + 8)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING, 0, 0 }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} } }) {
                    CLAY_TEXT(CLAY_STRING("Layout Config"), infoTextConfig);
                    CLAY({ .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) } } }) {}
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
                CLAY({ .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                    // .boundingBox
                    CLAY_TEXT(CLAY_STRING("Bounding Box"), infoTitleConfig);
                    CLAY({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
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
                    CLAY({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
                        CLAY_TEXT(CLAY_STRING("width: "), infoTextConfig);
                        Clay__RenderDebugLayoutSizing(layoutConfig->sizing.width, infoTextConfig);
                    }
                    CLAY({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
                        CLAY_TEXT(CLAY_STRING("height: "), infoTextConfig);
                        Clay__RenderDebugLayoutSizing(layoutConfig->sizing.height, infoTextConfig);
                    }
                    // .padding
                    CLAY_TEXT(CLAY_STRING("Padding"), infoTitleConfig);
                    CLAY({ .id = CLAY_ID("Clay__DebugViewElementInfoPadding") }) {
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
                    CLAY({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
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
                        case CLAY__ELEMENT_CONFIG_TYPE_SHARED: {
                            Clay_SharedElementConfig *sharedConfig = elementConfig->config.sharedElementConfig;
                            CLAY({ .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM }}) {
                                // .backgroundColor
                                CLAY_TEXT(CLAY_STRING("Background Color"), infoTitleConfig);
                                Clay__RenderDebugViewColor(sharedConfig->backgroundColor, infoTextConfig);
                                // .cornerRadius
                                CLAY_TEXT(CLAY_STRING("Corner Radius"), infoTitleConfig);
                                Clay__RenderDebugViewCornerRadius(sharedConfig->cornerRadius, infoTextConfig);
                            }
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_TEXT: {
                            Clay_TextElementConfig *textConfig = elementConfig->config.textElementConfig;
                            CLAY({ .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
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
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_IMAGE: {
                            Clay_ImageElementConfig *imageConfig = elementConfig->config.imageElementConfig;
                            CLAY({ .id = CLAY_ID("Clay__DebugViewElementInfoImageBody"), .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                                // .sourceDimensions
                                CLAY_TEXT(CLAY_STRING("Source Dimensions"), infoTitleConfig);
                                CLAY({ .id = CLAY_ID("Clay__DebugViewElementInfoImageDimensions") }) {
                                    CLAY_TEXT(CLAY_STRING("{ width: "), infoTextConfig);
                                    CLAY_TEXT(Clay__IntToString(imageConfig->sourceDimensions.width), infoTextConfig);
                                    CLAY_TEXT(CLAY_STRING(", height: "), infoTextConfig);
                                    CLAY_TEXT(Clay__IntToString(imageConfig->sourceDimensions.height), infoTextConfig);
                                    CLAY_TEXT(CLAY_STRING(" }"), infoTextConfig);
                                }
                                // Image Preview
                                CLAY_TEXT(CLAY_STRING("Preview"), infoTitleConfig);
                                CLAY({ .layout = { .sizing = { .width = CLAY_SIZING_GROW(0, imageConfig->sourceDimensions.width) }}, .image = *imageConfig }) {}
                            }
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_SCROLL: {
                            Clay_ScrollElementConfig *scrollConfig = elementConfig->config.scrollElementConfig;
                            CLAY({ .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                                // .vertical
                                CLAY_TEXT(CLAY_STRING("Vertical"), infoTitleConfig);
                                CLAY_TEXT(scrollConfig->vertical ? CLAY_STRING("true") : CLAY_STRING("false") , infoTextConfig);
                                // .horizontal
                                CLAY_TEXT(CLAY_STRING("Horizontal"), infoTitleConfig);
                                CLAY_TEXT(scrollConfig->horizontal ? CLAY_STRING("true") : CLAY_STRING("false") , infoTextConfig);
                            }
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_FLOATING: {
                            Clay_FloatingElementConfig *floatingConfig = elementConfig->config.floatingElementConfig;
                            CLAY({ .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                                // .offset
                                CLAY_TEXT(CLAY_STRING("Offset"), infoTitleConfig);
                                CLAY({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
                                    CLAY_TEXT(CLAY_STRING("{ x: "), infoTextConfig);
                                    CLAY_TEXT(Clay__IntToString(floatingConfig->offset.x), infoTextConfig);
                                    CLAY_TEXT(CLAY_STRING(", y: "), infoTextConfig);
                                    CLAY_TEXT(Clay__IntToString(floatingConfig->offset.y), infoTextConfig);
                                    CLAY_TEXT(CLAY_STRING(" }"), infoTextConfig);
                                }
                                // .expand
                                CLAY_TEXT(CLAY_STRING("Expand"), infoTitleConfig);
                                CLAY({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
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
                        case CLAY__ELEMENT_CONFIG_TYPE_BORDER: {
                            Clay_BorderElementConfig *borderConfig = elementConfig->config.borderElementConfig;
                            CLAY({ .id = CLAY_ID("Clay__DebugViewElementInfoBorderBody"), .layout = { .padding = attributeConfigPadding, .childGap = 8, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
                                CLAY_TEXT(CLAY_STRING("Border Widths"), infoTitleConfig);
                                CLAY({ .layout = { .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
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
                            break;
                        }
                        case CLAY__ELEMENT_CONFIG_TYPE_CUSTOM:
                        default: break;
                    }
                }
            }
        } else {
            CLAY({ .id = CLAY_ID("Clay__DebugViewWarningsScrollPane"), .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(300)}, .childGap = 6, .layoutDirection = CLAY_TOP_TO_BOTTOM }, .backgroundColor = CLAY__DEBUGVIEW_COLOR_2, .scroll = { .horizontal = true, .vertical = true } }) {
                Clay_TextElementConfig *warningConfig = CLAY_TEXT_CONFIG({ .textColor = CLAY__DEBUGVIEW_COLOR_4, .fontSize = 16, .wrapMode = CLAY_TEXT_WRAP_NONE });
                CLAY({ .id = CLAY_ID("Clay__DebugViewWarningItemHeader"), .layout = { .sizing = {.height = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING, 0, 0 }, .childGap = 8, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} } }) {
                    CLAY_TEXT(CLAY_STRING("Warnings"), warningConfig);
                }
                CLAY({ .id = CLAY_ID("Clay__DebugViewWarningsTopBorder"), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(1)} }, .backgroundColor = {200, 200, 200, 255} }) {}
                int32_t previousWarningsLength = context->warnings.length;
                for (int32_t i = 0; i < previousWarningsLength; i++) {
                    Clay__Warning warning = context->warnings.internalArray[i];
                    CLAY({ .id = CLAY_IDI("Clay__DebugViewWarningItem", i), .layout = { .sizing = {.height = CLAY_SIZING_FIXED(CLAY__DEBUGVIEW_ROW_HEIGHT)}, .padding = {CLAY__DEBUGVIEW_OUTER_PADDING, CLAY__DEBUGVIEW_OUTER_PADDING, 0, 0 }, .childGap = 8, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} } }) {
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
    uintptr_t nextAllocOffset = arena->nextAllocation + (64 - (arena->nextAllocation % 64));
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
        fakeContext.maxMeasureTextCacheWordCount = currentContext->maxElementCount;
    }
    // Reserve space in the arena for the context, important for calculating min memory size correctly
    Clay__Context_Allocate_Arena(&fakeContext.internalArena);
    Clay__InitializePersistentMemory(&fakeContext);
    Clay__InitializeEphemeralMemory(&fakeContext);
    return fakeContext.internalArena.nextAllocation + 128;
}

CLAY_WASM_EXPORT("Clay_CreateArenaWithCapacityAndMemory")
Clay_Arena Clay_CreateArenaWithCapacityAndMemory(uint32_t capacity, void *memory) {
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
            Clay_LayoutElement *currentElement = Clay_LayoutElementArray_Get(&context->layoutElements, Clay__int32_tArray_GetValue(&dfsBuffer, (int)dfsBuffer.length - 1));
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

                    if (mapItem->idAlias != 0) {
                        Clay__ElementIdArray_Add(&context->pointerOverIds, CLAY__INIT(Clay_ElementId) { .id = mapItem->idAlias });
                    }
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
        if (found && Clay__ElementHasConfig(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING) &&
                Clay__FindElementConfigWithType(rootElement, CLAY__ELEMENT_CONFIG_TYPE_FLOATING).floatingElementConfig->pointerCaptureMode == CLAY_POINTER_CAPTURE_MODE_CAPTURE) {
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
        Clay_ScrollElementConfig *scrollConfig = Clay__FindElementConfigWithType(scrollElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL).scrollElementConfig;
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
    Clay__ConfigureOpenElement(CLAY__INIT(Clay_ElementDeclaration) {
            .id = CLAY_ID("Clay__RootContainer"),
            .layout = { .sizing = {CLAY_SIZING_FIXED((rootDimensions.width)), CLAY_SIZING_FIXED(rootDimensions.height)} }
    });
    Clay__int32_tArray_Add(&context->openLayoutElementStack, 0);
    Clay__LayoutElementTreeRootArray_Add(&context->layoutElementTreeRoots, CLAY__INIT(Clay__LayoutElementTreeRoot) { .layoutElementIndex = 0 });
}

CLAY_WASM_EXPORT("Clay_EndLayout")
Clay_RenderCommandArray Clay_EndLayout(void) {
    Clay_Context* context = Clay_GetCurrentContext();
    Clay__CloseElement();
    bool elementsExceededBeforeDebugView = context->booleanWarnings.maxElementsExceeded;
    if (context->debugModeEnabled && !elementsExceededBeforeDebugView) {
        context->warningsEnabled = false;
        Clay__RenderDebugView();
        context->warningsEnabled = true;
    }
    if (context->booleanWarnings.maxElementsExceeded) {
        Clay_String message;
        if (!elementsExceededBeforeDebugView) {
            message = CLAY_STRING("Clay Error: Layout elements exceeded Clay__maxElementCount after adding the debug-view to the layout.");
        } else {
            message = CLAY_STRING("Clay Error: Layout elements exceeded Clay__maxElementCount");
        }
        Clay__AddRenderCommand(CLAY__INIT(Clay_RenderCommand ) {
            .boundingBox = { context->layoutDimensions.width / 2 - 59 * 4, context->layoutDimensions.height / 2, 0, 0 },
            .renderData = { .text = { .stringContents = CLAY__INIT(Clay_StringSlice) { .length = message.length, .chars = message.chars, .baseChars = message.chars }, .textColor = {255, 0, 0, 255}, .fontSize = 16 } },
            .commandType = CLAY_RENDER_COMMAND_TYPE_TEXT
        });
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
                .config = *Clay__FindElementConfigWithType(scrollContainerData->layoutElement, CLAY__ELEMENT_CONFIG_TYPE_SCROLL).scrollElementConfig,
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
