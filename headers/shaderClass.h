//
// Created by roufox on 10/28/25.
//

#ifndef SHADERCLASS_H
#define SHADERCLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string get_file_contents(const char *file_path);

class Shader {
    public:
        GLuint ID;

        void checkCompilationErrors(unsigned int shader, const char* type);
        Shader(const char* vertexFile, const char* fragmentFile);

    void Activate();
    void Delete();
};

#endif //SHADERCLASS_H
