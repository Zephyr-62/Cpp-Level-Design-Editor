#pragma once

#include "scene/SceneObject.hpp"

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

class Scene {
public:
    SceneObject& createObject(std::string name);
    void removeObject(SceneObject* scnObj);

    void Update(ApplicationContext& context) {
        deletePendingObjects(context);

        for (const auto& obj : m_objects){
            obj->Update(context);
        }
    }

    const std::vector<std::unique_ptr<SceneObject>>& objects() const { return m_objects; }

private:
    std::vector<std::unique_ptr<SceneObject>> m_objects;

    // Remove objects from scene
    // Watch out for iterations/dangling pointers before invoking this method
    void deletePendingObjects(ApplicationContext& context);
    std::unordered_set<SceneObject*> m_deletePendingObjs; 
};