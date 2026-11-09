#pragma once

#include "resources/Mesh.hpp"

namespace MeshFactory{

    Mesh eqTriangle(float side);
    Mesh quad(float side);
    Mesh cube(float side);
    // Mesh sphere(float radius, std::uint16_t sectionSegments);

    Mesh cylinder(const char* id, float height, float radius, std::uint16_t segments);
    Mesh cone(const char* id, float height, float radius, std::uint16_t segments);
    Mesh torus(const char* id, float cirlceRadius, float girthRadius, int segments, int girthsegments);
}