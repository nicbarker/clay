cmake_minimum_required(VERSION 3.10)

project(MyProject)

set(SRC main.c)

# Detect OS
if(WIN32)
    set(LIBS -ggdb -lshell32 -lwinmm -lgdi32 -lopengl32)
    set(EXT ".exe")
elseif(APPLE)
    set(LIBS -lm -framework Foundation -framework AppKit -framework IOKit -framework OpenGL)
    set(EXT "")
elseif(UNIX)
    set(LIBS -lXrandr -lX11 -lm -lGL -ldl -lpthread)
    set(EXT "")
else()
    set(detected_OS "Unknown")
endif()

# Settings for emcc
if(CMAKE_C_COMPILER MATCHES "emcc")
    set(EXPORTED_JS "-s EXPORTED_RUNTIME_METHODS=['stringToNewUTF8']")
    set(LIBS "-s FULL_ES3 -s WASM=1 -s ASYNCIFY -s USE_WEBGL2=1 -s GL_SUPPORT_EXPLICIT_SWAP_CONTROL=1 ${EXPORTED_JS}")
    set(LIBS "${LIBS} -s EXPORTED_FUNCTIONS=['_malloc', '_main']")
    set(EXT ".js")
    set(CMAKE_C_COMPILER "emcc")
    set(LIBS "${LIBS} --preload-file ./")
endif()

# Include directories
include_directories(../../../ ../../../renderers/RSGL)

# Add executable
add_executable(main${EXT} ${SRC})

# Link libraries
target_link_libraries(main${EXT} ${LIBS})