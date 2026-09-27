cmake_minimum_required(VERSION 3.20)
project(ProceduralNodeEditor CXX)

# Require modern C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ------------------------------------------------------------------------------
# 1. Fetch GLFW (Windowing & Input)
# ------------------------------------------------------------------------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# ------------------------------------------------------------------------------
# 2. Fetch GLM (OpenGL Mathematics)
# ------------------------------------------------------------------------------
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)


FetchContent_Declare(
    dearIMGUI
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.89.2
)
FetchContent_MakeAvailable(dearIMGUI)

# ------------------------------------------------------------------------------
# 3. Add Executable & Source Files
# ------------------------------------------------------------------------------
add_executable(ProceduralNodeEditor src/main.cpp)

target_link_libraries(ProceduralNodeEditor PRIVATE glfw glm::glm opengl32 dearIMGUI)
target_include_directories(ProceduralNodeEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)