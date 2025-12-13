GLES3 Renderer Video Demo (Using GLFW)
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

This example uses GLFW, and the renderer is framework agnostic

How to build it with CMake?
---------------------------

Cmake build is the easiest way to build it:

    mkdir build
    cmake -S . -B ./build

How to build and run on Emscripten:
----------------------------------

For Emscripten the build is a bit custom,
but it depends on CMakeBuild to install header-stb dependency.
So you still need to build it with CMake first.

And then you have to source the Emscripten SDK:

    source /path/to/emscripten/emsdk/emsdk_env.sh

Then build it with hand-crafted Makefile.emscripten:

    make -f Makefile.emscripten test

and then navigate to http://localhost:8080

