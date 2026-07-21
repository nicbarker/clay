---
icon: lucide/box
---

# Data structures

### Clay_TextElementConfig

The configuration struct for [`CLAY_TEXT()`](macros.md#clay_text).

```c
Clay_TextElementConfig {
    Clay_Color textColor;
    uint16_t fontId;
    uint16_t fontSize;
    uint16_t letterSpacing;
    uint16_t lineHeight;
    Clay_TextElementConfigWrapMode wrapMode;
    Clay_TextAlignment textAlignment;
    void *userData;
};
```

See [Layout and styling — Text](../guide/styling-layout.md#text) for the full field-by-field breakdown.

---

### Clay_ElementDeclaration

The only argument to `CLAY()`.

```c
typedef struct {
    Clay_LayoutConfig layout;
    Clay_Color backgroundColor;
    Clay_Color overlayColor;
    Clay_CornerRadius cornerRadius;
    Clay_AspectRatioElementConfig aspectRatio;
    Clay_ImageElementConfig image;
    Clay_FloatingElementConfig floating;
    Clay_CustomElementConfig custom;
    Clay_ClipElementConfig clip;
    Clay_BorderElementConfig border;
    Clay_TransitionElementConfig transition;
    void *userData;
} Clay_ElementDeclaration;
```

`.overlayColor` blends a color over the element and its children — see [Layout and styling — Background color, corner radius, and overlays](../guide/styling-layout.md#background-color-corner-radius-and-overlays). `.transition` configures [element transitions](../guide/transitions.md). `.userData` is passed through transparently to the corresponding `Clay_RenderCommand`s. See [Building UI hierarchies](../guide/building-ui.md) and [Layout and styling](../guide/styling-layout.md) for field-by-field usage.

---

### Clay_LayoutConfig

Layout-affecting options — settings that change the position/size of an element, its parents, siblings, and children.

```c
Clay_LayoutConfig {
    Clay_LayoutDirection layoutDirection = CLAY_LEFT_TO_RIGHT (default) | CLAY_TOP_TO_BOTTOM;
    Clay_Padding padding { u16 left; u16 right; u16 top; u16 bottom; };
    uint16_t childGap;
    Clay_ChildAlignment childAlignment {
        .x = CLAY_ALIGN_X_LEFT (default) | CLAY_ALIGN_X_CENTER | CLAY_ALIGN_X_RIGHT;
        .y = CLAY_ALIGN_Y_TOP (default) | CLAY_ALIGN_Y_CENTER | CLAY_ALIGN_Y_BOTTOM;
    };
    Clay_Sizing sizing {
        .width  = CLAY_SIZING_FIT(min, max) (default) | CLAY_SIZING_GROW(min, max) | CLAY_SIZING_FIXED(w) | CLAY_SIZING_PERCENT(p)
        .height = CLAY_SIZING_FIT(min, max) (default) | CLAY_SIZING_GROW(min, max) | CLAY_SIZING_FIXED(h) | CLAY_SIZING_PERCENT(p)
    };
};
```

See [Layout and styling](../guide/styling-layout.md) for details on every field.

---

### Clay_ImageElementConfig

```c
Clay_ImageElementConfig {
    void * imageData;
};
```

`.imageData` is an opaque pointer passed through to the renderer. Combine with `.aspectRatio` to preserve an image's original proportions when scaling. See [Layout and styling — Images](../guide/styling-layout.md#images-and-aspect-ratio).

---

### Clay_AspectRatioElementConfig

```c
Clay_AspectRatioElementConfig {
    float aspectRatio;
};
```

Enforces a fixed width/height ratio — mostly for images, but usable on any element. `.aspectRatio = 16.0 / 9.0` and `.aspectRatio = { .aspectRatio = 16.0 / 9.0 }` are equivalent (C passes the value to the first nested field).

---

### Clay_ClipElementConfig

```c
Clay_ClipElementConfig {
    bool horizontal;
    bool vertical;
    Clay_Vector2 childOffset;
};
```

Configures scrolling/clipping. `Clay_SetPointerState()` and `Clay_UpdateScrollContainers()` must be called **before** `BeginLayout` for scroll input to register. See [Scrolling elements](../guide/scrolling.md).

---

### Clay_BorderElementConfig

```c
typedef struct Clay_BorderElementConfig {
    Clay_Color color;
    Clay_BorderWidth width {
        uint16_t left; uint16_t right; uint16_t top; uint16_t bottom; uint16_t betweenChildren;
    };
} Clay_BorderElementConfig;
```

Border width insets and overlaps content — it does not affect layout sizing. See [Layout and styling — Borders](../guide/styling-layout.md#borders).

---

### Clay_FloatingElementConfig

```c
Clay_FloatingElementConfig {
    Clay_Vector2 offset;
    Clay_Dimensions expand;
    uint32_t parentId;
    int16_t zIndex;
    Clay_FloatingAttachPoints attachPoints { .element = ...; .parent = ...; };
    Clay_PointerCaptureMode pointerCaptureMode {
        CLAY_POINTER_CAPTURE_MODE_CAPTURE (default), CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH
    };
    Clay_FloatingAttachToElement attachTo {
        CLAY_ATTACH_TO_NONE (default), CLAY_ATTACH_TO_PARENT, CLAY_ATTACH_TO_ELEMENT_WITH_ID, CLAY_ATTACH_TO_ROOT
    };
    Clay_FloatingClipToElement clipTo {
        CLAY_CLIP_TO_NONE (default), CLAY_CLIP_TO_ATTACHED_PARENT
    };
};
```

Attach point values: `CLAY_ATTACH_POINT_{LEFT,CENTER,RIGHT}_{TOP,CENTER,BOTTOM}`. See [Floating elements](../guide/floating-elements.md).

---

### Clay_CustomElementConfig

```c
typedef struct { void * customData; } Clay_CustomElementConfig;
```

See [Custom elements](../guide/custom-elements.md).

---

### Clay_TransitionElementConfig

```c
typedef struct Clay_TransitionElementConfig {
    bool (*handler)(Clay_TransitionCallbackArguments arguments);
    float duration;
    Clay_TransitionProperty properties; // bitfield, see below
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
```

See [Transitions](../guide/transitions.md) for the full field breakdown and the `Clay_TransitionProperty` flag values.

---

### Clay_Color

```c
typedef struct { float r, g, b, a; } Clay_Color;
```

RGBA color. By convention channels are `0`–`255`, but interpretation is left to the renderer.

---

### Clay_String

```c
typedef struct {
    bool isStaticallyAllocated;
    int32_t length;
    const char *chars;
} Clay_String;
```

Clay's internal string representation.

- **`.isStaticallyAllocated`** — whether the string lives for the entire program lifetime.
- **`.length`** — character count, **excluding** any null terminator.
- **`.chars`** — pointer to string data; **not guaranteed to be null terminated.**

---

### Clay_ElementId

```c
typedef struct {
    uint32_t id;
    uint32_t offset;
    uint32_t baseId;
    Clay_String stringId;
} Clay_ElementId;
```

Returned by [`CLAY_ID`](macros.md#clay_id) / [`CLAY_IDI`](macros.md#clay_idi).

- **`.id`** — unique hash derived from the input string.
- **`.offset`** — the index passed to `CLAY_IDI`; always `0` for `CLAY_ID`.
- **`.baseId`** — hash of the base string before hashing with `.offset`; equal to `.id` for `CLAY_ID`.
- **`.stringId`** — the original source string.

---

### Clay_RenderCommandArray

```c
typedef struct {
    int32_t capacity;
    int32_t length;
    Clay_RenderCommand *internalArray;
} Clay_RenderCommandArray;
```

Returned by [`Clay_EndLayout`](functions.md#clay_endlayout). Valid entries run from `.internalArray[0]` to `.internalArray[.length - 1]`.

---

### Clay_RenderCommand

```c
typedef struct {
    Clay_BoundingBox boundingBox;
    Clay_RenderData renderData;
    void* userData;
    uint32_t id;
    int16_t zIndex;
    Clay_RenderCommandType commandType;
} Clay_RenderCommand;
```

**`.commandType`** values:

| Value | Meaning |
|---|---|
| `CLAY_RENDER_COMMAND_TYPE_NONE` | Ignored; never emitted under normal conditions |
| `CLAY_RENDER_COMMAND_TYPE_RECTANGLE` | Draw a rectangle — `.renderData.rectangle` |
| `CLAY_RENDER_COMMAND_TYPE_BORDER` | Draw a border — `.renderData.border` |
| `CLAY_RENDER_COMMAND_TYPE_TEXT` | Draw text — `.renderData.text` |
| `CLAY_RENDER_COMMAND_TYPE_IMAGE` | Draw an image — `.renderData.image` |
| `CLAY_RENDER_COMMAND_TYPE_SCISSOR_START` / `_END` | Begin/end pixel culling outside `.boundingBox` ([glScissor](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glScissor.xhtml)-style); not subject to visibility culling |
| `CLAY_RENDER_COMMAND_TYPE_OVERLAY_COLOR_START` / `_END` | Begin/end an overlay color blend, similar to `mix(source, target, alpha)`; nested overlays may need a renderer-side stack |
| `CLAY_RENDER_COMMAND_TYPE_CUSTOM` | User-controlled — `.renderData.custom` |

`.boundingBox` is a `{x, y, width, height}` rect with `.x/.y` at the top-left. `.id` is the originating element's ID. `.zIndex` derives from the enclosing floating configuration, if any — higher renders on top.

**`.renderData`** is a union selected by `.commandType`:

```c
typedef union {
    Clay_RectangleRenderData rectangle;
    Clay_TextRenderData text;
    Clay_ImageRenderData image;
    Clay_CustomRenderData custom;
    Clay_BorderRenderData border;
} Clay_RenderData;
```

```c
typedef struct { Clay_StringSlice stringContents; Clay_Color textColor; uint16_t fontId; uint16_t fontSize; uint16_t letterSpacing; uint16_t lineHeight; } Clay_TextRenderData;
typedef struct { Clay_Color backgroundColor; Clay_CornerRadius cornerRadius; } Clay_RectangleRenderData;
typedef struct { Clay_Color backgroundColor; Clay_CornerRadius cornerRadius; void* imageData; } Clay_ImageRenderData;
typedef struct { Clay_Color backgroundColor; Clay_CornerRadius cornerRadius; void* customData; } Clay_CustomRenderData;
typedef struct { Clay_Color color; Clay_CornerRadius cornerRadius; Clay_BorderWidth width; } Clay_BorderRenderData;
```

---

### Clay_ScrollContainerData

```c
typedef struct {
    Clay_Vector2 *scrollPosition; // live pointer — mutating it changes final layout
    Clay_Dimensions scrollContainerDimensions;
    Clay_Dimensions contentDimensions;
    Clay_ClipElementConfig config;
    bool found;
} Clay_ScrollContainerData;
```

Returned by [`Clay_GetScrollContainerData`](functions.md#clay_getscrollcontainerdata). `.scrollPosition` is a pointer into Clay's real internal state — intended for scroll bars or auto-scroll logic. Scrolling is only possible when `.contentDimensions` exceeds `.scrollContainerDimensions` in at least one axis.

---

### Clay_ElementData

```c
typedef struct {
    Clay_BoundingBox boundingBox; // relative to the root of the layout
    bool found;
} Clay_ElementData;
```

Returned by [`Clay_GetElementData`](functions.md#clay_getelementdata). If `.found` is `false`, `.boundingBox` is zeroed.

---

### Clay_PointerData

```c
typedef struct {
    Clay_Vector2 position;
    Clay_PointerDataInteractionState state;
} Clay_PointerData;
```

`.state` values — see [Mouse and pointer interactions](../guide/interactions.md#pointer-interaction-states):

```c
typedef enum {
    CLAY_POINTER_DATA_PRESSED_THIS_FRAME,
    CLAY_POINTER_DATA_PRESSED,
    CLAY_POINTER_DATA_RELEASED_THIS_FRAME,
    CLAY_POINTER_DATA_RELEASED,
} Clay_PointerDataInteractionState;
```

---

### Clay_ErrorHandler / Clay_ErrorData

See [Error handling](errors.md).
