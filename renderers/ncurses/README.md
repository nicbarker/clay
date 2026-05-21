# Clay Ncurses Renderer

This directory contains a backend renderer for [Clay](https://github.com/nicbarker/clay) that targets the terminal using the `ncurses` library. It allows you to build text-based user interfaces (TUI) using the same Clay layout engine used for graphical applications.

## Features

- **Responsive Layouts in the Terminal**: Use flex-box like layout rules to organize text and panels in a terminal window.
- **Color Support**:
  - Automatically matches Clay's `RGB` colors to the nearest available terminal color.
  - Supports **256-color** terminals (xterm-256color) for richer palettes.
  - Graceful fallback to standard 8 ANSI colors for older terminals.
- **UTF-8 Support**: Correctly measures and renders multibyte characters (assuming the terminal is configured for UTF-8).
- **Primitives Supported**:
  - `Rectangle`: Renders as solid blocks of color.
  - `Text`: Renders colored text.
  - `Border`: Renders lines using ACS (Alternate Character Set) box-drawing characters, supporting rounded corners and different line styles where possible.
  - `Scissor/Clipping`: Fully supports nested clipping rectangles (e.g., for scroll containers).
- **Input Handling**:
  - sets up standard ncurses input modes (cbreak, noecho, keypad).
  - Enables mouse event reporting.

## Usage

To use the ncurses renderer in your Clay application:

### 1. Include the Renderer

```c
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "renderers/ncurses/clay_renderer_ncurses.c"
```

### 2. Initialization

Initialize the renderer before your main loop. This sets up the terminal screen, colors, and input modes.

```c
Clay_Initialize(arena, (Clay_Dimensions){0,0}, (Clay_ErrorHandler){NULL});
Clay_SetMeasureTextFunction(Clay_Ncurses_MeasureText, NULL);
Clay_Ncurses_Initialize();
```

### 3. Rendering Loop

In your main loop, update the layout dimensions based on the terminal size, run the layout, and then pass the render commands to the ncurses renderer.

```c
while (!shouldQuit) {
    // 1. Get current terminal size
    Clay_Dimensions dims = Clay_Ncurses_GetLayoutDimensions();
    Clay_SetLayoutDimensions(dims);

    // 2. Handle Input
    int key = Clay_Ncurses_ProcessInput(stdscr); 
    if (key == 'q') break;

    // 3. Define Layout
    Clay_BeginLayout();

    // Example: Clickable Element
    CLAY(CLAY_ID("Clickable"), {0}) {
        Clay_Ncurses_OnClick(MyCallback, myData);
        CLAY_TEXT(CLAY_STRING("Click Me"), CLAY_TEXT_CONFIG({0}));
    }

    Clay_RenderCommandArray commands = Clay_EndLayout();

    // 4. Render
    Clay_Ncurses_Render(commands);
}
```

### 4. Input & Interaction

The renderer provides helper functions to easy integration of mouse interactions:

- **`Clay_Ncurses_ProcessInput(WINDOW *window)`**: Call this instead of `getch` or `wgetch`. It handles mouse events (including scroll wheel mapping to `Clay_UpdateScrollContainers`), updates the internal Clay pointer state, and returns the key code for your application to handle.
- **`Clay_Ncurses_OnClick(void (*userData)(...), void *userData)`**: A helper to attach a click listener to the current element. It uses `Clay_OnHover` internally. Your callback function should check if `pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME` for instant click feedback.

### 5. Font Styling

You can apply **Bold** and **Underline** styles using the `fontId` configuration in `CLAY_TEXT`.
Use the provided macros:

```c
CLAY_TEXT(CLAY_STRING("Bold Text"), CLAY_TEXT_CONFIG({ .fontId = CLAY_NCURSES_FONT_BOLD }));
CLAY_TEXT(CLAY_STRING("Underline"), CLAY_TEXT_CONFIG({ .fontId = CLAY_NCURSES_FONT_UNDERLINE }));
CLAY_TEXT(CLAY_STRING("Both"),      CLAY_TEXT_CONFIG({ .fontId = CLAY_NCURSES_FONT_BOLD | CLAY_NCURSES_FONT_UNDERLINE }));
```

### 6. Cleanup

Restore the terminal to its normal state before exiting.

```c
Clay_Ncurses_Terminate();
```

## Compilation

You must link against the `ncurses` (and potentially `tinfo`) library.

```bash
gcc main.c -lncurses -o my_app
```

On some systems attempting to use wide characters/UTF-8 might require linking `ncursesw` instead:

```bash
gcc main.c -lncursesw -o my_app
```

## How it Works

The renderer maps Clay's floating-point coordinate system to the integer grid of the terminal.
- **Cell Size**: It assumes a logical "pixel" size for each character cell (defaults to 8x16 internally) to map Clay's high-precision layout to character columns and rows.
- **Double Buffering**: It uses ncurses' standard buffering mechanisms (`refresh()`) to prevent flickering during updates.
- **Clipping**: It uses a software scissor stack to determine visibility, as terminals do not natively support arbitrary clipping regions for drawing commands.

## Limitations

- **Images**: Rendering images is not currently supported.
- **Fonts**: Text size is fixed to the terminal's cell size. `fontSize` configs are ignored for layout measurement, though they affect the logical ID generation.
- **Pixel Precision**: Since the output is quantized to character cells, fine-grained pixel alignment (e.g., a 1px shift) will snap to the nearest cell boundary.
