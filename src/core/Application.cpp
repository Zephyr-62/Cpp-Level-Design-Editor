#include "core/Application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>
#include <memory>
#include <chrono>
#include <algorithm>

// Temporary includes for testing
#include "resources/Shader.hpp"
#include "resources/Material.hpp"
#include "resources/Mesh.hpp"
#include "components/Transform.hpp"
#include "scene/SceneObject.hpp"
#include "components/MeshRenderer.hpp"
#include "renderer/OverlayItem.hpp"

Application::Application() {
    m_resourceManager.initializeBuiltinResources();
    m_renderer.initialize(m_context);
}
Application::~Application() { Shutdown(); }

void InitializeSmallScene(Scene& scene, ResourceManager& resourceManager);

void Application::LoadScene(const std::string& scenePath) {

	//TODO Implement scene loading from a .scn file (JSON format)
    m_scene = Scene(); 
    
    // For now, just create a new empty scene and populate it with some test objects
    InitializeSmallScene(m_scene, m_resourceManager);
    return;
}


// Adds 2 shaders, 1 material, 1 triangle mesh, 1 scene object
void InitializeSmallScene(Scene& scene, ResourceManager& resourceManager) {

    SceneObject& sceneObj = scene.createObject("Cone");
    auto& meshRenderer = sceneObj.addComponent<MeshRenderer>();
    meshRenderer.material = resourceManager.get<Material>("builtin:defaultMat");
    meshRenderer.mesh = resourceManager.get<Mesh>("builtin:cone");
    sceneObj.transform->position = glm::vec3(0, 1, 0);

    SceneObject& sceneObj2 = scene.createObject("Cube");
    auto& meshRenderer2 = sceneObj2.addComponent<MeshRenderer>();
    meshRenderer2.material = resourceManager.get<Material>("builtin:defaultMat");
    meshRenderer2.mesh = resourceManager.get<Mesh>("builtin:torus");
    sceneObj2.transform->position = glm::vec3(-2);

}

void Application::Run() {

    LoadScene("builtin:defaultScene"); // Load default empty scene on startup.

    m_window.registerScrollCallback([this](double x, double y) { 
        m_camera.processMouseScroll(x, y);
    });

    while (!m_window.shouldClose()) {
        m_window.pollEvents();

		// Frame time calculation
        auto const now = std::chrono::steady_clock::now();
        double const currentFrameTime = std::chrono::duration<double, std::milli>(now.time_since_epoch()).count();

        m_elapsedTime = currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = currentFrameTime;
        m_applicationTime += m_elapsedTime;

        // Clear selection if an object was deleted last frame
        if(m_context.hierarchyDeselectRequested){
            m_editor.clearSelectedSceneObject();
            m_context.hierarchyDeselectRequested = false;
        }

        /// Update Camera
        // Focus on object if requested
        if(m_camera.consumeRequestedFocus()){        
            if(auto sceneObj = m_editor.getSelectedSceneObject())
                m_camera.focusPosition(sceneObj->transform->position);
        }
        // Move acording to input
        m_camera.update(m_window, m_elapsedTime/1000.0, m_editor.isViewportFocused());


        /// Scene Update loop
        // Iterate through scene objects and run their update methods
        // What should be the update order?
        // For now its just scene->m_objects->m_components in list order
        // (maybe it should follow the hierarchy once that is implemented)
        m_scene.Update(m_context);


        /// Editor+Render loop
        m_editor.beginFrame();
        m_editor.processInput(m_scene, m_context);
        auto viewportSize = m_editor.viewportPanelSize();
        auto overlayItems = m_editor.buildOverlay(m_context);
        m_renderer.Render(m_scene, m_context, viewportSize.x, viewportSize.y, overlayItems);
        auto colorBuffer = m_renderer.getColorBuffer();
        m_editor.draw(m_scene, m_context, colorBuffer);
        m_editor.endFrame();

        m_window.swapBuffers();
    }

    return;
}

void Application::Shutdown() {

	// TODO: Save scene? Or maybe warn user that unsaved changes will be lost
}
