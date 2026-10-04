#include "core/Window.hpp"
#include "core/Constants.hpp"

Window::Window(int width, int height, const char* title){
    if (!glfwInit()) {
        ERROR_LOG("Failed to initialize GLFW.", ERROR_CODE_GLFW_INIT_FAILED);
        return;
    }

    // Ask for an OpenGL 3.3 core-profile context, matching what we told GLAD to generate
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        ERROR_LOG("Failed to create GLFW window.", ERROR_CODE_GLFW_WINDOW_CREATION_FAILED);
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        ERROR_LOG("Failed to initialize GLAD.", ERROR_CODE_GLAD_INIT_FAILED);
        return;
    }
}

Window::~Window(){
    glfwTerminate();
}

bool Window::shouldClose() const{
    return glfwWindowShouldClose(m_window);
}

void Window::pollEvents(){
    glfwPollEvents();
}

void Window::swapBuffers(){
    glfwSwapBuffers(m_window);
}
