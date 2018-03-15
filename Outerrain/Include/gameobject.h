#pragma once

#include <vector>

#include "transform.h"
#include "vec.h"
#include "quaternion.h"

class Component;

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

	GameObject* parent = nullptr;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
	bool transformNeedsToUpdate = true;

public:
	// Constructor & Destructor
	GameObject();
	~GameObject();

	// Names
	void SetName(const std::string& s);
	std::string GetName() const;

	// Components
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
	template<typename T> T* GetComponent() const
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			T* castAttempt = dynamic_cast<T*>(components[i]);
			if (castAttempt != nullptr)
				return castAttempt;
		}
		return nullptr;
	}
	std::vector<Component*> GetAllComponents() const;

	// Children
	void AddChild(GameObject* child);
	void RemoveChildAt(int i);
	GameObject* GetChildAt(int i) const;
	void SetParent(GameObject* parent);
	GameObject* GetParent() const;

	// Position, Scale, Rotation
	Vector3 GetPosition() const;
	void SetPosition(const Vector3& vector);
	void SetPosition(float x, float y, float z);

	Vector3 GetScale() const;
	void SetScale(const Vector3& vector);
	void SetScale(float x, float y, float z);

	void SetRotation(TQuaternion<float, Vector3> quat);
	void RotateAround(const Vector3& axis, float degrees);
	void RotateAroundRadian(const Vector3& axis, float radian);
	void LookAt(const Vector3& destPoint);
	void LookAtUpVector(const Vector3& destPoint);

	// Transform
	Vector3 GetRightVector();
	Vector3 GetUpVector();
	Vector3 GetForwardVector();
	Transform GetTRS();
	Transform GetObjectToWorldMatrix();
	void MarkTransformAsChanged();
	void UpdateTransformIfNeeded();
};
