#version 330 core

// Import vertex data
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

// Export the vertex color, texture coordinates, normal and position to the fragment shader
out vec3 currentPos;
out vec3 normal;
out vec3 color;
out vec2 textureCoordinates;

// Import uniforms
uniform mat4 camMatrix;
uniform mat4 model;
uniform float modelScale;

void main() {
    currentPos = vec3(model * modelScale * vec4(aPos, 1.0f));
    // Calculate the world position by multiplying the model's matrix by the vertex position.

    // Modify the normal to handle non-uniform scaling
    normal = mat3(transpose(inverse(model))) * aNormal;

    color = aColor;

    // Set the texture coordinates to the imported UV texture coordinates (used to export it later)
    textureCoordinates = aTex;

    gl_Position = camMatrix * vec4(currentPos, 1.0);

}