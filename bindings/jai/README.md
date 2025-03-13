### Jai Language Bindings

This directory contains bindings for the [Jai](https://jai.community/t/overview-of-jai/128) programming language, as well as an example implementation of the Clay demo from the video in Jai.

If you haven't taken a look at the [full documentation for clay](https://github.com/nicbarker/clay/blob/main/README.md), it's recommended that you take a look there first to familiarise yourself with the general concepts. This README is abbreviated and applies to using clay in Jai specifically.

The **most notable difference** between the C API and the Jai bindings is the use of for statements to create the scope for declaring child elements. This is done using some [for_expansion](https://jai.community/t/loops/147) magic.
When using the equivalent of the [Element Macros](https://github.com/nicbarker/clay/blob/main/README.md#element-macros):

```C
// C form of element macros
// Parent element with 8px of padding
CLAY(CLAY_LAYOUT({ .padding = 8 })) {
    // Child element 1
    CLAY_TEXT(CLAY_STRING("Hello World"), CLAY_TEXT_CONFIG({ .fontSize = 16 }));
    // Child element 2 with red background
    CLAY(CLAY_RECTANGLE({ .color = COLOR_RED })) {
        // etc
    }
}
```

```Jai
// Jai form of element macros
// Parent element with 8px of padding
for Clay.Element(Clay.Layout(.{padding = 8})) {
	// Child element 1
	Clay.Text("Hello World", Clay.TextConfig(.{fontSize = 16}));
	// Child element 2 with red background
	for Clay.Element(Clay.Rectangle(.{color = COLOR_RED})) {
		// etc
	}
}
```

> [!WARNING]  
> For now, the Jai and Odin bindings are missing the OnHover() and Hovered() functions.
> You can you PointerOver instead, an example of that is in `examples/introducing_clay_video_demo`.

### Quick Start

1. Download the clay-jai directory and copy it into your modules folder.

```Jai
Clay :: #import "clay-jai";
```

1. Ask clay for how much static memory it needs using [Clay.MinMemorySize()](https://github.com/nicbarker/clay/blob/main/README.md#clay_minmemorysize), create an Arena for it to use with [Clay.CreateArenaWithCapacityAndMemory()](https://github.com/nicbarker/clay/blob/main/README.md#clay_createarenawithcapacityandmemory), and initialize it with [Clay.Initialize()](https://github.com/nicbarker/clay/blob/main/README.md#clay_initialize).

```Jai
clay_required_memory := Clay.MinMemorySize();
memory := alloc(clay_required_memory);
clay_memory := Clay.CreateArenaWithCapacityAndMemory(clay_required_memory, memory);
Clay.Initialize(
	clay_memory, 
	Clay.Dimensions.{cast(float, GetScreenWidth()), cast(float, GetScreenHeight())},
	.{handle_clay_errors, 0}
);
``` 

3. Provide a `measure_text(text, config)` proc marked with `#c_call` with [Clay.SetMeasureTextFunction(function)](https://github.com/nicbarker/clay/blob/main/README.md#clay_setmeasuretextfunction) so that clay can measure and wrap text.

```Jai
// Example measure text function
measure_text :: (text: *Clay.String, config: *Clay.TextElementConfig) -> Clay.Dimensions #c_call {
}

// Tell clay how to measure text
Clay.SetMeasureTextFunction(measure_text)
``` 

4. **Optional** - Call [Clay.SetPointerPosition(pointerPosition)](https://github.com/nicbarker/clay/blob/main/README.md#clay_setpointerposition) if you want to use mouse interactions.

```Jai
// Update internal pointer position for handling mouseover / click / touch events
Clay.SetPointerPosition(.{ mousePositionX, mousePositionY })
```

5. Call [Clay.BeginLayout(screenWidth, screenHeight)](https://github.com/nicbarker/clay/blob/main/README.md#clay_beginlayout) and declare your layout using the provided macros.

```Jai
// An example function to begin the "root" of your layout tree
CreateLayout :: () -> Clay.RenderCommandArray {
	Clay.BeginLayout(windowWidth, windowHeight);

	for Clay.Element(
		Clay.ID("OuterContainer"),
		Clay.Layout(.{
			sizing = .{Clay.SizingGrow(), Clay.SizingGrow()},
			padding = .{16, 16},
			childGap = 16
		}),
		Clay.Rectangle(.{color = .{250, 250, 255, 255}}),
	) {
		// ...
	}
}
```

1. Call [Clay.EndLayout()](https://github.com/nicbarker/clay/blob/main/README.md#clay_endlayout) and process the resulting [Clay.RenderCommandArray](https://github.com/nicbarker/clay/blob/main/README.md#clay_rendercommandarray) in your choice of renderer.

```Jai
render_commands: Clay.RenderCommandArray = Clay.EndLayout();

for 0..render_commands.length - 1 {
	render_command := Clay.RenderCommandArray_Get(*render_commands, cast(s32) it);

	if #complete render_command.commandType == {
	case .RECTANGLE;
		DrawRectangle(render_command.boundingBox, render_command.config.rectangleElementConfig.color)
	// ... Implement handling of other command types
	}
}
```

Please see the [full C documentation for clay](https://github.com/nicbarker/clay/blob/main/README.md) for API details. All public C functions and Macros have Jai binding equivalents, generally of the form `CLAY_RECTANGLE` (C) -> `Clay.Rectangle` (Jai)
