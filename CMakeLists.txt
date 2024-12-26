cmake_minimum_required(VERSION 3.27)
project(clay)

add_subdirectory("examples/cpp-project-example")

# Don't try to compile C99 projects using MSVC
if(NOT MSVC)
  add_subdirectory("examples/raylib-sidebar-scrolling-container")
  add_subdirectory("examples/cairo-pdf-rendering")
  add_subdirectory("examples/clay-official-website")
  add_subdirectory("examples/terminal-example")
endif()
