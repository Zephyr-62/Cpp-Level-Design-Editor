
#include "scene/Scene.hpp"

SceneObject& Scene::createObject(std::string name) {
	auto newObj = std::make_unique<SceneObject>(std::move(name));
	SceneObject& ref = *newObj;
	m_objects.push_back(std::move(newObj));

	return ref;
}

void Scene::removeObject(SceneObject* obj){
	m_deletePendingObjs.insert(obj);
}

void Scene::deletePendingObjects(ApplicationContext& context){
	if(m_deletePendingObjs.empty())
		return;

	std::erase_if(m_objects, [&](const std::unique_ptr<SceneObject>& obj){
		return m_deletePendingObjs.contains(obj.get());
	});
	m_deletePendingObjs.clear();
	
	context.hierarchyDeselectRequested = true;
}