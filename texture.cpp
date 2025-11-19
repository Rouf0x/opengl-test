#include "headers/texture.h"
#include "stb/stb_image.h"

// Create a texture from an image file
texture::texture(const char* textureImagePath, const char* textureType, const GLuint slot, const GLenum format, const GLenum pixelType) {
    int widthImg, heightImg, numColCh;

    // Flip the image vertically for OpenGL
    //stbi_set_flip_vertically_on_load(1);
    unsigned char* bytes = stbi_load(textureImagePath, &widthImg, &heightImg, &numColCh, 0);

    if (!bytes) {
        throw std::runtime_error(std::string("Failed to load texture: ") + textureImagePath);
    }

    // Store texture type and unit
    this->textureType = textureType;
    this->unit = slot;

    // Generate and bind the texture
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Set texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Load the texture into OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free image memory and unbind
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Assign the texture to a shader uniform
void texture::texUnit(Shader& shader, const char* uniformName, const GLint unit) {
    shader.Activate();
    const GLint texUniformLoc = glGetUniformLocation(shader.ID, uniformName);
    glUniform1i(texUniformLoc, unit);
}

// Bind the texture
void texture::Bind() const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

// Unbind the texture
void texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Delete the texture
void texture::Delete() const {
    glDeleteTextures(1, &ID);
}
