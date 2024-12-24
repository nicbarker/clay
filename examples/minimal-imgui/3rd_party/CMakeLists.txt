add_library(sokol INTERFACE)
target_include_directories(sokol INTERFACE sokol)

add_library(imgui STATIC
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC imgui)
