---
icon: lucide/rocket
---

# Getting started

## Installing

Clay is distributed as a single header file, `clay.h`, with no external dependencies. Download or clone it from the [Clay repository](https://github.com/nicbarker/clay) and include it after defining `CLAY_IMPLEMENTATION` in **exactly one** translation unit:

```c
// Must be defined in one file, _before_ #include "clay.h"
#define CLAY_IMPLEMENTATION
#include "../../clay.h"
```

Every other file that needs Clay's types and functions can `#include "clay.h"` without the `CLAY_IMPLEMENTATION` define.

## A minimal example

```c
const Clay_Color COLOR_LIGHT = (Clay_Color) {224, 215, 210, 255};
const Clay_Color COLOR_RED = (Clay_Color) {168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};

void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    printf("%s", errorData.errorText.chars);
    switch(errorData.errorType) {
        // etc
    }
}

// Example measure text function
static inline Clay_Dimensions MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData) {
    // Clay_TextElementConfig contains members such as fontId, fontSize, letterSpacing etc
    // Note: Clay_String->chars is not guaranteed to be null terminated
    return (Clay_Dimensions) {
            .width = text.length * config->fontSize, // <- this will only work for monospace fonts, see the renderers/ directory for more advanced text measurement
            .height = config->fontSize
    };
}

// Layout config is just a struct that can be declared statically, or inline
Clay_ElementDeclaration sidebarItemConfig = (Clay_ElementDeclaration) {
    .layout = {
        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50) }
    },
    .backgroundColor = COLOR_ORANGE
};

int main() {
    // Note: malloc is only used here as an example, any allocator that provides
    // a pointer to addressable memory of at least totalMemorySize will work
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));

    // Note: screenWidth and screenHeight will need to come from your environment, Clay doesn't handle window related tasks
    Clay_Initialize(arena, (Clay_Dimensions) { screenWidth, screenHeight }, (Clay_ErrorHandler) { HandleClayErrors });
    Clay_SetMeasureTextFunction(MeasureText, 0);

    while (renderLoop()) { // Will be different for each renderer / environment
        Clay_SetLayoutDimensions((Clay_Dimensions) { screenWidth, screenHeight });
        Clay_SetPointerState((Clay_Vector2) { mousePositionX, mousePositionY }, isMouseDown);
        Clay_UpdateScrollContainers(true, (Clay_Vector2) { mouseWheelX, mouseWheelY }, deltaTime);

        Clay_BeginLayout();

        CLAY(CLAY_ID("OuterContainer"), { .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = CLAY_PADDING_ALL(16), .childGap = 16 }, .backgroundColor = {250,250,255,255} }) {
            CLAY(CLAY_ID("SideBar"), {
                .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(16), .childGap = 16 },
                .backgroundColor = COLOR_LIGHT
            }) {
                CLAY(CLAY_ID("MainContent"), { .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) } }, .backgroundColor = COLOR_LIGHT }) {}
            }
        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout(); // deltaTime is used internally for transitions

        for (int i = 0; i < renderCommands.length; i++) {
            Clay_RenderCommand *renderCommand = &renderCommands.internalArray[i];
            switch (renderCommand->commandType) {
                case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                    DrawRectangle(renderCommand->boundingBox, renderCommand->renderData.rectangle.backgroundColor);
                    break;
                }
                // ... Implement handling of other command types
            }
        }
    }
}
```

More comprehensive, renderer-specific versions of this example can be found in the [examples directory](https://github.com/nicbarker/clay/tree/main/examples).

## The frame lifecycle

Clay is stateless between frames except for its internal arena, so every frame follows the same fixed order of calls:

1. [`Clay_SetLayoutDimensions`](reference/functions.md#clay_setlayoutdimensions)
2. [`Clay_SetPointerState`](reference/functions.md#clay_setpointerstate)
3. [`Clay_UpdateScrollContainers`](reference/functions.md#clay_updatescrollcontainers)
4. [`Clay_BeginLayout`](reference/functions.md#clay_beginlayout)
5. Declare your layout with the [element macros](reference/macros.md)
6. [`Clay_EndLayout`](reference/functions.md#clay_endlayout)
7. Render the results from the returned [`Clay_RenderCommandArray`](reference/structures.md#clay_rendercommandarray)

Startup is a separate, one-time sequence:

`Clay_MinMemorySize` → `Clay_CreateArenaWithCapacityAndMemory` → `Clay_Initialize` → `Clay_SetMeasureTextFunction`

!!! warning "The measure text function is on the hot path"
    `Clay_SetMeasureTextFunction` takes a pointer to a function that Clay calls many times per layout for text-heavy UIs. Despite Clay's internal text measurement cache, a slow measure function can easily dominate total layout cost.

## Naming conventions

- `CAPITAL_LETTERS()` are macros.
- `Clay__` (double underscore) marks internal functions not intended for external use, and subject to change without notice.
- `Clay_` (single underscore) marks the public API.

## Next steps

Continue with [Building UI hierarchies](guide/building-ui.md) to learn how the `CLAY()` macro composes into full layouts.
