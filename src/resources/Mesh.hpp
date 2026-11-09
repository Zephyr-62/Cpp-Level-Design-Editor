#pragma once

#include "resources/Resource.hpp"

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh : public Resource {
public:
    Mesh(std::string id, std::vector<Vertex> vertices, std::vector<std::uint32_t> indices);
    ~Mesh() override;

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&& other) noexcept;

	unsigned int vao() const { return m_vao; }
	std::uint32_t indicesCount() const { return static_cast<std::uint32_t>(m_indices.size()); }

    
    virtual bool drawableOnInspector() const override { return true; }
    virtual const char* inspectorName() const override { return "Mesh"; }
    virtual void drawInspector(ApplicationContext& context) override;

private:
    void upload();
    void release();

    std::vector<Vertex> m_vertices;
    std::vector<std::uint32_t> m_indices;
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
};
