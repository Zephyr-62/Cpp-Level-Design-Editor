#pragma once

#include "renderer/Framebuffer.hpp"
#include "resources/Mesh.hpp"
#include "resources/Material.hpp"

#include <memory>

class Scene;
class Shader;

class Renderer {

public:
	void Render(const Scene& scene, const ApplicationContext& context, int width, int height);
	GLuint getColorBuffer() const { return m_framebuffer.colorTexture(); }

private:
	void applyMaterialProperties(const std::shared_ptr<Material>& material) const;

 	Framebuffer m_framebuffer{1280, 720};
};
