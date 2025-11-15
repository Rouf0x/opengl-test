//
// Created by roufox on 10/29/25.
//

#include "headers/texture.h"
#include "stb/stb_image.h"

texture::texture(const char* textureImagePath, GLenum textureType, GLuint slot, GLenum format, GLenum pixelType)
{
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* bytes = stbi_load(textureImagePath, &widthImg, &heightImg, &numColCh, 0);

    if (!bytes) {
        throw std::runtime_error(std::string("Failed to load texture: ") + textureImagePath);
    }

    this->textureType = textureType;
    this->unit = slot;

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(textureType, ID);

    // Filtering
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Wrapping
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Load texture data
    glTexImage2D(textureType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(textureType);

    // Free image data
    stbi_image_free(bytes);

    // Unbind texture
    glBindTexture(textureType, 0);
}

void texture::GenerateTex(Shader& shader, const char* uniformName, GLint unit) const
{
    shader.Activate();
    GLint texUniformLoc = glGetUniformLocation(shader.ID, uniformName);
    glUniform1i(texUniformLoc, unit);
}

void texture::Bind() const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(textureType, ID);
}

void texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::Delete() const
{
    glDeleteTextures(1, &ID);
}
