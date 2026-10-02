#pragma once

#include <iostream>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Level Editor"

#define INFO_LOG(msg) std::cout << "[INFO] " << msg << std::endl
#define WARN_LOG(msg) std::cout << "[WARN] " << msg << std::endl
#define ERROR_LOG(msg, err_code) std::cerr << "[ERROR] " << msg << " (Error Code: " << err_code << ")" << std::endl

// Error codes
#define ERROR_CODE_SUCCESS 0

#define ERROR_CODE_GLFW_INIT_FAILED 1
#define ERROR_CODE_GLFW_WINDOW_CREATION_FAILED 2
#define ERROR_CODE_GLAD_INIT_FAILED 3
#define ERROR_CODE_IMGUI_INIT_FAILED 4

#define ERROR_CODE_SHADER_FILE_NOT_FOUND 10
#define ERROR_CODE_SHADER_COMPILATION_FAILED 11
#define ERROR_CODE_SHADER_LINKING_FAILED 12
#define ERROR_CODE_PROPERTY_NOT_FOUND 13

#define ERROR_CODE_APPLICATION_NOT_INITIALIZED 20

