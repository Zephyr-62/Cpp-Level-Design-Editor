#pragma once

#include "editor/Inspectable.hpp"

#include <imgui.h>
#include <memory>

// Wrapping to differentiate from other inspectable elements 
// These can be direct additions to a Scene Object, unlike e.g. resources
class Component : public Inspectable {

public:
    /// If required, the component cannot be removed from the scene object (it might be accessible by T* in the scene object)
    /// e.g. Transform in any SceneObject
    bool isRequired = false;

    // Invoked once per frame, before rendering
    virtual void Update(ApplicationContext& context) {};

    virtual std::unique_ptr<Component> clone() const = 0;

};