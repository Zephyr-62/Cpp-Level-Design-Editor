#include "core/Application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// Time tracking and logging
#include <chrono>
#include <iostream>

// Temporary includes for testing
#include "Shader.hpp"
#include "resources/Mesh.hpp"
#include "components/Transform.hpp"
#include "scene/SceneObject.hpp"

Application::Application() {
	Initialize();
}

Application::~Application() {
	Shutdown();
}

void Application::LoadScene(const std::string& scenePath) {

	//TODO Implement scene loading from a .scn file (JSON format)
	m_scene = Scene(); // For now, just create a new empty scene
    return;
}

void Application::Run() {

	if (!m_isInitialized) {
		ERROR_LOG("Trying to run Application while not initialized.", ERROR_CODE_APPLICATION_NOT_INITIALIZED);
        return;
	}

    //TODO Temporary code to check structure is working
    m_scene.createObject("Camera");
    m_scene.createObject("Camera2");
    SceneObject* selectedObject = nullptr;

    Mesh triangle("builtin:triangle",
        {
            { {-0.5f, -0.5f, 0.0f}, {0, 0, 1}, {0, 0} },
            { { 0.5f, -0.5f, 0.0f}, {0, 0, 1}, {1, 0} },
            { { 0.0f,  0.5f, 0.0f}, {0, 0, 1}, {0.5f, 1} },
        },
        { 0, 1, 2 });

    Shader triangleShader("builtin_resources/shaders/triangle.vert", "builtin_resources/shaders/triangle.frag");
    triangleShader.use();

    auto lastFrameTime = std::chrono::steady_clock::now();
    float totalTime = 0.0f;

    Transform transform;

    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        SetupDockspace();

        ImGui::Begin("Viewport");
        ImGui::End();

        ImGui::Begin("Hierarchy", 0, ImGuiWindowFlags_NoCollapse & ImGuiWindowFlags_AlwaysAutoResize);
        for (const auto& obj : m_scene.objects()) {
            if (ImGui::Selectable(obj->name.c_str(), selectedObject == obj.get()))
                selectedObject = obj.get();
        }
        ImGui::End();

        ImGui::Begin("Inspector", 0, ImGuiWindowFlags_NoCollapse & ImGuiWindowFlags_AlwaysAutoResize);
        if (selectedObject)
            selectedObject->drawInspector();
        ImGui::End();

        ImGui::Begin("Triangle", 0, ImGuiWindowFlags_AlwaysAutoResize);
        transform.drawInspector();
        ImGui::End();

        auto const currentFrameTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> const elapsedTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        totalTime += elapsedTime.count();

        glClearColor(0.10f, 0.10f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glm::mat4 model = transform.localMatrix();
        model = glm::rotate(model, totalTime, glm::vec3(0.0f, 1.0f, 0.5f));

        triangleShader.setMat4("model", model);
        triangle.draw();

        // Draw editor UI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(m_window);
    }

    return;
}

void Application::Initialize() {
	InitializeGLFW();
	if (exit_code != ERROR_CODE_SUCCESS) return;
	
	InitializeGLAD();
	if (exit_code != ERROR_CODE_SUCCESS) return;

	InitializeImGui();
	if (exit_code != ERROR_CODE_SUCCESS) return;

	m_isInitialized = true;
}

void Application::InitializeGLFW() {
    if (!glfwInit()) {
        ERROR_LOG("Failed to initialize GLFW.", ERROR_CODE_GLFW_INIT_FAILED);
        exit_code = ERROR_CODE_GLFW_INIT_FAILED;
        return;
    }

    // Ask for an OpenGL 3.3 core-profile context, matching what we told GLAD to generate
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(1280, 720, "Level Editor", nullptr, nullptr);
    if (!m_window) {
        ERROR_LOG("Failed to create GLFW window.", ERROR_CODE_GLFW_WINDOW_CREATION_FAILED);
        glfwTerminate();
        exit_code = ERROR_CODE_GLFW_WINDOW_CREATION_FAILED;
        return;
    }

    glfwMakeContextCurrent(m_window);
}

void Application::InitializeGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ERROR_LOG("Failed to initialize GLAD.", ERROR_CODE_GLAD_INIT_FAILED);
        exit_code = ERROR_CODE_GLAD_INIT_FAILED;
        return;
    }
}

void Application::InitializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Application::Shutdown() {
	// TODO: Save scene? Or maybe warn user that unsaved changes will be lost

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

}

void Application::SetupDockspace() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags hostFlags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("DockHost", nullptr, hostFlags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspaceId = ImGui::GetID("MainDockspace");
    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    static bool builtLayout = false;
    if (!builtLayout) {
        builtLayout = true;

        ImGui::DockBuilderRemoveNode(dockspaceId);
        ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceId, viewport->Size);

        ImGuiID rightId;
        ImGuiID leftId = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.20f, nullptr, &rightId);
        ImGuiID inspectorId;
        rightId = ImGui::DockBuilderSplitNode(rightId, ImGuiDir_Right, 0.25f, &inspectorId, nullptr);

        ImGui::DockBuilderDockWindow("Hierarchy", leftId);
        ImGui::DockBuilderDockWindow("Inspector", inspectorId);
        ImGui::DockBuilderDockWindow("Viewport", rightId);

        ImGui::DockBuilderFinish(dockspaceId);
    }

    ImGui::End();
}