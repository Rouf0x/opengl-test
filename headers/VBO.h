//
// Created by roufox on 10/28/25.
//

#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>

class VBO {
    public:
        GLuint ID{};
        VBO(GLfloat* vertices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();
};

#endif //VBO_H