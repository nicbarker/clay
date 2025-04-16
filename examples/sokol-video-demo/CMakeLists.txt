cmake_minimum_required(VERSION 3.27)
project(sokol_video_demo C)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Linux -pthread shenanigans
if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

FetchContent_Declare(
    fontstash
    GIT_REPOSITORY "https://github.com/memononen/fontstash.git"
    GIT_TAG "b5ddc9741061343740d85d636d782ed3e07cf7be"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(fontstash)

FetchContent_Declare(
    sokol
    GIT_REPOSITORY "https://github.com/floooh/sokol.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(sokol)
set(sokol_HEADERS
    ${sokol_SOURCE_DIR}/sokol_app.h
    ${sokol_SOURCE_DIR}/sokol_gfx.h
    ${sokol_SOURCE_DIR}/sokol_glue.h
    ${sokol_SOURCE_DIR}/sokol_log.h
    ${sokol_SOURCE_DIR}/util/sokol_gl.h
    ${fontstash_SOURCE_DIR}/src/fontstash.h
    ${sokol_SOURCE_DIR}/util/sokol_fontstash.h)
if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    add_library(sokol STATIC sokol.c ${sokol_HEADERS})
    target_compile_options(sokol PRIVATE -x objective-c)
    target_link_libraries(sokol PUBLIC
        "-framework QuartzCore"
        "-framework Cocoa"
        "-framework MetalKit"
        "-framework Metal")
else()
    add_library(sokol STATIC sokol.c ${sokol_HEADERS})
    if (CMAKE_SYSTEM_NAME STREQUAL Linux)
        target_compile_definitions(sokol PRIVATE SOKOL_GLCORE=1)
        target_link_libraries(sokol INTERFACE X11 Xi Xcursor GL dl m)
        target_link_libraries(sokol PUBLIC Threads::Threads)
    endif()
endif()
target_include_directories(sokol INTERFACE ${sokol_SOURCE_DIR} ${fontstash_SOURCE_DIR}/src
                                 PRIVATE ${sokol_SOURCE_DIR} ${fontstash_SOURCE_DIR}/src)



if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    add_executable(sokol_video_demo WIN32 main.c)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sokol_video_demo)
else()
    add_executable(sokol_video_demo main.c)
endif()
target_link_libraries(sokol_video_demo PUBLIC sokol)

add_custom_command(
        TARGET sokol_video_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)
