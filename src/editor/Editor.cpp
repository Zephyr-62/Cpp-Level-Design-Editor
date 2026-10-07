#include "editor/Editor.hpp"
#include "editor/EditorUtils.hpp"

#include "core/Window.hpp"
#include "scene/Scene.hpp"
#include "editor/EditorCamera.hpp"
#include "resources/ResourceManager.hpp"
#include "core/Constants.hpp"

#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <memory>

Editor::Editor(const Window& window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window.handle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

Editor::~Editor(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void Editor::beginFrame(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Editor::draw(Scene& scene, ApplicationContext& context, unsigned int colorTexture){
    setupDockspace();

    // Draw all the editor panels
    // Hierarchy, Inspector, Viewport, Resources...

    // Viewport Panel
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_MenuBar);
    ImVec2 viewportCursorPos = ImGui::GetCursorPos();
    m_viewportFocused = ImGui::IsWindowHovered();
    m_viewportSize = ImGui::GetContentRegionAvail();
    ImGui::Image((ImTextureID)(intptr_t)colorTexture, m_viewportSize, ImVec2(0, 1), ImVec2(1, 0));
    
    // Viewport Overlay
    if(m_showViewportStats){
        float overlayWidth = ImGui::GetContentRegionAvail().x * 0.20f;
        ImGui::SetCursorPos(ImVec2(viewportCursorPos.x, viewportCursorPos.y));
        ImGui::PushItemWidth(overlayWidth);

        {
            ImGui::LabelText("Zoom Level", "%.2f", context.camera.getArcBallDist());
        }

        ImGui::PopItemWidth();
    }

    // Viewport Settigns Menu
    if(ImGui::BeginMenuBar()){
        if(ImGui::BeginMenu("Viewport Overlay")){
            ImGui::Checkbox("Display Stats", &m_showViewportStats);
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Camera Settings")){
            ImGui::SliderFloat("Field Of View", &context.camera.fov, 22.5f, 120.0f);
            ImGui::InputFloat("Near Plane", &context.camera.nearPlane);
            ImGui::InputFloat("Far Plane", &context.camera.farPlane);
            
            static const char* filterNames[] = {
                "Perspective",
                "Isometric"
            };
            int currentCameraMode = static_cast<int>(context.camera.perspectiveMode);
            if(ImGui::Combo("Perspective/Isometric", &currentCameraMode, filterNames, IM_ARRAYSIZE(filterNames))){
                context.camera.perspectiveMode = static_cast<EditorCamera::CameraMode>(currentCameraMode);
            }            

            ImGui::Separator();

            ImGui::SliderFloat("Camera Speed", &context.camera.moveSpeed, 0, 50);
            ImGui::SliderFloat("Mouse Sensitivity", &context.camera.mouseSensitivity, 0.01f, 0.75f);
            ImGui::SliderFloat("Zoom Speed", &context.camera.zoomSpeed, 0, 5);
            ImGui::InputFloat("Zoom Level", &context.camera.getArcBallDist());

            ImGui::EndMenu();
        }    
        ImGui::EndMenuBar();
    }
    ImGui::End();

    // SceneObject Hierarchy Panel
    ImGui::Begin("Hierarchy", 0, ImGuiWindowFlags_NoCollapse & ImGuiWindowFlags_AlwaysAutoResize);
    for (const auto& obj : scene.objects()) {
        if (ImGui::Selectable(obj->name.c_str(), m_selectedInspectable == obj.get()))
            m_selectedInspectable = obj.get();
    }
    ImGui::End();

    // Resources Panel
    ImGui::Begin("Resources", 0, ImGuiWindowFlags_NoCollapse & ImGuiWindowFlags_AlwaysAutoResize);
    for(const auto& resourceType: context.resourceManager.getResourceTypes()){
        if (ImGui::CollapsingHeader(resourceType.c_str(), ImGuiTreeNodeFlags_Framed)) {
            for (const auto& resource : context.resourceManager.getAll<Resource>()) {
                if(std::string(resource.get()->inspectorName()) == std::string(resourceType)){
                    if (ImGui::Selectable(resource->id().c_str(), m_selectedInspectable == resource.get())) {
                        m_selectedInspectable = resource.get();
                    }
                }
            }
        }
    }
    ImGui::End();


    // Inspector Panel
    ImGui::Begin("Inspector", 0, ImGuiWindowFlags_NoCollapse & ImGuiWindowFlags_AlwaysAutoResize);
    if (m_selectedInspectable) {

        // Scene Object Inspector
        if(dynamic_cast<SceneObject*>(m_selectedInspectable)){
            EditorUtils::DrawHeader("Scene Object");

            m_selectedInspectable->drawInspector(context);
            ImGui::Separator();
            if (ImGui::Button("Add Component")) {
                // TODO: Display dropdown of available components to add e.g. MeshRenderer, Camera...
                INFO_LOG("Add Component button clicked. (Functionality not implemented yet)");
            }
        
        // Resource Inspector
        } else {
            EditorUtils::DrawHeader("Resource");
            EditorUtils::DrawInspectableName(m_selectedInspectable->inspectorName());
            ImGui::Separator();
            m_selectedInspectable->drawInspector(context);
        }

    }
    ImGui::End();
    

}

void Editor::endFrame(){
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Editor::setupDockspace(){
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
        ImGuiID leftId;
        ImGuiID leftBottomId;
        
        ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.20f, &leftId, &dockspaceId);
        ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Right, 0.25f, &rightId, &dockspaceId);
        ImGui::DockBuilderSplitNode(leftId, ImGuiDir_Down, 0.20f, &leftBottomId, &leftId);

        ImGui::DockBuilderDockWindow("Hierarchy", leftId);
        ImGui::DockBuilderDockWindow("Resources", leftBottomId);
        ImGui::DockBuilderDockWindow("Viewport", dockspaceId);
        ImGui::DockBuilderDockWindow("Inspector", rightId);

        ImGui::DockBuilderFinish(dockspaceId);
    }

    ImGui::End();
}


Inspectable* Editor::getSelectedSceneObject() const{
    if(m_selectedInspectable){
        if(dynamic_cast<SceneObject*>(m_selectedInspectable))
            return m_selectedInspectable;
    }

    return nullptr;
}