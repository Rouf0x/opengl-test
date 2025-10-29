cmake_minimum_required(VERSION 4.0)
project(OpenGLProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- GLAD ---
add_library(glad glad.c
        headers/shaderClass.h
        shaderClass.cpp
        shaderClass.cpp
        headers/VBO.h
        VBO.cpp
        VBO.cpp
        EBO.cpp
        headers/EBO.h
        VAO.cpp
        headers/VAO.h
        shaderClass.cpp
        libraries/include/stb/stb_image.h
        stb.cpp
)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/libraries/include)

# --- Main executable ---
add_executable(opengl main.cpp)
target_include_directories(opengl PRIVATE ${CMAKE_SOURCE_DIR}/libraries/include)

# --- Find OpenGL ---
find_package(OpenGL REQUIRED)

# --- Link GLFW static library (.a file) and GLAD ---
target_link_libraries(opengl
        PRIVATE
        glad
        ${CMAKE_SOURCE_DIR}/libraries/lib/libglfw3.a
        OpenGL::GL
        dl
        pthread
        X11
        Xrandr
        Xi
        Xxf86vm
        m
)

# --- Optional: show build info ---
message(STATUS "Using CMake version: ${CMAKE_VERSION}")
message(STATUS "Using compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Linked GLFW: ${CMAKE_SOURCE_DIR}/libraries/lib/libglfw3.a")
