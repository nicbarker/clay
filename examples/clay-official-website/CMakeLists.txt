cmake_minimum_required(VERSION 3.27)
project(clay_official_website C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER clang)

# Specify WebAssembly as target
set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -Wall -Werror -Os -DCLAY_WASM -mbulk-memory --target=wasm32 -nostdlib"
)
set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -Wl,--strip-all,--export-dynamic,--no-entry,--export=__heap_base,--export=ACTIVE_RENDERER_INDEX,--initial-memory=6553600"
)

add_executable(clay_official_website main.c)

target_compile_options(clay_official_website PUBLIC -Wall -Werror -Wno-unknown-pragmas -Wno-error=missing-braces)
target_include_directories(clay_official_website PUBLIC .)

# Adjust WebAssembly output binary name
set_target_properties(clay_official_website PROPERTIES
    OUTPUT_NAME index.wasm
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/clay
)

# Custom commands to copy additional resources
add_custom_command(TARGET clay_official_website POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/index.html ${CMAKE_BINARY_DIR}/clay/index.html
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/fonts ${CMAKE_BINARY_DIR}/clay/fonts
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/images ${CMAKE_BINARY_DIR}/clay/images
)

set(CMAKE_C_FLAGS_RELEASE "-O3")
