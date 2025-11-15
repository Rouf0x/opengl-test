//
// Created by roufox on 10/30/25.
//

#include "headers/camera.h"

// Camera constructor
camera::camera(const int width, const int height, const glm::vec3 position) {
    camera::width = width;
    camera::height = height;
    camera::position = position;

    orientation = glm::vec3(0.0f, 0.0f, -1.0f); // Looking forward
    up = glm::vec3(0.0f, 1.0f, 0.0f);           // Up vector
}

// Set the cameraMatrix uniform to the camera matrix
void camera::matrix(const Shader &shader, const char *uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

// Function to update the camera's matrix (like FOV, near plane and far plane)
void camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
    // Move the world to place the camera at the position
    auto view = glm::lookAt(position, position + orientation, up);
    // Creates a projection matrix
    auto projection = glm::perspective(glm::radians(FOVdeg), static_cast<float>(width) / static_cast<float>(height), nearPlane, farPlane);

    // Multiplies the projection matrice with the view matrice to get the final cameraMatrix
    cameraMatrix = projection * view;
}

// Update the viewport size (mainly used for window resizes)
void camera::updateViewportSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
}

// Handle camera movements
void camera::inputs(GLFWwindow* window) {
    // WASD keys
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

    // If the right mouse button is clicked
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        // Hide the cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // If it's the first click then snap the cursor to the middle of the screen
        if (firstClick) {
            glfwSetCursorPos(window, (static_cast<float>(width)/2.0f), (static_cast<float>(height)/2.0f));
            firstClick = false;
        }

        // Set mouseX and mouseY to the mouse X and Y position
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Calculate the pitch and yaw rotational value
        const float rotX = sensitivity * (static_cast<float>(mouseY) - (static_cast<float>(height) / 2.0f)) / static_cast<float>(height);
        const float rotY = sensitivity * (static_cast<float>(mouseX) - (static_cast<float>(width) / 2.0f)) / static_cast<float>(width);

        // Rotate the camera to the pitch/yaw
        glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

        // If the angle is between 5 and 175 degrees, update the camera to the new orientation
        if (!(glm::angle(newOrientation, up) <= glm::radians(5.0f) or glm::angle(newOrientation, up) >= glm::radians(175.0f))) {
            orientation = newOrientation;
        }

        // Rotate to the yaw
        orientation = glm::rotate(orientation, glm::radians(-rotY), up);

        // Reset the cursor to the middle of the screen
        glfwSetCursorPos(window, (static_cast<float>(width)/2), (static_cast<float>(height)/2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}