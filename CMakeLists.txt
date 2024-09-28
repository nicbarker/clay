cmake_minimum_required(VERSION 3.28)
project(clay)

add_subdirectory("examples/cpp-project-example")

if(NOT MSVC)
add_subdirectory("examples/raylib-sidebar-scrolling-container")
add_subdirectory("examples/clay-official-website")
endif()