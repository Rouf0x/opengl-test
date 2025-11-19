//
// Created by roufox on 11/16/25.
//

#include "mesh.h"

mesh::mesh(std::vector<vertex>& vertices,
           std::vector<GLuint>& indices,
           std::vector<texture>& textures)
{
    // Copy the mesh data into the class members
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // Bind the VAO so we can set up the vertex attributes
    VAO1.Bind();

    // Create a VBO with the vertex data
    VBO VBO1(vertices);

    // Create an EBO with the index data
    EBO EBO1(indices);

    // Link vertex attributes to the VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(vertex), nullptr);                  // Position
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, sizeof(vertex), (void*)(3 * sizeof(float))); // Normal
    VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, sizeof(vertex), (void*)(6 * sizeof(float))); // Tangent
    VAO1.LinkAttrib(VBO1, 3, 2, GL_FLOAT, sizeof(vertex), (void*)(9 * sizeof(float))); // Texture coordinates

    // Unbind everything to avoid accidentally modifying them later
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
}

void mesh::draw(Shader& shader, camera& camera)
{
    shader.Activate();
    VAO1.Bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    if (textures.empty()) {
        texture defaultDiffuse(nullptr, "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE);
        texture defaultSpecular(nullptr, "specular", 1, GL_RGBA, GL_UNSIGNED_BYTE);

        // Bind global default textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, defaultDiffuse.ID);
        glUniform1i(glGetUniformLocation(shader.ID, "diffuse0"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, defaultSpecular.ID);
        glUniform1i(glGetUniformLocation(shader.ID, "specular0"), 1);
    } else {
        // Bind each mesh texture
        for (unsigned int i = 0; i < textures.size(); i++) {
            std::string num;
            std::string type = textures[i].textureType;

            if (type == "diffuse")
                num = std::to_string(numDiffuse++);
            else if (type == "specular")
                num = std::to_string(numSpecular++);

            textures[i].texUnit(shader, (type + num).c_str(), i);
            textures[i].Bind();
        }
    }

    // Pass camera info
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"),
                camera.position.x, camera.position.y, camera.position.z);
    camera.matrix(shader, "camMatrix");

    // Draw the mesh
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    VAO1.Unbind();
}

