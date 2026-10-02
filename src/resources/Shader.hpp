#pragma once

#include "resources/Resource.hpp"

#include "core/GLCommon.hpp"
#include <string>
#include <vector>
#include <glm/glm.hpp>

enum class UniformType { Float, Vec2, Vec3, Vec4, Mat4, Unknown };

struct UniformInfo {
    std::string name;
    UniformType type;
    bool isArray;
    unsigned int arraySize;
};

//Shaders cant be copied.
class Shader : public Resource {
    public:
        Shader(std::string id, const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&&) = default;
        Shader& operator=(Shader&&) = default;
        
        void use() const;
        std::vector<UniformInfo> retrieveUniforms() const;
        
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

        const char* inspectorName() const override { return "Shader";}
        
    private:
        unsigned int m_programId;

        UniformType toUniformType(GLenum glType) const;
        bool isEngineUniform(const std::string& name) const;

        static std::string readFile(const std::string& path);
        static unsigned int compileShader(unsigned int type, const std::string& source);
};