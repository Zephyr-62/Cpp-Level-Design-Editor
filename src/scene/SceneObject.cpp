#include "scene/SceneObject.hpp"
#include "editor/EditorUtils.hpp"

#include <imgui.h>
#include <algorithm>

SceneObject::SceneObject(std::string objectName, std::unique_ptr<Transform> transform) : name(std::move(objectName)) {
    this->transform = transform.get();
    this->m_components.push_back(std::move(transform));
}

SceneObject::SceneObject(std::string objectName) : 
    SceneObject::SceneObject(std::move(objectName), std::make_unique<Transform>()){}

std::unique_ptr<SceneObject> SceneObject::duplicate() const {
    auto scnObjCopy = std::make_unique<SceneObject>(this->name, std::make_unique<Transform>(*this->transform));
    
    for(const auto& component : m_components){
        if (component.get() == this->transform)
            continue;
        scnObjCopy->m_components.push_back(component->clone());
    }

    return scnObjCopy;
}


void SceneObject::Update(ApplicationContext& context){
    // Complete all pending component deletions
    deletePendingComponents();

    // Update all components
    for (const auto& component : m_components) {
        component->Update(context);
    }
}


void SceneObject::deletePendingComponents(){

    std::erase_if(m_components, [&](const std::unique_ptr<Component>& component) {
        return m_deletePendingComponent.contains(component.get()) && !component->isRequired;
    });
    
    m_deletePendingComponent.clear();
}

void SceneObject::updateName(const char* newName){
    std::string newNameStr(newName);
    if(newNameStr != "")
        name = std::move(newName);
}

void SceneObject::drawInspector(ApplicationContext& context) {

    EditorUtils::DrawInspectableName(name.c_str());
    
    if(ImGui::IsItemClicked(ImGuiMouseButton_Left)){
        EditorUtils::OpenRenamePopup(name.c_str());           
    }
    if(auto newName = EditorUtils::DrawRenamePopup()){
        updateName(newName);
    }
    
    ImGui::Separator();
    
    for (auto& component : m_components) {
        ImGui::PushID(component.get());
        if (ImGui::CollapsingHeader(component->inspectorName(), ImGuiTreeNodeFlags_DefaultOpen)) {
            // Open Context menu
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
                ImGui::OpenPopup("componentContext");

            if(ImGui::BeginPopup("componentContext")){
                
                // Remove Component if allowed
                ImGui::BeginDisabled(component->isRequired);                
                if(ImGui::Selectable("Remove Component")){
                    m_deletePendingComponent.insert(component.get());
                }
                ImGui::EndDisabled();
                ImGui::EndPopup();
            }

            // Draw component
            component->drawInspector(context);
        }
        ImGui::PopID();
    }

    ImGui::Separator();
    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("addComponent");
    }

    if(ImGui::BeginPopup("addComponent")){
        ImGui::Text("Add Component:");
        for(const ComponentType& entry : componentFactory){
            if(ImGui::Selectable(entry.name)){
                entry.addComponent(*this);
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}
