GLES3 Renderer Video Demo (Using SDL2)
======================================

This directory contains a standard Video-Demo example
using work-in-progress GLES3 renderer.
While it still needs refinement, the renderer is already functional and demonstrates the core rendering pipeline.

Current features

-	Supports all draw commands except custom.
-	In the best-case scenario (no clipping):
-	All quad-based commands (Rectangle, Image, Border) are rendered in a single draw call.
-	All glyphs belonging to the same font are rendered in one instanced draw call.
-	When clipping (scissoring) is used:
-	The renderer flushes draw calls before and after each scissor region.
-	Supports up to 4 fonts and 4 image textures.
-	Image textures may also be used as texture atlases.
-	Custom UserData provides per-image UV coordinates, allowing multiple images to share a single OpenGL texture.
-	Uses stb_image.h and stb_truetype.h as single-header dependencies for asset loading.
-	The loading layer is modular and can be replaced with a different asset pipeline if needed.

Currently builds on:
-	Emscripten
-	clang++ / macOS
-	CMake support is not available yet.

Windowing and platform support

This example uses SDL2, but the renderer is framework agnostic.

For sake of example you can also build it with
hand-crafted Makefile.macos

    make -f Makefile.emscripten test

and then navigate to http://localhost:8080
On Emscripten it works well.

