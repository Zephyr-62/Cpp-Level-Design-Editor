#pragma once

class ResourceManager;
class EditorCamera;

struct EditorContext {
    ResourceManager& resourceManager;
    EditorCamera& camera;
    float viewportAspectRatio = 1.0f;
};