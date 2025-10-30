cmake_minimum_required(VERSION 3.16)
project(OpenGLProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Paths ---
set(LIB_DIR ${CMAKE_SOURCE_DIR}/libraries)
set(INCLUDE_DIR ${LIB_DIR}/include)
set(IMGUI_DIR ${LIB_DIR}/include/imgui)

# --- GLAD + your utility classes ---
add_library(glad
        glad.c
        shaderClass.cpp
        VBO.cpp
        EBO.cpp
        VAO.cpp
        stb.cpp
        texture.cpp
        camera.cpp

        headers/shaderClass.h
        headers/VBO.h
        headers/EBO.h
        headers/VAO.h
        headers/texture.h
        headers/camera.h
)

target_include_directories(glad PUBLIC ${INCLUDE_DIR} headers)

# --- Dear ImGui ---
set(IMGUI_SRC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends ${INCLUDE_DIR})
target_link_libraries(imgui PRIVATE ${LIB_DIR}/lib/libglfw3.a)

# --- Main executable ---
add_executable(opengl main.cpp)
target_include_directories(opengl PRIVATE ${INCLUDE_DIR} headers ${IMGUI_DIR} ${IMGUI_DIR}/backends)

# --- Find OpenGL ---
find_package(OpenGL REQUIRED)

# --- Link everything together ---
target_link_libraries(opengl
        PRIVATE
        glad
        imgui
        ${LIB_DIR}/lib/libglfw3.a
        OpenGL::GL
        dl
        pthread
        X11
        Xrandr
        Xi
        Xxf86vm
        m
)

# --- Build info (optional) ---
message(STATUS "CMake version: ${CMAKE_VERSION}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "GLFW linked from: ${LIB_DIR}/lib/libglfw3.a")
