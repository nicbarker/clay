cmake_minimum_required(VERSION 3.27)
project(QRhiVideoDemo C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.9 COMPONENTS Gui GuiPrivate Quick ShaderTools REQUIRED)

qt_standard_project_setup(REQUIRES 6.9)

add_executable(${PROJECT_NAME}
    clayimplementation.c
    clayqquickitem.cpp
    clay_renderer_qrhi_impl.cpp
    main.cpp
    videodemoitem.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    .
    ../..
    ../../renderers/QRhi
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Gui
    Qt6::GuiPrivate
    Qt6::Quick
)

qt_add_qml_module(${PROJECT_NAME}
    URI com.${PROJECT_NAME}
    VERSION 1.0
    QML_FILES
        main.qml
    NO_GENERATE_EXTRA_QMLDIRS
)

qt6_add_shaders(${PROJECT_NAME} "shaders"
    # gl_VertexId is in earliest OpenGL ES 3.00, and GLSL 130
    GLSL "300 es,130"
    PREFIX
        "/qt/qml/com/clay"
    FILES
        "../../renderers/QRhi/clayquad.vert"
        "../../renderers/QRhi/clayquad.frag"
)
