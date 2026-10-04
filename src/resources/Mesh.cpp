#include "resources/Mesh.hpp"

#include "core/GLCommon.hpp"
#include <cstddef>
#include <imgui.h>

Mesh::Mesh(std::string id, std::vector<Vertex> vertices, std::vector<std::uint32_t> indices)
            : Resource(std::move(id))
            , m_vertices(std::move(vertices))
            , m_indices(std::move(indices)) {
    upload();
}

Mesh::~Mesh() {
    release();
}

Mesh::Mesh(Mesh&& other) noexcept
            : Resource(std::move(other))
            , m_vertices(std::move(other.m_vertices))
            , m_indices(std::move(other.m_indices))
            , m_vao(other.m_vao)
            , m_vbo(other.m_vbo)
            , m_ebo(other.m_ebo)
{
    other.m_vao = 0;
    other.m_vbo = 0;
    other.m_ebo = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        release();		

        Resource::operator=(std::move(other));
		m_vertices = std::move(other.m_vertices);
		m_indices = std::move(other.m_indices);
		m_vao = other.m_vao;
		m_vbo = other.m_vbo;
		m_ebo = other.m_ebo;

		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_ebo = 0;
    }
    return *this;
}

void Mesh::upload() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
        m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(std::uint32_t),
        m_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, uv)));

    glBindVertexArray(0);
}

void Mesh::release() {
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
    if (m_ebo != 0) glDeleteBuffers(1, &m_ebo);
    m_vao = m_vbo = m_ebo = 0;
}

// ###### OnInspectorDraw ######

void Mesh::drawInspector(ApplicationContext& context) {
    ImGui::Text("Vertices: %zu", m_vertices.size());
    ImGui::Text("Indices: %zu", m_indices.size());
}
