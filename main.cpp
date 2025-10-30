//#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <stb/stb_image.h>

#include "headers/shaderClass.h"
#include "headers/VAO.h"
#include "headers/VBO.h"
#include "headers/EBO.h"
#include "headers/texture.h"

using namespace std;

const int width = 800;
const int height = 600;

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
        // Positions            // Colors           // Texture ST
        -0.5f, 0.0f,  0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // 0
         0.5f, 0.0f,  0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // 1
         0.5f, 0.0f, -0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // 2
        -0.5f, 0.0f, -0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // 3
         0.0f, 1.0f,  0.0f,     1.0f, 1.0f, 1.0f,   0.5f, 1.0f,   // 4 apex
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,

        2, 3, 4,
        0, 3, 4,
        1, 0, 4,
        2, 1, 4
    };



    // Create a new window element
    GLFWwindow* window = glfwCreateWindow(width,height, "OpenGL Application", nullptr, nullptr);
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

    // Create a new Shader Program with the vertex and fragment shaders
    Shader shaderProgram("../resources/shaders/default_vertex.glsl", "../resources/shaders/default_fragment.glsl");

    // Create a VAO and bind it
    VAO VAO1;
    VAO1.Bind();

    // Create a VBO and an EBO, passing in the vertices and indices data, as well as their byte size
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    // Link the VBO to the VAO1
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), nullptr);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind (stop using) to prevent accidental changes to the VAO, VBO or EBO
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Create a scale uniform that can be edited to make the object appear larger or smaller
    // GLint U_Scale = glGetUniformLocation(shaderProgram.ID, "scale");


    // Create a new texture
    const char* texturePath = "../texture.jpg";
    texture texture(texturePath, GL_TEXTURE_2D, GL_RGB, GL_TEXTURE0);
    // Generate the texture object
    texture.GenerateTex(shaderProgram, "tex0", 0);

    // Set a rotation and a previous time value
    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    // Enable Depth_Test to avoid faces behind other ones from being rendered
    glEnable(GL_DEPTH_TEST);

    // Loop that stops once the window should close.
    while (!glfwWindowShouldClose(window)) {
        // Sets the background color to rgba float values
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        // Clear the color buffer bit and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program
        shaderProgram.Activate();

        // Increase the rotation by one 60 times per second.
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60) {
            rotation += 1.0f;
            prevTime = crntTime;
        }

        // Create a new matrix filled with 1's for the model, the view and proj objects
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        // Apply a rotation matrix from the Y axis to the model.
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

        // Move the world away from the camera (-0.5f downwards and -2.0f backwards)
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        // Set the clip to 45.0f radians fov, with the aspect ratio of the size of the window, without rendering objects that are closer than 0.1f and further away than 100.0f
        proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

        // Create the uniform for every objects giving their matrices
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Set the scale uniform to 1
        //glUniform1f(U_Scale, 1);

        // Bind (use) the VAO and the texture
        VAO1.Bind();
        texture.Bind();

        // Clear the error log
        GLClearError();

        // Draw the VAO elements
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int),GL_UNSIGNED_INT, nullptr);
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
    texture.Delete();

    // removes the window
    glfwDestroyWindow(window);
    // Terminate cleanly GLFW
    glfwTerminate();
    return 0;
}