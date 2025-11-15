//
// Created by roufox on 10/30/25.
//

#ifndef OPENGLPROJECT_CAMERA_H
#define OPENGLPROJECT_CAMERA_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>


#include "shaderClass.h"

class camera {
    public:
        glm::vec3 position{};
        glm::vec3 orientation{};
        glm::vec3 up{};
        glm::mat4 cameraMatrix = glm::mat4(1.0f);

        bool firstClick = true;

        int width, height;

        float speed = 0.05f;
        float sensitivity = 100.0f;

        camera(int width, int height, glm::vec3 position);

        void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
        void matrix(const Shader& shader, const char* uniform);

        void inputs(GLFWwindow* window);
        void updateViewportSize(int newWidth, int newHeight);
};


#endif //OPENGLPROJECT_CAMERA_H