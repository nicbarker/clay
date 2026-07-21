---
icon: lucide/home
---

# Clay

**Clay** (short for **C Layout**) is a high performance 2D UI layout library.

## Major features

- Microsecond layout performance
- Flex-box like layout model for complex, responsive layouts including text wrapping, scrolling containers and aspect ratio scaling
- Transition API for easy layout animations
- Single ~4.8k LOC `clay.h` file with **zero** dependencies (including no standard library linking)
- Wasm support: compile with clang to a 15kb uncompressed `.wasm` file for use in the browser
- Static arena based memory use with no malloc / free, and low total memory overhead (e.g. ~3.5mb for 8192 layout elements)
- React-like nested declarative syntax
- Renderer agnostic: outputs a sorted list of rendering primitives that can be easily composited in any 3D engine, and even compiled to HTML

Take a look at the [clay website](https://nicbarker.com/clay) for an example of Clay compiled to Wasm and running in the browser, or browse the [examples directory](https://github.com/nicbarker/clay/tree/main/examples) for renderer-specific demos.

You can also watch the [introduction video](https://youtu.be/DYWTw19_8r4) for an overview of the motivation behind Clay's development and a short demo of its usage.

## Where to go next

- [Getting started](getting-started.md) — install Clay and run your first layout
- [Building UI hierarchies](guide/building-ui.md) — the `CLAY()` macro and element IDs
- [API reference](reference/functions.md) — full function, macro, and struct reference
- [Ecosystem](ecosystem.md) — renderers, language bindings, and other implementations

!!! note "Single source header"
    Clay ships as a single `clay.h` header. There is nothing to build or link — `#define CLAY_IMPLEMENTATION` once, then `#include "clay.h"`.
