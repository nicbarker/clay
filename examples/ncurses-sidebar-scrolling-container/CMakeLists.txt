cmake_minimum_required(VERSION 3.27)
project(clay_examples_textui_sidebar_scrolling_container C)
set(CMAKE_C_STANDARD 99)

add_executable(clay_examples_textui_sidebar_scrolling_container main.c multi-compilation-unit.c)

target_compile_options(clay_examples_textui_sidebar_scrolling_container PUBLIC)
target_include_directories(clay_examples_textui_sidebar_scrolling_container PUBLIC .)

target_link_libraries(clay_examples_textui_sidebar_scrolling_container PUBLIC ncurses)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Werror -DCLAY_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_custom_command(
        TARGET clay_examples_textui_sidebar_scrolling_container POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)
