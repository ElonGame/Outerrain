#pragma once
#include <vector>
#include "component.h"
#include "transform.h"
#include "vec.h"
#include "quaternion.h"

class GameObject
{
private:
	std::string name = "GameObject";
	Transform translation = Identity();
	Transform rotation = Identity();
	Transform scale = Identity();
	Transform localTRS = Identity();
	Transform objectToWorld = Identity();
	TQuaternion<float, Vector3> rotationQuat;

	std::vector<Component*> components;
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	bool transformNeedsToUpdate = true;

public:
	GameObject() 
	{
		rotationQuat = TQuaternion<float, Vector3>(0, 0, 0, 1);
	}

	~GameObject();

	void CallStartOnComponents();
	void SetName(std::string s);
	std::string GetName();

	/*-------------Components-------------*/
	void AddComponent(Component* component);
	template<typename T> void RemoveComponent()
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			T* castAttempt = dynamic_cast<T*>(components[i]);
			if (castAttempt != nullptr)
			{
				auto it = components.erase(components.begin() + i);
				return;
			}
		}
	}
	template<typename T> T* GetComponent()
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			T* castAttempt = dynamic_cast<T*>(components[i]);
			if (castAttempt != nullptr)
				return castAttempt;
		}
		return nullptr;
	}
	std::vector<Component*> GetAllComponents();

	/*-------------Children-------------*/
	void AddChild(GameObject* child);
	void RemoveChildAt(int i);
	GameObject* GetChildAt(int i);
	std::vector<GameObject*> GetAllChildren();
	void SetParent(GameObject* parent);
	GameObject* GetParent();

	/*-------------Position Functions-------------*/
	Vector3 GetPosition();
	void SetPosition(Vector3 vector);
	void SetPosition(float x, float y, float z);

	/*-------------Scale Functions-------------*/
	Vector3 GetScale();
	void SetScale(Vector3 vector);
	void SetScale(float x, float y, float z);

	/*-------------Rotation Functions-------------*/
	void SetRotation(TQuaternion<float, Vector3> quat);
	void RotateAround(Vector3 axis, float degrees);
	void RotateAroundRadian(Vector3 axis, float radian);
	void LookAt(Vector3 destPoint);
	void LookAtUpVector(Vector3 destPoint);

	/*-------------Transform Management-------------*/
	Vector3 GetRightVector();
	Vector3 GetUpVector();
	Vector3 GetForwardVector();
	Transform GetTRS();
	Transform GetObjectToWorldMatrix();
	void MarkTransformAsChanged();
	void UpdateTransformIfNeeded();
};