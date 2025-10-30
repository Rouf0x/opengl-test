//#include <stdio.h>
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

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
#include "headers/camera.h"

using namespace std;

int scr_width = 1366;
int scr_height = 768;
camera camera(scr_width, scr_height, glm::vec3(0.0f, 0.0f, 2.0f));

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError() {
    while (const GLenum Error = glGetError()) {
        cout << "OpenGL Error: " << Error << endl;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    scr_width = width;
    scr_height = height;
    camera.updateViewportSize(width, height); // update camera aspect ratio
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
    GLFWwindow* window = glfwCreateWindow(scr_width,scr_height, "OpenGL Application", nullptr, nullptr);
    // If window doesn't exist error out.
    if (!window) {
        cout << "Failed to initialize GLFW Window!";
        glfwTerminate();
        return -1;
    }
    // Use the window as the current context.
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL using glad.
    gladLoadGL();

    // Sets the OpenGL Viewport from 0,0 to 800, 600
    glViewport(0,0,scr_width,scr_height);

    // Initialize IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // FPS counter stuff
    const double begin_time = glfwGetTime();
    int frames = 0;

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
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));

    // Unbind (stop using) to prevent accidental changes to the VAO, VBO or EBO
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Create a new texture

    const char* texturePath = "../bricks01.jpg";
    const texture texture(texturePath, GL_TEXTURE_2D, GL_RGB, GL_TEXTURE0);
    // Generate the texture object
    texture::GenerateTex(shaderProgram, "tex0", 0);

    // Enable Depth_Test to avoid faces behind other ones from being rendered
    glEnable(GL_DEPTH_TEST);

    float rotation = 0.0f;
    float rotation_speed = 0.05f;
    float FOV = 45.0f;

    // Loop that stops once the window should close.
    while (!glfwWindowShouldClose(window)) {
        // Sets the background color to rgba float values
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        // Clear the color buffer bit and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program
        shaderProgram.Activate();

        camera.matrix(FOV, 0.1f, 100.0f, shaderProgram, "camMatrix");
        camera.inputs(window);

        // Pyramid matrix
        auto model = glm::mat4(1.0f);

        rotation += rotation_speed;
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Bind (use) the VAO and the texture
        VAO1.Bind();
        //texture.Bind();

        // Clear the error log
        GLClearError();

        // Draw the VAO elements
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int),GL_UNSIGNED_INT, nullptr);
        // Print errors if we encountered any
        GLCheckError();

        // FPS counter
        double fps = ++frames / glfwGetTime() - begin_time;

        // Imgui stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(400, 400));
        ImGui::Begin("Debug Settings");
        ImGui::Text("Current FPS: %.1f", fps);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Text("Camera Settings");
        ImGui::PushItemWidth(250.0f);
        ImGui::SliderFloat("Camera Speed", &camera.speed, 0.005f, 1.0f);
        ImGui::SliderFloat("Camera Sensitivity", &camera.sensitivity, 5.00f, 1000.0f);
        ImGui::SliderFloat("Camera FOV", &FOV, 0.00f, 180.0f);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Text("Model Settings");
        ImGui::SliderFloat("Rotation Speed", &rotation_speed, 0.0f, 10.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap the back buffer to the front buffer to correctly display the color.
        glfwSwapBuffers(window);

        // Responds to GLFW events.
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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