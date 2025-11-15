//
// Created by roufox on 10/29/25.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include "shaderClass.h"

class texture {
public:
    GLuint ID{};             // Texture ID
    GLenum textureType{};    // Texture target type (e.g., GL_TEXTURE_2D)
    GLuint unit{};           // Texture unit index (slot)

    texture(const char* textureImagePath, GLenum textureType, GLuint slot, GLenum format, GLenum pixelType);

    void Bind() const;
    static void Unbind();
    void Delete() const;
    static void GenerateTex(Shader& shader, const char* uniformName, GLint unit) ;
};

#endif //TEXTURE_H
