#version 330 core

// Export the fragment color
out vec4 FragColor;

// Import the uniform light color
uniform vec4 lightColor;

void main() {
    // Set the fragment color to the imported light color
    FragColor = lightColor;
}