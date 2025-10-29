//#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "headers/shaderClass.h"
#include "headers/VAO.h"
#include "headers/VBO.h"
#include "headers/EBO.h"

using namespace std;

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError() {
    while (const GLenum Error = glGetError()) {
        cout << "OpenGL Error: " << Error << endl;
    }
}

int main() {
    // Initializes GLFW
    glfwInit();

    // Tells GLFW to use version 3.3 of OpenGL, as well as the "core" profile.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[] = {
        -0.5f, -0.5f,0.0f,  1.0f, 0.0f, 0.0f,
        0.0f, -0.5f,0.0f,   1.0f, 0.0f, 1.0f,
        0.5f, -0.5f,0.0f,   0.0f, 0.0f, 1.0f,
        0.25f, 0.0f, 0.0f,  0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
        -0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    };

    GLuint indices[] = {
        0, 1, 5,
        1, 2, 3,
        5, 3, 4
    };

    // Create a new window element
    GLFWwindow* window = glfwCreateWindow(800,600, "OpenGL Application", nullptr, nullptr);
    // If window doesn't exist error out.
    if (!window) {
        cout << "Failed to initialize GLFW Window!";
        glfwTerminate();
        return -1;
    }
    // Use the window as the current context.
    glfwMakeContextCurrent(window);

    // Load OpenGL using glad.
    gladLoadGL();

    // Sets the OpenGL Viewport from 0,0 to 800, 600
    glViewport(0,0,800,600);

    Shader shaderProgram("../resources/shaders/default_vertex.glsl", "../resources/shaders/default_fragment.glsl");

    // Create a VAO and bind it
    VAO VAO1;
    VAO1.Bind();

    // Create a VBO and an EBO, passing in the vertices and indices data, as well as their byte size
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    // Link the VBO to the VAO1
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), nullptr);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // Unbind (stop using) to prevent accidental changes to the VAO, VBO or EBO
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Loop that stops once the window should close.
    while (!glfwWindowShouldClose(window)) {
        // Sets the background color to rgba float values
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        // Clear the color buffer bit and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        shaderProgram.Activate();

        // Binds to the VAO1 to draw the scene
        VAO1.Bind();

        // Clear the error log
        GLClearError();

        // Draw the VAO elements
        glDrawElements(GL_TRIANGLES, 9,GL_UNSIGNED_INT, nullptr);
        // Print errors if we encountered any
        GLCheckError();

        // Swap the back buffer to the front buffer to correctly display the color.
        glfwSwapBuffers(window);

        // Responds to GLFW events.
        glfwPollEvents();
    }

    // Cleans all objects (VAO, VBO, EBO, ShaderProgram...)
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    // removes the window
    glfwDestroyWindow(window);
    // Terminate cleanly GLFW
    glfwTerminate();
    return 0;
}