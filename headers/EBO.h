//
// Created by roufox on 10/28/25.
//

#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>

class EBO {
    public:
        GLuint ID;
        EBO(GLuint* indices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();
};


#endif //EBO_H