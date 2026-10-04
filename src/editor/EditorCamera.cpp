#include "editor/EditorCamera.hpp"

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

glm::mat4 EditorCamera::viewMatrix() const
{
    auto cameraUp = glm::cross(right(), forward());
    return glm::lookAt(position, position + forward(), cameraUp);
}

glm::mat4 EditorCamera::projectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}