#include "components/MeshRenderer.hpp"
#include "resources/ResourceManager.hpp"
#include "editor/EditorUtils.hpp"

#include <imgui.h>


std::unique_ptr<Component> MeshRenderer::clone() const{
    auto copy = std::make_unique<MeshRenderer>(*this);
    return copy;
}

void MeshRenderer::drawInspector(ApplicationContext& context) {
    EditorUtils::DrawResourcePicker<Mesh>(context, "Mesh", mesh);
    EditorUtils::DrawResourcePicker<Material>(context, "Material", material);
}
