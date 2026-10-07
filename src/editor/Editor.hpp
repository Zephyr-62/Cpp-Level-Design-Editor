#pragma once

#include <imgui.h>

class Window;
class Scene;
class ResourceManager;
class Inspectable;
struct ApplicationContext;

class Editor {
public:
    explicit Editor(const Window& window);
    ~Editor();

    Editor(const Editor&) = delete;
    Editor& operator=(const Editor&) = delete;

    void beginFrame();
    void draw(Scene& scene, ApplicationContext& context, unsigned int colorTexture);
    void endFrame();

    ImVec2 viewportPanelSize() const { return m_viewportSize; }
    bool isViewportFocused() const { return m_viewportFocused; }

    Inspectable* getSelectedSceneObject() const;

private:
    void setupDockspace();

    ImVec2 m_viewportSize;
    Inspectable* m_selectedInspectable = nullptr;
    bool m_viewportFocused = false;
    bool m_showViewportStats = true;
};