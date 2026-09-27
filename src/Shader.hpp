#pragma once

#include <string>
#include <glm/glm.hpp>

//TODO Update programId when object is copied or prevent copying altogether
class Shader {
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();

        void use() const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

    private:
        unsigned int m_programId;

        static std::string readFile(const std::string& path);
        static unsigned int compileShader(unsigned int type, const std::string& source);
};