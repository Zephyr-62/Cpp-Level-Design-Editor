#pragma once

#include "editor/Inspectable.hpp"

#include <string>
#include <utility>

class Resource : Inspectable {
public:
    explicit Resource(std::string id) : m_id(std::move(id)) {}
    virtual ~Resource() = default;

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    Resource(Resource&&) = default;
    Resource& operator=(Resource&&) = default;

    const std::string& id() const { return m_id; }

    virtual const char* inspectorName() const override { return ""; }
    virtual void drawInspector(EditorContext& context) override { return; }
    virtual bool drawableOnInspector() const { return false; }

private:
    std::string m_id;
};