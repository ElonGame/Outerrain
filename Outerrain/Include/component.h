#pragma once

class GameObject;

class Component
{
protected:
	GameObject* gameObject = nullptr;

public:
	virtual void Start() { }

	virtual void OnDestroy() { }

	virtual void Update(float dt) { }

	void SetGameObject(GameObject* gameobject)
	{
		this->gameObject = gameobject;
	}
	
	GameObject* GetGameObject()
	{
		return this->gameObject;
	}
};