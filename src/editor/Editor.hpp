#pragma once

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
    void draw(Scene& scene, ApplicationContext& context);
    void endFrame();

private:
    void setupDockspace();

    Inspectable* m_selectedInspectable = nullptr;

};