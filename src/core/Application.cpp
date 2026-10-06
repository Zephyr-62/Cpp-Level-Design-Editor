#include "core/Application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <iostream>
#include <memory>
#include <filesystem>
#include <chrono>
#include <algorithm>

// Temporary includes for testing
#include "resources/Shader.hpp"
#include "resources/Material.hpp"
#include "resources/Mesh.hpp"
#include "components/Transform.hpp"
#include "scene/SceneObject.hpp"
#include "components/MeshRenderer.hpp"

Application::Application() {}
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
    SceneObject& sceneObj = scene.createObject("Triangle");

    auto whiteShader = std::make_shared<Shader>("builtin:white",
        std::filesystem::path::path("builtin_resources/shaders/unlit.vert").string(),
        std::filesystem::path::path("builtin_resources/shaders/white.frag").string());
    resourceManager.add(whiteShader);

    auto unlitShader = std::make_shared<Shader>("builtin:unlit",
        std::filesystem::path::path("builtin_resources/shaders/unlit.vert").string(),
        std::filesystem::path::path("builtin_resources/shaders/unlit.frag").string());
    resourceManager.add(unlitShader);
    
    auto defaultMat = std::make_shared<Material>("builtin:defaultMat", unlitShader);
    resourceManager.add(defaultMat);

    auto& meshRenderer = sceneObj.addComponent<MeshRenderer>();
    meshRenderer.material = defaultMat;
    std::vector<Vertex> vertices = {
        Vertex{{-0.433f, -0.25f, 0.0f}, {0, 0, 1}, {0, 0}},
        Vertex{{0.433f, -0.25f, 0.0f},  {0, 0, 1}, {1, 0}},
        Vertex{{0.0f,  0.5f, 0.0f},     {0, 0, 1}, {0.5f, 1}},
    };
    std::vector<std::uint32_t> indices = { 0, 1, 2 };
    resourceManager.add(std::make_shared<Mesh>("builtin:triangle",
        vertices,
        indices
    ));
    meshRenderer.mesh = resourceManager.get<Mesh>("builtin:triangle");

    vertices = {
        Vertex{{-0.5f, -0.5f, 0}, {0, 0, 1}, {0, 0} },
        Vertex{{-0.5f,  0.5f, 0}, {0, 0, 1}, {0, 1} },
        Vertex{{ 0.5f,  0.5f, 0}, {0, 0, 1}, {1, 1}},
        Vertex{{ 0.5f, -0.5f, 0}, {0, 0, 1}, {1, 0}},
    };
    indices = { 0, 1, 2, 2, 3, 0 };
    resourceManager.add(std::make_shared<Mesh>("builtin:quad",
        vertices,
        indices
    ));
}

void Application::Run() {

    LoadScene("builtin:defaultScene"); // Load default empty scene on startup.

    while (!m_window.shouldClose()) {
        m_window.pollEvents();

		// Frame time calculation
        float const currentFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        m_elapsedTime = currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = currentFrameTime;
        m_applicationTime += m_elapsedTime;

        // Update loop
        // Iterate through scene objects and run their update methods for every 'Runnable' component
        // What should be the update order?

        // Render loop
        m_editor.beginFrame();
        auto viewportSize = m_editor.viewportPanelSize();
        m_renderer.Render(m_scene, m_context, viewportSize.x, viewportSize.y);
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
