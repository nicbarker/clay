cmake_minimum_required(VERSION 3.25)
project(clay_examples_termbox2_demo C)
set(CMAKE_C_STANDARD 11)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    termbox2
    GIT_REPOSITORY "https://github.com/termbox/termbox2.git"
    GIT_TAG "9c9281a9a4c971a2be57f8645e828ec99fd555e8"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(termbox2)

FetchContent_Declare(
    stb
    GIT_REPOSITORY "https://github.com/nothings/stb.git"
    GIT_TAG "f58f558c120e9b32c217290b80bad1a0729fbb2c"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(stb)

add_executable(clay_examples_termbox2_demo main.c)

target_compile_options(clay_examples_termbox2_demo PUBLIC)
target_include_directories(clay_examples_termbox2_demo PUBLIC . PRIVATE ${termbox2_SOURCE_DIR} PRIVATE ${stb_SOURCE_DIR})
target_link_libraries(clay_examples_termbox2_demo PRIVATE m) # Used by stb_image.h

add_custom_command(
        TARGET clay_examples_termbox2_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)
