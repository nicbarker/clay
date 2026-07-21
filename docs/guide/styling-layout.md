---
icon: lucide/ruler
---

# Layout and styling

Layout options — settings that affect the final position and size of an element, its parents, siblings, and children — live under `.layout` in [`Clay_LayoutConfig`](../reference/structures.md#clay_layoutconfig). Visual-only options such as `.backgroundColor` and `.cornerRadius` live directly on [`Clay_ElementDeclaration`](../reference/structures.md#clay_elementdeclaration).

## Layout direction

`.layoutDirection` controls the axis along which children are laid out: `CLAY_LEFT_TO_RIGHT` (default) or `CLAY_TOP_TO_BOTTOM`.

```c
CLAY(CLAY_ID("Element"), { .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM } })
```

## Padding

`.padding` controls white-space around the **outside** of child elements:

```c
CLAY(CLAY_ID("Element"), { .layout = { .padding = { .left = 16, .right = 16, .top = 8, .bottom = 8 } } })
```

## Child gap

`.childGap` controls white-space **between** child elements. It applies horizontally when `.layoutDirection` is `CLAY_LEFT_TO_RIGHT` (default), and vertically for `CLAY_TOP_TO_BOTTOM`:

```c
CLAY(CLAY_ID("Element"), { .layout = { .childGap = 16 } })
```

## Child alignment

`.childAlignment` positions children relative to the height and width of the parent container:

```c
CLAY(CLAY_ID("Element"), { .layout = { .childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_CENTER } } })
```

Available values:

```c
.x = CLAY_ALIGN_X_LEFT (default) | CLAY_ALIGN_X_CENTER | CLAY_ALIGN_X_RIGHT;
.y = CLAY_ALIGN_Y_TOP (default) | CLAY_ALIGN_Y_CENTER | CLAY_ALIGN_Y_BOTTOM;
```

## Sizing

`.sizing` controls how the final width and height of an element are calculated. The same options apply to both `.width` and `.height`:

- **`CLAY_SIZING_FIT(min, max)`** (default) — sized to fit children (plus padding and gaps), up to `max` (defaults to `FLOAT_MAX`). Won't shrink below `min` when compressed.
- **`CLAY_SIZING_GROW(min, max)`** — grows to fill available space in its parent, up to `max` (defaults to `FLOAT_MAX`). Won't shrink below `min` when compressed.
- **`CLAY_SIZING_FIXED(fixed)`** — always exactly `fixed`. Shorthand for `CLAY_SIZING_FIT(fixed, fixed)`.
- **`CLAY_SIZING_PERCENT(percent)`** — a percentage (`0`–`1`) of parent size, minus padding and child gaps.

```c
CLAY(CLAY_ID("Button"), { .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(16), .childGap = 16 } }) {
    // Children will be laid out vertically with 16px of padding around and between
}
```

## Background color, corner radius, and overlays

```c
CLAY(CLAY_ID("Element"), { .backgroundColor = { 120, 120, 120, 255 } })
CLAY(CLAY_ID("Element"), { .cornerRadius = { .topLeft = 16, .topRight = 16, .bottomLeft = 16, .bottomRight = 16 } })
```

Use the `CLAY_CORNER_RADIUS(radius)` shorthand to set all four corners at once: `CLAY_CORNER_RADIUS(10)`.

`.overlayColor` blends a color over an element and its children — similar to GLSL's `mix(source, target, alpha)`. Zero alpha leaves child colors unchanged; full alpha replaces them entirely. This is a cheap way to fade elements in and out without true alpha compositing:

```c
CLAY(CLAY_ID("Element"), { .overlayColor = { 255, 120, 120, 255 } })
```

Specifying `.overlayColor` emits two extra render commands: `OVERLAY_COLOR_BEGIN` before the subtree, and `OVERLAY_COLOR_END` after it.

## Text

Text is rendered with `CLAY_TEXT(text, config)`, using [`Clay_TextElementConfig`](../reference/structures.md#clay_textelementconfig):

```c
CLAY_TEXT(CLAY_STRING("John Smith"), { .fontId = FONT_ID_LATO, .fontSize = 24, .textColor = {255, 0, 0, 255} });
```

Key fields:

| Field | Type | Notes |
|---|---|---|
| `.textColor` | `Clay_Color` | Conventionally `rgba` 0–255 |
| `.fontId` | `uint16_t` | Mapped to a real font in user code |
| `.fontSize` | `uint16_t` | Generally "x pixels tall" |
| `.letterSpacing` | `uint16_t` | Horizontal space between characters |
| `.lineHeight` | `uint16_t` | Forces wrapped line height; `0` uses the measured font height |
| `.wrapMode` | enum | `CLAY_TEXT_WRAP_WORDS` (default), `CLAY_TEXT_WRAP_NEWLINES`, `CLAY_TEXT_WRAP_NONE` |
| `.textAlignment` | enum | `CLAY_TEXT_ALIGN_LEFT` (default), `CLAY_TEXT_ALIGN_CENTER`, `CLAY_TEXT_ALIGN_RIGHT` — controls alignment of **wrapping** lines; for single-line alignment use the parent's `.childAlignment` |

Text elements are subject to [visibility culling](advanced.md#visibility-culling) and may emit multiple `Clay_RenderCommand`s — one per wrapped line.

## Images and aspect ratio

`.image` configures an element to render an image background, via [`Clay_ImageElementConfig`](../reference/structures.md#clay_imageelementconfig):

```c
CLAY(CLAY_ID("Image"), { .image = { .imageData = &profilePicture } }) {}
```

`.imageData` is an opaque `void *` your renderer resolves to an actual image/texture. To preserve an image's original aspect ratio while scaling, combine it with `.aspectRatio` ([`Clay_AspectRatioElementConfig`](../reference/structures.md#clay_aspectratioelementconfig)):

```c
CLAY(CLAY_ID("ProfilePicture"), {
    .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) } },
    .aspectRatio = profilePicture.width / profilePicture.height,
    .image = { .imageData = &profilePicture },
}) {}
```

## Borders

`.border` draws colored lines along an element's edges, via [`Clay_BorderElementConfig`](../reference/structures.md#clay_borderelementconfig). Border width is inset and **does not affect layout** — it overlaps content rather than adding to it:

```c
// 1px red border around all edges
CLAY(CLAY_ID("OuterBorder"), {
    .layout = { .sizing = { .width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_FIXED(300) } },
    .border = { .width = { 1, 1, 1, 1, 0 }, .color = COLOR_RED }
}) {}

// 5px blue border between top-to-bottom children
CLAY(CLAY_ID("OuterBorder"), {
    .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM },
    .border = { .width = { 5, 5, 5, 5, 5 }, .color = COLOR_BLUE }
}) {}
```

`.width.betweenChildren` draws lines between siblings — vertical for `CLAY_LEFT_TO_RIGHT` parents, horizontal for `CLAY_TOP_TO_BOTTOM`. Unlike the edge widths, these generate additional rectangle render commands, so the renderer needs no special handling for them.

## Next steps

- [Mouse and pointer interactions](interactions.md)
- [Scrolling elements](scrolling.md)
