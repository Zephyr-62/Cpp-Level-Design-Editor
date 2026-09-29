#include "components/Transform.hpp"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

void Transform::drawInspector() {
    ImGui::DragFloat3("Position", &position.x, 0.05f);

    glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));
    if (ImGui::DragFloat3("Rotation", &euler.x, 0.5f)) {
        rotation = glm::quat(glm::radians(euler));
    }

    ImGui::DragFloat3("Scale", &scale.x, 0.05f);
}

glm::mat4 Transform::localMatrix() const {
	return glm::translate(glm::mat4(1.0f), position) *
		   glm::mat4_cast(rotation) *
		   glm::scale(glm::mat4(1.0f), scale);
}