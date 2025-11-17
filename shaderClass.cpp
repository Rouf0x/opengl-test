//
// Created by roufox on 10/28/25.
//

#include "headers/shaderClass.h"

// Function that returns the contents of a file
std::string get_file_contents(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "File could not be opened: " << filename << std::endl;
        return "";
    }
    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

// Create a new shader object
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
    // Get the vertex and fragment shaders source code
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    // Check for compilation errors
    checkCompilationErrors(vertexShader, "Vertex Shader");

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // Check for compilation errors
    checkCompilationErrors(fragmentShader, "Fragment Shader");

    // Create the shader program by linking the shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // Check for compilation errors
    checkCompilationErrors(ID, "Program Link");

    // Delete shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::checkCompilationErrors(unsigned int shader, const char* type) {
    int success;
    char infoLog[512];

    if (std::string(type) != "Program Link") { // shader
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << type << ": compilation failed!\n" << infoLog << std::endl;
        }
    } else { // program
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            std::cout << type << ": linking failed!\n" << infoLog << std::endl;
        }
    }
}


void Shader::Activate() const {
    glUseProgram(ID);
}

void Shader::Delete() const {
    glDeleteProgram(ID);
}
