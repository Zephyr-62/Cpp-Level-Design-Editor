#pragma once

class Component {


public:
    virtual ~Component() = default;

    virtual const char* name() const = 0;
    virtual void drawInspector() = 0;
};