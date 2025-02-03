cmake_minimum_required(VERSION 3.27)
project(clay_examples_cairo_pdf_rendering C)
set(CMAKE_C_STANDARD 99)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")

add_executable(clay_examples_cairo_pdf_rendering main.c)

find_package(Cairo REQUIRED)

target_compile_options(clay_examples_cairo_pdf_rendering PUBLIC)
target_include_directories(clay_examples_cairo_pdf_rendering PUBLIC . ${CAIRO_INCLUDE_DIRS})

target_link_libraries(clay_examples_cairo_pdf_rendering PUBLIC Cairo::Cairo)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")

add_custom_command(
        TARGET clay_examples_cairo_pdf_rendering POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)
