//
// Created by roufox on 10/28/25.
//

#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
    public:
        GLuint ID;
        VAO();

        void LinkAttrib(VBO& VBO, GLuint layout, GLint numComponents,GLenum type, GLsizeiptr size, void* offset);
        void Bind();
        void Unbind();
        void Delete();
};


#endif //VAO_H