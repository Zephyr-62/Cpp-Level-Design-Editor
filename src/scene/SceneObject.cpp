#include "scene/SceneObject.hpp"

#include <imgui.h>

SceneObject::SceneObject(std::string objectName)
    : name(std::move(objectName)) {}

void SceneObject::drawInspector(EditorContext& context) {
    ImGui::Text("%s", name.c_str());
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        transform.drawInspector(context);
    }

    for (auto& component : m_components) {
        ImGui::PushID(component.get());
        if (ImGui::CollapsingHeader(component->inspectorName(), ImGuiTreeNodeFlags_DefaultOpen)) {
            component->drawInspector(context);
        }
        ImGui::PopID();
    }
}
