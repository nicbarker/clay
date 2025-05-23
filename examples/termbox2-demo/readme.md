# Termbox2 renderer demo

Terminal-based renderer using [termbox2](https://github.com/termbox/termbox2)

Configuration can be overriden by environment variables:
- CLAY_TB_COLOR_MODE
  - NORMAL
  - 256
  - 216
  - GRAYSCALE
  - TRUECOLOR
  - NOCOLOR
- CLAY_TB_BORDER_CHARS
  - DEFAULT
  - ASCII
  - UNICODE
  - NONE
- CLAY_TB_TRANSPARENCY
  - 1
  - 0
- CLAY_TB_CELL_PIXELS
  - widthxheight

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
