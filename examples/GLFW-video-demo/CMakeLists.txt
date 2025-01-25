cmake_minimum_required(VERSION 3.27)
project(GLFW_video_demo C)
set(CMAKE_C_STANDARD 99)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Raylib may already have fetched the dependency.
if (NOT DEFINED GLFW_SOURCE_DIR)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw"
        GIT_TAG "3.4"
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glfw)
endif()

FetchContent_Declare(
    nanovg
    GIT_REPOSITORY "https://github.com/memononen/nanovg"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(nanovg)

add_executable(GLFW_video_demo
    main.c
    glad/gl.c
    "${nanovg_SOURCE_DIR}/src/nanovg.c"
)

target_link_libraries(GLFW_video_demo PUBLIC glfw)

target_compile_options(GLFW_video_demo PUBLIC)
target_include_directories(GLFW_video_demo PUBLIC . "${nanovg_SOURCE_DIR}/src")

set(CMAKE_C_FLAGS_DEBUG "-Wall -Werror -DCLAY_DEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3")

add_custom_command(
    TARGET GLFW_video_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)
