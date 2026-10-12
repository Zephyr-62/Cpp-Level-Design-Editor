#pragma once

#include <imgui.h>

class Window;
class Scene;
class SceneObject;
class ResourceManager;
class Inspectable;
struct ApplicationContext;

class Editor {
public:
    explicit Editor(const Window& window);
    ~Editor();

    Editor(const Editor&) = delete;
    Editor& operator=(const Editor&) = delete;

    void processInput(Scene& scene, ApplicationContext& context);

    void beginFrame();
    void draw(Scene& scene, ApplicationContext& context, unsigned int colorTexture);
    void endFrame();

    ImVec2 viewportPanelSize() const { return m_viewportSize; }
    bool isViewportFocused() const { return m_viewportFocused; }

    SceneObject* getSelectedSceneObject() const;
    void clearSelectedSceneObject();

private:
    void setupDockspace();

    void drawViewport(ApplicationContext& context, unsigned int colorTexture);
    void drawHierarchy(Scene& scene);
    void drawResourcesPanel(ApplicationContext& context);
    void drawInspector(ApplicationContext& context);


    ImVec2 m_viewportSize;
    Inspectable* m_selectedInspectable = nullptr;
    bool m_viewportFocused = false;
    bool m_showViewportStats = true;


    // Input flags
    bool m_input_newObjectRequested = false;
    bool m_input_duplicateSelectedObject = false;
    bool m_input_deleteSelectedObjects = false;
};