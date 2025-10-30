//
// Created by roufox on 10/30/25.
//

#include "headers/camera.h"

camera::camera(int width, int height, glm::vec3 position) {
    camera::width = width;
    camera::height = height;
    camera::position = position;

    camera::orientation = glm::vec3(0.0f, 0.0f, -1.0f); // Looking forward
    camera::up = glm::vec3(0.0f, 1.0f, 0.0f);            // Up vector
}

void camera::matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(position, position + orientation, up);
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height, nearPlane, farPlane);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void camera::updateViewportSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
}

void camera::inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += speed * orientation;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += speed * -glm::normalize(glm::cross(orientation, glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position += -speed * orientation;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += speed * -glm::normalize(glm::cross(orientation, glm::vec3(0.0f, -1.0f, 0.0f)));
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        position += speed * up;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        position += speed * -up;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (firstClick) {
            glfwSetCursorPos(window, (width/2), (height/2));
            firstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotx = sensitivity * (float)(mouseY - (height /2)) / height;
        float roty = sensitivity * (float)(mouseX - (width/2)) / width;

        glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotx), glm::normalize(glm::cross(orientation, up)));

        if (!(glm::angle(newOrientation, up) <= glm::radians(5.0f) or glm::angle(newOrientation, up) >= glm::radians(175.0f))) {
            orientation = newOrientation;
        }

        orientation = glm::rotate(orientation, glm::radians(-roty), up); // Added semicolon
        glfwSetCursorPos(window, (width/2), (height/2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}