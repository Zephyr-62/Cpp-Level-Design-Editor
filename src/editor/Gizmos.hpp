#pragma once

#include "renderer/OverlayItem.hpp"
#include "components/Transform.hpp"
#include <tuple>

enum class GizmoMode{
    TRANSFORM,
    TRANSLATE,
    ROTATE,
    SCALE,
};

enum class TransformMode{
    LOCAL_SPACE,
    GLOBAL_SPACE,
};

class Gizmos{

public:
    Gizmos();

    std::vector<OverlayItem> buildOverlay(Transform& transform, float unitsPerPixel);

    TransformMode getTransformMode() { return m_transformMode; }
    void toggleTransformMode();
    void setGizmoMode(GizmoMode mode) {m_gizmoMode = mode;}

private:
    GizmoMode m_gizmoMode = GizmoMode::TRANSFORM;
    TransformMode m_transformMode = TransformMode::LOCAL_SPACE;

    // Meshes used to render the transform gizmos
    Mesh m_cone;
    Mesh m_cube;
    Mesh m_cylinder;
    Mesh m_torus;

    std::tuple<OverlayItem, OverlayItem> getAxisShaft(Transform localTransform, Mesh* knobMesh, float unitsPerPixel, glm::vec3 dirVect, glm::vec3 upVect, glm::vec3 color);
    OverlayItem getRotationTorus(Transform localTransform, float unitsPerPixel, glm::vec3 dirVect, glm::vec3 upVect, glm::vec3 color);

    const glm::vec3 m_whiteColor{1,1,1};
    const glm::vec3 m_redColor{1,0.18f,0.18f};
    const glm::vec3 m_greenColor{0.18f,1,0.18f};
    const glm::vec3 m_blueColor{0.18f,0.18f,1}; 
};

