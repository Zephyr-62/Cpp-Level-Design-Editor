#pragma once

#include <glm/glm.hpp>

class EditorCamera {
public:
    glm::vec3 position{0.0f, 1.0f, 3.0f};
    float yaw = -90.0f;   // degrees; -90 so "forward" starts out pointing down -Z
    float pitch = 0.0f;   // degrees

    float fov = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    glm::vec3 forward() const;
    glm::vec3 right() const;

    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix(float aspectRatio) const;
};