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
static void GLCheckError() {
    while (const GLenum Error = glGetError()) {
        std::cout << "OpenGL Error: " << Error << std::endl;
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
    GLfloat vertices[] =
    { //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
         0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
         0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

         0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
    };

    // Indices for pyramid's vertices order
    GLuint indices[] =
    {
        0, 1, 2, // Bottom side
        0, 2, 3, // Bottom side
        4, 6, 5, // Left side
        7, 9, 8, // Non-facing side
        10, 12, 11, // Right side
        13, 15, 14 // Facing side
    };

    // Vertices position data for the light source
    GLfloat light_source_cube_vertices[] = {
        // Positions
        // Front face
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // Back face
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    // Light source's indices
    GLuint light_source_cube_indices[] = {
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

    // Create a new Shader Program with the vertex and fragment shaders
    Shader shaderProgram("../resources/shaders/default_vertex.glsl", "../resources/shaders/default_fragment.glsl");

    // Create a VAO and bind it
    VAO VAO1;
    VAO1.Bind();

    // Create a VBO and an EBO, passing in the vertices and indices data, as well as their byte size
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    // Link the VBO to the VAO1
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), nullptr);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), reinterpret_cast<void *>(8 * sizeof(float)));

    // Unbind (stop using) to prevent accidental changes to the VAO, VBO or EBO
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Create a new light shader object
    Shader lightShader("../resources/shaders/light_vertex.glsl", "../resources/shaders/light_fragment.glsl");

    // Create a light VAO and to bind it
    VAO lightVAO;
    lightVAO.Bind();

    // Create a VBO and an EBO for the light VAO, passing in the vertices and indices data, as well as their byte size
    VBO lightVBO(light_source_cube_vertices, sizeof(light_source_cube_vertices));
    EBO lightEBO(light_source_cube_indices, sizeof(light_source_cube_indices));

    // Links the light VBO data to the light VAO
    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), nullptr);

    // Unbind to prevent accidental changes to the light VAO, VBO or EBO
    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();

    // Texturing
    // Set texture path
    const char* texturePath = "../diamondplate01.jpg";
    // Create a new texture object
    texture diamondplateTexture(texturePath, GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
    // Generate a texture for the diamondplate texture
    diamondplateTexture.GenerateTex(shaderProgram, "tex0", 0);

    // Create a new texture object
    texture diamondplateSpecular("../diamondplate01_spec.jpg", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
    // Generate a texture for the diamondplate's specular texture
    diamondplateSpecular.GenerateTex(shaderProgram, "tex1", 1);

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

    // Loop that stops once the window should close.
    while (!glfwWindowShouldClose(window)) {
        // Sets the background color to rgba float values
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        // Clear the color buffer bit and the depth buffer bit
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Binds to the light shader.
        lightShader.Activate();

        // Creates the matrix for the light
        //auto lightModel = glm::mat4(1.0f);
        // Set the position of the light to the camera's position
        //lightModel = glm::translate(lightModel, lightPos);
        //lightModel = glm::scale(lightModel, glm::vec3(0.05f, 0.05f, 0.05f));
        //int lightLoc = glGetUniformLocation(lightShader.ID, "model");

        glm::vec3 lightPos = camera.position; //glm::vec3(0.6f, 0.7f, 0.6f);
        glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        //glUniformMatrix4fv(lightLoc, 1, GL_FALSE, glm::value_ptr(lightModel));

        // Use the shader program
        shaderProgram.Activate();

        // Set all uniforms to their respective variables
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lookVector"), camera.orientation.x, camera.orientation.y, camera.orientation.z);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);

        glUniform1i(glGetUniformLocation(shaderProgram.ID, "lightType"), lightingType);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "specularIntensity"), specularIntensity);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "ambientIntensity"), ambientIntensity);

        // Update the camera matrix and handle inputs
        camera.updateMatrix(FOV, 0.1f, 1000.0f);
        camera.matrix(shaderProgram, "camMatrix");
        camera.inputs(window);

        // Create a matrix for the pyramid model
        auto model = glm::mat4(1.0f);
        // Increase the rotation by the rotation speed
        rotation += rotation_speed;
        // Rotate the model to the rotation
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

        // Send the pyramid's model matrix and the light color to the shader program
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

        // Bind to the main VAO and texture
        VAO1.Bind();
        diamondplateTexture.Bind();
        diamondplateSpecular.Bind();

        // Clear the error log
        GLClearError();

        // Draw the main VAO elements
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int),GL_UNSIGNED_INT, nullptr);

        // Binds to the light shader, apply the camera matrix (by sending it to the light shader) and binds to the light VAO
        lightShader.Activate();
        camera.matrix(lightShader, "camMatrix");
        lightVAO.Bind();
        // Draw the light VAO elements
        glDrawElements(GL_TRIANGLES, sizeof(light_source_cube_indices) / sizeof(int), GL_UNSIGNED_INT, nullptr);

        // Print errors if we encountered any
        GLCheckError();

        // Set the FPS by incrementing the frame and diving it by the difference of the current time and the begin time
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
        ImGui::ColorPicker4("Light Color", (float*)&lightColor);
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

        // Swap the back buffer to the front buffer to correctly display the color.
        glfwSwapBuffers(window);

        // Responds to GLFW events.
        glfwPollEvents();
    }

    // Cleans all the ImGui stuff
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleans all objects (VAO, VBO, EBO, ShaderProgram...)
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    lightVAO.Delete();
    lightVBO.Delete();
    lightEBO.Delete();
    shaderProgram.Delete();
    diamondplateTexture.Delete();
    diamondplateSpecular.Delete();
    shaderProgram.Delete();
    lightShader.Delete();

    // Destroy the window
    glfwDestroyWindow(window);
    // Terminate cleanly GLFW
    glfwTerminate();
    return 0;
}