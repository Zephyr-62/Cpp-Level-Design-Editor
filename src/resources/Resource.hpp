#pragma once

#include <string>
#include <utility>

class Resource {
public:
    explicit Resource(std::string id) : m_id(std::move(id)) {}
    virtual ~Resource() = default;

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    Resource(Resource&&) = default;
    Resource& operator=(Resource&&) = default;

    const std::string& id() const { return m_id; }

private:
    std::string m_id;
};