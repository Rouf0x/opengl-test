#include "headers/VBO.h"
#include <iostream>
#include <ostream>

// Generate a VBO from vertex data
VBO::VBO(std::vector<vertex>& vertices) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
}

// Bind the VBO
void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbind the VBO
void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Delete the VBO
void VBO::Delete() {
    glDeleteBuffers(1, &ID);
}
