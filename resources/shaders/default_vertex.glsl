#version 330 core

// Import vertex data
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

// Export the vertex color, texture coordinates, normal and position to the fragment shader
out vec3 color;
out vec2 textureCoordinates;
out vec3 normal;
out vec3 currentPos;

// Import uniforms
uniform mat4 camMatrix;
uniform mat4 model;

void main() {
    currentPos = vec3(model * vec4(aPos, 1.0f));
    // Calculate the world position by multiplying the model's matrix by the vertex position.
    vec4 worldPos = model * vec4(aPos, 1.0);

    // Set the final vertex position in clip space by the view matrix multiplied by the world position
    gl_Position = camMatrix * worldPos;
    color = aColor;
    // Set the texture coordinates to the imported UV texture coordinates (used to export it later)
    textureCoordinates = aTex;
    // Modify the normal to handle non-uniform scaling
    normal = mat3(transpose(inverse(model))) * aNormal;
}