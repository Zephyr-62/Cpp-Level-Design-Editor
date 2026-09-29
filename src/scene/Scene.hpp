#pragma once

#include "scene/SceneObject.hpp"

#include <memory>
#include <string>
#include <vector>

class Scene {
public:
    SceneObject& createObject(std::string name);

    const std::vector<std::unique_ptr<SceneObject>>& objects() const { return m_objects; }

private:
    std::vector<std::unique_ptr<SceneObject>> m_objects;
};