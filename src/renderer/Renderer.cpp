
#include "renderer/Renderer.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneObject.hpp"
#include "components/MeshRenderer.hpp"
#include "core/Constants.hpp"
#include "core/ApplicationContext.hpp"
#include "editor/EditorCamera.hpp"

#include "resources/Mesh.hpp"
#include "resources/Material.hpp"
#include "resources/Shader.hpp"

#include <glad/glad.h>



void Renderer::Render(const Scene& scene, const ApplicationContext& context) const {

	// TODO render to texture, change texture size to editor.viewportSize
	// glGenFramebuffers(1, &m_fbo);
	// glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, 1280, 720);
	glClearColor(0.10f, 0.10f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);	

	auto& camera = context.camera;
	glm::mat4 viewProjectionMatrix = camera.projectionMatrix(context.viewportAspectRatio) * camera.viewMatrix();

	// Traverse scene and render each object with a MeshRenderer component
	for (auto& obj : scene.objects()) {
		if (auto meshRenderer = obj.get()->getComponent<MeshRenderer>()) {
			if(!meshRenderer->mesh || !meshRenderer->material) {
				// w/o material I could still render the mesh with a default material, but for now I will just skip it
				continue;
			}

			auto shader = meshRenderer->material->getShader();
			shader->use();
			shader->setMat4("engine_mvp_mat", viewProjectionMatrix * obj.get()->transform.localMatrix());
			applyMaterialProperties(meshRenderer->material); // Use shader and set material properties into uniforms
			
			glBindVertexArray(meshRenderer->mesh->vao());
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(meshRenderer->mesh->indicesCount()), GL_UNSIGNED_INT, nullptr);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::applyMaterialProperties(const std::shared_ptr<Material>& material) const {
		
	for(auto properties : material->properties()) {
		const std::string& propertyName = properties.first;
		const MaterialPropertyValue& value = properties.second;

		if(std::holds_alternative<float>(value)) {
			material->getShader()->setFloat(propertyName, std::get<float>(value));
		} else if (std::holds_alternative<glm::vec2>(value)) {
			material->getShader()->setVec2(propertyName, std::get<glm::vec2>(value));
		} else if (std::holds_alternative<glm::vec3>(value)) {
			material->getShader()->setVec3(propertyName, std::get<glm::vec3>(value));
		} else if (std::holds_alternative<glm::vec4>(value)) {
			material->getShader()->setVec4(propertyName, std::get<glm::vec4>(value));
		}
	}
}