cmake_minimum_required(VERSION 3.16)

project(demo C)

set(CMAKE_C_STANDARD 99)

# -----------------------------
# EDIT ME WITH RELATIVE PATH TO:
# tigr.c & tigr.h
set(TIGR_DIR "../../../tigr")

add_executable(demo
    main.c
    ${TIGR_DIR}/tigr.c
    ../../renderers/tigr/tigr_utils.c
    ../../renderers/tigr/clay_renderer_tigr.c
)

target_include_directories(demo PRIVATE
    ${TIGR_DIR}
)

# Platform-specific libraries
if(WIN32)
    target_link_libraries(demo PRIVATE
        opengl32
        gdi32
    )

elseif(APPLE)
    find_library(OPENGL_FRAMEWORK OpenGL)
    find_library(COCOA_FRAMEWORK Cocoa)

    target_link_libraries(demo PRIVATE
        ${OPENGL_FRAMEWORK}
        ${COCOA_FRAMEWORK}
    )

elseif(UNIX)
    target_link_libraries(demo PRIVATE
        m
        GLU
        GL
        X11
    )
endif()

add_custom_command(TARGET demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:demo>/resources
)
