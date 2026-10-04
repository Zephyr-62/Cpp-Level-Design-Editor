#pragma once

#include "resources/Resource.hpp"
#include "resources/Shader.hpp"

#include <glm/glm.hpp>
#include <unordered_map>
#include <variant>
#include <memory>

using MaterialPropertyValue = std::variant<float, glm::vec2, glm::vec3, glm::vec4>;

class Material : public Resource {
    public:
    Material(std::string id, std::shared_ptr<Shader> shader);
    
    MaterialPropertyValue getPropertyValue(const std::string& propertyName);
    void set(const std::string& propertyName, const MaterialPropertyValue& value){
        m_properties[propertyName] = value;
    }

    const std::unordered_map<std::string, MaterialPropertyValue>& properties() const { return m_properties; }
    std::shared_ptr<Shader> getShader() const { return m_shader; }  
    void setShader(std::shared_ptr<Shader> shader) { 
        m_shader = shader;
        syncShaderProperties();
    }

    virtual bool drawableOnInspector() const override { return true; }
    virtual const char* Inspectable::inspectorName() const override { return "Material"; }
    virtual void drawInspector(ApplicationContext& context) override;


private:
    std::shared_ptr<Shader> m_shader;
    std::unordered_map<std::string, MaterialPropertyValue> m_properties;

    void syncShaderProperties();
};