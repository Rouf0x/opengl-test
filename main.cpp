//#include <stdio.h>
/*#include <iostream>

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
#include "headers/camera.h"*/

#include "mesh.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// Initialize the base window size
int scr_width = 1366;
int scr_height = 768;

// Create the camera object
camera camera(scr_width, scr_height, glm::vec3(0.0f, 0.0f, 2.0f));

// Function that clears OpenGL errors
static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

// Function that prints out OpenGL errors
static void GLCheckError(const char* loc = "") {
    while (GLenum Error = glGetError()) {
        std::cout << "OpenGL Error: " << Error;
        if (loc[0] != '\0') std::cout << " at " << loc;
        std::cout << std::endl;
    }
}

// Adjust the viewport and the aspect ratio when the window is resized
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

    // Pyramid vertices data
    vertex pyramid_vertices[] = {
        // Bottom side
        vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},
        vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 5.0f)},
        vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 5.0f)},
        vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},

        // Left Side
        vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(-0.8f,  0.5f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},
        vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(-0.8f,  0.5f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},
        vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(-0.8f,  0.5f, 0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f)},

        // Non-facing side
        vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3( 0.0f,  0.5f,-0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},
        vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3( 0.0f,  0.5f,-0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},
        vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3( 0.0f,  0.5f,-0.8f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f)},

        // Right side
        vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3( 0.8f,  0.5f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},
        vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3( 0.8f,  0.5f, 0.0f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},
        vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3( 0.8f,  0.5f, 0.0f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f)},

        // Facing side
        vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3( 0.0f,  0.5f, 0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(5.0f, 0.0f)},
        vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3( 0.0f,  0.5f, 0.8f), glm::vec3(0.83f, 0.70f, 0.44f), glm::vec2(0.0f, 0.0f)},
        vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3( 0.0f,  0.5f, 0.8f), glm::vec3(0.92f, 0.86f, 0.76f), glm::vec2(2.5f, 5.0f)}
    };

    // Indices for pyramid's vertices order
    GLuint pyramid_indices[] =
    {
        0, 1, 2, // Bottom side
        0, 2, 3, // Bottom side
        4, 6, 5, // Left side
        7, 9, 8, // Non-facing side
        10, 12, 11, // Right side
        13, 15, 14 // Facing side
    };

    // Vertices position data for the light source
    vertex light_cube_vertices[] = {
        // Front face
        vertex{glm::vec3(-0.5f, -0.5f,  0.5f)}, // Color set to white
        vertex{glm::vec3( 0.5f, -0.5f,  0.5f)},
        vertex{glm::vec3( 0.5f,  0.5f,  0.5f)},
        vertex{glm::vec3(-0.5f,  0.5f,  0.5f)},

        // Back face
        vertex{glm::vec3(-0.5f, -0.5f, -0.5f)},
        vertex{glm::vec3( 0.5f, -0.5f, -0.5f)},
        vertex{glm::vec3( 0.5f,  0.5f, -0.5f)},
        vertex{glm::vec3(-0.5f,  0.5f, -0.5f)}
    };

    // Light source's indices
    GLuint light_cube_indices[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,

        // Right face
        1, 5, 6,
        6, 2, 1,

        // Back face
        7, 6, 5,
        5, 4, 7,

        // Left face
        4, 0, 3,
        3, 7, 4,

        // Bottom face
        4, 5, 1,
        1, 0, 4,

        // Top face
        3, 2, 6,
        6, 7, 3
    };


    // Create a new window element
    GLFWwindow* window = glfwCreateWindow(scr_width,scr_height, "OpenGL Application", nullptr, nullptr);
    // If window doesn't exist error out.
    if (!window) {
        std::cout << "Failed to initialize GLFW Window!";
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
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Set up the start time, as well as a frame counter variable to count FPS
    const double begin_time = glfwGetTime();
    int frames = 0;

    texture textures[] {
        texture("../diamondplate01.jpg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
        texture("../diamondplate01_spec.jpg", "diffuse", 1, GL_RED, GL_UNSIGNED_BYTE)
    };

    // Create a new Shader Program with the vertex and fragment shaders
    Shader shaderProgram("../resources/shaders/default_vertex.glsl", "../resources/shaders/default_fragment.glsl");
    std::vector <vertex> pyramid_verts(pyramid_vertices, pyramid_vertices + sizeof(pyramid_vertices) / sizeof(vertex));
    std::vector <GLuint> pyramid_ind(pyramid_indices, pyramid_indices + sizeof(pyramid_indices) / sizeof(GLuint));
    std::vector <texture> tex(textures, textures + sizeof(textures) / sizeof(texture));
    mesh pyramid(pyramid_verts, pyramid_ind, tex);

    // Create a new light shader object
    Shader lightShader("../resources/shaders/light_vertex.glsl", "../resources/shaders/light_fragment.glsl");
    std::vector <vertex> light_verts(light_cube_vertices, light_cube_vertices + sizeof(light_cube_vertices) / sizeof(vertex));
    std::vector <GLuint> light_ind(light_cube_indices, light_cube_indices + sizeof(light_cube_indices) / sizeof(GLuint));
    mesh light(light_verts, light_ind, tex);

    // Enable Depth_Test to avoid faces behind other ones from being rendered
    glEnable(GL_DEPTH_TEST);

    // Initializes the pyramids, rotation, rotation speed and the camera's FOV variables
    float rotation = 0.0f;
    float rotation_speed = 0.1f;
    float FOV = 60.0f;

    // Initializes the specular, ambient and other light variables.
    int lightingType = 1;
    float specularIntensity = 5.0f;
    float ambientIntensity = 0.1f;

    // Set the light color to solid white
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    GLCheckError("initialization");

    // Loop that stops once the window should close.
    while (!glfwWindowShouldClose(window)) {
        GLClearError();

        // Specify the color of the background
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handles camera inputs
        camera.inputs(window);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.updateMatrix(FOV, 0.1f, 100.0f);

        shaderProgram.Activate();

        // Create and send the model matrix for the pyramid
        glm::mat4 pyramidModel = glm::mat4(1.0f);
        // Update the rotation
        rotation += rotation_speed;
        pyramidModel = glm::rotate(pyramidModel, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));

        glm::vec3 lightPos = camera.position;
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lookVector"), camera.orientation.x, camera.orientation.y, camera.orientation.z);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "lightType"), lightingType);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "specularIntensity"), specularIntensity);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "ambientIntensity"), ambientIntensity);

        lightShader.Activate();
        glUniform3f(glGetUniformLocation(lightShader.ID, "lookVector"), camera.orientation.x, camera.orientation.y, camera.orientation.z);
        auto lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.05f));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
        glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

        pyramid.draw(shaderProgram, camera);
        light.draw(lightShader, camera);

        // Set the FPS by incrementing the frame and diving it by the difference of the current time and the start time
        double fps = ++frames / glfwGetTime() - begin_time;

        // Create a new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);

        // Add lots of settings to mess around with during runtime
        //ImGui::SetNextWindowSize(ImVec2(400, 400));
        ImGui::Begin("Debug Settings");
        ImGui::Text("Current FPS: %.1f", fps);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Text("Camera Settings");
        ImGui::PushItemWidth(250.0f);
        ImGui::SliderFloat("Camera Speed", &camera.speed, 0.005f, 1.0f);
        ImGui::SliderFloat("Camera Sensitivity", &camera.sensitivity, 5.00f, 1000.0f);
        ImGui::SliderFloat("Camera FOV", &FOV, 0.001f, 180.0f);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Text("Model Settings");
        ImGui::SliderFloat("Rotation Speed", &rotation_speed, 0.0f, 10.0f);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Text("Light Settings");
        ImGui::ColorPicker4("Light Color", reinterpret_cast<float *>(&lightColor));
        ImGui::Spacing();
        ImGui::SliderFloat("Specular Intensity", &specularIntensity, 0.0f, 10.0f);
        ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, 0.0f, 1.0f);
        const char* lightingTypesArray[] = { "Point Light", "Direct Light", "Spot Light" };
        static int current_item_index = 0;
        if (ImGui::Combo("Lighting Type", &current_item_index, lightingTypesArray, IM_ARRAYSIZE(lightingTypesArray))) {
            lightingType = current_item_index + 1;
        }
        ImGui::End();

        // Render and draw the ImGui window
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();

        GLCheckError("runtime");
    }

    // Cleans all the ImGui stuff
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleans all objects (VAO, VBO, EBO, ShaderProgram...)
    shaderProgram.Delete();
    lightShader.Delete();

    // Destroy the window
    glfwDestroyWindow(window);
    // Terminate cleanly GLFW
    glfwTerminate();
    return 0;
}