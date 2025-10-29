//
// Created by roufox on 10/29/25.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include "shaderClass.h"

class texture {
    public:
        GLuint ID{};
        texture(const char* textureImagePath,GLenum textureType, GLenum format, GLenum textureUnit);

        void Bind() const;
        static void Unbind();
        void Delete() const;
        static void GenerateTex(Shader shader, const char* uniformValue, GLuint unit);
};


#endif //TEXTURE_H