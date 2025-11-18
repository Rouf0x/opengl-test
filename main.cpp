#include "mesh.h"
#include "model.h"

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
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    scr_width = width;
    scr_height = height;
    camera.updateViewportSize(width, height); // update camera aspect ratio
}

int main() {
    // -------------------------- GLFW Initialization --------------------------
    glfwInit();

    // Tells GLFW to use version 3.3 of OpenGL, as well as the "core" profile.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // -------------------------- Vertex Data --------------------------
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

    // -------------------------- GLFW Window Creation --------------------------
    GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "OpenGL Application", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to initialize GLFW Window!";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // -------------------------- OpenGL Loader --------------------------
    gladLoadGL();
    glViewport(0, 0, scr_width, scr_height);

    // -------------------------- IMGUI Initialization --------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // -------------------------- Timing Variables --------------------------
    const double begin_time = glfwGetTime();
    int frames = 0;

    // -------------------------- Shader and Model Setup --------------------------
    Shader shaderProgram("../resources/shaders/default_vertex.glsl",
                         "../resources/shaders/default_fragment.glsl");

    // Load your 3D model (e.g., from an OBJ or GLTF file)
    // Adjust the path to where your model file is located!
    Model loadedModel("../resources/models/sponza_palace/sponza.obj");

    // Light shader and mesh
    Shader lightShader("../resources/shaders/light_vertex.glsl", "../resources/shaders/light_fragment.glsl");
    std::vector<vertex> light_verts(light_cube_vertices, light_cube_vertices + sizeof(light_cube_vertices)/sizeof(vertex));
    std::vector<GLuint> light_ind(light_cube_indices, light_cube_indices + sizeof(light_cube_indices)/sizeof(GLuint));
    std::vector<texture> tex;
    mesh light(light_verts, light_ind, tex);

    // Enable Depth_Test to avoid faces behind other ones from being rendered
    glEnable(GL_DEPTH_TEST);

    // -------------------------- Model & Light Variables --------------------------
    float rotation = 0.0f;
    float rotation_speed = 0.0f;
    float FOV = 60.0f;

    int lightingType = 1;
    float specularIntensity = 5.0f;
    float ambientIntensity = 0.1f;

    auto lightColor = glm::vec4(1.0f); // Set the light color to solid white

    GLCheckError("initialization");

    // -------------------------- Render Loop --------------------------
    while (!glfwWindowShouldClose(window)) {
        GLClearError();

        // Clear screen
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handle camera inputs and update matrix
        camera.inputs(window);
        camera.updateMatrix(FOV, 0.1f, 10000.0f);

        // -------------------------- Shader Uniforms --------------------------
        shaderProgram.Activate();
        auto pyramidModel = glm::mat4(1.0f);
        rotation += rotation_speed;
        pyramidModel = glm::rotate(pyramidModel, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));

        glm::vec3 lightPos = camera.position;
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lookVector"), camera.orientation.x, camera.orientation.y, camera.orientation.z);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform4fv(glGetUniformLocation(shaderProgram.ID, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(shaderProgram.ID, "camPos"), 1, glm::value_ptr(camera.position));
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "lightType"), lightingType);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "specularIntensity"), specularIntensity);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "ambientIntensity"), ambientIntensity);

        // -------------------------- Light Shader --------------------------
        lightShader.Activate();
        glUniform3f(glGetUniformLocation(lightShader.ID, "lookVector"), camera.orientation.x, camera.orientation.y, camera.orientation.z);
        glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.05f));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
        glUniform4fv(glGetUniformLocation(lightShader.ID, "lightColor"), 1, glm::value_ptr(lightColor));

        // Draw model and light
        loadedModel.draw(shaderProgram, camera);
        light.draw(lightShader, camera);

        // -------------------------- FPS Calculation --------------------------
        double fps = ++frames / (glfwGetTime() - begin_time);

        // -------------------------- ImGui Frame --------------------------
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowCollapsed(true, ImGuiCond_Once);

        ImGui::Begin("Debug Settings");
        ImGui::Text("Current FPS: %.1f", fps);
        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        ImGui::Text("Camera Settings");
        ImGui::PushItemWidth(250.0f);
        ImGui::SliderFloat("Camera Speed", &camera.speed, 0.005f, 10.0f);
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

        // -------------------------- Render ImGui --------------------------
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // -------------------------- Swap Buffers & Poll Events --------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        GLCheckError("runtime");
    }

    // -------------------------- Cleanup --------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    shaderProgram.Delete();
    lightShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}