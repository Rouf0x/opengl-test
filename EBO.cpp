#include "headers/EBO.h"
#include <iostream>
#include <ostream>

// Generate an EBO from index data
EBO::EBO(std::vector<GLuint>& indices) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// Bind the EBO
void EBO::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbind the EBO
void EBO::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Delete the EBO
void EBO::Delete() {
    glDeleteBuffers(1, &ID);
}
