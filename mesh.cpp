//
// Created by roufox on 11/16/25.
//

#include "mesh.h"

mesh::mesh(std::vector <vertex>& vertices, std::vector <GLuint>& indices, std::vector <texture>& textures) {
    mesh::vertices = vertices;
    mesh::indices = indices;
    mesh::textures = textures;

    VAO1.Bind();

    // Create a VBO and an EBO, passing in the vertices and indices data, as well as their byte size
    VBO VBO1(vertices);
    EBO EBO1(indices);

    // Link the VBO to the VAO1
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(vertex), nullptr);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, sizeof(vertex), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, sizeof(vertex), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 2, GL_FLOAT, sizeof(vertex), (void*)(9 * sizeof(float)));

    // Unbind (stop using) to prevent accidental changes to the VAO, VBO or EBO
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
}

void mesh::draw(Shader& shader, camera& camera) {
    shader.Activate();
    VAO1.Bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string num;
        std::string type = textures[i].textureType;

        if (type == "diffuse") {
            num = std::to_string(numDiffuse++);
        }
        else if (type == "specular") {
            num = std::to_string(numSpecular++);
        }

        textures[i].texUnit(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }

    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
    camera.matrix(shader, "camMatrix");

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}