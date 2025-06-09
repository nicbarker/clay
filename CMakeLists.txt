cmake_minimum_required(VERSION 3.27)
project(clay)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(CLAY_INCLUDE_ALL_EXAMPLES "Build all examples" ON)
option(CLAY_INCLUDE_DEMOS "Build video demo and website" OFF)
option(CLAY_INCLUDE_CPP_EXAMPLE "Build C++ example" OFF)
option(CLAY_INCLUDE_RAYLIB_EXAMPLES "Build raylib examples" OFF)
option(CLAY_INCLUDE_SDL2_EXAMPLES "Build SDL 2 examples" OFF)
option(CLAY_INCLUDE_SDL3_EXAMPLES "Build SDL 3 examples" OFF)
option(CLAY_INCLUDE_WIN32_GDI_EXAMPLES "Build Win32 GDI examples" OFF)
option(CLAY_INCLUDE_SOKOL_EXAMPLES "Build Sokol examples" OFF)
option(CLAY_INCLUDE_PLAYDATE_EXAMPLES "Build Playdate examples" OFF)

message(STATUS "CLAY_INCLUDE_DEMOS: ${CLAY_INCLUDE_DEMOS}")

if(APPLE)
  enable_language(OBJC)
endif()

if(CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_CPP_EXAMPLE)
  add_subdirectory("examples/cpp-project-example")
endif()
if(CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_DEMOS)
  if(NOT MSVC)
    add_subdirectory("examples/clay-official-website")
    add_subdirectory("examples/terminal-example")
  endif()
  add_subdirectory("examples/introducing-clay-video-demo")
endif ()

if(CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_RAYLIB_EXAMPLES)
  add_subdirectory("examples/raylib-multi-context")
  add_subdirectory("examples/raylib-sidebar-scrolling-container")
endif ()
if(CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_SDL2_EXAMPLES)
  add_subdirectory("examples/SDL2-video-demo")
endif ()
if(NOT MSVC AND (CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_SDL3_EXAMPLES))
    add_subdirectory("examples/SDL3-simple-demo")
endif()
if(CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_SOKOL_EXAMPLES)
  add_subdirectory("examples/sokol-video-demo")
  add_subdirectory("examples/sokol-corner-radius")
endif()

# Playdate example not included in ALL because users need to install the playdate SDK first which requires a license agreement
if(CLAY_INCLUDE_PLAYDATE_EXAMPLES)
  add_subdirectory("examples/playdate-project-example")
endif()

if(WIN32)   # Build only for Win or Wine
    if(CLAY_INCLUDE_ALL_EXAMPLES OR CLAY_INCLUDE_WIN32_GDI_EXAMPLES)
        add_subdirectory("examples/win32_gdi")
    endif()
endif()

#  add_subdirectory("examples/cairo-pdf-rendering") Some issue with github actions populating cairo, disable for now

#add_library(${PROJECT_NAME} INTERFACE)
#target_include_directories(${PROJECT_NAME} INTERFACE .)
