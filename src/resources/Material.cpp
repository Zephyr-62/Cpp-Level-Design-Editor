#include "resources/Material.hpp"
#include "core/Constants.hpp"
#include "editor/EditorUtils.hpp"

#include <imgui.h>

Material::Material(std::string id, std::shared_ptr<Shader> shader) : Resource(std::move(id)), m_shader(shader) {
    syncShaderProperties();
}

void Material::syncShaderProperties(){
    std::unordered_map<std::string, MaterialPropertyValue> newProperties;
    
    for (const auto& uniform : m_shader->retrieveUniforms()) {
        if (m_properties.contains(uniform.name)){
            newProperties[uniform.name] = m_properties[uniform.name];
            continue; // Preserve already existing values in the property bag, while skipping properties not present in the new shader
        }

        switch (uniform.type) {
            case UniformType::Float: newProperties[uniform.name] = 0.0f; break;
            case UniformType::Vec2: newProperties[uniform.name] = glm::vec2(0.0f, 0.0f); break;
            case UniformType::Vec3: newProperties[uniform.name] = glm::vec3(0.0f, 0.0f, 0.0f); break;
            case UniformType::Vec4: newProperties[uniform.name] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); break;
            default: break; // Ignore unsupported types for now.
        }
    }
    m_properties = newProperties;
}

MaterialPropertyValue Material::getPropertyValue(const std::string& propertyName) {
    auto it = m_properties.find(propertyName);
    if (it != m_properties.end()) {
        return it->second;
    } else {
        ERROR_LOG("Property not found in material " + id() + ": " + propertyName, ERROR_CODE_PROPERTY_NOT_FOUND);

        return 0.0f;
    }
}

void Material::drawInspector(ApplicationContext& context) {

    // TODO beautify property names for inspector rendering!! (editor/EditorUtils.hpp)
    for (auto& [propertyName, value] : m_properties) {
        if(std::holds_alternative<float>(value)){
            ImGui::DragFloat(propertyName.c_str(), &std::get<float>(value), 0.1f);
        } else if (std::holds_alternative<glm::vec2>(value)){
            ImGui::DragFloat2(propertyName.c_str(), &(std::get<glm::vec2>(value).x), 0.01f);
        } else if (std::holds_alternative<glm::vec3>(value)){
            ImGui::DragFloat3(propertyName.c_str(), &(std::get<glm::vec3>(value).x), 0.01f);
            ImGui::SameLine();
            auto valueExtracted = std::get<glm::vec3>(value);
            if(ImGui::ColorButton(("##" + propertyName).c_str(), ImVec4(valueExtracted.x, valueExtracted.y, valueExtracted.z, 1.0f), ImGuiColorEditFlags_NoLabel)){
                ImGui::OpenPopup(("ColorPicker##" + propertyName).c_str());
            }
        } else if (std::holds_alternative<glm::vec4>(value)){
            ImGui::DragFloat4(propertyName.c_str(), &(std::get<glm::vec4>(value).x), 0.01f);
            auto valueExtracted = std::get<glm::vec4>(value);
            if(ImGui::ColorButton(("##" + propertyName).c_str(), ImVec4(valueExtracted.x, valueExtracted.y, valueExtracted.z, valueExtracted.w), ImGuiColorEditFlags_NoLabel)){
                ImGui::OpenPopup(("ColorPicker##" + propertyName).c_str());
            }
        } else {
            continue;
        }

        if (ImGui::BeginPopup(("ColorPicker##" + propertyName).c_str())) {
            if (std::holds_alternative<glm::vec3>(value)){
                ImGui::ColorPicker3(("##" + propertyName).c_str(), &(std::get<glm::vec3>(value).x));
            } else if (std::holds_alternative<glm::vec4>(value)){
                ImGui::ColorPicker4(("##" + propertyName).c_str(), &(std::get<glm::vec4>(value).x));
            }
            ImGui::EndPopup();
        }
    }

    if(EditorUtils::DrawResourcePicker<Shader>(context, "Shader", m_shader)){
        syncShaderProperties();
    }
}