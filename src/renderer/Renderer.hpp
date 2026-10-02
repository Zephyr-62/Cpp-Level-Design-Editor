#pragma once

#include "resources/Mesh.hpp"
#include "resources/Material.hpp"

#include <memory>

class Scene;
class Shader;

class Renderer {

public:
	void Render(const Scene& scene) const;

private:
	void applyMaterialProperties(const std::shared_ptr<Material>& material) const;

};
