#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Shader.hpp"
#include "resources/Mesh.hpp"


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Ask for an OpenGL 3.3 core-profile context, matching what we told GLAD to generate
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Level Editor", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // GLAD needs a way to ask the driver "where is this function?" per platform;
    // GLFW already knows how to do that, so we hand it GLFW's function.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    {
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

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            auto const currentFrameTime = std::chrono::steady_clock::now();
            std::chrono::duration<float> const elapsedTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;
            totalTime += elapsedTime.count();

            glClearColor(0.10f, 0.10f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glm::mat4 model = glm::rotate(glm::mat4(1.0f), totalTime, glm::vec3(0.0f, 1.0f, 0.5f));

            triangleShader.setMat4("model", model);
		    triangle.draw();

		    // Draw editor UI
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            glfwSwapBuffers(window);
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    glfwTerminate();
    return 0;
}