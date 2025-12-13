cmake_minimum_required(VERSION 3.27)
project(GLES3_GLFW_video_demo C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -------------------------------------------------
# FetchContent
# -------------------------------------------------
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# -------------------------------------------------
# STB (header-only)
# -------------------------------------------------
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# -------------------------------------------------
# GLFW
# -------------------------------------------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

# Disable examples/tests/docs (important)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

# -------------------------------------------------
# Executable
# -------------------------------------------------
add_executable(GLES3_GLFW_video_demo
    main.c
)

target_include_directories(GLES3_GLFW_video_demo
    PUBLIC
        .
        ../..
        ${stb_SOURCE_DIR}
)

# -------------------------------------------------
# Link libraries
# -------------------------------------------------
target_link_libraries(GLES3_GLFW_video_demo
    PRIVATE
        glfw
)

# -------------------------------------------------
# Platform-specific OpenGL / GLES
# -------------------------------------------------
if(APPLE)
    find_library(OPENGL_FRAMEWORK OpenGL)
    target_link_libraries(GLES3_GLFW_video_demo PRIVATE ${OPENGL_FRAMEWORK})

    # Needed for GLFW on macOS
    target_link_libraries(GLES3_GLFW_video_demo
        PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
elseif(WIN32)
    target_link_libraries(GLES3_GLFW_video_demo PRIVATE opengl32)
elseif(UNIX)
    target_link_libraries(GLES3_GLFW_video_demo PRIVATE GL)
endif()

# -------------------------------------------------
# Build flags (kept minimal)
# -------------------------------------------------
if(MSVC)
    target_compile_options(GLES3_GLFW_video_demo PRIVATE /W3)
else()
    target_compile_options(GLES3_GLFW_video_demo PRIVATE -Wall -Wextra)
endif()

# -------------------------------------------------
# Copy resources
# -------------------------------------------------
add_custom_command(
    TARGET GLES3_GLFW_video_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            ${CMAKE_CURRENT_BINARY_DIR}/resources
)
