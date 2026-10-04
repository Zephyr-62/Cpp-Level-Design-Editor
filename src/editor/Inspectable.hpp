#pragma once

#include "core/ApplicationContext.hpp"

class Inspectable{

public:
    ~Inspectable() = default;

    virtual const char* inspectorName() const = 0;   // Label shown in the UI
    virtual void drawInspector(ApplicationContext& context) = 0;
};