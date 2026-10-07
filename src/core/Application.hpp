#pragma once

#include <string>

#include "core/Window.hpp"
#include "editor/Editor.hpp"
#include "resources/ResourceManager.hpp"
#include "renderer/Renderer.hpp"
#include "scene/Scene.hpp"
#include "core/Constants.hpp"

#include "core/ApplicationContext.hpp"
#include "editor/EditorCamera.hpp"

class Application {

public:
	Application();
	~Application();

	void Run(); // Main Loop. If no scene is loaded, this will run an empty scene.

	// TODO: Maybe move this to a serializer class (Resource : Serializable)
	void LoadScene(const std::string& scenePath); // Load a scene from a .scn file (JSON format)
	
	double applicationTime() const { return m_applicationTime; }
	double lastFrameTime() const { return m_lastFrameTime; }
	double elapsedTime() const { return m_elapsedTime; }

private: 
	void Shutdown();

	Window m_window{ 1600, 900, "Level Editor" };
	Renderer m_renderer;
	Editor m_editor{ m_window };
	EditorCamera m_camera;
	ResourceManager m_resourceManager;
	Scene m_scene;

	ApplicationContext m_context{ m_resourceManager, m_camera, m_window };

	double m_applicationTime = 0.0f;		// Total time since application started, in ms
	double m_lastFrameTime = 0.0f;		// Time at which the last frame was rendered, in ms
	double m_elapsedTime = 0.0f;			// Time taken to render the last frame, in ms

};
