//
// Created by roufox on 10/28/25.
//

#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <glm/glm.hpp>

struct vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};

class VBO {
    public:
        GLuint ID{};
        VBO(std::vector<vertex>& vertices);

        void Bind();
        void Unbind();
        void Delete();
};

#endif //VBO_H