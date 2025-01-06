# textui/console renderer example

## Inroduction
This renderer example utilizes ncurses and a mostly implemented library which converts the clay draw commands into ncurses commands.
A console rendering is very limited, and all coordinates are simply rounded via integer division to the coordinate system of a terminal.

## What works
- Rectangles draw approximately correctly with the right color
- Text wraps, and is well placed
- Clicking a location on a scroll bar moves the scrolled area
- Debug mode highlights on click
- Window resize and layout reaction

## What doesn't work
- Image rendering
- Sub character rectangles
- Text fonts and colors
- Clay extensions
- Clay scissor mode

## Reasonable expectations
This renderer is intended to allow for a nearly dependency free implementation of Clay, and nearly all linux distributions have ncurses installed.
This renderer will allow for quick visualization of layouts. There is no graphics acceleration though this example is quite responsive.
The unit of a terminal is a character, not a pixel, so there is only so much detail that can be rendered. The default character size defined in the renderer is 5x8 pixels.

## What could work
- More precise rendering of borders and rectangles, this could be implemented with box-drawing characters: https://en.wikipedia.org/wiki/Box-drawing_characters
- The mouse wheel on some terminals
- Images with an additional dependency on something like kitty: https://sw.kovidgoyal.net/kitty/graphics-protocol/
- Better text coloring by matching the background

## Setup
You'll have to setup an event loop using getch(), and pay attention to the various init functions in main.c
Then it's the regular Clay layout definition and calls to the renderer function.

## Conclusion
The truth is that if you're willing to learn ncurses and how it handles windows you can get a better terminal experience without Clay.
This example shows that for quick terminal apps, Clay could be extremely effective for organizing a user interface in the terminal.
I hope that this renderer helps many people build fun and cool projects by lowering the barrier to a framwork.
