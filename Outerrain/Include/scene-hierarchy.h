#pragma once
#include "gameobject.h"

class SceneHierarchy
{
private:
	std::vector<GameObject*> sceneObjects;

public:
	SceneHierarchy();
	~SceneHierarchy();

	GameObject* GetObject(int index) const;
	GameObject* GetObject(const std::string& name) const;

	void AddObject(GameObject* obj);
	void DeleteObject(int index);
	void DeleteObject(const std::string& name);
	void DeleteObject(GameObject* obj);

	std::vector<GameObject*> GetAllObjects() const;
};
