#include "scene-hierarchy.h"

SceneHierarchy::SceneHierarchy()
{

}

SceneHierarchy::~SceneHierarchy()
{
}

GameObject* SceneHierarchy::GetObject(int index) const
{
	return sceneObjects[index];
}

GameObject* SceneHierarchy::GetObject(const std::string& name) const
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		if (sceneObjects[i]->GetName() == name)
			return sceneObjects[i];
	}
	return nullptr;
}

void SceneHierarchy::AddObject(GameObject* obj)
{
	sceneObjects.push_back(obj);
}

void SceneHierarchy::DeleteObject(int index)
{
	if (sceneObjects.size() > index)
	{
		delete sceneObjects[index];
		sceneObjects.erase(sceneObjects.begin() + index);
	}
}

void SceneHierarchy::DeleteObject(const std::string& name)
{
	int index = -1;
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		if (sceneObjects[i]->GetName() == name)
		{
			index = i;
			break;
		}
	}
	DeleteObject(index);
}

void SceneHierarchy::DeleteObject(GameObject* obj)
{
	int index = -1;
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		if (sceneObjects[i] == obj)
		{
			index = i;
			break;
		}
	}
	DeleteObject(index);
}

std::vector<GameObject*> SceneHierarchy::GetAllObjects() const
{
	return sceneObjects;
}
