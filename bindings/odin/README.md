### Odin Language Bindings

This directory contains bindings for the [Odin](odin-lang.org) programming language, as well as an example implementation of the [Clay website](https://nicbarker.com/clay) in Odin.

Special thanks to

- [laytan](https://github.com/laytan)
- [Dudejoe870](https://github.com/Dudejoe870)
- MrStevns from the Odin Discord server

If you haven't taken a look at the [full documentation for Clay](https://github.com/nicbarker/clay/blob/main/README.md), it's recommended that you take a look there first to familiarise yourself with the general concepts. This README is abbreviated and applies to using Clay in Odin specifically.

The **most notable difference** between the C API and the Odin bindings is the use of `if` statements to create the scope for declaring child elements, when using the equivalent of the [Element Macros](https://github.com/nicbarker/clay/blob/main/README.md#element-macros):
```C
// C form of element macros
// Define an element with 16px of x and y padding
CLAY({ .id = CLAY_ID("Outer"), .layout = { .padding = CLAY_PADDING_ALL(16) } }) {
  // Child elements here
}
```

```Odin
// Odin form of element macros
if clay.UI()({ id = clay.ID("Outer"), layout = { padding = clay.PaddingAll(16) }}) {
    // Child elements here
}
```

### Quick Start

1. Download the [clay-odin](https://github.com/nicbarker/clay/tree/main/bindings/odin/clay-odin) directory and copy it into your project.

```Odin
import clay "clay-odin"
```

2. Ask Clay for how much static memory it needs using [clay.MinMemorySize()](https://github.com/nicbarker/clay/blob/main/README.md#clay_minmemorysize), create an Arena for it to use with [clay.CreateArenaWithCapacityAndMemory(minMemorySize, memory)](https://github.com/nicbarker/clay/blob/main/README.md#clay_createarenawithcapacityandmemory), and initialize it with [clay.Initialize(clay.Arena, clay.Dimensions, clay.ErrorHandler)](https://github.com/nicbarker/clay/blob/main/README.md#clay_initialize).

```Odin
error_handler :: proc "c" (errorData: clay.ErrorData) {
    // Do something with the error data.
}

min_memory_size := clay.MinMemorySize()
memory := make([^]u8, min_memory_size)
arena: clay.Arena = clay.CreateArenaWithCapacityAndMemory(uint(min_memory_size), memory)
clay.Initialize(arena, {1080, 720}, { handler = error_handler })
``` 

3. Provide a `measure_text(text, config)` proc "c" with [clay.SetMeasureTextFunction(function)](https://github.com/nicbarker/clay/blob/main/README.md#clay_setmeasuretextfunction) so that Clay can measure and wrap text.

```Odin
// Example measure text function
measure_text :: proc "c" (
    text: clay.StringSlice,
    config: ^clay.TextElementConfig,
    userData: rawptr,
) -> clay.Dimensions {
    // clay.TextElementConfig contains members such as fontId, fontSize, letterSpacing, etc..
    // Note: clay.String->chars is not guaranteed to be null terminated
    return {
        width = f32(text.length * i32(config.fontSize)),
        height = f32(config.fontSize),
    }
}


// Tell clay how to measure text
clay.SetMeasureTextFunction(measure_text, nil)
```

4. **Optional** - Call [clay.SetPointerState(pointerPosition, isPointerDown)](https://github.com/nicbarker/clay/blob/main/README.md#clay_setpointerstate) if you want to use mouse interactions.

```Odin
// Update internal pointer position for handling mouseover / click / touch events
clay.SetPointerState(
    { mouse_pos_x, mouse_pos_y },
    is_mouse_down,
)
```

5. Call [clay.BeginLayout()](https://github.com/nicbarker/clay/blob/main/README.md#clay_beginlayout) and declare your layout using the provided macros.

```Odin
// Define some colors.
COLOR_LIGHT :: clay.Color{224, 215, 210, 255}
COLOR_RED :: clay.Color{168, 66, 28, 255}
COLOR_ORANGE :: clay.Color{225, 138, 50, 255}
COLOR_BLACK :: clay.Color{0, 0, 0, 255}

// Layout config is just a struct that can be declared statically, or inline
sidebar_item_layout := clay.LayoutConfig {
    sizing = {
        width = clay.SizingGrow({}),
        height = clay.SizingFixed(50)
    },
}

// Re-useable components are just normal procs.
sidebar_item_component :: proc(index: u32) {
    if clay.UI()({
        id = clay.ID("SidebarBlob", index),
        layout = sidebar_item_layout,
        backgroundColor = COLOR_ORANGE,
    }) {}
}

// An example function to create your layout tree
create_layout :: proc() -> clay.ClayArray(clay.RenderCommand) {
    // Begin constructing the layout.
    clay.BeginLayout()

    // An example of laying out a UI with a fixed-width sidebar and flexible-width main content
    // NOTE: To create a scope for child components, the Odin API uses `if` with components that have children
    if clay.UI()({
        id = clay.ID("OuterContainer"),
        layout = {
            sizing = { width = clay.SizingGrow({}), height = clay.SizingGrow({}) },
            padding = { 16, 16, 16, 16 },
            childGap = 16,
        },
        backgroundColor = { 250, 250, 255, 255 },
    }) {
        if clay.UI()({
            id = clay.ID("SideBar"),
            layout = {
                layoutDirection = .TopToBottom,
                sizing = { width = clay.SizingFixed(300), height = clay.SizingGrow({}) },
                padding = { 16, 16, 16, 16 },
                childGap = 16,
            },
            backgroundColor = COLOR_LIGHT,
        }) {
            if clay.UI()({
                id = clay.ID("ProfilePictureOuter"),
                layout = {
                    sizing = { width = clay.SizingGrow({}) },
                    padding = { 16, 16, 16, 16 },
                    childGap = 16,
                    childAlignment = { y = .Center },
                },
                backgroundColor = COLOR_RED,
                cornerRadius = { 6, 6, 6, 6 },
            }) {
                if clay.UI()({
                    id = clay.ID("ProfilePicture"),
                    layout = {
                        sizing = { width = clay.SizingFixed(60), height = clay.SizingFixed(60) },
                    },
                    image = {
                        // How you define `profile_picture` depends on your renderer.
                        imageData = &profile_picture,
                        sourceDimensions = {
                            width = 60,
                            height = 60,
                        },
                    },
                }) {}

                clay.Text(
                    "Clay - UI Library",
                    clay.TextConfig({ textColor = COLOR_BLACK, fontSize = 16 }),
                )
            }

            // Standard Odin code like loops, etc. work inside components.
            // Here we render 5 sidebar items.
            for i in u32(0)..<5 {
                sidebar_item_component(i)
            }
        }

        if clay.UI()({
            id = clay.ID("MainContent"),
            layout = {
                sizing = { width = clay.SizingGrow({}), height = clay.SizingGrow({}) },
            },
            backgroundColor = COLOR_LIGHT,
        }) {}
    }

    // Returns a list of render commands
    return clay.EndLayout()
}
```

6. Call your layout proc and process the resulting [clay.ClayArray(clay.RenderCommand)](https://github.com/nicbarker/clay/blob/main/README.md#clay_rendercommandarray) in your choice of renderer.

```Odin
render_commands := create_layout()

for i in 0..<i32(render_commands.length) {
    render_command := clay.RenderCommandArray_Get(render_commands, i)

    switch render_command.commandType {
    case .Rectangle:
        DrawRectangle(render_command.boundingBox, render_command.config.rectangleElementConfig.color)
    // ... Implement handling of other command types
    }
}
```

Please see the [full C documentation for Clay](https://github.com/nicbarker/clay/blob/main/README.md) for API details. All public C functions and Macros have Odin binding equivalents, generally of the form `CLAY_ID` (C) -> `clay.ID` (Odin).
