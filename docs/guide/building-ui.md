---
icon: lucide/layers
---

# Building UI hierarchies

## The CLAY() macro

Clay UIs are built using the C macro `CLAY(id, { configuration })`. This macro creates a new, empty element in the UI hierarchy and supports modular customization of layout, styling, and functionality. `CLAY()` can be nested, similar to other declarative UI systems like HTML.

Child elements are added by opening a block (`{}`) after calling `CLAY()`, exactly like you would with an `if` statement or `for` loop:

```c
// Parent element with 8px of padding
CLAY(CLAY_ID("parent"), { .layout = { .padding = CLAY_PADDING_ALL(8) } }) {
    // Child element 1
    CLAY_TEXT(CLAY_STRING("Hello World"), { .fontSize = 16 });
    // Child element 2 with red background
    CLAY(CLAY_ID("child"), { .backgroundColor = COLOR_RED }) {
        // etc
    }
}
```

Unlike HTML and other declarative DSLs, this macro is just C. You can use arbitrary C code — loops, functions, conditionals — inside your layout declaration:

```c
// Re-usable "components" are just functions that declare more UI
void ButtonComponent(Clay_String buttonText) {
    // Red box button with 8px of padding
    CLAY_AUTO_ID({ .layout = { .padding = CLAY_PADDING_ALL(8) }, .backgroundColor = COLOR_RED }) {
        CLAY_TEXT(buttonText, textConfig);
    }
}

// Parent element
CLAY(CLAY_ID("parent"), { .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
    // Render a bunch of text elements
    for (int i = 0; i < textArray.length; i++) {
        CLAY_TEXT(textArray.elements[i], textConfig);
    }
    // Only render this element if we're on a mobile screen
    if (isMobileScreen) {
        CLAY(0) {
            // etc
        }
    }
    // Re-usable components
    ButtonComponent(CLAY_STRING("Click me!"));
    ButtonComponent(CLAY_STRING("No, click me!"));
}
```

## Configuring layout and styling

The layout and style of an element is configured with the [`Clay_ElementDeclaration`](../reference/structures.md#clay_elementdeclaration) struct passed to `CLAY()`:

```c
CLAY(CLAY_ID("box"), { .layout = { .padding = { 8, 8, 8, 8 }, .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
    // Children are 8px inset into parent, and laid out top to bottom
}
```

This macro isn't magic — it just wraps standard designated initializer syntax. A `Clay_ElementDeclaration` can also be defined in file scope and reused across elements:

```c
// Define a style in the global / file scope
Clay_ElementDeclaration reuseableStyle = (Clay_ElementDeclaration) {
    .layout = { .padding = { .left = 12 } },
    .backgroundColor = { 120, 120, 120, 255 },
    .cornerRadius = { 12, 12, 12, 12 }
};

CLAY(CLAY_ID("box"), reuseableStyle) {
    // ...
}
```

See [Layout and styling](styling-layout.md) for the full set of layout options.

## Element IDs

The `CLAY()` macro accepts an ID as its first argument, usually provided by [`CLAY_ID()`](../reference/macros.md#clay_id). Elements can also be created with auto-generated IDs using [`CLAY_AUTO_ID()`](../reference/macros.md#clay_auto_id):

```c
// Will always produce the same ID from the same input string
CLAY(CLAY_ID("OuterContainer"), { ...configuration }) {}

// Generates a unique ID that may not be the same between two layout calls
CLAY_AUTO_ID({ ...configuration }) {}
```

IDs serve two purposes:

1. **Querying** — tagging an element with an ID lets you later ask about its [hover state](../reference/functions.md#clay_hovered) or dimensions.
2. **Readability** — IDs make it easier to read and modify UI code, and are used by the built-in [debug tools](../ecosystem.md#debug-tools).

To avoid constructing dynamic strings at runtime inside loops, use [`CLAY_IDI(string, index)`](../reference/macros.md#clay_idi) ("**ID** + **I**ndex") to generate different IDs from a single input string:

```c
// This is the equivalent of calling CLAY_ID("Item0"), CLAY_ID("Item1") etc
for (int index = 0; index < items.length; index++) {
    CLAY(CLAY_IDI("Item", index), { ..configuration }) {}
}
```

This ID is forwarded to the final [`Clay_RenderCommandArray`](../reference/structures.md#clay_rendercommandarray) for use in retained mode UIs. **Duplicate IDs may cause some functionality to misbehave** — for example, if you're attaching a [floating container](floating-elements.md) to a specific element by ID and that ID is duplicated, it may not attach where you expect.

For components and loops where an ID only needs to be unique among siblings (not globally), use [`CLAY_ID_LOCAL()`](../reference/macros.md#clay_id_local) instead.

## Next steps

- [Layout and styling](styling-layout.md) — sizing, padding, alignment, and text
- [Mouse and pointer interactions](interactions.md) — hover and click handling
