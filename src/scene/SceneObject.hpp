#pragma once

#include "components/Transform.hpp"
#include "scene/Component.hpp"

#include <memory>
#include <string>
#include <vector>

class SceneObject {
public:
    explicit SceneObject(std::string objectName);

    template <typename T>
    T& addComponent() {
        auto component = std::make_unique<T>();
        T& ref = *component;
        m_components.push_back(std::move(component));
        return ref;
    }

    template <typename T>
    T* getComponent() const {
        for (const auto& component : m_components) {
            if (T* match = dynamic_cast<T*>(component.get())) {
                return match;
            }
        }
        return nullptr;
    }

    void drawInspector();

    std::string name;
    Transform transform;

private:
    std::vector<std::unique_ptr<Component>> m_components;
};