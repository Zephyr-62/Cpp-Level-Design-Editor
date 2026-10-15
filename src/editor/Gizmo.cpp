#include "editor/Gizmos.hpp"

#include "resources/MeshFactory.hpp"

#include <functional>

Gizmos::Gizmos():
    m_cube(MeshFactory::cube(0.1f)),
    m_cone(MeshFactory::cone("gizmo:cone", 0.2f, 0.05f, 5)),
    m_cylinder(MeshFactory::cylinder("gizmos:shaft", 1, 0.015f, 5)),
    m_torus(MeshFactory::torus("gizmos:torus",1.6f, 0.025f, 32, 5))
    //m_torus(MeshFactory::torus(0.1f))
{ }

void Gizmos::toggleTransformMode(){
    m_transformMode = m_transformMode == TransformMode::GLOBAL_SPACE? TransformMode::LOCAL_SPACE : TransformMode::GLOBAL_SPACE;
}

std::vector<OverlayItem> Gizmos::buildOverlay(Transform& transform, float unitsPerPixel){

    std::vector<OverlayItem> items;
    auto gizmosTransform = transform;    
    gizmosTransform.scale = glm::vec3(unitsPerPixel);

    auto upDir = glm::vec3(0,1,0);
    auto rightDir = glm::vec3(1,0,0);
    auto forwardDir = glm::vec3(0,0,1);

    if(m_transformMode == TransformMode::LOCAL_SPACE){
        upDir = gizmosTransform.rotation * upDir;
        rightDir = gizmosTransform.rotation * rightDir;
        forwardDir = gizmosTransform.rotation * forwardDir;        
    }

    // Origin Cube
    items.push_back(
        {
            &m_cube,
            gizmosTransform.localMatrix(),     
            m_whiteColor
        }
    );
    
    if(m_gizmoMode == GizmoMode::ROTATE || m_gizmoMode == GizmoMode::TRANSFORM){
        items.push_back(getRotationTorus(gizmosTransform, unitsPerPixel, forwardDir, upDir, m_blueColor));
        items.push_back(getRotationTorus(gizmosTransform, unitsPerPixel, rightDir, upDir, m_redColor));
        items.push_back(getRotationTorus(gizmosTransform, unitsPerPixel, upDir, rightDir, m_greenColor));
    }

    if(m_gizmoMode != GizmoMode::ROTATE){

        auto knobMesh = m_gizmoMode == GizmoMode::SCALE ? &m_cube : &m_cone;

        auto axisItems = getAxisShaft(gizmosTransform, knobMesh, unitsPerPixel, forwardDir, upDir, m_blueColor);
        items.push_back(std::get<0>(axisItems));
        items.push_back(std::get<1>(axisItems));
        axisItems = getAxisShaft(gizmosTransform, knobMesh, unitsPerPixel, rightDir, upDir, m_redColor);
        items.push_back(std::get<0>(axisItems));
        items.push_back(std::get<1>(axisItems));
        axisItems = getAxisShaft(gizmosTransform, knobMesh, unitsPerPixel, upDir, rightDir, m_greenColor);
        items.push_back(std::get<0>(axisItems));
        items.push_back(std::get<1>(axisItems));
    }

    return items;
}


std::tuple<OverlayItem, OverlayItem> Gizmos::getAxisShaft(Transform localTransform, Mesh* knobMesh, float unitsPerPixel, glm::vec3 dirVect, glm::vec3 upVect, glm::vec3 color){

    auto halfRight = dirVect * 0.5f * unitsPerPixel;
    localTransform.position += halfRight;
    localTransform.rotation =  glm::quatLookAt(upVect, dirVect);

    auto shaft = OverlayItem{
            &m_cylinder,
            localTransform.localMatrix(),
            color,
        };
    localTransform.position += halfRight;
    auto knob = OverlayItem{
            knobMesh,
            localTransform.localMatrix(),
            color,
        };

    return {shaft, knob};
}

OverlayItem Gizmos::getRotationTorus(Transform localTransform, float unitsPerPixel,
     glm::vec3 dirVect, glm::vec3 upVect, glm::vec3 color){

    localTransform.rotation =  glm::quatLookAt(upVect, dirVect);
    return OverlayItem{
            &m_torus,
            localTransform.localMatrix(),
            color,
        };
}

