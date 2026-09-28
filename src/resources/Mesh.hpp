
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

    void draw() const;

    const std::vector<Vertex>& vertices() const { return m_vertices; }
    const std::vector<std::uint32_t>& indices() const { return m_indices; }

private:
    void upload();
    void release();

    std::vector<Vertex> m_vertices;
    std::vector<std::uint32_t> m_indices;
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
};
