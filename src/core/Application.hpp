#pragma once

#include <string>

#include "core/WindowCommon.hpp"
#include "core/Constants.hpp"
#include "editor/EditorContext.hpp"
#include "editor/EditorCamera.hpp"
#include "resources/ResourceManager.hpp"
#include "renderer/Renderer.hpp"
#include "scene/Scene.hpp"

class Application {

public:
	Application();
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;

	void Run(); // If no scene is loaded, this will run an empty scene. If a scene is loaded, it will run that scene.
	void LoadScene(const std::string& scenePath); // Load a scene from a .scn file (JSON format)
	
	unsigned int exit_code = ERROR_CODE_SUCCESS;

	float applicationTime() const { return m_applicationTime; }
	float lastFrameTime() const { return m_lastFrameTime; }
	float elapsedTime() const { return m_elapsedTime; }

private: 

	bool m_isInitialized = false;

	void Initialize();
	void Shutdown();

	void InitializeGLFW();
	void InitializeGLAD();
	void InitializeImGui();

	void SetupDockspace();
	void RenderScene();
	void RenderUI();

	GLFWwindow* m_window;
	Scene m_scene;
	EditorCamera m_camera;
	ResourceManager m_resourceManager;
	Renderer m_renderer;

	EditorContext m_context{ m_resourceManager, m_camera, 1.0f };

	float m_applicationTime = 0.0f;		// Total time since application started, in ms
	float m_lastFrameTime = 0.0f;		// Time at which the last frame was rendered, in ms
	float m_elapsedTime = 0.0f;			// Time taken to render the last frame, in ms


};