cmake_minimum_required(VERSION 3.28)
project(clay_examples_raylib_sidebar_scrolling_container C)
set(CMAKE_C_STANDARD 99)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(clay_examples_raylib_sidebar_scrolling_container main.c multi-compilation-unit.c)

target_compile_options(clay_examples_raylib_sidebar_scrolling_container PUBLIC -DCLAY_DEBUG -DCLAY_OVERFLOW_TRAP -Wall -Werror -Wno-unknown-pragmas)
target_include_directories(clay_examples_raylib_sidebar_scrolling_container PUBLIC .)

target_link_libraries(clay_examples_raylib_sidebar_scrolling_container PUBLIC raylib)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_custom_command(
        TARGET clay_examples_raylib_sidebar_scrolling_container POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)