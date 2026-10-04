#pragma once

class ResourceManager;
class EditorCamera;
class Window;

struct ApplicationContext {
    ResourceManager& resourceManager;
    EditorCamera& camera;
    Window& window;
    float viewportAspectRatio = 1.0f;
};