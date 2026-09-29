#pragma once

#include <string>

#include "core/WindowCommon.hpp"
#include "core/Constants.hpp"
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


};