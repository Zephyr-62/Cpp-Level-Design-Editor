#pragma once

#include "resources/Resource.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

class ResourceManager {
public:
    template <typename T>
    std::shared_ptr<T> get(const std::string& id) const {
        auto it = m_resources.find(id);
        if (it == m_resources.end()) return nullptr;
        return std::dynamic_pointer_cast<T>(it->second);
    }

    void add(std::shared_ptr<Resource> resource) {
        m_resourceTypes.insert(resource.get()->inspectorName());
        m_resources[resource->id()] = std::move(resource);
    }
    
    const std::unordered_map<std::string, std::shared_ptr<Resource>>& getAll() const { return m_resources; }

    template <typename T>
    const std::vector<std::shared_ptr<T>> getAll() const {
        std::vector<std::shared_ptr<T>> result;
        for (const auto& pair : m_resources) {
            if (std::dynamic_pointer_cast<T>(pair.second)) {
                result.push_back(std::dynamic_pointer_cast<T>(pair.second));
            }
        }
        return result;
    }

    template <typename T>
    bool hasAny() const{
        for (const auto& pair : m_resources) {
            if (std::dynamic_pointer_cast<T>(pair.second)) {
                return true;
            }
        }
        return false;
    }

    const std::unordered_set<std::string> getResourceTypes() const { return m_resourceTypes; }

private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
    std::unordered_set<std::string> m_resourceTypes;
};
