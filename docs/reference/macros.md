---
icon: lucide/braces
---

# Element macros

### CLAY()

**Usage:** `CLAY(...configuration) { ...children }`

**Lifecycle:** `Clay_BeginLayout()` → `CLAY()` → `Clay_EndLayout()`

Opens a generic, configurable container that supports nested children. Requires a parameter — use `CLAY(0)` for an unconfigured element.

```c
CLAY(CLAY_ID("Outer"), { .layout = { .padding = CLAY_PADDING_ALL(16) } }) {
    CLAY(CLAY_ID("SideBar"), { .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 16 } }) {
        // Children laid out top to bottom with a 16 px gap between them
    }
    CLAY(CLAY_ID("ScrollContainer"), {
        .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 16 },
        .backgroundColor = { 200, 200, 100, 255 },
        .cornerRadius = CLAY_CORNER_RADIUS(10),
        .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() }
    }) {
        // child elements
    }
}
```

---

### CLAY_AUTO_ID()

A version of `CLAY()` that generates an ID automatically instead of requiring one as the first argument.

The generated ID is based on the element's position in the hierarchy (same mechanism as [`CLAY_ID_LOCAL()`](#clay_id_local)) and may change between layout calls if elements are added or removed before it. For [transitions](../guide/transitions.md) and retained-mode backends to work correctly, prefer explicit IDs.

```c
CLAY_AUTO_ID({ .layout = { .padding = CLAY_PADDING_ALL(16) } }) {
    // child elements
}
```

---

### CLAY_TEXT()

**Usage:** `CLAY_TEXT(Clay_String textContents, Clay_TextElementConfig *textConfig);`

A measured, auto-wrapped text element. `.fontId` is a `uint32_t` mapped to a font asset in user code and passed through to render commands. See [Layout and styling — Text](../guide/styling-layout.md#text) for the full field list.

```c
const uint32_t FONT_ID_LATO = 3;
CLAY_TEXT(CLAY_STRING("John Smith"), { .fontId = FONT_ID_LATO, .fontSize = 24, .textColor = {255, 0, 0, 255} });
```

Subject to [visibility culling](../guide/advanced.md#visibility-culling); may emit multiple `Clay_RenderCommand`s with `commandType = CLAY_RENDER_COMMAND_TYPE_TEXT`, one per wrapped line. `Clay_RenderCommand.textContent` is a `Clay_String` **slice** of the original string — wrapping never reallocates.

---

### CLAY_ID()

`Clay_ElementId CLAY_ID(STRING_LITERAL idString)`

Generates and attaches a [`Clay_ElementId`](structures.md#clay_elementid) during declaration. Only works with string literals — for heap-allocated `char *`, use [`CLAY_SID`](#clay_sid). To regenerate the same ID outside layout declaration (e.g. for [`Clay_PointerOver`](functions.md#clay_pointerover)), use [`Clay_GetElementId`](functions.md#clay_getelementid).

```c
CLAY(CLAY_ID("Button"), { .layout = { .padding = CLAY_PADDING_ALL(16) } }) { /* ... */ }

// Later, outside layout code
bool buttonIsHovered = Clay_PointerOver(Clay_GetElementId("Button"));
```

---

### CLAY_SID()

`Clay_ElementId CLAY_SID(Clay_String idString)`

Version of [`CLAY_ID`](#clay_id) for heap-allocated `char *` data. The underlying data is not copied and must live until at least the next frame.

---

### CLAY_IDI()

`Clay_ElementId CLAY_IDI(STRING_LITERAL idString, int32_t index)`

An offset version of [`CLAY_ID`](#clay_id) — "**ID** + **I**ndex". Combines a label with an index to generate IDs for sequential elements (e.g. inside a `for` loop) without constructing dynamic strings at runtime. String-literal only; use [`CLAY_SIDI`](#clay_sidi) for heap-allocated strings.

```c
for (int index = 0; index < items.length; index++) {
    CLAY(CLAY_IDI("Item", index), { /* ... */ }) {}
}
```

---

### CLAY_SIDI()

`Clay_ElementId CLAY_SIDI(Clay_String idString, int32_t index)`

Version of [`CLAY_IDI`](#clay_idi) for heap-allocated `char *` data.

---

### CLAY_ID_LOCAL()

**Usage:** `Clay_ElementId CLAY_ID_LOCAL(STRING_LITERAL idString)`

Unlike [`CLAY_ID`](#clay_id), which must be globally unique, a local ID only needs to be unique among its siblings — it's based on the ID of its parent. Ideal for reusable components and loops.

```c
void RenderHeaderButton(ButtonData button) {
    CLAY({
        .id = CLAY_ID_LOCAL("HeaderButton"),
        .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .padding = CLAY_PADDING_ALL(16) }
    }) { /* ... */ }
}

for (int i = 0; i < headerButtons.length; i++) {
    RenderHeaderButton(headerButtons.items[i]);
}
```

---

### CLAY_SID_LOCAL()

`Clay_ElementId CLAY_SID_LOCAL(Clay_String idString)`

Version of [`CLAY_ID_LOCAL`](#clay_id_local) for heap-allocated `char *` data.

---

### CLAY_IDI_LOCAL()

`Clay_ElementId CLAY_IDI_LOCAL(STRING_LITERAL idString, int32_t index)`

Offset version of [`CLAY_ID_LOCAL`](#clay_id_local), for sequential elements inside loops without dynamic string construction.

---

### CLAY_SIDI_LOCAL()

`Clay_ElementId CLAY_SIDI_LOCAL(Clay_String idString, int32_t index)`

Version of [`CLAY_IDI_LOCAL`](#clay_idi_local) for heap-allocated `char *` data.
