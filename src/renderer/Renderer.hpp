#pragma once

#include "resources/Mesh.hpp"
#include "resources/Material.hpp"

#include <memory>

class Scene;
class Shader;

class Renderer {

public:
	void Render(const Scene& scene, const ApplicationContext& context) const;

private:
	void applyMaterialProperties(const std::shared_ptr<Material>& material) const;

	unsigned int m_fbo = 0;
	unsigned int m_fboTexture = 0;
	unsigned int m_fboDepthBuffer = 0;

};
