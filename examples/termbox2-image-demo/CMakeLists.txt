cmake_minimum_required(VERSION 3.25)
project(clay_examples_termbox2_image_demo C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS_RELEASE "-O3")

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    termbox2
    GIT_REPOSITORY "https://github.com/termbox/termbox2.git"
    GIT_TAG "ffd159c2a6106dd5eef338a6702ad15d4d4aa809"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(termbox2)

FetchContent_Declare(
    stb
    GIT_REPOSITORY "https://github.com/nothings/stb.git"
    GIT_TAG "fede005abaf93d9d7f3a679d1999b2db341b360f"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(stb)

add_executable(clay_examples_termbox2_image_demo main.c)

target_compile_options(clay_examples_termbox2_image_demo PUBLIC)
target_include_directories(clay_examples_termbox2_image_demo PUBLIC . PRIVATE ${termbox2_SOURCE_DIR} PRIVATE ${stb_SOURCE_DIR})
target_link_libraries(clay_examples_termbox2_image_demo PRIVATE m) # Used by stb_image.h

add_custom_command(
        TARGET clay_examples_termbox2_image_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)
