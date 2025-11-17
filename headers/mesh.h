//
// Created by roufox on 11/16/25.
//

#ifndef OPENGLPROJECT_MESH_H
#define OPENGLPROJECT_MESH_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "camera.h"
#include "texture.h"

class mesh {
public:
    std::vector <vertex> vertices;
    std::vector <GLuint> indices;
    std::vector <texture> textures;

    VAO VAO1;

    mesh(std::vector <vertex>& vertices, std::vector <GLuint>& indices, std::vector <texture>& textures);
    void draw(Shader& shader, camera& camera);
};


#endif //OPENGLPROJECT_MESH_H