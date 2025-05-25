cmake_minimum_required(VERSION 3.25)
project(clay_examples_termbox2_demo C)
set(CMAKE_C_STANDARD 99)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    termbox2
    GIT_REPOSITORY "https://github.com/termbox/termbox2.git"
    GIT_TAG "8ee9dc17e1ca61c630f91db0aa7f81fa29a32040"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(termbox2)

add_executable(clay_examples_termbox2_demo main.c)

target_compile_options(clay_examples_termbox2_demo PUBLIC)
target_include_directories(clay_examples_termbox2_demo PUBLIC . PRIVATE ${termbox2_SOURCE_DIR})

add_custom_command(
        TARGET clay_examples_termbox2_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)
