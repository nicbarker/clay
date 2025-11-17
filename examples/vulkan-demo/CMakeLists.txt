cmake_minimum_required(VERSION 3.27)
project(clay_examples_vulkan_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# GLFW is used only for window creation and Vulkan surface management in this demo.
# When other examples are enabled (e.g., raylib), GLFW may already be present via
# their dependencies. Guarding the population prevents duplicate target errors
# (CMP0002) while still reusing the existing GLFW target when available.
if(NOT TARGET glfw)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw.git"
        GIT_TAG "3.4"
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glfw)
else()
    message(STATUS "Reusing existing GLFW target for Vulkan demo")
endif()

find_package(Vulkan REQUIRED)

add_executable(clay_examples_vulkan_demo
    main.cpp
)

target_include_directories(clay_examples_vulkan_demo PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(clay_examples_vulkan_demo PRIVATE
    glfw
    Vulkan::Vulkan
)

if(MSVC)
    # Keep MSVC warnings visible while allowing the demo to compile cleanly.
    target_compile_options(clay_examples_vulkan_demo PRIVATE /W4)
else()
    target_compile_options(clay_examples_vulkan_demo PRIVATE -Wall -Wextra)
endif()

# Allow Visual Studio users to copy sample resources (fonts, textures) beside the executable later.
add_custom_target(clay_examples_vulkan_demo_copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMENT "Prepare resource directory for future font/texture assets"
)
add_dependencies(clay_examples_vulkan_demo clay_examples_vulkan_demo_copy_resources)