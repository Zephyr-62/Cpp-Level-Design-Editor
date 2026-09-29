#include "scene/SceneObject.hpp"

#include <imgui.h>

SceneObject::SceneObject(std::string objectName)
    : name(std::move(objectName)) {}

void SceneObject::drawInspector() {
    ImGui::Text("%s", name.c_str());
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        transform.drawInspector();
    }

    for (auto& component : m_components) {
        ImGui::PushID(component.get());
        if (ImGui::CollapsingHeader(component->name(), ImGuiTreeNodeFlags_DefaultOpen)) {
            component->drawInspector();
        }
        ImGui::PopID();
    }
}
