cmake_minimum_required(VERSION 3.27)
project(clay_examples_cairo_pdf_rendering C)
set(CMAKE_C_STANDARD 99)

add_executable(clay_examples_cairo_pdf_rendering main.c)

target_compile_options(clay_examples_cairo_pdf_rendering PUBLIC)
target_include_directories(clay_examples_cairo_pdf_rendering PUBLIC .)

target_link_libraries(clay_examples_cairo_pdf_rendering PUBLIC cairo)

set(CMAKE_C_FLAGS_DEBUG "-Wall -Werror -Wno-error=missing-braces")
set(CMAKE_C_FLAGS_RELEASE "-O3")

add_custom_command(
        TARGET clay_examples_cairo_pdf_rendering POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)
