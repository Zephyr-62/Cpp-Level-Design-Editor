#pragma once

#include "resources/Mesh.hpp"

#include <glm/glm.hpp>

struct OverlayItem {
    Mesh* mesh;
    glm::mat4 modelMatrix;
    glm::vec3 color;
};
