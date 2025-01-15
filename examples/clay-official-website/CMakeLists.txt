cmake_minimum_required(VERSION 3.27)
project(clay_official_website C)

if(NOT ${CMAKE_C_COMPILER} MATCHES "clang")
  message(FATAL_ERROR "clang is required for webassembly build")
endif()

set(CMAKE_C_STANDARD 99)

add_executable(clay_official_website main.c)

# expand @URL@ macro from index.html.in to index.html
set(URL "http://localhost:8080/")
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/index.html.in" INDEX_HTML)
string(REPLACE @URL@ ${URL} INDEX_HTML "${INDEX_HTML}")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/index.html" "${INDEX_HTML}")

set_target_properties(clay_official_website PROPERTIES OUTPUT_NAME "index")
set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
target_compile_options(clay_official_website PUBLIC
  --target=wasm32
  -Wall
  -Werror
  -Wno-unknown-pragmas
  -emit-llvm
  -Os
  -DCLAY_WASM
  -mbulk-memory)
target_include_directories(clay_official_website PUBLIC .)
target_link_options(clay_official_website
  PUBLIC
  --target=wasm32
  -nostdlib
  -Wl,--strip-all
  -Wl,--export-dynamic
  -Wl,--no-entry
  -Wl,--export=__heap_base
  -Wl,--export=ACTIVE_RENDERER_INDEX
  -Wl,--initial-memory=16908288)
add_custom_command(
  TARGET clay_official_website POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
  ${CMAKE_CURRENT_BINARY_DIR}/web)
add_custom_command(
  TARGET clay_official_website POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/fonts
  ${CMAKE_CURRENT_BINARY_DIR}/web/fonts)
add_custom_command(
  TARGET clay_official_website POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/images
  ${CMAKE_CURRENT_BINARY_DIR}/web/images)
add_custom_command(
  TARGET clay_official_website POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_BINARY_DIR}/index.html
  ${CMAKE_CURRENT_BINARY_DIR}/index.wasm
  ${CMAKE_CURRENT_BINARY_DIR}/web)

set(CMAKE_C_FLAGS_RELEASE "-O3")
