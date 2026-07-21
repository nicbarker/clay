---
icon: lucide/function-square
---

# Public functions

## Lifecycle

**At startup, run once:**
`Clay_MinMemorySize` → `Clay_CreateArenaWithCapacityAndMemory` → `Clay_Initialize` → `Clay_SetMeasureTextFunction`

**Every frame:**
`Clay_SetLayoutDimensions` → `Clay_SetPointerState` → `Clay_UpdateScrollContainers` → `Clay_BeginLayout` → `CLAY() etc...` → `Clay_EndLayout`

---

### Clay_MinMemorySize

`uint32_t Clay_MinMemorySize()`

Returns the minimum bytes Clay needs to accommodate the current [`CLAY_MAX_ELEMENT_COUNT`](../guide/advanced.md#preprocessor-directives).

---

### Clay_CreateArenaWithCapacityAndMemory

`Clay_Arena Clay_CreateArenaWithCapacityAndMemory(size_t capacity, void *memory)`

Creates a `Clay_Arena` with the given capacity and base memory pointer, for use with [`Clay_Initialize`](#clay_initialize).

---

### Clay_SetMeasureTextFunction

`void Clay_SetMeasureTextFunction(Clay_Dimensions (*measureTextFunction)(Clay_StringSlice text, Clay_TextElementConfig *config, void* userData), void* userData)`

Sets the function Clay uses to measure `width, height` of a string during layout, for sizing and wrapping `CLAY_TEXT` elements.

**The provided string is not guaranteed to be null terminated** — Clay uses slices to avoid cloning strings during wrapping. If your renderer needs null-terminated C strings (e.g. Raylib), clone before rendering.

**This function is on the hot path.** For text-heavy UIs it is called many times per layout; despite Clay's internal caching, a slow measure function easily dominates total layout cost.

---

### Clay_ResetMeasureTextCache

`void Clay_ResetMeasureTextCache(void)`

Clay caches measurements from the measure-text function. Call this to force recalculation when an external factor the cache doesn't know about changes (e.g. a DPI change).

---

### Clay_SetMaxElementCount

`void Clay_SetMaxElementCount(int32_t maxElementCount)`

Sets the max element count used by subsequent [`Clay_Initialize`](#clay_initialize) / [`Clay_MinMemorySize`](#clay_minmemorysize) calls. **Requires reinitializing Clay** with a recalculated memory size afterward.

---

### Clay_SetMaxMeasureTextCacheWordCount

`void Clay_SetMaxMeasureTextCacheWordCount(int32_t maxMeasureTextCacheWordCount)`

Sets how many separate words the text measurement cache can hold, used by subsequent [`Clay_Initialize`](#clay_initialize) / [`Clay_MinMemorySize`](#clay_minmemorysize) calls. **Requires reinitializing Clay** afterward.

---

### Clay_Initialize

`Clay_Context* Clay_Initialize(Clay_Arena arena, Clay_Dimensions layoutDimensions, Clay_ErrorHandler errorHandler)`

Initializes internal memory mapping, sets layout dimensions, and binds the error handler. Returns a `Clay_Context*` usable with [`Clay_SetCurrentContext`](#clay_setcurrentcontext) for [multiple Clay instances](../guide/advanced.md#running-more-than-one-clay-instance), and sets it as the current context.

---

### Clay_SetCurrentContext

`void Clay_SetCurrentContext(Clay_Context* context)`

Sets the context subsequent Clay commands operate on. See [multiple instances](../guide/advanced.md#running-more-than-one-clay-instance).

---

### Clay_GetCurrentContext

`Clay_Context* Clay_GetCurrentContext()`

Returns the currently active context, or null if none has been set.

---

### Clay_SetLayoutDimensions

`void Clay_SetLayoutDimensions(Clay_Dimensions dimensions)`

Sets internal layout dimensions. Cheap enough to call every frame to respond to window resizing.

---

### Clay_SetPointerState

`void Clay_SetPointerState(Clay_Vector2 position, bool isPointerDown)`

Sets internal pointer position/state and recalculates overlap info, used by [`Clay_PointerOver`](#clay_pointerover) and [`Clay_UpdateScrollContainers`](#clay_updatescrollcontainers). `isPointerDown` should reflect the current frame's state — `true` for the entire duration the button is held. Clay handles detecting click/touch start & end internally.

---

### Clay_UpdateScrollContainers

`void Clay_UpdateScrollContainers(bool enableDragScrolling, Clay_Vector2 scrollDelta, float deltaTime)`

Handles scrolling from `scrollDelta` (mouse wheel / trackpad this frame) and touch/drag scrolling. Drag scrolling only occurs if `enableDragScrolling` is `true` **and** [`Clay_SetPointerState`](#clay_setpointerstate) was called this frame — pass `false` to disable touch scrolling entirely. `deltaTime` is seconds since the last frame, used to normalize scrolling across refresh rates.

---

### Clay_GetScrollOffset

`Clay_Vector2 Clay_GetScrollOffset()`

Returns the internally tracked scroll offset for the currently open element. Intended for `.clip.childOffset` — see [Scrolling elements](../guide/scrolling.md).

---

### Clay_BeginLayout

`void Clay_BeginLayout()`

Prepares Clay to calculate a new layout. Call once per frame before any [element macros](macros.md).

---

### Clay_EndLayout

`Clay_RenderCommandArray Clay_EndLayout()`

Ends element declaration and calculates the layout. Returns a [`Clay_RenderCommandArray`](structures.md#clay_rendercommandarray) with the results.

---

### Clay_Hovered

`bool Clay_Hovered()`

Called **during** layout declaration; returns `true` if the last-set pointer position is inside the currently open element's **previous frame** bounding box.

---

### Clay_OnHover

`void Clay_OnHover(void (*onHoverFunction)(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData), void *userData)`

Attaches a function pointer to the currently open element, called once per layout while the pointer overlaps it. See [Mouse and pointer interactions](../guide/interactions.md).

---

### Clay_PointerOver

`bool Clay_PointerOver(Clay_ElementId id)`

Returns `true` if the last-set pointer position is inside the bounding box of the element matching `id`, using **last frame's** position. For frame-accurate results, run layout twice in the same frame.

---

### Clay_GetOpenElementId

`Clay_ElementId Clay_GetOpenElementId()`

Returns the [`Clay_ElementId`](structures.md#clay_elementid) of the currently open element — useful for elements opened with [`CLAY_AUTO_ID`](macros.md#clay_auto_id).

---

### Clay_GetScrollContainerData

`Clay_ScrollContainerData Clay_GetScrollContainerData(Clay_ElementId id)`

Returns [`Clay_ScrollContainerData`](structures.md#clay_scrollcontainerdata) for the scroll container matching `id`, for imperative scroll position manipulation (scroll bars, "jump to" buttons, etc).

---

### Clay_GetElementData

`Clay_ElementData Clay_GetElementData(Clay_ElementId id)`

Returns [`Clay_ElementData`](structures.md#clay_elementdata) for the element matching `id`, including its final calculated bounding box.

---

### Clay_GetElementId

`Clay_ElementId Clay_GetElementId(Clay_String idString)`

Returns a [`Clay_ElementId`](structures.md#clay_elementid) for the given string, for querying hover state, scroll data, etc. outside of layout declaration.
