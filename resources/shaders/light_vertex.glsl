#version 330 core
// Import the position from the vertex data
layout (location = 0) in vec3 aPos;

// Import the camera matrix and model uniforms
uniform mat4 camMatrix;
uniform mat4 model;

void main() {
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
}