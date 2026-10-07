#include "editor/EditorCamera.hpp"
#include "core/Window.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>


glm::vec3 EditorCamera::forward() const
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    return glm::normalize(direction);
}

glm::vec3 EditorCamera::right() const
{
    return glm::normalize(glm::cross(forward(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 EditorCamera::up() const
{
    return glm::normalize(glm::cross(forward(), right()));
}

glm::mat4 EditorCamera::viewMatrix()
{
    if(perspectiveMode == CameraMode::PERSPECTIVE){
        auto cameraUp = glm::cross(right(), forward());
        return glm::lookAt(position, position + forward(), cameraUp);
    }
    else{
        // yaw = 45; force yaw?     
        return glm::lookAt(position, position + forward() * m_arcBallDistance, glm::vec3(0.0f, 1.0f, 0.0f));
    }    
}

glm::mat4 EditorCamera::projectionMatrix(float aspectRatio) const
{
    if(perspectiveMode == CameraMode::PERSPECTIVE){
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }
    else{
        float halfWidth  = (m_arcBallDistance * aspectRatio) * 0.5f;
        float halfHeight = m_arcBallDistance * 0.5f;

        // glm::ortho(left, right, bottom, top, zNear, zFar)
        return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
    }
}

void EditorCamera::focusPosition(glm::vec3 targetPos){
    position = targetPos - forward() * m_arcBallDistance;
}

void EditorCamera::processMouseScroll(double xScroll, double yScroll){
    m_lastArcBallPos = position + forward() * m_arcBallDistance;
    m_arcBallDistance += m_arcBallDistance/20 * yScroll * -zoomSpeed;
    focusPosition(m_lastArcBallPos);
};

void EditorCamera::update(const Window& window, float deltaTime, bool allowInput){

    m_arcBallDistance = std::clamp(m_arcBallDistance, 0.0f, 50.0f);

    CameraMoveMode m_cameraMode;
    if(window.isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)){
        m_cameraMode = CameraMoveMode::FREE_CAM;
    } else if (window.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)){
        m_cameraMode = CameraMoveMode::ARC_BALL;
    } else if (window.isMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)){
        m_cameraMode = CameraMoveMode::PAN;
    }

    bool isLooking = allowInput && 
        (window.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) ||
            window.isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) || 
            window.isMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE));

    // Lock curson on first/last frame
    if (isLooking && !m_wasLooking) {
        window.setCursorMode(GLFW_CURSOR_DISABLED);
        m_lastCursorPos = window.cursorPosition();
        m_lastArcBallPos = position + m_arcBallDistance * forward();
    } else if (!isLooking && m_wasLooking) {
        window.setCursorMode(GLFW_CURSOR_NORMAL);
    }

    m_wasLooking = isLooking;
    if (!allowInput) return;

    if (window.isKeyDown(GLFW_KEY_F)){
        m_requestedFocus = true;
    }

    float speed = moveSpeed;
    if (window.isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        speed *= fastMultiplier;
    }
    speed *= deltaTime;

    // Camera Rotation
    switch (m_cameraMode)
    {
        case CameraMoveMode::FREE_CAM:
            // Update camera rotation based on cursor delta
            if (isLooking) {
                glm::vec2 current = window.cursorPosition();
                glm::vec2 delta = current - m_lastCursorPos;
                m_lastCursorPos = current;

                yaw += delta.x * mouseSensitivity;
                pitch -= delta.y * mouseSensitivity; // screen Y grows downward; invert so "mouse up" looks up
                pitch = std::clamp(pitch, -89.0f, 89.0f);
            }
            break;
        case CameraMoveMode::ARC_BALL:
            // Update camera rotation based on cursor delta
            if (isLooking) {
                glm::vec2 current = window.cursorPosition();
                glm::vec2 delta = current - m_lastCursorPos;
                m_lastCursorPos = current;

                yaw += delta.x * mouseSensitivity;
                pitch -= delta.y * mouseSensitivity; // screen Y grows downward; invert so "mouse up" looks up
                pitch = std::clamp(pitch, -89.0f, 89.0f);

                focusPosition(m_lastArcBallPos);
            }
            break;
        
        // Rotation disabled while panning
        case CameraMoveMode::PAN:
            break;
        default:
            break;
    }

    // Camera Position
    switch (m_cameraMode)
    {
        // Pan camera changes position on its own but also responds to free cam inputs
        case CameraMoveMode::PAN:        
            if(isLooking){
                glm::vec2 current = window.cursorPosition();
                glm::vec2 delta = current - m_lastCursorPos;
                m_lastCursorPos = current;

                auto panSpeed = delta * (mouseSensitivity * m_arcBallDistance * 0.02f); // pan speed depends on zoom level

                position -= right() * panSpeed.x; 
                position -= up() * panSpeed.y;
            }
        case CameraMoveMode::FREE_CAM:
            if (window.isKeyDown(GLFW_KEY_W)) position += forward() * speed;
            if (window.isKeyDown(GLFW_KEY_S)) position -= forward() * speed;
            if (window.isKeyDown(GLFW_KEY_D)) position += right() * speed;
            if (window.isKeyDown(GLFW_KEY_A)) position -= right() * speed;
            if (window.isKeyDown(GLFW_KEY_E)) position += glm::vec3(0.0f, 1.0f, 0.0f) * speed;
            if (window.isKeyDown(GLFW_KEY_Q)) position -= glm::vec3(0.0f, 1.0f, 0.0f) * speed;
            break;
        case CameraMoveMode::ARC_BALL:
            break;
        
        default:
            break;
    }

}

bool EditorCamera::consumeRequestedFocus(){
    bool retVal = m_requestedFocus;
    m_requestedFocus = false;
    return retVal;
}