cmake_minimum_required(VERSION 3.28)
project(clay_official_website C)

set(CMAKE_C_STANDARD 99)

add_executable(clay_official_website main.c)

target_compile_options(clay_official_website PUBLIC -DCLAY_OVERFLOW_TRAP -Wno-initializer-overrides)
target_include_directories(clay_official_website PUBLIC .)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_custom_command(
        TARGET clay_official_website POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)