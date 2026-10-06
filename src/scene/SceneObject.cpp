#include "scene/SceneObject.hpp"
#include "editor/EditorUtils.hpp"

#include <imgui.h>

SceneObject::SceneObject(std::string objectName)
    : name(std::move(objectName)) {}

void SceneObject::drawInspector(ApplicationContext& context) {
    EditorUtils::DrawInspectableName(name.c_str());
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
