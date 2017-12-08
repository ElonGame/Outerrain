#pragma once
#include <vector>
#include "component.h"
#include "transform.h"
#include "vec.h"
//#include "quaternion.h"

class GameObject
{
private:
	std::string name = "GameObject";
	Transform translation = Identity();
	Transform rotation = Identity();
	Transform scale = Identity();
	Transform localTRS = Identity();
	Transform objectToWorld = Identity();
	//TQuaternion<float, Vector3> rotationQuat = TQuaternion<float, Vector3>(0, 0, 0, 1);

	std::vector<Component*> components;
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;
	bool transformNeedsToUpdate = true;

public:
	/*-------------Components-------------*/
	void AddComponent(Component* component)
	{
		component->SetGameObject(this);
		this->components.push_back(component);
	}

	template<typename T> void RemoveComponent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			T* castAttempt = dynamic_cast<T*>(components[i]);
			if (castAttempt != nullptr)
				components.erase(components.begin() + i);
		}
		return nullptr;
	}

	template<typename T> T* GetComponent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			T* castAttempt = dynamic_cast<T*>(components[i]);
			if (castAttempt != nullptr)
				return castAttempt;
		}
		return nullptr;
	}

	std::vector<Component*> GetAllComponents()
	{
		return components;
	}
	/*-------------Components-------------*/


	/*-------------Children-------------*/
	void AddChild(GameObject* child)
	{
		child->SetParent(this);
		this->children.push_back(child);
	}
	
	void RemoveChildAt(int i)
	{
		children.erase(children.begin() + i);
	}
	
	GameObject* GetChildAt(int i)
	{
		return children[i];
	}
	
	std::vector<GameObject*> GetAllChildren()
	{
		return children;
	}
	
	void SetParent(GameObject* parent)
	{
		this->parent = parent;
	}

	GameObject* GetParent()
	{
		return this->parent;
	}
	/*-------------Components-------------*/


	/*-------------Position Functions-------------*/
	Vector3 GetPosition()
	{
		return Vector3(translation.m[0][3], translation.m[1][3], translation.m[2][3]);
	}

	void SetPosition(Vector3 vector)
	{
		translation.m[0][3] = vector.x;
		translation.m[1][3] = vector.y;
		translation.m[2][3] = vector.z;
		MarkTransformAsChanged();
	}

	void SetPosition(float x, float y, float z)
	{
		translation.m[0][3] = x;
		translation.m[1][3] = y;
		translation.m[2][3] = z;
		MarkTransformAsChanged();
	}
	/*-------------Position Functions-------------*/


	/*-------------Scale Functions-------------*/
	Vector3 GetScale()
	{
		return Vector(scale.m[0][0], translation.m[1][1], translation.m[2][2]);
	}

	void SetScale(Vector3 vector)
	{
		scale.m[0][0] = vector.x;
		scale.m[1][1] = vector.y;
		scale.m[2][2] = vector.z;
		MarkTransformAsChanged();
	}

	void SetScale(float x, float y, float z)
	{
		scale.m[0][0] = x;
		scale.m[1][1] = y;
		scale.m[2][2] = z;
		MarkTransformAsChanged();
	}
	/*-------------Scale Functions-------------*/


	/*-------------Rotation Functions-------------*/
	//TQuaternion<float, Vector> GetRotation()
	//{
	//	return rotationQuat;
	//}

	///*!
	//*  \brief Applique une rotation locale au gameobject.
	//*  \param : quat la rotation en quaternion.
	//*/
	//void SetRotation(TQuaternion<float, Vector> quat)
	//{
	//	rotationQuat = quat;
	//	const float* temp = rotationQuat.matrix();
	//	for (int i = 0; i < 4; i++)
	//	{
	//		for (int j = 0; j < 4; j++)
	//		{
	//			rotation.m[i][j] = temp[i * 4 + j];
	//		}
	//	}
	//	/*rotation.m[2][0] = -rotation.m[2][0];
	//	rotation.m[2][1] = -rotation.m[2][1];
	//	rotation.m[2][2] = -rotation.m[2][2];*/
	//	MarkTransformAsChanged();
	//}

	///*!
	//*  \brief Applique une rotation locale en degr� autour d'un axe.
	//*  \param : axis l'axe autour duquel la rotation aura lieu.
	//*  \param : degrees la roatation en degr�.
	//*/
	//void RotateAround(Vector axis, float degrees)
	//{
	//	degrees *= 0.0174533f;
	//	TQuaternion<float, Vector> quat = TQuaternion<float, Vector>(axis, degrees);
	//	SetRotation(rotationQuat * quat);
	//}

	///*!
	//*  \brief Applique une rotation locale en radian autour d'un axe.
	//*  \param : axis l'axe autour duquel la rotation aura lieu.
	//*  \param : degree la roatation en radian.
	//*/
	//void RotateAroundRadian(Vector axis, float radian)
	//{
	//	TQuaternion<float, Vector> quat = TQuaternion<float, Vector>(axis, radian);
	//	SetRotation(rotationQuat * quat);
	//}

	///*!
	//*  \brief Applique une rotation pour que le vecteur Forward du gameobject regarde dans la direction d'une position.
	//*  \param : destPoint la position � regarder.
	//*/
	//void LookAt(Vector destPoint)
	//{
	//	Vector newForward = normalize(GetPosition() - destPoint);

	//	float dotValue = dot(GetForwardVector(), newForward);
	//	if (dotValue > 1.0f)
	//		dotValue = 1.0f;

	//	if (abs(dotValue + 1.0f) < 0.000001f)
	//	{
	//		RotateAroundRadian(GetUpVector(), 3.14159265f);
	//		return;
	//	}

	//	if (abs(dotValue - 1.0f) < 0.000001f)
	//	{
	//		return;
	//	}

	//	float rotAngle = (float)acos(dotValue);
	//	Vector rotAxis = cross(GetForwardVector(), newForward);
	//	rotAxis = normalize(rotAxis);
	//	RotateAroundRadian(rotAxis, -rotAngle);
	//}

	///*!
	//*  \brief Applique une rotation pour que le vecteur Up du gameobject regarde dans la direction d'une position.
	//*  \param : destPoint la position � regarder.
	//*/
	//void LookAtUpVector(Vector3 destPoint)
	//{
	//	Vector newForward = normalize(GetPosition() - destPoint);

	//	float dotValue = dot(-GetUpVector(), newForward);
	//	if (dotValue > 1.0f)
	//		dotValue = 1.0f;

	//	if (abs(dotValue + 1.0f) < 0.000001f)
	//	{
	//		RotateAroundRadian(GetRightVector(), 3.14159265f);
	//		return;
	//	}

	//	if (abs(dotValue - 1.0f) < 0.000001f)
	//	{
	//		return;
	//	}

	//	float rotAngle = (float)acos(dotValue);
	//	Vector rotAxis = cross(-GetUpVector(), newForward);
	//	rotAxis = normalize(rotAxis);
	//	RotateAroundRadian(rotAxis, -rotAngle);
	//}
	/*-------------Rotation Functions-------------*/


	/*-------------Transform Management-------------*/
	Vector3 GetRightVector()
	{
		return rotation[0];
	}

	Vector3 GetUpVector()
	{
		return rotation[1];
	}

	Vector3 GetForwardVector()
	{
		return rotation[2];
	}

	Transform GetTRS()
	{
		return localTRS;
	}

	Transform GetObjectToWorldMatrix()
	{
		return objectToWorld;
	}

	void MarkTransformAsChanged()
	{
		transformNeedsToUpdate = true;
		for (int i = 0; i < children.size(); i++)
			children[i]->MarkTransformAsChanged();
	}

	void UpdateTransformIfNeeded()
	{
		if (transformNeedsToUpdate)
		{
			// Update local TRS matrix
			localTRS = translation * rotation * scale;

			// Update object to world matrix
			GameObject* temp = parent;
			objectToWorld = Identity();
			while (temp != nullptr && temp->transformNeedsToUpdate == true)
			{
				temp->UpdateTransformIfNeeded();
				temp = temp->GetParent();
			}

			if (this->parent != nullptr)
				objectToWorld = localTRS * this->parent->GetObjectToWorldMatrix();
			else
				objectToWorld = localTRS;
			transformNeedsToUpdate = false;
		}
	}
	/*-------------Transform Management-------------*/


	void SetName(std::string s)
	{
		name = s;
	}

	std::string GetName()
	{
		return name;
	}
};