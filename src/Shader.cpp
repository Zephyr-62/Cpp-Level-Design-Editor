#include "Shader.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string const vertexSrc = readFile(vertexPath);
    std::string const fragmentSrc = readFile(fragmentPath);

    unsigned int const vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    unsigned int const fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    m_programId = glCreateProgram();
    glAttachShader(m_programId, vertexShader);
    glAttachShader(m_programId, fragmentShader);
    glLinkProgram(m_programId);

    int success;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_programId, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << '\n';
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(m_programId);
}

void Shader::use() const {
    glUseProgram(m_programId);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    int const location = glGetUniformLocation(m_programId, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

// ############ ############### ############ 
// ############ PRIVATE METHODS ############ 
// ############ ############### ############ 

std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << '\n';
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int const shader = glCreateShader(type);
    char const* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << '\n';
    }
    return shader;
}