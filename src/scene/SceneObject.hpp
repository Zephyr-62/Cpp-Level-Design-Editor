#pragma once

#include "scene/Component.hpp"

// All components
#include "components/Transform.hpp"
#include "components/MeshRenderer.hpp"

#include <memory>
#include <string>
#include <vector>
#include <concepts>
#include <unordered_set>

#include <iostream>

class SceneObject : public Inspectable {
public:

    SceneObject(std::string objectName, std::unique_ptr<Transform> transform);
    explicit SceneObject(std::string objectName);

    ~SceneObject() {
        std::cout << "object deleted: '" << name << "'" << std::endl;
    }

    Transform* transform;
    std::string name;
    
    // Invoked once per frame, before rendering
    void Update(ApplicationContext& context);

    // New name is rejected if empty
    void updateName(const char* newName);

    /// Adds a new component of the given type
    /// If the component already exists this method returns it 
    template <typename T>
        requires std::derived_from<T, Component>
    T& addComponent() {
        if(T* match = getComponent<T>()) // Safe guard to avoid component duplicates
            return *match;

        auto component = std::make_unique<T>();
        T& ref = *component;
        m_components.push_back(std::move(component));
        return ref;
    }

    template <typename T>
        requires std::derived_from<T, Component>
    T* getComponent() const {
        for (const auto& component : m_components) {
            if (T* match = dynamic_cast<T*>(component.get())) {
                return match;
            }
        }
        return nullptr;
    }

    struct ComponentType{
        const char* name;
        void (*addComponent)(SceneObject&);
    };

    const std::vector<ComponentType> componentFactory = { 
        // {"Transform", [](SceneObject& so) {so.addComponent<Transform>();}}, //NOTE: Transforms cant be added 
        {"Mesh Renderer", [](SceneObject& so) {so.addComponent<MeshRenderer>();}},
        // Cameras, lights, custom script components
        // TODO (must be discoverable automatically! --> components register themselves?)...
        // This is only a temporal solution
    };

    std::unique_ptr<SceneObject> duplicate() const;

    virtual const char* inspectorName() const { return name.c_str(); }
    void drawInspector(ApplicationContext& context) override;

private:
	//SceneObject* parent = nullptr;
	//std::vector<SceneObject*> children;

    std::vector<std::unique_ptr<Component>> m_components;

    // Remove components from m_componets
    // Watch out for iterations/dangling pointers before invoking this method 
    void deletePendingComponents();
    std::unordered_set<Component*> m_deletePendingComponent; 
};