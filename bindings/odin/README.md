### Odin Language Bindings

This directory contains bindings for the [Odin](odin-lang.org) programming language, as well as an example implementation of the [clay website](https://nicbarker.com/clay) in Odin.

Special thanks to

- [laytan](https://github.com/laytan)
- [Dudejoe870](https://github.com/Dudejoe870)
- MrStevns from the Odin Discord server

If you haven't taken a look at the [full documentation for clay](https://github.com/nicbarker/clay/blob/main/README.md), it's recommended that you take a look there first to familiarise yourself with the general concepts. This README is abbreviated and applies to using clay in Odin specifically.

The **most notable difference** between the C API and the Odin bindings is the use of if statements to create the scope for declaring child elements. When using the equivalent of the [Element Macros](https://github.com/nicbarker/clay/blob/main/README.md#element-macros):

```C
// C form of element macros
CLAY_RECTANGLE(CLAY_ID("SideBar"), CLAY_LAYOUT(.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW() }, .padding = {16, 16}), CLAY_RECTANGLE_CONFIG(.color = COLOR_LIGHT), {
	// Child elements here
});
```

```Odin
// Odin form of element macros
if clay.Rectangle(clay.ID("SideBar"), clay.Layout({ layoutDirection = .TOP_TO_BOTTOM, sizing = { width = clay.SizingFixed(300), height = clay.SizingGrow({}) }, padding = {16, 16} }), clay.RectangleConfig({ color = COLOR_LIGHT })) {
	// Child elements here
}
```

### Quick Start

1. Download the [clay-odin](https://github.com/nicbarker/clay/tree/main/bindings/odin/clay-odin) directory and copy it into your project.

```Odin
import clay "clay-odin"
```

2. Ask clay for how much static memory it needs using [clay.MinMemorySize()](https://github.com/nicbarker/clay/blob/main/README.md#clay_minmemorysize), create an Arena for it to use with [clay.CreateArenaWithCapacityAndMemory(minMemorySize, memory)](https://github.com/nicbarker/clay/blob/main/README.md#clay_createarenawithcapacityandmemory), and initialize it with [clay.Initialize(arena)](https://github.com/nicbarker/clay/blob/main/README.md#clay_initialize).

```Odin
minMemorySize: u32 = clay.MinMemorySize()
memory := make([^]u8, minMemorySize)
arena: clay.Arena = clay.CreateArenaWithCapacityAndMemory(minMemorySize, memory)
clay.Initialize(arena)
``` 

3. Provide a `measureText(text, config)` proc "c" with [clay.SetMeasureTextFunction(function)](https://github.com/nicbarker/clay/blob/main/README.md#clay_setmeasuretextfunction) so that clay can measure and wrap text.

```Odin
// Example measure text function
measureText :: proc "c" (text: ^clay.String, config: ^clay.TextElementConfig) -> clay.Dimensions {
    // clay.TextElementConfig contains members such as fontId, fontSize, letterSpacing etc
    // Note: clay.String->chars is not guaranteed to be null terminated
}

// Tell clay how to measure text
clay.SetMeasureTextFunction(measureText)
``` 

4. **Optional** - Call [clay.SetPointerPosition(pointerPosition)](https://github.com/nicbarker/clay/blob/main/README.md#clay_setpointerposition) if you want to use mouse interactions.

```Odin
// Update internal pointer position for handling mouseover / click / touch events
clay.SetPointerPosition(clay.Vector2{ mousePositionX, mousePositionY })
```

5. Call [clay.BeginLayout(screenWidth, screenHeight)](https://github.com/nicbarker/clay/blob/main/README.md#clay_beginlayout) and declare your layout using the provided macros.

```Odin
COLOR_LIGHT :: clay.Color{224, 215, 210, 255}
COLOR_RED :: clay.Color{168, 66, 28, 255}
COLOR_ORANGE :: clay.Color{225, 138, 50, 255}

// Layout config is just a struct that can be declared statically, or inline
sidebarItemLayout := clay.LayoutConfig {
    sizing = {width = clay.SizingGrow({}), height = clay.SizingFixed(50)},
}

// Re-useable components are just normal functions
SidebarItemComponent :: proc(index: u32) {
    if clay.Rectangle(clay.ID("SidebarBlob", index), &sidebarItemLayout, clay.RectangleConfig({color = COLOR_ORANGE})) {}
}

// An example function to begin the "root" of your layout tree
CreateLayout :: proc() -> clay.ClayArray(clay.RenderCommand) {
    clay.BeginLayout(windowWidth, windowHeight)

    // An example of laying out a UI with a fixed width sidebar and flexible width main content
    // NOTE: To create a scope for child components, the Odin api uses `if` with components that have children
    if clay.Rectangle(
        clay.ID("OuterContainer"),
        clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingGrow({})}, padding = {16, 16}, childGap = 16}),
        clay.RectangleConfig({color = {250, 250, 255, 255}}),
    ) {
        if clay.Rectangle(
            clay.ID("SideBar"),
            clay.Layout({layoutDirection = .TOP_TO_BOTTOM, sizing = {width = clay.SizingFixed(300), height = clay.SizingGrow({})}, padding = {16, 16}, childGap = 16}),
            clay.RectangleConfig({color = COLOR_LIGHT}),
        ) {
            if clay.Rectangle(
                clay.ID("ProfilePictureOuter"),
                clay.Layout({sizing = {width = clay.SizingGrow({})}, padding = {16, 16}, childGap = 16, childAlignment = {y = .CENTER}}),
                clay.RectangleConfig({color = COLOR_RED}),
            ) {
                if clay.Image(
                    clay.ID("ProfilePicture"),
                    clay.Layout({sizing = {width = clay.SizingFixed(60), height = clay.SizingFixed(60)}}),
                    clay.ImageConfig({imageData = &profilePicture, sourceDimensions = {height = 60, width = 60}}),
                ) {}
                clay.Text(clay.ID("ProfileTitle"), "Clay - UI Library", clay.TextConfig({fontSize = 24, textColor = {255, 255, 255, 255}}))
            }

            // Standard Odin code like loops etc work inside components
            for i in 0..<10 {
                SidebarItemComponent(i)
            }
        }

        if clay.Rectangle(
            clay.ID("MainContent"),
            clay.Layout({sizing = {width = clay.SizingGrow({}), height = clay.SizingGrow({})}}),
            clay.RectangleConfig({color = COLOR_LIGHT}),
        ) {}
    }
    // ...
}
```

6. Call [clay.EndLayout(screenWidth, screenHeight)](https://github.com/nicbarker/clay/blob/main/README.md#clay_endlayout) and process the resulting [clay.RenderCommandArray](https://github.com/nicbarker/clay/blob/main/README.md#clay_rendercommandarray) in your choice of renderer.

```Odin
renderCommands: clay.ClayArray(clay.RenderCommand) = clay.EndLayout(windowWidth, windowHeight)

for i: u32 = 0; i < renderCommands.length; i += 1 {
    renderCommand := clay.RenderCommandArray_Get(&renderCommands, cast(i32)i)

    switch renderCommand.commandType {
    case .Rectangle:
        DrawRectangle(renderCommand.boundingBox, renderCommand.config.rectangleElementConfig.color)
    // ... Implement handling of other command types
    }
}
```

Please see the [full C documentation for clay](https://github.com/nicbarker/clay/blob/main/README.md) for API details. All public C functions and Macros have Odin binding equivalents, generally of the form `CLAY_RECTANGLE` (C) -> `clay.Rectangle` (Odin)
