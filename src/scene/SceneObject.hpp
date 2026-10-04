#pragma once

#include "components/Transform.hpp"
#include "scene/Component.hpp"

#include <memory>
#include <string>
#include <vector>

class SceneObject : public Inspectable {
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

	virtual const char* inspectorName() const { return name.c_str(); }
    void drawInspector(ApplicationContext& context) override;

    Transform transform;
    std::string name;

private:
	//SceneObject* parent = nullptr;
	//std::vector<SceneObject*> children;

    std::vector<std::unique_ptr<Component>> m_components;
};