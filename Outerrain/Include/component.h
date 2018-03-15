#pragma once

#include <memory>
#include "gameobject.h"

class Component
{
protected:
	std::unique_ptr<GameObject> gameObject;

public:
	Component()
	{

	}

	virtual ~Component()
	{
		gameObject.release();
	}
	
	void SetGameObject(GameObject* o)
	{
		gameObject = std::unique_ptr<GameObject>(o);
	}

	GameObject* GetGameObject()
	{
		return gameObject.get();
	}
};
