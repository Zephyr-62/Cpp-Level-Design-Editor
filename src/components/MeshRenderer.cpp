#include "components/MeshRenderer.hpp"
#include "resources/ResourceManager.hpp"
#include "editor/EditorUtils.hpp"

#include <imgui.h>


void MeshRenderer::drawInspector(EditorContext& context) {
    EditorUtils::DrawResourcePicker<Mesh>(context, "Mesh", mesh);
    EditorUtils::DrawResourcePicker<Material>(context, "Material", material);
}
