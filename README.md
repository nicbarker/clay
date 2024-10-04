# Clay
**_Clay_** (short for **C Layout**) is a high performance 2D UI layout library.

### Major Features
- Microsecond layout performance
- Flex-box like layout model for complex, responsive layouts including text wrapping, scrolling containers and aspect ratio scaling
- Single ~2k LOC **clay.h** file with **zero** dependencies (including no standard library)
- Wasm support: compile with clang to a 15kb uncompressed **.wasm** file for use in the browser
- Static arena based memory use with no malloc / free, and low total memory overhead (e.g. ~3.5mb for 8192 layout elements).
- React-like nested declarative syntax
- Renderer agnostic: outputs a sorted list of rendering primitives that can be easily composited in any 3D engine, and even compiled to HTML (examples provided)

Take a look at the [clay website](https://nicbarker.com/clay) for an example of clay compiled to wasm and running in the browser, or others in the [examples directory](https://github.com/nicbarker/clay/tree/main/examples).

<img width="1394" alt="A screenshot of a code IDE with lots of visual and textual elements" src="https://github.com/user-attachments/assets/9986149a-ee0f-449a-a83e-64a392267e3d">

_An example GUI application built with clay_

## Quick Start

1. Download or clone clay.h and include it after defining `CLAY_IMPLEMENTATION` in one file.

```C
// Must be defined in one file, _before_ #include "clay.h"
#define CLAY_IMPLEMENTATION
#include "clay.h"
```

2. Ask clay for how much static memory it needs using [Clay_MinMemorySize()](#clay_minmemorysize), create an Arena for it to use with [Clay_CreateArenaWithCapacityAndMemory(size, void *memory)](#clay_createarenawithcapacityandmemory), and initialize it with [Clay_Initialize(arena, dimensions)](#clay_initialize).

```C
// Note: malloc is only used here as an example, any allocator that provides
// a pointer to addressable memory of at least totalMemorySize will work
uint64_t totalMemorySize = Clay_MinMemorySize();
Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
Clay_Initialize(arena, (Clay_Dimensions) { screenWidth, screenHeight });
``` 

3. Provide a `MeasureText(text, config)` function pointer with [Clay_SetMeasureTextFunction(function)](#clay_setmeasuretextfunction) so that clay can measure and wrap text.

```C
// Example measure text function
static inline Clay_Dimensions MeasureText(Clay_String *text, Clay_TextElementConfig *config) {
    // Clay_TextElementConfig contains members such as fontId, fontSize, letterSpacing etc
    // Note: Clay_String->chars is not guaranteed to be null terminated
}

// Tell clay how to measure text
Clay_SetMeasureTextFunction(MeasureText);
```

4. **Optional** - Call [Clay_SetLayoutDimensions(dimensions)](#clay_setlayoutdimensions) if the window size of your application has changed.

```C
// Update internal layout dimensions
Clay_SetLayoutDimensions((Clay_Dimensions) { screenWidth, screenHeight }, isMouseDown);
```

5. **Optional** - Call [Clay_SetPointerState(pointerPosition, isPointerDown)](#clay_setpointerstate) if you want to use mouse interactions.

```C
// Update internal pointer position for handling mouseover / click / touch events
Clay_SetPointerState((Clay_Vector2) { mousePositionX, mousePositionY }, isMouseDown);
```

6. **Optional** - Call [Clay_UpdateScrollContainers(enableDragScrolling, scrollDelta, deltaTime)](#clay_updatescrollcontainers) if you want to use clay's built in scrolling containers.

```C
// Update internal pointer position for handling mouseover / click / touch events
Clay_UpdateScrollContainers(true, (Clay_Vector2) { mouseWheelX, mouseWheelY }, deltaTime);
```

7. Call [Clay_BeginLayout()](#clay_beginlayout) and declare your layout using the provided macros.

```C
const Clay_Color COLOR_LIGHT = (Clay_Color) {224, 215, 210, 255};
const Clay_Color COLOR_RED = (Clay_Color) {168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};

// Layout config is just a struct that can be declared statically, or inline
Clay_LayoutConfig sidebarItemLayout = (Clay_LayoutConfig) {
    .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(50) },
};

// Re-useable components are just normal functions
void SidebarItemComponent(int index) {
    CLAY_RECTANGLE(CLAY_IDI("SidebarBlob", index), sidebarItemLayout, CLAY_RECTANGLE_CONFIG(.color = COLOR_ORANGE)) {}
}

// An example function to begin the "root" of your layout tree
Clay_RenderCommandArray CreateLayout() {
    Clay_BeginLayout();

    // An example of laying out a UI with a fixed width sidebar and flexible width main content
    CLAY_RECTANGLE(CLAY_ID("OuterContainer"), CLAY_LAYOUT(.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, .padding = {16, 16}, .childGap = 16), CLAY_RECTANGLE_CONFIG(.color = {250,250,255,255})) {
        CLAY_RECTANGLE(CLAY_ID("SideBar"), CLAY_LAYOUT(.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW() }, .padding = {16, 16}, .childGap = 16), CLAY_RECTANGLE_CONFIG(.color = COLOR_LIGHT)) {
            CLAY_RECTANGLE(CLAY_ID("ProfilePictureOuter"), CLAY_LAYOUT(.sizing = { .width = CLAY_SIZING_GROW() }, .padding = {16, 16}, .childGap = 16, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER }), CLAY_RECTANGLE_CONFIG(.color = COLOR_RED)) {
                CLAY_IMAGE(CLAY_ID("ProfilePicture"), CLAY_LAYOUT( .sizing = { .width = CLAY_SIZING_FIXED(60), .height = CLAY_SIZING_FIXED(60) }), CLAY_IMAGE_CONFIG(.imageData = &profilePicture, .height = 60, .width = 60)) {}
                CLAY_TEXT(CLAY_ID("ProfileTitle"), CLAY_STRING("Clay - UI Library"), CLAY_TEXT_CONFIG(.fontSize = 24, .textColor = {255, 255, 255, 255}));
            }

            // Standard C code like loops etc work inside components
            for (int i = 0; i < 5; i++) {
                SidebarItemComponent(i);
            }
        }

        CLAY_RECTANGLE(CLAY_ID("MainContent"), CLAY_LAYOUT(.sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW() }), CLAY_RECTANGLE_CONFIG(.color = COLOR_LIGHT)) {}
    }
    // ...
});
```

8. Call [Clay_EndLayout()](#clay_endlayout) and process the resulting [Clay_RenderCommandArray](#clay_rendercommandarray) in your choice of renderer.

```C
Clay_RenderCommandArray renderCommands = Clay_EndLayout(windowWidth, windowHeight);

for (int i = 0; i < renderCommands.length; i++) {
    Clay_RenderCommand *renderCommand = &renderCommands.internalArray[i];
    
    switch (renderCommand->commandType) {
        case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
            DrawRectangle(
                renderCommand->boundingBox,
                renderCommand->config.rectangleElementConfig->color);
        }
        // ... Implement handling of other command types
    }
}
```

The above example, rendered correctly will look something like the following:

![Clay Example](https://github.com/user-attachments/assets/1928c6d4-ada9-4a4c-a3d1-44fe9b23b3bd)

In summary, the general order of steps is:

1. [Clay_SetLayoutDimensions(dimensions)](#clay_setdimensions)	
2. [Clay_SetPointerState(pointerPosition, isPointerDown)](#clay_setpointerstate)
3. [Clay_UpdateScrollContainers(enableDragScrolling, scrollDelta, deltaTime)](#clay_updatescrollcontainers)
4. [Clay_BeginLayout()](#clay_beginlayout)
5. Declare your layout with the provided [Element Macros](#element-macros)
6. [Clay_EndLayout()](#clay_endlayout)
7. Render the results using the outputted [Clay_RenderCommandArray](#clay_rendercommandarray)

For help starting out or to discuss clay, considering joining [the discord server.](https://discord.gg/b4FTWkxdvT)

## High Level Documentation

### Building UI Hierarchies
Clay UI hierarchies are built using C macros that allow _nested_ declarations, similar to other declarative UI systems like HTML.

This means that child elements are declared _inside_ their parent elements. Clay element macros work by opening a block: `{}` after calling the element macro (exactly like you would with an `if` statement or `for` loop), and declaring child components inside the braces.
```C
// Parent element
CLAY_CONTAINER(id, layout) {
    // Child element 1
    CLAY_TEXT(id, text, config);
    // Child element 2
    CLAY_RECTANGLE(id, layout, config) {
        // etc
    }
}
```

However, unlike HTML and other declarative DSLs, these macros are just C. As a result, you can use arbitrary C code such as loops, functions and conditions inside your layout declaration code:
```C
// Re-usable "components" are just functions that declare more UI
void ButtonComponent(Clay_String buttonText) {
    CLAY_RECTANGLE(id, layout, config) {
        CLAY_TEXT(id, buttonText, config);
    }
}

// Parent element
CLAY_CONTAINER(id, layout, {
    // Render a bunch of text elements
    for (int i = 0; i < textArray.length; i++) {
        CLAY_TEXT(id, textArray.elements[i], config);
    }
    // Only render this element if we're on a mobile screen
    if (isMobileScreen) {
        CLAY_CONTAINER(id, layout) {
            // etc
        }
    }
    // Re-usable components
    ButtonComponent(CLAY_STRING("Click me!"));
    ButtonComponent(CLAY_STRING("No, click me!"));
});
```

### Configuring Layout and Styling UI Elements
Many of the element macros in Clay take a `Clay_LayoutConfig` as the second argument. Clay provides a convenience macro, [CLAY_LAYOUT()](#clay_layout) for easy construction of element styles.
```C
CLAY_CONTAINER(id, CLAY_LAYOUT(.padding = {.x = 8, .y = 8}, .backgroundColor = {120, 120, 120, 255})) {
    // ...
}
```
This macro isn't magic - all it's doing is wrapping the standard designated initializer syntax and adding the result to an internal array. e.g. `(Clay_LayoutConfig) { .padding = { .x = 8, .y = 8 } ...`.

See the [Clay_LayoutConfig](#clay_layout) API for the full list of options.

A `Clay_LayoutConfig` struct can be defined in file scope or elsewhere, as long as the lifetime ends after `EndLayout` is called.
```C
// Define a style in the global / file scope
Clay_LayoutConfig reusableStyle = (Clay_LayoutConfig) {.backgroundColor = {120, 120, 120, 255}};

CLAY_CONTAINER(id, &reusableStyle) {
    // ...
}
```

Some of the other element macros, such as [CLAY_TEXT()](#clay_text) and [CLAY_RECTANGLE()](#clay_rectangle) take an element-specific config object as their 3rd argument. These config objects also have convenience macros for constructing them, generally of the form [CLAY_TEXT_CONFIG()](#clay_text_config) or [CLAY_RECTANGLE_CONFIG()](#clay_rectangle_config):

```C
CLAY_TEXT(id, CLAY_STRING("button text"), CLAY_TEXT_CONFIG(.fontSize = 24, .fontId = FONT_ID_LATO, .textColor = {255, 255, 255, 255}));
```

See the [Full API](#api) for details on the specific config macros. 

### Element IDs

All element macros take a `Clay_ElementId` ID as their first argument. Clay provides the [CLAY_ID()](#clay_id) macro to generate these IDs as string hashes:
```C
// Will always produce the same ID from the same input string
CLAY_CONTAINER(CLAY_ID("OuterContainer"), style) {}
```  

To avoid having to construct dynamic strings at runtime to differentiate ids, clay provides the [CLAY_IDI(string, index)](#clay_idi) macro to generate different ids from a single input string. Think of IDI as "**ID** + **I**ndex"
```C
// This is the equivalent of calling CLAY_ID("Item0"), CLAY_ID("Item1") etc
for (int index = 0; index < items.length; index++) {
    CLAY_CONTAINER(CLAY_IDI("Item", index), style) {}
}
```

**_Generally, you should try to keep IDs unique if possible._**

This ID is used for querying mouse / pointer events, and will be forwarded to the final `Clay_RenderCommandArray` for use in retained mode UIs. Using duplicate IDs may cause some functionality to misbehave (i.e. if you're trying to attach a floating container to a specific element with ID that is duplicated, it may not attach to the one you expect)

### Mouse, Touch and Pointer Interactions

Clay provides a very simple unified API for handling mouse and pointer interactions, with specific handling left to user code.

All pointer interactions depend on the function `void Clay_SetPointerState(Clay_Vector2 position)` being called after each mouse position update and before any other clay functions.

The function `bool Clay_PointerOver(Clay_ElementId id)` takes an element id that was used during layout creation and returns a bool representing whether the current pointer position is within its bounding box. 
```C
// Reminder: Clay_SetPointerState must be called before functions that rely on pointer position otherwise it will have no effect
Clay_Vector2 mousePosition = { x, y };
Clay_SetPointerState(mousePosition);
// ...
// If profile picture was clicked
if (mouseButtonDown(0) && Clay_PointerOver(CLAY_ID("ProfilePicture"))) {
    // Handle profile picture clicked
}
```

Querying `Clay_PointerOver` also works _during_ layout construction, and can be used as a convenient way for applying "hover" states, for example:

```C
// Reminder: Clay_SetPointerState must be called before functions that rely on pointer position otherwise it will have no effect
Clay_Vector2 mousePosition = { x, y };
Clay_SetPointerState(mousePosition);
// ...
Clay_ElementId buttonId = CLAY_ID("HeaderButton");
// An orange button that turns blue when hovered
CLAY_CONTAINER(buttonId, CLAY_LAYOUT(.backgroundColor = Clay_PointerOver(buttonId) ? COLOR_BLUE : COLOR_ORANGE)) {
    CLAY_TEXT(CLAY_IDI("Button", index), text, &headerTextConfig);
}
```
Note that the bounding box queried by `Clay_PointerOver` is from the last frame. This shouldn't make a difference except in the case of animations that move at high speed.
If this is an issue for you, performing layout twice per frame with the same data will give you the correct interaction the second time.

### Scrolling Containers

Scrolling containers are defined with the `CLAY_SCROLL_CONTAINER` element macro and function just like normal containers, however to make scroll containers respond to mouse wheel and scroll events, two functions need to be called:
```C
// Reminder: Clay_SetPointerState must be called before Clay_UpdateScrollContainers otherwise it will have no effect
Clay_Vector2 mousePosition = { x, y };
Clay_SetPointerState(mousePosition);
// Clay_UpdateScrollContainers needs to be called before Clay_BeginLayout for the position to avoid a 1 frame delay
Clay_UpdateScrollContainers(
    true, // Enable drag scrolling
    scrollDelta, // Clay_Vector2 scrollwheel / trackpad scroll x and y delta this frame
    float deltaTime, // Time since last frame in seconds as a float e.g. 8ms is 0.008f
);
// ...
```

More specific details can be found in the full [Scroll Container API](#clay_scroll_container).

### Floating Containers ("Absolute" Positioning)

All standard elements in clay are laid out on top of, and _within_ their parent, positioned according to their parent's layout rules, and affect the positioning and sizing of siblings.

**"Floating"** elements are defined with the `CLAY_FLOATING_CONTAINER` element macro and don't affect the parent they are defined in, or the position of their siblings.
They also have a **z-index**, and as a result can intersect and render over the top of other elements.
Aside from positioning, `CLAY_FLOATING_CONTAINER` elements function like standard `CLAY_CONTAINER` elements.

A classic example use case for floating elements is tooltips and modals.

```C
// The two text elements will be laid out top to bottom, and the floating container
// will be attached to "Outer"
CLAY_CONTAINER(CLAY_ID("Outer"), CLAY_LAYOUT(.layoutDirection = TOP_TO_BOTTOM)) {
    CLAY_TEXT(CLAY_ID("Button"), text, &headerTextConfig);
    CLAY_FLOATING(CLAY_ID("Tooltip"), &CLAY_LAYOUT_DEFAULT, CLAY_FLOATING_CONFIG) {}
    CLAY_TEXT(CLAY_ID("Button"), text, &headerTextConfig);
}
```

More specific details can be found in the full [Floating Container API](#clay_floating_container).

### Laying Out Your Own Custom Elements

Clay only supports a simple set of UI element primitives, such as rectangles, text and images. Clay provides a simple, singular API for layout out custom elements:
```C
// Extend CLAY_CUSTOM_ELEMENT_CONFIG with your custom data
#define CLAY_EXTEND_CONFIG_CUSTOM struct t_CustomElementData customData;
// Extensions need to happen _before_ the clay include
#include "clay.h"

// A rough example of how you could handle laying out 3d models in your UI
typedef struct t_CustomElementData {
    CustomElementType type;
    union {
        Model model;
        Video video;
        // ...
    };
} CustomElementData;

Model myModel = Load3DModel(filePath);
CustomElement modelElement = (CustomElement) { .type = CUSTOM_ELEMENT_TYPE_MODEL, .model = myModel }
// ...
CLAY_CONTAINER(id, style) {
    // This config is type safe and contains the CustomElementData struct
    CLAY_CUSTOM_ELEMENT(id, layout, CLAY_CUSTOM_ELEMENT_CONFIG(.customData = { .type = CUSTOM_ELEMENT_TYPE_MODEL, .model = myModel })) {}
}

// Later during your rendering
switch (renderCommand->commandType) {
    // ...
    case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
        // Your extended struct is passed through
        CustomElementData *data = renderCommand->elementConfig.customElementConfig->customData;
        if (!customElement) continue;
        switch (customElement->type) {
            case CUSTOM_ELEMENT_TYPE_MODEL: {
                // Render your 3d model here
                break;
            }
            case CUSTOM_ELEMENT_TYPE_VIDEO: {
                // Render your video here
                break;
            }
            // ...
        }
        break;
    }
}
```

More specific details can be found in the full [Custom Container API](#clay_custom_element).

### Retained Mode Rendering
Clay was originally designed for [Immediate Mode](https://www.youtube.com/watch?v=Z1qyvQsjK5Y) rendering - where the entire UI is redrawn every frame. This may not be possible with your platform, renderer design or performance constraints.

There are some general techniques that can be used to integrate clay into a retained mode rendering system:

- `Clay_RenderCommand` includes the `uint32_t id` that was used to declare the element. If unique ids are used, these can be mapped to persistent graphics objects across multiple frames / layouts.
- Render commands are culled automatically to only currently visible elements, and `Clay_RenderCommand` is a small enough struct that you can simply compare the memory of two render commands with matching IDs to determine if the element is "dirty" and needs to be re-rendered or updated.

For a worked example, see the provided [HTML renderer](https://github.com/nicbarker/clay/blob/main/renderers/web/html/clay-html-renderer.html). This renderer converts clay layouts into persistent HTML documents with minimal changes per frame.  

### Visibility Culling
Clay provides a built-in visibility-culling mechanism that is **enabled by default**. It will only output render commands for elements that are visible - that is, **at least one pixel of their bounding box is inside the viewport.**

This culling mechanism can be disabled via the use of the `#define CLAY_DISABLE_CULLING` directive. See [Preprocessor Directives](#preprocessor-directives) for more information.

### Preprocessor Directives
Clay supports C preprocessor directives to modulate functionality at compile time. These can be set either in code using `#define CLAY_DISABLE_CULLING` or on the command line when compiling using the appropriate compiler specific arguments, e.g. `clang -DCLAY_DISABLE_CULLING main.c ...`

The supported directives are:

- `CLAY_MAX_ELEMENT_COUNT` - Controls the maximum number of clay elements that memory is pre-allocated for. Defaults to **8192**, which should be more than enough for the majority of use cases. Napkin math is ~450 bytes of memory overhead per element (8192 elements is ~3.5mb of memory) 
- `CLAY_DISABLE_CULLING` - Disables [Visibility Culling](#visibility-culling) of render commands.
- `CLAY_WASM` - Required when targeting Web Assembly.
- `CLAY_OVERFLOW_TRAP` - By default, clay will continue to allow function calls without crashing even when it exhausts all its available pre-allocated memory.  This can produce erroneous layout results that are difficult to interpret. If `CLAY_OVERFLOW_TRAP` is defined, clay will raise a `SIGTRAP` signal that will be caught by your debugger. Relies on `signal.h` being available in your environment.
- `CLAY_DEBUG` - Used for debugging clay's internal implementation. Useful if you want to modify or debug clay, or learn how things work. It enables a number of debug features such as preserving source strings for hash IDs to make debugging easier.
- `CLAY_EXTEND_CONFIG_RECTANGLE` - Provide additional struct members to `CLAY_RECTANGLE_CONFIG` that will be passed through with output render commands.
- `CLAY_EXTEND_CONFIG_TEXT` - Provide additional struct members to `CLAY_TEXT_CONFIG` that will be passed through with output render commands.
- `CLAY_EXTEND_CONFIG_IMAGE` - Provide additional struct members to `CLAY_IMAGE_CONFIG` that will be passed through with output render commands.
- `CLAY_EXTEND_CONFIG_CUSTOM` - Provide additional struct members to `CLAY_IMAGE_CONFIG` that will be passed through with output render commands.

### Bindings for non C

Clay is usable out of the box as a `.h` include in both C99 and C++20 with designated initializer support.
There are also supported bindings for other languages, including:

- [Odin Bindings](https://github.com/nicbarker/clay/tree/main/bindings/odin)

Unfortunately clay does **not** support Microsoft C11 or C17 via MSVC at this time.

### Debug Tools

Clay includes built-in UI debugging tools, similar to the "inspector" in browsers such as Chrome or Firefox. These tools are included in `clay.h`, and work by injecting additional render commands into the output [Clay_RenderCommandArray](#clay_rendercommandarray).

As long as the renderer that you're using works correctly, no additional setup or configuration is required to use the debug tools.

To enable the debug tools, use the function `Clay_SetDebugModeEnabled(bool enabled)`. This boolean is persistent and does not need to be set every frame.

The debug tooling by default will render as a panel to the right side of the screen, compressing your layout by its width. The default width is 400 and is currently configurable via the direct mutation of the internal variable `Clay__debugViewWidth`, however this is an internal API and is potentially subject to change.

<img width="1506" alt="Screenshot 2024-09-12 at 12 54 03 PM" src="https://github.com/user-attachments/assets/2d122658-3305-4e27-88d6-44f08c0cb4e6">

_The official Clay website with debug tooling visible_

# API

### Naming Conventions

- "**CAPITAL_LETTERS()**" are used for macros.
- "**Clay__**" ("Clay" followed by **double** underscore) is used for internal functions that are not intended for use and are subject to change.
- "**Clay_**" ("Clay" followed by **single** underscore) is used for external functions that can be called by the user.

## Public Functions

### Lifecycle for public functions

**At startup / initialization time, run once**
`Clay_MinMemorySize` -> `Clay_CreateArenaWithCapacityAndMemory` -> `Clay_SetMeasureTextFunction` -> `Clay_Initialize`

**Each Frame**
`Clay_SetLayoutDimensions` -> `Clay_SetPointerState` -> `Clay_UpdateScrollContainers` -> `Clay_BeginLayout` -> `CLAY_CONTAINER() etc...` -> `Clay_EndLayout`

### Clay_MinMemorySize

`uint32_t Clay_MinMemorySize()`

Returns the minimum amount of memory **in bytes** that clay needs to accomodate the current [CLAY_MAX_ELEMENT_COUNT](#preprocessor-directives).

### Clay_CreateArenaWithCapacityAndMemory

`Clay_Arena Clay_CreateArenaWithCapacityAndMemory(uint32_t capacity, void *offset)`

Creates a `Clay_Arena` struct with the given capacity and base memory pointer, which can be passed to [Clay_Initialize](#clay_initialize).

### Clay_SetMeasureTextFunction

`void Clay_SetMeasureTextFunction(Clay_Dimensions (*measureTextFunction)(Clay_String *text, Clay_TextElementConfig *config))`

Takes a pointer to a function that can be used to measure the `width, height` dimensions of a string. Used by clay during layout to determine [CLAY_TEXT](#clay_text) element sizing and wrapping.

**Note 1: This string is not guaranteed to be null terminated.** Clay saves significant performance overhead by using slices when wrapping text instead of having to clone new null terminated strings. If your renderer does not support **ptr, length** style strings (e.g. Raylib), you will need to clone this to a new C string before rendering.

**Note 2: It is essential that this function is as fast as possible.** For text heavy use-cases this function is called many times, and despite the fact that clay caches text measurements internally, it can easily become the dominant overall layout cost if the provided function is slow. **This is on the hot path!**

### Clay_Initialize

`void Clay_Initialize(Clay_Arena arena, Clay_Dimensions layoutDimensions)`

Initializes the internal memory mapping, and sets the internal dimensions for layout.

### Clay_SetLayoutDimensions

`void Clay_SetLayoutDimensions(Clay_Dimensions dimensions)`

Sets the internal layout dimensions. Cheap enough to be called every frame with your screen dimensions to automatically respond to window resizing, etc.

### Clay_SetPointerState

`void Clay_SetPointerState(Clay_Vector2 position, bool isPointerDown)`

Sets the internal pointer position and state (i.e. current mouse / touch position) and recalculates overlap info, which is used for mouseover / click calculation (via [Clay_PointerOver](#clay_pointerover) and updating scroll containers with [Clay_UpdateScrollContainers](#clay_updatescrollcontainers). **isPointerDown should represent the current state this frame, e.g. it should be `true` for the entire duration the left mouse button is held down.** Clay has internal handling for detecting click / touch start & end.

### Clay_UpdateScrollContainers

`void Clay_UpdateScrollContainers(bool enableDragScrolling, Clay_Vector2 scrollDelta, float deltaTime)`

This function handles scrolling of containers. It responds to both `scrollDelta`, which represents mouse wheel or trackpad scrolling this frame, as well as "touch scrolling" on mobile devices, or "drag scrolling" with a mouse or similar device.

Touch / drag scrolling only occurs if the `enableDragScrolling` parameter is `true`, **and** [Clay_SetPointerState](#clay_setpointerstate) has been called this frame. As a result, you can simply always call it with `false` as the first argument if you want to disable touch scrolling.

`deltaTime` is the time **in seconds** since the last frame (e.g. 0.016 is **16 milliseconds**), and is used to normalize & smooth scrolling across different refresh rates.

### Clay_BeginLayout

`void Clay_BeginLayout()`

Prepares clay to calculate a new layout. Called each frame / layout **before** any of the [Element Macros](#element-macros).

### Clay_EndLayout

`Clay_RenderCommandArray Clay_EndLayout()`

Ends declaration of element macros and calculates the results of the currrent layout. Renders a [Clay_RenderCommandArray](#clay_rendercommandarrray) containing the results of the layout calculation.

### Clay_PointerOver

`bool Clay_PointerOver(Clay_ElementId id)`

Returns `true` if the pointer position previously set with `Clay_SetPointerState` is inside the bounding box of the layout element with the provided `id`. Note: this is based on the element's position from the **last** frame. If frame-accurate pointer overlap detection is required, perhaps in the case of significant change in UI layout between frames, you can simply run your layout code twice that frame. The second call to `Clay_PointerOver` will be frame-accurate.

### Clay_GetScrollContainerData

`Clay_ScrollContainerData Clay_GetScrollContainerData(Clay_ElementId id)`

Returns [Clay_ScrollContainerData](#clay_scrollcontainerdata) for the scroll container matching the provided ID. This function allows imperative manipulation of scroll position, allowing you to build things such as scroll bars, buttons that "jump" to somewhere in a scroll container, etc.

## Element Macros

### CLAY_CONTAINER
**Usage**

`CLAY_CONTAINER(Clay_ElementId id, Clay_LayoutConfig *layoutConfig);`

**Lifecycle**

`Clay_BeginLayout()` -> `CLAY_CONTAINER()` -> `Clay_EndLayout()` 

**Notes**

**CONTAINER** is a generic rectangular container that supports child elements. It uses a [Clay_LayoutConfig](#clay_layout) for styling and layout.

**Examples**
```C
// Define a container with 16px of x and y padding
CLAY_CONTAINER(CLAY_ID("SideBar"), CLAY_LAYOUT(.padding = {16, 16})) {
    // A nested child container
    CLAY_CONTAINER(CLAY_ID("SideBar"), CLAY_LAYOUT(.layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 16)) {
        // Children laid out top to bottom
    }
}
```

**Rendering**

`CLAY_CONTAINER` elements will not generate any render commands.

### CLAY_TEXT
**Usage**

`CLAY_TEXT(Clay_ElementId id, Clay_String textContents, Clay_TextElementConfig *textConfig);`

**Lifecycle**

`Clay_BeginLayout()` -> `CLAY_TEXT()` -> `Clay_EndLayout()` 

**Notes**

**TEXT** is a measured, auto-wrapped text element. It uses [Clay_TextElementConfig](#clay_text_config) to configure text specific options.

Note that `Clay_TextElementConfig` uses `uint32_t fontId`. Font ID to font asset mapping is managed in user code and passed to render commands.

**Examples**

```C
// Define a font somewhere in your code
const uint32_t FONT_ID_LATO = 3;
// ..
CLAY_TEXT(CLAY_ID("Username"), CLAY_STRING("John Smith"), CLAY_TEXT_CONFIG(.fontId = FONT_ID_LATO, .fontSize = 24, .textColor = {255, 0, 0, 255}));
// Rendering example
Font fontToUse = LoadedFonts[renderCommand->elementConfig.textElementConfig->fontId];
```

**Rendering**

Element is subject to [culling](#visibility-culling). Otherwise, multiple `Clay_RenderCommand`s with `commandType = CLAY_RENDER_COMMAND_TYPE_TEXT` may be created, one for each wrapped line of text.

`Clay_RenderCommand.textContent` will be populated with a `Clay_String` _slice_ of the original string passed in (i.e. wrapping doesn't reallocate, it just returns a `Clay_String` pointing to the start of the new line with a `length`)

### CLAY_IMAGE
**Usage**

`CLAY_IMAGE(Clay_ElementId id, Clay_LayoutConfig *layoutConfig, Clay_ImageElementConfig *imageConfig);`

**Lifecycle**

`Clay_BeginLayout()` -> `CLAY_IMAGE()` -> `Clay_EndLayout()` 

**Notes**

**IMAGE_CONTAINER** is a used to layout images, and can optionally have children. It uses [Clay_LayoutConfig](#clay_layout) for styling and layout, and [Clay_ImageElementConfig](#clay_image_config) to configure image specific options.

**Examples**

```C
// Load an image somewhere in your code
Image profilePicture = LoadImage("profilePicture.png");
// ..
CLAY_IMAGE(CLAY_ID("ProfilePicture"), &CLAY_LAYOUT_DEFAULT, CLAY_IMAGE_CONFIG(.imageData = &profilePicture, .height = 60, .width = 60)) {}
// Rendering example
Image *imageToRender = renderCommand->elementConfig.imageElementConfig->imageData;
```

**Rendering**

Element is subject to [culling](#visibility-culling). Otherwise, a single `Clay_RenderCommand`s with `commandType = CLAY_RENDER_COMMAND_TYPE_IMAGE` will be created. The user will need to access `renderCommand->elementConfig.imageElementConfig->imageData` to retrieve image data referenced during layout creation. It's also up to the user to decide how / if they wish to blend `renderCommand->color` with the image.

### CLAY_SCROLL_CONTAINER
**Usage**

`CLAY_IMAGE(Clay_ElementId id, Clay_LayoutConfig *layoutConfig, Clay_ScrollElementConfig *scrollConfig);`

**Lifecycle**

`Clay_SetPointerState()` -> `Clay_UpdateScrollContainers()` -> `Clay_BeginLayout()` -> `CLAY_SCROLL_CONTAINER()` -> `Clay_EndLayout()` 

**Notes**

**SCROLL_CONTAINER** creates a masked container that allows layout of children to extend beyond its boundaries. It uses [Clay_LayoutConfig](#clay_layout) for styling and layout, and [Clay_ScrollElementConfig](#clay_scroll_config) to configure scroll specific options.

Note: In order to process scrolling based on pointer position and mouse wheel or touch interactions, you must call `Clay_SetPointerState()` and `Clay_UpdateScrollContainers()` _before_ calling `BeginLayout`.

**Examples**

```C
CLAY_SCROLL_CONTAINER(CLAY_ID("MainContent"), &CLAY_LAYOUT_DEFAULT, CLAY_SCROLL_CONFIG(.vertical = true)) {
    // Create child content with a fixed height of 5000
    CLAY_CONTAINER(CLAY_ID("ScrollInner"), CLAY_LAYOUT(.sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(5000) })) {}
}
```

**Rendering**

Scroll containers will result in two render commands: 
- `commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_START`, which should create a rectangle mask with its `boundingBox` and is **not** subject to [culling](#visibility-culling)
- `commandType = CLAY_RENDER_COMMAND_TYPE_SCISSOR_END`, which disables the previous rectangle mask and is **not** subject to [culling](#visibility-culling)

### CLAY_BORDER_CONTAINER
**Usage**

`CLAY_BORDER_CONTAINER(Clay_ElementId id, Clay_LayoutConfig *layoutConfig, Clay_BorderElementConfig *borderConfig);`

**Lifecycle**

`Clay_BeginLayout()` -> `CLAY_BORDER_CONTAINER()` -> `Clay_EndLayout()` 

**Notes**

**BORDER_CONTAINER** is functionally identical to **CONTAINER** but also allows configuration of a border around the element. It uses [Clay_LayoutConfig](#clay_layout) for styling and layout, and [Clay_BorderElementConfig](#clay_border_config) to configure border specific options.

**Examples**

```C
// 300x300 container with a 1px red border around all the edges
CLAY_BORDER_CONTAINER(CLAY_ID("OuterBorder"), CLAY_LAYOUT(.sizing = { .width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_FIXED(300)}), CLAY_BORDER_CONFIG_OUTSIDE(.color = COLOR_RED, .width = 1)) {
    // ...
}

// Container with a 3px yellow bottom border
CLAY_BORDER_CONTAINER(CLAY_ID("OuterBorder"), &CLAY_LAYOUT_DEFAULT, CLAY_BORDER_CONFIG(.bottom = { .color = COLOR_YELLOW, .width = 3 })) {
    // ...
}

// Container with a 5px curved border around the edges, and a 5px blue border between all children laid out top to bottom
CLAY_BORDER_CONTAINER(CLAY_ID("OuterBorder"), CLAY_LAYOUT(.layoutDirection = CLAY_TOP_TO_BOTTOM), CLAY_BORDER_CONFIG_ALL_RADIUS({ .color = COLOR_BLUE, .width = 5 })) {
    // Child
    // -- border will be here --
    // Child
}
```

**Rendering**

Element is subject to [culling](#visibility-culling). Otherwise, a single `Clay_RenderCommand` with `commandType = CLAY_RENDER_COMMAND_TYPE_BORDER` representing the container will be created.
Rendering of borders and rounded corners is left up to the user. See the provided [Raylib Renderer](https://github.com/nicbarker/clay/tree/main/renderers/raylib) for examples of how to draw borders using line and curve primitives.

### CLAY_FLOATING_CONTAINER
**Usage**

`CLAY_FLOATING_CONTAINER(Clay_ElementId id, Clay_LayoutConfig *layoutConfig, Clay_FloatingElementConfig *floatingConfig);`

**Lifecycle**

`Clay_BeginLayout()` -> `CLAY_FLOATING_CONTAINER()` -> `Clay_EndLayout()` 

**Notes**

**FLOATING_CONTAINER** defines an element that "floats" above other content. Typical use-cases include tooltips and modals.

Floating containers:

- With the [default configuration](#clay_floating_config), attach to the top left corner of their "parent" 
- Don't affect the width and height of their parent
- Don't affect the positioning of sibling elements
- Depending on their z-index can appear above or below other elements, partially or completely occluding them
- Apart from positioning, function just like standard `CLAY_CONTAINER` elements - including expanding to fit their children, etc.

The easiest mental model to use when thinking about floating containers is that they are a completely separate UI hierarchy, attached to a specific x,y point on their "parent".

Floating elements use [Clay_LayoutConfig](#clay_layout) for styling and layout, and [Clay_FloatingElementConfig](#clay_floating_config) to configure specific options.

**Examples**

```C
// Horizontal container with three option buttons
CLAY_CONTAINER(CLAY_ID("OptionsList"), CLAY_LAYOUT(.childGap = 16)) {
    CLAY_RECTANGLE(CLAY_IDI("Option", 1), CLAY_LAYOUT(.padding = {16, 16}), CLAY_RECTANGLE_CONFIG(.color = COLOR_BLUE)) {
        CLAY_TEXT(CLAY_IDI("OptionText", 1), CLAY_STRING("Option 1"), CLAY_TEXT_CONFIG());
    }
    CLAY_RECTANGLE(CLAY_IDI("Option", 2), CLAY_LAYOUT(.padding = {16, 16}), CLAY_RECTANGLE_CONFIG(.color = COLOR_BLUE)) {
        CLAY_TEXT(CLAY_IDI("OptionText", 2), CLAY_STRING("Option 2"), CLAY_TEXT_CONFIG());
        // Floating tooltip will attach above the "Option 2" container and not affect widths or positions of other elements
        CLAY_FLOATING_CONTAINER(CLAY_ID("OptionTooltip"), &CLAY_LAYOUT_DEFAULT, CLAY_FLOATING_CONFIG(.zIndex = 1, .attachment = { .element = CLAY_ATTACH_POINT_CENTER_BOTTOM, .parent = CLAY_ATTACH_POINT_CENTER_TOP })) {
            CLAY_TEXT(CLAY_IDI("OptionTooltipText", 1), CLAY_STRING("Most popular!"), CLAY_TEXT_CONFIG());
        }
    }
    CLAY_RECTANGLE(CLAY_IDI("Option", 3), CLAY_LAYOUT(.padding = {16, 16}), CLAY_RECTANGLE_CONFIG(.color = COLOR_BLUE)) {
        CLAY_TEXT(CLAY_IDI("OptionText", 3), CLAY_STRING("Option 3"), CLAY_TEXT_CONFIG());
    }
}

// Floating containers can also be declared elsewhere in a layout, to avoid branching or polluting other UI
for (int i = 0; i < 1000; i++) {
    CLAY_CONTAINER(CLAY_IDI("Option", i + 1), &CLAY_LAYOUT_DEFAULT) {
        // ...
    }
}
// Note the use of "parentId".
// Floating tooltip will attach above the "Option 2" container and not affect widths or positions of other elements
CLAY_FLOATING_CONTAINER(CLAY_ID("OptionTooltip"), &CLAY_LAYOUT_DEFAULT, CLAY_FLOATING_CONFIG(.parentId = CLAY_IDI("Option", 2).id, .zIndex = 1, .attachment = { .element = CLAY_ATTACH_POINT_CENTER_BOTTOM, .parent = CLAY_ATTACH_POINT_TOP_CENTER })) {
    CLAY_TEXT(CLAY_IDI("OptionTooltipText", 1), CLAY_STRING("Most popular!"), CLAY_TEXT_CONFIG());
}
```

When using `.parentId`, the floating container can be declared anywhere after `BeginLayout` and before `EndLayout`. The target element matching the `.parentId` doesn't need to exist when `CLAY_FLOATING_CONTAINER` is called.

**Rendering**

`CLAY_FLOATING_CONTAINER` elements will not generate any render commands.

### CLAY_CUSTOM_ELEMENT
**Usage**

`CLAY_CUSTOM_ELEMENT(Clay_ElementId id, Clay_LayoutConfig *layoutConfig, Clay_CustomElementConfig *customConfig);`

**Lifecycle**

`Clay_BeginLayout()` -> `CLAY_CUSTOM_ELEMENT()` -> `Clay_EndLayout()` 

**Notes**

**CUSTOM_ELEMENT** uses [Clay_LayoutConfig](#clay_layout) for styling and layout, and allows the user to pass custom data to the renderer. 

**Examples**
```C
// Extend CLAY_CUSTOM_ELEMENT_CONFIG with your custom data
#define CLAY_EXTEND_CONFIG_CUSTOM struct t_CustomElementData customData;
// Extensions need to happen _before_ the clay include
#include "clay.h"

// A rough example of how you could handle laying out 3d models in your UI
typedef struct t_CustomElementData {
    CustomElementType type;
    union {
        Model model;
        Video video;
        // ...
    };
} CustomElementData;

Model myModel = Load3DModel(filePath);
CustomElement modelElement = (CustomElement) { .type = CUSTOM_ELEMENT_TYPE_MODEL, .model = myModel }
// ...
CLAY_CONTAINER(id, style) {
    // This config is type safe and contains the CustomElementData struct
    CLAY_CUSTOM_ELEMENT(id, style, CLAY_CUSTOM_ELEMENT_CONFIG(.customData = { .type = CUSTOM_ELEMENT_TYPE_MODEL, .model = myModel })) {}
}

// Later during your rendering
switch (renderCommand->commandType) {
    // ...
    case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
        // Your extended struct is passed through
        CustomElementData *data = renderCommand->elementConfig.customElementConfig->customData;
        if (!customElement) continue;
        switch (customElement->type) {
            case CUSTOM_ELEMENT_TYPE_MODEL: {
                // Render your 3d model here
                break;
            }
            case CUSTOM_ELEMENT_TYPE_VIDEO: {
                // Render your video here
                break;
            }
            // ...
        }
        break;
    }
}
```

**Rendering**

Element is subject to [culling](#visibility-culling). Otherwise, a single `Clay_RenderCommand` with `commandType = CLAY_RENDER_COMMAND_TYPE_CUSTOM` will be created.
The user will need to access [Clay_CustomElementConfig](#clay_custom_element_config) to retrieve custom data referenced during layout creation.

## Config Macros

A number of clay element macros take element-specific **config** structs.

### CLAY_LAYOUT

**CLAY_LAYOUT()** is used for configuring layout for most clay elements.

**Struct API (Pseudocode)**

```C
// CLAY_LAYOUT(.member = value) supports these options
Clay_LayoutConfig {
    Clay_LayoutDirection layoutDirection = CLAY_LEFT_TO_RIGHT (default) | CLAY_TOP_TO_BOTTOM;
    Clay_Padding padding {
        float x; float y; 
    };
    uint16_t childGap;
    Clay_ChildAlignment childAlignment {
        .x = CLAY_ALIGN_X_LEFT (default) | CLAY_ALIGN_X_CENTER | CLAY_ALIGN_X_RIGHT;
        .y = CLAY_ALIGN_Y_TOP (default) | CLAY_ALIGN_Y_CENTER | CLAY_ALIGN_Y_BOTTOM;
    };
    Clay_Sizing sizing { // Recommended to use the provided macros here - see #sizing for more in depth explanation
        .width = CLAY_SIZING_FIT(float min, float max) (default) | CLAY_SIZING_GROW(float min, float max) | CLAY_SIZING_FIXED(width) | CLAY_SIZING_PERCENT(float percent)
        .height = CLAY_SIZING_FIT(float min, float max) (default) | CLAY_SIZING_GROW(float min, float max) | CLAY_SIZING_FIXED(height) | CLAY_SIZING_PERCENT(float percent)
    }; // See CLAY_SIZING_GROW() etc for more details
};
```
As with all config macros, `CLAY_LAYOUT()` accepts designated initializer syntax and provides default values for any unspecified struct members. 

**Fields**

**`.layoutDirection`** - `Clay_LayoutDirection`

`CLAY_LAYOUT(.layoutDirection = CLAY_TOP_TO_BOTTOM)`

Controls the axis / direction in which child elements are laid out. Available options are `CLAY_LEFT_TO_RIGHT` (default) and `CLAY_TOP_TO_BOTTOM`.

_Did you know that "left to right" and "top to bottom" both have 13 letters?_

<img width="580" alt="Screenshot 2024-08-22 at 11 10 27 AM" src="https://github.com/user-attachments/assets/7008aa47-8826-4338-9257-8bc83f7813ce">

---

**`.padding`** - `Clay_Padding`

`CLAY_LAYOUT(.padding = { .x = 16, .y = 16 })`

Controls horizontal and vertical white-space "padding" around the **outside** of child elements.

<img width="486" alt="Screenshot 2024-08-22 at 10 50 49 AM" src="https://github.com/user-attachments/assets/9311cf10-b8aa-40fe-922a-5dee3663f1a0">

---

**`.childGap`** - `uint16_t`

`CLAY_LAYOUT(.childGap = 16)`

Controls the white-space **between** child elements as they are laid out. When `.layoutDirection` is `CLAY_LEFT_TO_RIGHT` (default), this will be horizontal space, whereas for `CLAY_TOP_TO_BOTTOM` it will be vertical space.

<img width="600" alt="Screenshot 2024-08-22 at 11 05 15 AM" src="https://github.com/user-attachments/assets/fa0dae1f-1936-47f6-a299-634bd7d40d58">

---

**`.childAlignment`** - `Clay_ChildAlignment`

`CLAY_LAYOUT(.childAlignment = { .x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_CENTER })`

Controls the alignment of children relative to the height and width of the parent container. Available options are:
```C
.x = CLAY_ALIGN_X_LEFT (default) | CLAY_ALIGN_X_CENTER | CLAY_ALIGN_X_RIGHT;
.y = CLAY_ALIGN_Y_TOP (default) | CLAY_ALIGN_Y_CENTER | CLAY_ALIGN_Y_BOTTOM;
```

<img width="1030" alt="Screenshot 2024-08-22 at 11 25 16 AM" src="https://github.com/user-attachments/assets/be61b4a7-db4f-447c-b6d6-b2d4a91fc664">

---

**`.sizing`** - `Clay_Sizing`

`CLAY_LAYOUT(.sizing = { .width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_PERCENT(0.5) })`

Controls how final width and height of element are calculated. The same configurations are available for both the `.width` and `.height` axis. There are several options:

- `CLAY_SIZING_FIT(float min, float max) (default)` - The element will be sized to fit its children (plus padding and gaps), up to `max`. If `max` is left unspecified, it will default to `FLOAT_MAX`. When elements are compressed to fit into a smaller parent, this element will not shrink below `min`.

- `CLAY_SIZING_GROW(float min, float max)` - The element will grow to fill available space in its parent, up to `max`. If `max` is left unspecified, it will default to `FLOAT_MAX`. When elements are compressed to fit into a smaller parent, this element will not shrink below `min`.

- `CLAY_SIZING_FIXED(float fixed)` - The final size will always be exactly the provided `fixed` value. Shorthand for `CLAY_SIZING_FIT(fixed, fixed)`

- `CLAY_SIZING_PERCENT(float percent)` - Final size will be a percentage of parent size, minus padding and child gaps. `percent` is assumed to be a float between `0` and `1`.

<img width="1056" alt="Screenshot 2024-08-22 at 2 10 33 PM" src="https://github.com/user-attachments/assets/1236efb1-77dc-44cd-a207-7944e0f5e500">

<img width="1141" alt="Screenshot 2024-08-22 at 2 19 04 PM" src="https://github.com/user-attachments/assets/a26074ff-f155-4d35-9ca4-9278a64aac00">


**Example Usage**

```C
CLAY_CONTAINER(CLAY_ID("Button"), CLAY_LAYOUT(.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_GROW() }, .padding = {16, 16}, .childGap = 16)) {
    // Children will be laid out vertically with 16px of padding around and between
}
``` 

### CLAY_RECTANGLE_CONFIG

**CLAY_RECTANGLE_CONFIG()** is used for configuring rendering for [CLAY_RECTANGLE()](#clay_rectangle) elements. The config will be passed through to render commands as `Clay_RenderCommand.config.rectangleElementConfig`

**Struct API (Pseudocode)**

```C
// CLAY_RECTANGLE_CONFIG(.member = value) supports these options
Clay_RectangleConfig {
    Clay_Color color {
        float r; float g; float b; float a;
    };
    float cornerRadius;

    #ifdef CLAY_EXTEND_CONFIG_RECTANGLE
        // Contents of CLAY_EXTEND_CONFIG_RECTANGLE will be pasted here
    #endif
}
```

As with all config macros, `CLAY_RECTANGLE_CONFIG()` accepts designated initializer syntax and provides default values for any unspecified struct members.

**Extension**

The underlying `Clay_RectangleElementConfig` can be extended with new members by using:
```C
#define CLAY_EXTEND_CONFIG_RECTANGLE float newField;
#include "clay.h" // Define your extension before including clay.h
```

**Fields**

**`.color`** - `Clay_Color`

`CLAY_RECTANGLE_CONFIG(.color = {120, 120, 120, 255})`

Conventionally accepts `rgba` float values between 0 and 255, but interpretation is left up to the renderer and does not affect layout.

---

**`.cornerRadius`** - `float`

`CLAY_RECTANGLE_CONFIG(.cornerRadius = 16)`

Defines the radius in pixels for the arc of rectangle corners (`0` is square, `rectangle.width / 2` is circular).

Note that the `CLAY_CORNER_RADIUS(radius)` function-like macro is available to provide short hand for setting all four corner radii to the same value. e.g. `CLAY_BORDER_CONFIG(.cornerRadius = CLAY_CORNER_RADIUS(10))`

### CLAY_TEXT_CONFIG

**CLAY_TEXT_CONFIG()** is a macro used to create and store `Clay_TextElementConfig` structs, which are for configuring [CLAY_TEXT](#clay_text) elements. The config used in declaration will be passed both as an argument to the user-provided `Clay_MeasureText(Clay_String *text, Clay_TextElementConfig *config)` function as well as the in the final output as `Clay_RenderCommand.config.textElementConfig`.

**Struct API (Pseudocode)**

```C
// CLAY_TEXT_CONFIG(.member = value) supports these options
Clay_TextElementConfig {
    Clay_Color textColor {
        float r; float g; float b; float a;
    };
    uint16_t fontId;
    uint16_t fontSize;
    uint16_t letterSpacing;
    uint16_t lineHeight;
    Clay_TextElementConfigWrapMode wrapMode {
    	CLAY_TEXT_WRAP_WORDS (default),
	CLAY_TEXT_WRAP_NEWLINES,
	CLAY_TEXT_WRAP_NONE,
    };

    #ifdef CLAY_EXTEND_CONFIG_TEXT
        // Contents of CLAY_EXTEND_CONFIG_TEXT will be pasted here
    #endif
};
```
As with all config macros, `CLAY_TEXT_CONFIG()` accepts designated initializer syntax and provides default values for any unspecified struct members. 

**Extension**

The underlying `Clay_TextElementConfig` can be extended with new members by using:
```C
#define CLAY_EXTEND_CONFIG_TEXT float newField;
#include "clay.h" // Define your extension before including clay.h
```

**Fields**

**`.textColor`**

`CLAY_TEXT_CONFIG(.textColor = {120, 120, 120, 255})`

Conventionally accepts `rgba` float values between 0 and 255, but interpretation is left up to the renderer and does not affect layout.

---

**`.fontId`**

`CLAY_TEXT_CONFIG(.fontId = FONT_ID_LATO)`
                     
It's up to the user to load fonts and create a mapping from `fontId` to a font that can be measured and rendered.

---

**`.fontSize`**

`CLAY_TEXT_CONFIG(.fontSize = 16)`

Font size is generally thought of as `x pixels tall`, but interpretation is left up to the user & renderer.

---

**`.letterSpacing`**

`CLAY_TEXT_CONFIG(.letterSpacing = 1)`

`.letterSpacing` results in **horizontal** white space between individual rendered characters.

---

**`.lineHeight`**

`CLAY_TEXT_CONFIG(.lineHeight = 20)`

`.lineHeight` - when non zero - forcibly sets the `height` of each wrapped line of text to `.lineheight` pixels tall. Will affect the layout of both parents and siblings. A value of `0` will use the measured height of the font.

---

**`.wrapMode`**

`CLAY_TEXT_CONFIG(.wrapMode = CLAY_TEXT_WRAP_NONE)`

`.wrapMode` specifies under what conditions text should [wrap](https://en.wikipedia.org/wiki/Line_wrap_and_word_wrap).

Available options are:

- `CLAY_TEXT_WRAP_WORDS` (default) - Text will wrap on whitespace characters as container width shrinks, preserving whole words.
- `CLAY_TEXT_WRAP_NEWLINES` -  will only wrap when encountering newline characters.
- `CLAY_TEXT_WRAP_NONE` - Text will never wrap even if its container is compressed beyond the text measured width.

---

**Example Usage**
```C
// A 24px, red text element that says "John Smith"
CLAY_TEXT(CLAY_ID("Username"), CLAY_STRING("John Smith"), CLAY_TEXT_CONFIG(.fontSize = 24, .fontId = 2, .textColor = {255, 0, 0, 255}));
```

### CLAY_IMAGE_CONFIG

**CLAY_IMAGE_CONFIG()** is a macro used to create and store `Clay_ImageElementConfig` structs, which are for configuring [CLAY_IMAGE](#clay_image) elements. The config will be passed through to render commands as `Clay_RenderCommand.config.imageElementConfig`

**Struct API (Pseudocode)**

```C
Clay_ImageElementConfig {
    Clay_Dimensions sourceDimensions {
        float width; float height; 
    };
    // --
    #ifndef CLAY_EXTEND_CONFIG_IMAGE
        void * imageData; // Note: This field will be replaced if #define CLAY_EXTEND_CONFIG_IMAGE is specified
    #else CLAY_EXTEND_CONFIG_IMAGE
        // Contents of CLAY_EXTEND_CONFIG_IMAGE will be pasted here
    #endif
};
```

As with all config macros, `CLAY_IMAGE_CONFIG()` accepts designated initializer syntax and provides default values for any unspecified struct members. 

**Extension**

The underlying `Clay_ImageElementConfig` can be extended with new members by using:
```C
#define CLAY_EXTEND_CONFIG_IMAGE float newField;
#include "clay.h" // Define your extension before including clay.h
```

**Fields**

**`.sourceDimensions`** - `Clay_Dimensions`

`CLAY_IMAGE_CONFIG(.sourceDimensions = { 1024, 768 })`

Used to perform **aspect ratio scaling** on the image element. As of this version of clay, aspect ratio scaling only applies to the `height` of an image (i.e. image height will scale with width growth and limitations, but width will not scale with height growth and limitations)

---

**`.imageData`** - `void *`

`CLAY_IMAGE_CONFIG(.imageData = &myImage)`

`.imageData` is a generic void pointer that can be used to pass through image data to the renderer. **Note:** this field is generally not recommended for usage due to the lack of type safety, see `#define CLAY_EXTEND_CONFIG_IMAGE` in [Preprocessor Directives](#preprocessor-directives) for an alternative.

```C
// Load an image somewhere in your code
Image profilePicture = LoadImage("profilePicture.png");
// Note that when rendering, .imageData will be void* type.
CLAY_IMAGE(CLAY_ID("ProfilePicture"), &CLAY_LAYOUT_DEFAULT, CLAY_IMAGE_CONFIG(.imageData = &profilePicture, .sourceDimensions = { 60, 60 })) {}

// OR ----------------

// Extend CLAY_CUSTOM_IMAGE_CONFIG with your custom image format
#define CLAY_EXTEND_CONFIG_IMAGE struct t_Image image;
// Extensions need to happen _before_ the clay include
#include "clay.h"

typedef struct t_Image {
    ImageFormat format;
    u8int_t *internalData;
    // ... etc
} Image;

// You can now use CLAY_IMAGE_CONFIG with your custom type and still have type safety & code completion
CLAY_IMAGE(CLAY_ID("ProfilePicture"), &CLAY_LAYOUT_DEFAULT, CLAY_IMAGE_CONFIG(.image = { .format = IMAGE_FORMAT_RGBA, .internalData = &imageData }, .sourceDimensions = { 60, 60 })) {}
```

### CLAY_FLOATING_CONFIG

**CLAY_FLOATING_CONFIG()** is a macro used to create and store `Clay_FloatingElementConfig` structs, which are for configuring [CLAY_FLOATING_CONTAINER](#clay_floating_container) elements.

**Struct Definition (Pseudocode)** 

```C
typedef struct
{
    Clay_Vector2 offset {
        float x, float y
    };
    Clay_Dimensions expand {
        float width, float height
    };
    uint16_t zIndex;
    uint32_t parentId;
    Clay_FloatingAttachPoints attachment {
        .element = CLAY_ATTACH_POINT_LEFT_TOP (default) | CLAY_ATTACH_POINT_LEFT_CENTER | CLAY_ATTACH_POINT_LEFT_BOTTOM | CLAY_ATTACH_POINT_CENTER_TOP | CLAY_ATTACH_POINT_CENTER_CENTER | CLAY_ATTACH_POINT_CENTER_BOTTOM | CLAY_ATTACH_POINT_RIGHT_TOP | CLAY_ATTACH_POINT_RIGHT_CENTER | CLAY_ATTACH_POINT_RIGHT_BOTTOM
        .parent = CLAY_ATTACH_POINT_LEFT_TOP (default) | CLAY_ATTACH_POINT_LEFT_CENTER | CLAY_ATTACH_POINT_LEFT_BOTTOM | CLAY_ATTACH_POINT_CENTER_TOP | CLAY_ATTACH_POINT_CENTER_CENTER | CLAY_ATTACH_POINT_CENTER_BOTTOM | CLAY_ATTACH_POINT_RIGHT_TOP | CLAY_ATTACH_POINT_RIGHT_CENTER | CLAY_ATTACH_POINT_RIGHT_BOTTOM
    };
} Clay_FloatingElementConfig;
```

As with all config macros, `CLAY_FLOATING_CONFIG()` accepts designated initializer syntax and provides default values for any unspecified struct members. 

**Fields**

**`.offset`** - `Clay_Vector2`

`CLAY_FLOATING_CONFIG(.offset = { -24, -24 })`

Used to apply a position offset to the floating container _after_ all other layout has been calculated. 

---

**`.expand`** - `Clay_Dimensions`

`CLAY_FLOATING_CONFIG(.expand = { 16, 16 })`

Used to expand the width and height of the floating container _before_ laying out child elements.

---

**`.zIndex`** - `float`

`CLAY_FLOATING_CONFIG(.zIndex = 1)`

All floating elements (as well as their entire child hierarchies) will be sorted by `.zIndex` order before being converted to render commands. If render commands are drawn in order, elements with higher `.zIndex` values will be drawn on top.

---

**`.parentId`** - `uint32_t`

`CLAY_FLOATING_CONFIG(.parentId = CLAY_ID("HeaderButton").id)`

By default, floating containers will "attach" to the parent element that they are declared inside. However, there are cases where this limitation could cause significant performance or ergonomics problems. `.parentId` allows you to specify a `CLAY_ID().id` to attach the floating container to. The parent element with the matching id can be declared anywhere in the hierarchy, it doesn't need to be declared before or after the floating container in particular.  

Consider the following case:
```C
// Load an image somewhere in your code
CLAY_CONTAINER(CLAY_IDI("SidebarButton", 1), &CLAY_LAYOUT_DEFAULT) {
    // .. some button contents
    if (tooltip.attachedButtonIndex == 1) {
        CLAY_FLOATING_CONTAINER(/* floating config... */)
    }
}
CLAY_CONTAINER(CLAY_IDI("SidebarButton", 2), &CLAY_LAYOUT_DEFAULT) {
    // .. some button contents
    if (tooltip.attachedButtonIndex == 2) {
        CLAY_FLOATING_CONTAINER(/* floating config... */)
    }
}
CLAY_CONTAINER(CLAY_IDI("SidebarButton", 3), &CLAY_LAYOUT_DEFAULT) {
    // .. some button contents
    if (tooltip.attachedButtonIndex == 3) {
        CLAY_FLOATING_CONTAINER(/* floating config... */)
    }
}
CLAY_CONTAINER(CLAY_IDI("SidebarButton", 4), &CLAY_LAYOUT_DEFAULT) {
    // .. some button contents
    if (tooltip.attachedButtonIndex == 4) {
        CLAY_FLOATING_CONTAINER(/* floating config... */)
    }
}
CLAY_CONTAINER(CLAY_IDI("SidebarButton", 5), &CLAY_LAYOUT_DEFAULT) {
    // .. some button contents
    if (tooltip.attachedButtonIndex == 5) {
        CLAY_FLOATING_CONTAINER(/* floating config... */)
    }
}
```

The definition of the above UI is significantly polluted by the need to conditionally render floating tooltips as a child of many possible elements. The alternative, using `parentId`, looks like this:

```C
// Load an image somewhere in your code
CLAY_CONTAINER(CLAY_IDI("SidebarButton", 1), &CLAY_LAYOUT_DEFAULT) {
    // .. some button contents
}
CLAY_CONTAINER(CLAY_IDI("SidebarButton", 2), &CLAY_LAYOUT_DEFAULT) {
    // .. some button contents
}
CLAY_CONTAINER(CLAY_IDI("SidebarButton", 3), &CLAY_LAYOUT_DEFAULT) {
    // .. some button contents
}
CLAY_CONTAINER(CLAY_IDI("SidebarButton", 4), &CLAY_LAYOUT_DEFAULT) {
    // .. some button contents
}
CLAY_CONTAINER(CLAY_IDI("SidebarButton", 5), &CLAY_LAYOUT_DEFAULT) {
    // .. some button contents
}

// Any other point in the hierarchy
CLAY_FLOATING_CONTAINER(CLAY_ID("OptionTooltip"), &CLAY_LAYOUT_DEFAULT, CLAY_FLOATING_CONFIG(.parentId = CLAY_IDI("SidebarButton", tooltip.attachedButtonIndex).id)) {
    // Tooltip contents...
}
```

---

**`.attachment`** - `Clay_FloatingAttachPoints`

`CLAY_FLOATING_CONFIG(.attachment = { .element = CLAY_ATTACH_POINT_LEFT_CENTER, .parent = CLAY_ATTACH_POINT_RIGHT_TOP });`

In terms of positioning the floating container, `.attachment` specifies 

- The point on the floating container (`.element`)
- The point on the parent element that it "attaches" to (`.parent`)

![Screenshot 2024-08-23 at 11 47 21 AM](https://github.com/user-attachments/assets/b8c6dfaa-c1b1-41a4-be55-013473e4a6ce)

You can mentally visualise this as finding a point on the floating container, then finding a point on the parent, and lining them up over the top of one another.

For example:

"Attach the LEFT_CENTER of the floating container to the RIGHT_TOP of the parent"

`CLAY_FLOATING_CONFIG(.attachment = { .element = CLAY_ATTACH_POINT_LEFT_CENTER, .parent = CLAY_ATTACH_POINT_RIGHT_TOP });`

![Screenshot 2024-08-23 at 11 53 24 AM](https://github.com/user-attachments/assets/ebe75e0d-1904-46b0-982d-418f929d1516)


### CLAY_SCROLL_CONFIG

**CLAY_SCROLL_CONFIG()** is a macro used to create and store `Clay_ScrollElementConfig` structs, which are for configuring [CLAY_SCROLL_CONTAINER](#clay_scroll_container) elements.

**Struct Definition (Pseudocode)**

```C
typedef struct
{
    bool horizontal;
    bool vertical;
} Clay_ScrollElementConfig;
```

As with all config macros, `CLAY_SCROLL_CONFIG()` accepts designated initializer syntax and provides default values for any unspecified struct members. 

**Fields**

**`.horizontal`** - `bool`

`CLAY_SCROLL_CONFIG(.horizontal = true)`

Enables or disables horizontal scrolling for this container element.

---

**`.vertical`** - `bool`

`CLAY_SCROLL_CONFIG(.vertical = true)`

Enables or disables vertical scrolling for this container element.

**Examples**

```C
CLAY_SCROLL_CONTAINER(CLAY_ID("MainContent"), &CLAY_LAYOUT_DEFAULT, CLAY_SCROLL_CONFIG(.vertical = true)) {
    // Create child content with a fixed height of 5000
    CLAY_CONTAINER(CLAY_ID("ScrollInner"), CLAY_LAYOUT(.sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(5000) })) {}
}
```

### CLAY_BORDER_CONFIG

**CLAY_BORDER_CONFIG()** is a macro used to create and store `Clay_BorderElementConfig` structs, which are for configuring [CLAY_BORDER_CONTAINER](#clay_border_container) elements.

**Struct Definition (Pseudocode)**

```C
typedef struct
{
    Clay_Border left {
        float width;
        Clay_Color color {
            float r; float g; float b; float a;
        };
    };
    Clay_Border right {
        float width;
        Clay_Color color {
            float r; float g; float b; float a;
        };
    }
    Clay_Border top {
        float width;
        Clay_Color color {
            float r; float g; float b; float a;
        };
    };
    Clay_Border bottom {
        float width;
        Clay_Color color {
            float r; float g; float b; float a;
        };
    };
    Clay_Border betweenChildren {
        float width;
        Clay_Color color {
            float r; float g; float b; float a;
        };
    };
    Clay_CornerRadius cornerRadius {
        float topLeft;
        float topRight;
        float bottomLeft;
        float bottomRight;
    };
} Clay_BorderElementConfig;
```

**Usage**

As with all config macros, `CLAY_BORDER_CONFIG()` accepts designated initializer syntax and provides default values for any unspecified struct members. 


**Fields**

**`.left, .right, .top, .bottom`** - `Clay_Border`

`CLAY_BORDER_CONFIG(.left = { 2, COLOR_RED }, .right = { 4, COLOR_YELLOW } /* etc */)`

Indicates to the renderer that a border of `.color` should be draw at the specified edges of the bounding box, **overlapping the box contents by `.width`**.

This means that border configuration does not affect layout, as the width of the border doesn't contribute to the total container width or layout position. Border containers with zero padding will be drawn over the top of child elements.

---

**`.betweenChildren`** - `Clay_Border`

`CLAY_BORDER_CONFIG(.betweenChildren = { 2, COLOR_RED })`

Configures the width and color of borders to be drawn between children. These borders will be vertical lines if the parent uses `.layoutDirection = CLAY_LEFT_TO_RIGHT` and horizontal lines if the parent uses `CLAY_TOP_TO_BOTTOM`. Unlike `.left, .top` etc, this option **will generate additional rectangle render commands representing the borders between children.** As a result, the renderer does not need to specifically implement rendering for these border elements.

---

**`.cornerRadius`** - `float`

`CLAY_BORDER_CONFIG(.cornerRadius = 16)`

Defines the radius in pixels for the arc of border corners (`0` is square, `rectangle.width / 2` is circular). It is up to the renderer to decide how to interpolate between differing border widths and colors across shared corners.

Note that the `CLAY_CORNER_RADIUS(radius)` function-like macro is available to provide short hand for setting all four corner radii to the same value. e.g. `CLAY_BORDER_CONFIG(.cornerRadius = CLAY_CORNER_RADIUS(10))`

**Convenience Macros**

There are some common cases for border configuration that are repetitive, i.e. specifying the same border around all four edges. Some convenience macros are provided for these cases:

- `CLAY_BORDER_CONFIG_OUTSIDE(.width = 2, .color = COLOR_RED)` - Shorthand for configuring all 4 outside borders at once.`
- `CLAY_BORDER_CONFIG_OUTSIDE_RADIUS(width, color, radius)` - Shorthand for configuring all 4 outside borders at once, with the provided `.cornerRadius`. Note this is a function-like macro and does not take `.member = value` syntax.
- `CLAY_BORDER_CONFIG_ALL(.width = 2, .color = COLOR_RED)` - Shorthand for configuring all 4 outside borders and `.betweenChildren` at once. 
- `CLAY_BORDER_CONFIG_ALL_RADIUS(width, color, radius)` - Shorthand for configuring all 4 outside borders and `.betweenChildren` at once, with the provided `cornerRadius`. Note this is a function-like macro and does not take `.member = value` syntax.

### CLAY_CUSTOM_ELEMENT_CONFIG

**CLAY_CUSTOM_ELEMENT_CONFIG()** is a macro used to create and store `Clay_CustomElementConfig` structs, which are for configuring [CLAY_CUSTOM_ELEMENT]() elements.

**Struct Definition (Pseudocode)** 

```C
typedef struct
{
    #ifndef CLAY_EXTEND_CONFIG_CUSTOM
        void * customData; // Note: This field will be replaced if #define CLAY_EXTEND_CONFIG_CUSTOM is specified
    #else CLAY_EXTEND_CONFIG_CUSTOM
        // Contents of CLAY_EXTEND_CONFIG_CUSTOM will be pasted here
    #endif
} Clay_CustomElementConfig;
```

As with all config macros, `CLAY_CUSTOM_ELEMENT_CONFIG()` accepts designated initializer syntax and provides default values for any unspecified struct members. 

**Extension**

The underlying `Clay_ImageCustomConfig` can be extended with new members by using:
```C
#define CLAY_EXTEND_CONFIG_CUSTOM float newField;
#include "clay.h" // Define your extension before including clay.h
```

**Fields**

`.customData` - `void *`

`CLAY_CUSTOM_CONFIG(.customData = &myCustomData)`

`.customData` is a generic void pointer that can be used to pass through custom data to the renderer. **Note:** this field is generally not recommended for usage due to the lack of type safety, see `#define CLAY_EXTEND_CONFIG_CUSTOM` in [Preprocessor Directives]() for an alternative.

```C
// Extend CLAY_CUSTOM_ELEMENT_CONFIG with your custom data
#define CLAY_EXTEND_CONFIG_CUSTOM struct t_CustomElementData customData;
// Extensions need to happen _before_ the clay include
#include "clay.h"

// A rough example of how you could handle laying out 3d models in your UI
typedef struct t_CustomElementData {
    CustomElementType type;
    union {
        Model model;
        Video video;
        // ...
    };
} CustomElementData;

Model myModel = Load3DModel(filePath);
CustomElement modelElement = (CustomElement) { .type = CUSTOM_ELEMENT_TYPE_MODEL, .model = myModel }
// ...
CLAY_CONTAINER(id, style) {
    // This config is type safe and contains the CustomElementData struct
    CLAY_CUSTOM_ELEMENT(id, layout, CLAY_CUSTOM_ELEMENT_CONFIG(.customData = { .type = CUSTOM_ELEMENT_TYPE_MODEL, .model = myModel })) {}
}

// Later during your rendering
switch (renderCommand->commandType) {
    // ...
    case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
        // Your extended struct is passed through
        CustomElementData *data = renderCommand->elementConfig.customElementConfig->customData;
        if (!customElement) continue;
        switch (customElement->type) {
            case CUSTOM_ELEMENT_TYPE_MODEL: {
                // Render your 3d model here
                break;
            }
            case CUSTOM_ELEMENT_TYPE_VIDEO: {
                // Render your video here
                break;
            }
            // ...
        }
        break;
    }
}
```

## Misc Macros

### CLAY_ID

`Clay_ElementId CLAY_ID(char *label)`

Generates a [Clay_ElementId](#clay_elementid) from the provided `char *label`. Used both to generate ids when defining element macros, as well as for referencing ids later when using utility functions such as [Clay_PointerOver](#clay-pointerover)

### CLAY_IDI()

`Clay_ElementId CLAY_IDI(char *label, int index)`

Generates a [Clay_ElementId](#clay_elementid) string id from the provided `char *label`, combined with the `int index`. Used for generating ids for sequential elements (such as in a `for` loop) without having to construct dynamic strings at runtime.

## Data Structures & Definitions

### Clay_ElementId

```C
typedef struct {
    uint32_t id;
    uint32_t offset;
    uint32_t baseId;
    Clay_String stringId;
} Clay_ElementId;
```

Returned by [CLAY_ID](#clay_id) and [CLAY_IDI](#clay_idi), this struct contains a hash id, as well as the source string that was used to generate it.

**Fields**

**`.id`** - `uint32_t`

A unique ID derived from the string passed to [CLAY_ID](#clay_id) or [CLAY_IDI](#clay_idi).

---

**`.offset`** - `uint32_t`

If this id was generated using [CLAY_IDI](#clay_idi), `.offset` is the value passed as the second argument. For [CLAY_ID](#clay_id), this will always be `0`.

---

**`.baseId`** - `uint32_t`

If this id was generated using [CLAY_IDI](#clay_idi), `.baseId` is the hash of the base string passed, **before it is additionally hashed with `.offset`**. For [CLAY_ID](#clay_id), this will always be the same as `.id`.

---

**`.stringId`** - `Clay_String`

Stores the original string that was passed in when [CLAY_ID](#clay_id) or [CLAY_IDI](#clay_idi) were called.


### Clay_RenderCommandArray

```C
typedef struct
{
	uint32_t capacity;
	uint32_t length;
	Clay_RenderCommand *internalArray;
} Clay_RenderCommandArray;
```

Returned by [Clay_EndLayout](#clay_endlayout), this array contains the [Clay_RenderCommand](#clay_rendercommand)s representing the calculated layout.

**Fields**

**`.capacity`** - `uint32_t`

Represents the total capacity of the allocated memory in `.internalArray`.

---

**`.length`** - `uint32_t`

Represents the total number of `Clay_RenderCommand` elements stored consecutively at the address `.internalArray`.


---

**`.internalArray`** - `Clay_RenderCommand`

An array of [Clay_RenderCommand](#clay_rendercommand)s representing the calculated layout. If there was at least one render command, this array will contain elements from `.internalArray[0]` to `.internalArray[.length - 1]`.

### Clay_RenderCommand

```C
typedef struct
{
    Clay_BoundingBox boundingBox;
    Clay_ElementConfigUnion config;
    Clay_String text;
    uint32_t id;
    Clay_RenderCommandType commandType;
} Clay_RenderCommand;
```

**Fields**

**`.commandType`** - `Clay_RenderCommandType`

An enum indicating how this render command should be handled. Possible values include:

- `CLAY_RENDER_COMMAND_TYPE_NONE` - Should be ignored by the renderer, and never emitted by clay under normal conditions.
- `CLAY_RENDER_COMMAND_TYPE_RECTANGLE` - A rectangle should be drawn, configured with `.config.rectangleElementConfig`
- `CLAY_RENDER_COMMAND_TYPE_BORDER` - A border should be drawn, configured with `.config.borderElementConfig`
- `CLAY_RENDER_COMMAND_TYPE_TEXT` - Text should be drawn, configured with `.config.textElementConfig`
- `CLAY_RENDER_COMMAND_TYPE_IMAGE` - An image should be drawn, configured with `.config.imageElementConfig`
- `CLAY_RENDER_COMMAND_TYPE_SCISSOR_START` - Named after [glScissor](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glScissor.xhtml), this indicates that the renderer should begin culling any subsequent pixels that are drawn outside the `.boundingBox` of this render command.
- `CLAY_RENDER_COMMAND_TYPE_SCISSOR_END` - Only ever appears after a matching `CLAY_RENDER_COMMAND_TYPE_SCISSOR_START` command, and indicates that the scissor has ended.
- `CLAY_RENDER_COMMAND_TYPE_CUSTOM` - A custom render command controlled by the user, configured with `.config.customElementConfig`

---

**`.boundingBox`** - `Clay_BoundingBox`

```C
typedef struct {
    float x, y, width, height;
} Clay_BoundingBox;
```

A rectangle representing the bounding box of this render command, with `.x` and `.y` representing the top left corner of the element.

---

**`.config`** - `Clay_ElementConfigUnion`

A C union containing various pointers to config data, with the type dependent on `.commandType`. Possible values include:

- `config.rectangleElementConfig` - Used when `.commandType == CLAY_RENDER_COMMAND_TYPE_RECTANGLE`. See [CLAY_RECTANGLE_CONFIG](#clay_rectangle_config) for details.
- `config.textElementConfig` - Used when `.commandType == CLAY_RENDER_COMMAND_TYPE_TEXT`. See [CLAY_TEXT_CONFIG](#clay_text_config) for details.
- `config.imageElementConfig` - Used when `.commandType == CLAY_RENDER_COMMAND_TYPE_IMAGE`. See [CLAY_IMAGE_CONFIG](#clay_image_config) for details.
- `config.borderElementConfig` - Used when `.commandType == CLAY_RENDER_COMMAND_TYPE_BORDER`. See [CLAY_BORDER_CONFIG](#clay_border_config) for details.
- `config.customElementConfig` - Used when `.commandType == CLAY_RENDER_COMMAND_TYPE_CUSTOM`. See [CLAY_CUSTOM_CONFIG](#clay_custom_config) for details.
- `config.floatingElementConfig` - Not used and will always be NULL.
- `config.scrollElementConfig` - Not used and will always be NULL.

---

**`.text`** - `Clay_String`

Only used if `.commandType == CLAY_RENDER_COMMAND_TYPE_TEXT`. A `Clay_String` containing a string slice (char *chars, int length) representing text to be rendered. **Note: This string is not guaranteed to be null terminated.** Clay saves significant performance overhead by using slices when wrapping text instead of having to clone new null terminated strings. If your renderer does not support **ptr, length** style strings (e.g. Raylib), you will need to clone this to a new C string before rendering.

---

**`.id`** - `uint32_t`

The id that was originally used with the element macro that created this render command. See [CLAY_ID](#clay_id) for details.

### Clay_ScrollContainerData

```C
typedef struct
{
    Clay_Vector2 *scrollPosition;
    Clay_Dimensions scrollContainerDimensions;
    Clay_Dimensions contentDimensions;
    Clay_ScrollElementConfig config;
    bool found;
} Clay_ScrollContainerData;
```

**Fields**

**`.scrollPosition`** - `Clay_Vector2 *`

A pointer to the internal scroll position of this scroll container. Mutating it will result in elements inside the scroll container shifting up / down (`.y`) or left / right (`.x`).

---

**`.scrollContainerDimensions`** - `Clay_Dimensions`

```C
typedef struct {
    float width, height;
} Clay_Dimensions;
```

Dimensions representing the outer width and height of the scroll container itself.

---

**`.contentDimensions`** - `Clay_Dimensions`

```C
typedef struct {
    float width, height;
} Clay_Dimensions;
```

Dimensions representing the inner width and height of the content _inside_ the scroll container. Scrolling is only possible when the `contentDimensions` are larger in at least one dimension than the `scrollContainerDimensions`.

---

**`.config`** - `Clay_ScrollElementConfig`

The [Clay_ScrollElementConfig](#clay_scroll_config) for the matching scroll container element.
