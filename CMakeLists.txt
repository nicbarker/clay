cmake_minimum_required(VERSION 3.28)

if (MSVC)
project(clay CXX)
set(CMAKE_CXX_STANDARD 20)
else ()
project(clay C)
set(CMAKE_C_STANDARD 99)
endif()

add_subdirectory("examples/raylib-sidebar-scrolling-container")
add_subdirectory("examples/clay-official-website")