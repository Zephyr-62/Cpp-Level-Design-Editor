#include "resources/Shader.hpp"

#include "core/GLCommon.hpp"
#include "core/Constants.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <imgui.h>


Shader::Shader(std::string id, const std::string& vertexPath, const std::string& fragmentPath) : Resource(std::move(id)) {
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

std::vector<UniformInfo> Shader::retrieveUniforms() const {
    std::vector<UniformInfo> result;

    GLint count = 0;
    glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &count);

    for (GLint i = 0; i < count; ++i) {
        char nameBuf[256];
        GLsizei nameLen;
        GLint size;
        GLenum glType;
        glGetActiveUniform(m_programId, i, sizeof(nameBuf), &nameLen, &size, &glType, nameBuf);

        std::string name(nameBuf, nameLen);
        if (isEngineUniform(name)) continue;

        UniformType type = toUniformType(glType);
        if (type == UniformType::Unknown) continue;

        result.push_back({ name, type, size > 1, static_cast<unsigned int>(size) });
    }

    return result;
}

UniformType Shader::toUniformType(GLenum glType) const{
    switch (glType) {
        case GL_FLOAT:      return UniformType::Float;
        case GL_FLOAT_VEC2: return UniformType::Vec2;
        case GL_FLOAT_VEC3: return UniformType::Vec3;
        case GL_FLOAT_VEC4: return UniformType::Vec4;
        case GL_FLOAT_MAT4: return UniformType::Mat4;
        default:            return UniformType::Unknown;
    }
}

bool Shader::isEngineUniform(const std::string& name) const {
    return name.rfind("engine_", 0) == 0; // Check if the name starts with "engine_"
}

void Shader::setFloat(const std::string& name, float value) const {
    int const location = glGetUniformLocation(m_programId, name.c_str());
    if(location == -1) {
        ERROR_LOG("Warning: uniform '" + name + "' not found in shader program.", ERROR_CODE_PROPERTY_NOT_FOUND);
        return;
    }
    glUniform1f(location, value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    int const location = glGetUniformLocation(m_programId, name.c_str());
    if(location == -1) {
        ERROR_LOG("Warning: uniform '" + name + "' not found in shader program.", ERROR_CODE_PROPERTY_NOT_FOUND);
        return;
    }
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    int const location = glGetUniformLocation(m_programId, name.c_str());
    if(location == -1) {
        ERROR_LOG("Warning: uniform '" + name + "' not found in shader program.", ERROR_CODE_PROPERTY_NOT_FOUND);
        return;
    }
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    int const location = glGetUniformLocation(m_programId, name.c_str());
    if(location == -1) {
        ERROR_LOG("Warning: uniform '" + name + "' not found in shader program.", ERROR_CODE_PROPERTY_NOT_FOUND);
        return;
    }
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    int const location = glGetUniformLocation(m_programId, name.c_str());
    if(location == -1) {
        ERROR_LOG("Warning: uniform '" + name + "' not found in shader program.", ERROR_CODE_PROPERTY_NOT_FOUND);
        return;
    }
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
