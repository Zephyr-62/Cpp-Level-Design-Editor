#pragma once

#include "scene/Component.hpp"
#include "resources/Mesh.hpp"
#include "resources/Material.hpp"

#include <memory>

class MeshRenderer : public Component {

public:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

	virtual std::unique_ptr<Component> clone() const override;

	const char* inspectorName() const override { return "Mesh Renderer"; }
	void drawInspector(ApplicationContext& context) override;
};