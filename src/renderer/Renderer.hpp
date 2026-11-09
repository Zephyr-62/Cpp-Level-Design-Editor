#pragma once

#include "renderer/Framebuffer.hpp"
#include "resources/Mesh.hpp"
#include "resources/Material.hpp"
#include "renderer/OverlayItem.hpp"

#include <memory>
#include <vector>

class Scene;
class Shader;

class Renderer {

public:
	void initialize(const ApplicationContext& context);

	void Render(const Scene& scene, const ApplicationContext& context, int width, int height, std::vector<OverlayItem> overlayItems);
	GLuint getColorBuffer() const { return m_framebuffer.colorTexture(); }

private:
	void applyMaterialProperties(const std::shared_ptr<Material>& material) const;

 	Framebuffer m_framebuffer{1280, 720};

	std::shared_ptr<Shader> m_overlayShader;
};
