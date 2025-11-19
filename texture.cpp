#include "headers/texture.h"
#include "stb/stb_image.h"


// Create a texture from an image file
texture::texture(const char* textureImagePath, const char* textureType, const GLuint slot, const GLenum format, const GLenum pixelType) {
    int widthImg, heightImg, numColCh;

    unsigned char* bytes = nullptr;
    if (textureImagePath) {
        // Attempt to load image
        bytes = stbi_load(textureImagePath, &widthImg, &heightImg, &numColCh, 0);
        if (!bytes) {
            std::cerr << "Failed to load texture: " << textureImagePath << " → using default 1x1 texture\n";
        }
    }

    // If no image or load failed, create a default 1x1 texture
    unsigned char defaultPixel[4] = {255, 255, 255, 255}; // White by default
    if (!bytes) {
        widthImg = heightImg = 1;
        bytes = defaultPixel;
    }

    this->textureType = textureType;
    this->unit = slot;

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Filtering and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Upload texture to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (textureImagePath) stbi_image_free(bytes); // Only free if loaded from file
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
