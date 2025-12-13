GLES3 Renderer Scrolling Container (Using SDL2)
===============================================

This directory contains a complete example thatn can me used to test all different draw commands
using work-in-progress GLES3 renderer.
While it still needs refinement, the renderer is already functional and demonstrates the core rendering pipeline.

The images used as resources in this example, namely:

- millbank.jpeg a window with a panoramic city view;
- and profile-picture.png showing objects aligned in a circle;

are taken with my phone camera and are dedicated to the Public Domain.

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

How to build and run on Mac:
----------------------------

Requires SDL2:

    brew install sdl2

Requires STB:

    cmake -B build

Build it with:

    make -f Makefile.macos

And run with:

    ./macos-sidebar-scrolling-container-sdl2
