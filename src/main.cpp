#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <iostream>

#include "Shader.hpp"

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
    // Triangle vertex data: 3 vertices, XYZ each
    float const vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader triangleShader("shaders/triangle.vert", "shaders/triangle.frag");

    auto lastFrameTime = std::chrono::steady_clock::now();
    float totalTime = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        auto const currentFrameTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> const elapsedTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        totalTime += elapsedTime.count();

        glClearColor(0.10f, 0.10f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), totalTime, glm::vec3(0.0f, 1.0f, 0.5f));

        triangleShader.use();
        triangleShader.setMat4("model", model);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}