#include "headers/shaderClass.h"

// Load the contents of a file and return it as a string
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

// Create a shader program from vertex and fragment shader files
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
    // Read shader source code from files
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    checkCompilationErrors(vertexShader, "Vertex Shader");

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    checkCompilationErrors(fragmentShader, "Fragment Shader");

    // Create program and link shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    checkCompilationErrors(ID, "Program Link");

    // Delete the shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Check for shader compilation or program linking errors
void Shader::checkCompilationErrors(unsigned int shader, const char* type) {
    int success;
    char infoLog[512];

    if (std::string(type) != "Program Link") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << type << " compilation failed:\n" << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            std::cout << type << " linking failed:\n" << infoLog << std::endl;
        }
    }
}

// Activate the shader program
void Shader::Activate() const {
    glUseProgram(ID);
}

// Delete the shader program
void Shader::Delete() const {
    glDeleteProgram(ID);
}
