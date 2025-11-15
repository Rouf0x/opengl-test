//
// Created by roufox on 10/29/25.
//

#include "headers/texture.h"
#include "stb/stb_image.h"

texture::texture(const char* textureImagePath, const GLenum textureType, const GLuint slot, const GLenum format, const GLenum pixelType)
{
    // Load the image, storing the image width, height and number of color channel
    // Flip the image vertically to read it correctly
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* bytes = stbi_load(textureImagePath, &widthImg, &heightImg, &numColCh, 0);

    // If not bytes something bad happend
    if (!bytes) {
        throw std::runtime_error(std::string("Failed to load texture: ") + textureImagePath);
    }

    // Store the texture type and unit
    this->textureType = textureType;
    this->unit = slot;

    // Generate the texture, activate and bind to it
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

void texture::GenerateTex(Shader& shader, const char* uniformName, const GLint unit)
{
    // Bind to the given shader
    shader.Activate();
    // Get the texture sampler2D uniform value location
    const GLint texUniformLoc = glGetUniformLocation(shader.ID, uniformName);
    // Set the texture uniform location to the texture unit
    glUniform1i(texUniformLoc, unit);
}

// Bind to the texture
void texture::Bind() const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(textureType, ID);
}

// Unbind to the texture
void texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Delete the texture
void texture::Delete() const
{
    glDeleteTextures(1, &ID);
}
