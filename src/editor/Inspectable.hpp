#pragma once

#include "editor/EditorContext.hpp"

class Inspectable{

public:
    ~Inspectable() = default;

    virtual const char* inspectorName() const = 0;   // Label shown in the UI
    virtual void drawInspector(EditorContext& context) = 0;
};