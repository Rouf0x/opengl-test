//
// Created by roufox on 10/28/25.
//

#ifndef EBO_H
#define EBO_H

#include <vector>
#include <glad/glad.h>

class EBO {
    public:
        GLuint ID;
        EBO(std::vector<GLuint>& indices);

        void Bind();
        void Unbind();
        void Delete();
};


#endif //EBO_H