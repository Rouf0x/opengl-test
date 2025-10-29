//
// Created by roufox on 10/29/25.
//

#include "headers/texture.h"
#include "stb/stb_image.h"

texture::texture(const char* textureImagePath,GLenum textureType, GLenum format, GLenum textureUnit) {
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* bytes = stbi_load(textureImagePath, &widthImg, &heightImg, &numColCh, 0);

    glGenTextures(1, &ID);
    glActiveTexture(textureUnit);
    glBindTexture(textureType, ID);

    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(textureType, 0, GL_RGBA, widthImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(textureType);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE0, 0);
}

void texture::GenerateTex(Shader shader, const char* uniformValue, GLuint unit) {
    GLint tex0Uni = glGetUniformLocation(shader.ID, uniformValue);
    shader.Activate();
    glUniform1i(tex0Uni, 0);
}

void texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::Delete() const {
    glDeleteTextures(1, &ID);
}
