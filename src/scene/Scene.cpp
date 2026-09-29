
#include "scene/Scene.hpp"

SceneObject& Scene::createObject(std::string name) {
	auto newObj = std::make_unique<SceneObject>(std::move(name));
	SceneObject& ref = *newObj;
	m_objects.push_back(std::move(newObj));

	return ref;
}