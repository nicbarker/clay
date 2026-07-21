---
icon: lucide/scroll
---

# Scrolling elements

Elements become scrollable via the `.clip` configuration ([`Clay_ClipElementConfig`](../reference/structures.md#clay_clipelementconfig)). Clipping instructs the renderer not to draw pixels outside the clipped element's bounds; specifying `.childOffset` shifts the clipped content to produce scrolling.

You can calculate scroll offsets yourself and pass them to `.childOffset` each frame, or let Clay manage scroll state for you.

## Built-in scroll tracking

Two functions must be called **before** `Clay_BeginLayout()` each frame to enable mouse wheel and drag scrolling:

```c
Clay_Vector2 mousePosition = { x, y };
// Reminder: Clay_SetPointerState must be called before Clay_UpdateScrollContainers otherwise it will have no effect
Clay_SetPointerState(mousePosition, isPointerDown);
// Clay_UpdateScrollContainers needs to be called before Clay_BeginLayout for the position to avoid a 1 frame delay
Clay_UpdateScrollContainers(
    true, // Enable drag scrolling
    scrollDelta, // Clay_Vector2 scrollwheel / trackpad scroll x and y delta this frame
    deltaTime, // Time since last frame in seconds
);
```

Then use [`Clay_GetScrollOffset()`](../reference/functions.md#clay_getscrolloffset) to read the internally tracked offset for the currently open element:

```c
CLAY(CLAY_ID("ScrollContainer"), { .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() } }) {
    // Scrolling contents
}
```

## Managing scroll position yourself

`.childOffset` can also be provided directly, bypassing Clay's internal tracking:

```c
CLAY(CLAY_ID("ScrollContainer"), { .clip = { .vertical = true, .childOffset = myData.scrollContainer.offset } }) {
    // Scrolling contents
}
```

To disable drag/touch scrolling while still processing mouse wheel input, call `Clay_UpdateScrollContainers` with `false` as the first argument.

## Reading and mutating scroll state

[`Clay_GetScrollContainerData(id)`](../reference/functions.md#clay_getscrollcontainerdata) returns [`Clay_ScrollContainerData`](../reference/structures.md#clay_scrollcontainerdata), which includes a **live pointer** to the internal scroll position. This is intended for building scroll bars, "jump to" buttons, or auto-scroll — mutating `.scrollPosition` shifts the container's contents on the next layout.

## Rendering clip elements

Enabling `.clip` emits two extra render commands, neither subject to [visibility culling](advanced.md#visibility-culling):

- `CLAY_RENDER_COMMAND_TYPE_SCISSOR_START` — create a rectangle mask with `.boundingBox`
- `CLAY_RENDER_COMMAND_TYPE_SCISSOR_END` — disable the previous mask

```c
CLAY(CLAY_ID("ScrollOuter"), { .clip = { .vertical = true } }) {
    // Fixed-height child content larger than the container triggers clipping
    CLAY(CLAY_ID("ScrollInner"), { .layout = { .sizing = { .height = CLAY_SIZING_FIXED(5000) } } }) {}
}
```
