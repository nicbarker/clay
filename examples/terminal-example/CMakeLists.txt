cmake_minimum_required(VERSION 3.27)
project(clay_examples_terminal C)
set(CMAKE_C_STANDARD 99)

add_executable(clay_examples_terminal main.c)

target_compile_options(clay_examples_terminal PUBLIC)
target_include_directories(clay_examples_terminal PUBLIC .)
if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    target_link_libraries(clay_examples_terminal INTERFACE m)
endif()

target_link_libraries(clay_examples_terminal PUBLIC ${CURSES_LIBRARY})
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Werror -DCLAY_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")