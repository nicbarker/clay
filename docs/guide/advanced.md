---
icon: lucide/settings-2
---

# Advanced topics

## Retained mode rendering

Clay was originally designed for [immediate mode](https://www.youtube.com/watch?v=Z1qyvQsjK5Y) rendering, where the entire UI is redrawn every frame. If your platform, renderer, or performance constraints require retained mode instead, two techniques help:

- Each [`Clay_RenderCommand`](../reference/structures.md#clay_rendercommand) includes the `uint32_t id` used to declare the element. With unique IDs, you can map these to persistent graphics objects across frames.
- Render commands are culled to only currently visible elements, and `Clay_RenderCommand` is small enough to memcmp two commands with matching IDs to detect whether an element is "dirty" and needs re-rendering.

See the [HTML renderer](https://github.com/nicbarker/clay/blob/main/renderers/web/html/clay-html-renderer.html) for a worked example that converts Clay layouts into persistent HTML documents with minimal per-frame changes.

## Visibility culling

Clay culls output by default: it only emits render commands for elements with at least one pixel of their bounding box inside the viewport. Disable this with `#define CLAY_DISABLE_CULLING`.

## Preprocessor directives

Set these either in code (`#define CLAY_DISABLE_CULLING`) or via compiler flags (`clang -DCLAY_DISABLE_CULLING main.c`):

- **`CLAY_WASM`** — required when targeting WebAssembly.
- **`CLAY_DLL`** — required when building a `.dll`.
- **`CLAY_DISABLE_CULLING`** — disables [visibility culling](#visibility-culling).

## Running more than one Clay instance

Clay supports multiple instances in the same program. [`Clay_Initialize`](../reference/functions.md#clay_initialize) returns a `Clay_Context*`. Activate a specific instance with [`Clay_SetCurrentContext`](../reference/functions.md#clay_setcurrentcontext) — if never called, Clay defaults to the most recently initialized context.

```c
Clay_Arena arena1, arena2;
// ... allocate arenas

Clay_Context* instance1 = Clay_Initialize(arena1, layoutDimensions, errorHandler);
Clay_Context* instance2 = Clay_Initialize(arena2, layoutDimensions, errorHandler);

Clay_SetCurrentContext(instance1);
Clay_BeginLayout();
// ... declare layout for instance1
Clay_RenderCommandArray renderCommands1 = Clay_EndLayout();
render(renderCommands1);

Clay_SetCurrentContext(instance2);
Clay_BeginLayout();
// ... declare layout for instance2
Clay_RenderCommandArray renderCommands2 = Clay_EndLayout();
render(renderCommands2);
```

!!! warning "Not thread-safe"
    Do not render instances across different threads simultaneously — Clay does not currently support multi-threading.

## Sizing an arena for a larger UI

If your layout exceeds the default element count or text cache, use [`Clay_SetMaxElementCount`](../reference/functions.md#clay_setmaxelementcount) or [`Clay_SetMaxMeasureTextCacheWordCount`](../reference/functions.md#clay_setmaxmeasuretextcachewordcount), then call [`Clay_MinMemorySize()`](../reference/functions.md#clay_minmemorysize) again and reinitialize with the new size. See [Error handling](../reference/errors.md) for the errors Clay raises when capacity is exceeded.
