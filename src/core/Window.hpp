#pragma once

#include "core/GLCommon.hpp"
#include <GLFW/glfw3.h>

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

private:
    GLFWwindow* m_window = nullptr;
};