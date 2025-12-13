cmake_minimum_required(VERSION 3.27)
set(CMAKE_C_STANDARD 99)
project(GLES3_SDL2_sidebar_scrolling_container C)

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
# SDL2
# -------------------------------------------------
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
    GIT_TAG "release-2.30.10"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL2)

# -------------------------------------------------
# Executable
# -------------------------------------------------
add_executable(GLES3_SDL2_sidebar_scrolling_container main.c)
target_compile_options(GLES3_SDL2_sidebar_scrolling_container PUBLIC)
target_include_directories(GLES3_SDL2_sidebar_scrolling_container
    PUBLIC
        .  # This renderer
        ../.. # Clay
        ${stb_SOURCE_DIR} # STB header only depencency that does not have its own CMake build

)

# -------------------------------------------------
# Link libraries
# -------------------------------------------------
target_link_libraries(GLES3_SDL2_sidebar_scrolling_container PUBLIC
    SDL2::SDL2main
    SDL2::SDL2-static
)

# -------------------------------------------------
# Platform-specific OpenGL / GLES
# -------------------------------------------------
find_package(SDL2 REQUIRED)
find_library(OPENGL_FRAMEWORK OpenGL)
target_link_libraries(GLES3_SDL2_sidebar_scrolling_container
    PRIVATE
        ${OPENGL_FRAMEWORK}
)

# -------------------------------------------------
# Build flags (kept minimal)
# -------------------------------------------------
if(MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
else()
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
endif()

# -------------------------------------------------
# Copy resources
# -------------------------------------------------
add_custom_command(
    TARGET GLES3_SDL2_sidebar_scrolling_container POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
)
