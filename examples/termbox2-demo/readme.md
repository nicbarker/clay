# Termbox2 renderer demo

Terminal-based renderer using [termbox2](https://github.com/termbox/termbox2)

This demo shows a color palette and a few different components. It allows
changing configuration settings for colors, border size rounding mode,
characters used for borders, and transparency.

```
Keybinds:
c/C - Cycle through color modes
b/B - Cycle through border modes
h/H - Cycle through border characters
i/I - Cycle through image modes
t/T - Toggle transparency
d/D - Toggle debug mode
q/Q - Quit
```

Configuration can be also be overriden by environment variables:
- `CLAY_TB_COLOR_MODE`
  - `NORMAL`
  - `256`
  - `216`
  - `GRAYSCALE`
  - `TRUECOLOR`
  - `NOCOLOR`
- `CLAY_TB_BORDER_CHARS`
  - `DEFAULT`
  - `ASCII`
  - `UNICODE`
  - `NONE`
- `CLAY_TB_IMAGE_MODE`
  - `DEFAULT`
  - `PLACEHOLDER`
  - `BG`
  - `ASCII_FG`
  - `ASCII`
  - `UNICODE`
  - `ASCII_FG_FAST`
  - `ASCII_FAST`
  - `UNICODE_FAST`
- `CLAY_TB_TRANSPARENCY`
  - `1`
  - `0`
- `CLAY_TB_CELL_PIXELS`
  - `widthxheight`

## Building

Build the binary with cmake

```sh
mkdir build
cd build
cmake ..
make
```

Then run the executable:

```sh
./clay_examples_termbox2_demo
```

## Attributions

Resources used:
- `512px-Shark_antwerp_zoo.jpeg`
  - Retrieved from <https://commons.wikimedia.org/wiki/File:Shark_antwerp_zoo.jpg>
  - License: [Creative Commons Attribution 3.0 Unported](https://creativecommons.org/licenses/by/3.0/deed.en)
  - No changes made
