#pragma once

#include "core/GLCommon.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <functional>

class Window {

public:
    Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();

    GLFWwindow* handle() const { return m_window; }

    // Input
    bool isKeyDown(int key) const;
    bool isMouseButtonDown(int button) const;
    glm::vec2 cursorPosition() const;
    void setCursorMode(int mode) const;

    using ScrollCallback = std::function<void(double xoffset, double yoffset)>;
    void registerScrollCallback(ScrollCallback callback) { m_scrollCallbacks.push_back(callback); }

private:
    GLFWwindow* m_window = nullptr;
    std::vector<ScrollCallback> m_scrollCallbacks;

    void invokeScrollCallbacks(double xoffset, double yoffset);
    static void StaticScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

};