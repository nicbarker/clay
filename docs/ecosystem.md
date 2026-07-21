---
icon: lucide/globe
---

# Ecosystem

## Bindings for non-C

Clay is usable out of the box as a `.h` include in both C99 and C++20 with designated initializer support. Maintained bindings for other languages:

- [Odin bindings](https://github.com/nicbarker/clay/tree/main/bindings/odin)
- [Rust bindings](https://github.com/clay-ui-rs/clay)
- [C++ bindings](https://github.com/nicbarker/clay/tree/main/bindings/cpp)
- [C# bindings](https://github.com/nicbarker/clay/tree/main/bindings/csharp)
- [D bindings](https://github.com/nicbarker/clay/tree/main/bindings/d)
- [Zig bindings](https://github.com/nicbarker/clay/tree/main/bindings/zig)

## Other implementations

Clay has also been reimplemented, rather than bound, in other languages:

- [`glay`](https://github.com/soypat/glay) — Go, line-by-line rewrite prioritizing readability.
- [`totallygamerjet/clay`](https://github.com/totallygamerjet/clay) — Go, via `cxgo` (a C to Go transpiler).
- [`goclay`](https://github.com/igadmg/goclay) — Go, line-by-line rewrite closely matching the reference implementation.

## Renderers

The [`renderers/`](https://github.com/nicbarker/clay/tree/main/renderers) directory contains reference implementations that turn a `Clay_RenderCommandArray` into actual pixels:

`cairo`, `GLES3`, `playdate`, `raylib`, `SDL2`, `SDL3`, `sokol`, `termbox2`, `terminal`, `web` (including the [HTML retained-mode renderer](https://github.com/nicbarker/clay/blob/main/renderers/web/html/clay-html-renderer.html)), `win32_gdi`.

## Examples

The [`examples/`](https://github.com/nicbarker/clay/tree/main/examples) directory pairs each renderer with runnable demos, including:

- `raylib-sidebar-scrolling-container`, `raylib-multi-context`, `raylib-transitions`
- `GLES3-SDL2-sidebar-scrolling-container`, `GLES3-SDL2-video-demo`, `GLES3-GLFW-video-demo`
- `SDL2-video-demo`, `SDL3-simple-demo`
- `sokol-video-demo`, `sokol-corner-radius`
- `cairo-pdf-rendering`
- `termbox2-demo`, `termbox2-image-demo`, `terminal-example`
- `win32_gdi`, `playdate-project-example`, `cpp-project-example`
- `shared-layouts`, `clay-official-website`, `introducing-clay-video-demo`

## Debug tools

Clay includes built-in UI debugging tools, similar to browser inspectors, injecting extra render commands into the output `Clay_RenderCommandArray`. No extra setup is required beyond a working renderer.

```c
Clay_SetDebugModeEnabled(true);
```

This is persistent and does not need to be set every frame. The debug panel renders on the right side of the screen by default, compressing the layout by its width (`400` px, configurable via the internal — and unstable — `Clay__debugViewWidth`).

## Community

For help or discussion, join the [Discord server](https://discord.gg/b4FTWkxdvT).
