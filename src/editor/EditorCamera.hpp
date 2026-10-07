#pragma once

#include <glm/glm.hpp>


class EditorCamera {
public:
    enum CameraMode: int{
        PERSPECTIVE,
        ISOMETRIC
    };

    glm::vec3 position{0.0f, 1.0f, 3.0f};
    float yaw = -90.0f;   // degrees; -90 so "forward" starts out pointing down -Z
    float pitch = 0.0f;   // degrees

    float fov = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float orthoSize = 10.0f;
    CameraMode perspectiveMode = CameraMode::PERSPECTIVE;
    
    float moveSpeed = 3.0f;         // units per second
    float fastMultiplier = 3.0f;    // applied while the speed-boost key is held
    float mouseSensitivity = 0.12f;
    float zoomSpeed = 2.0f;       // speed at which the orbital camera changes its pivot

    
    glm::vec3 forward() const;
    glm::vec3 right() const;
    glm::vec3 up() const;
    
    glm::mat4 viewMatrix();
    glm::mat4 projectionMatrix(float aspectRatio) const;

    void update(const class Window& window, float deltaTime, bool allowInput);
    
    void focusPosition(glm::vec3 targetPos);
    bool consumeRequestedFocus();
    void processMouseScroll(double xScroll, double yScroll);

    float& getArcBallDist() { return m_arcBallDistance; }

private:
    glm::vec2 m_lastCursorPos{0.0f};
    bool m_wasLooking = false;
    float m_arcBallDistance = 2; // measured in units
    glm::vec3 m_lastArcBallPos {0.0f};

    bool m_requestedFocus = false;

    enum CameraMoveMode{
        FREE_CAM,
        ARC_BALL,
        PAN
    };
};