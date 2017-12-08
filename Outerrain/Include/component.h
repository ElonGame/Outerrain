#pragma once

class GameObject;

class Component
{
protected:
	GameObject* gameObject = nullptr;

public:
	virtual void Start() = 0;

	virtual void OnDestroy() = 0;

	virtual void Update(float dt) = 0;

	void SetGameObject(GameObject* gameobject)
	{
		this->gameObject = gameobject;
	}
	
	GameObject* GetGameObject()
	{
		return this->gameObject;
	}
};