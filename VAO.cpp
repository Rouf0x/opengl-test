#include "headers/VAO.h"

// Generate a new VAO
VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

// Link a VBO to the VAO with a specified layout
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

// Bind the VAO
void VAO::Bind() {
    glBindVertexArray(ID);
}

// Unbind the VAO
void VAO::Unbind() {
    glBindVertexArray(0);
}

// Delete the VAO
void VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}
