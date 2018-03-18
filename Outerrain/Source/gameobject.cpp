#include "gameobject.h"
#include "component.h"


/* Constructor & Destructor */
GameObject::GameObject()
{
	rotationQuat = TQuaternion<float, Vector3>(0, 0, 0, 1);
	SetPosition(Vector3(0));
}

GameObject::~GameObject()
{
	for (int i = 0; i < children.size(); i++)
		delete children[i];
	children.clear();

	for (int i = 0; i < components.size(); i++)
		delete components[i];
	components.clear();
}


/* Name */
void GameObject::SetName(const std::string& s)
{
	name = s;
}

std::string GameObject::GetName() const
{
	return name;
}


/* Components */
void GameObject::AddComponent(Component* component)
{
	component->SetGameObject(this);
	components.push_back(component);
}

std::vector<Component*> GameObject::GetAllComponents() const
{
	return components;
}


/* Children*/
void GameObject::AddChild(GameObject* child)
{
	child->SetParent(this);
	children.push_back(child);
}

void GameObject::RemoveChildAt(int i)
{
	children.erase(children.begin() + i);
}

GameObject* GameObject::GetChildAt(int i) const
{
	return children[i];
}

void GameObject::SetParent(GameObject* parent)
{
	parent = parent;
}

GameObject* GameObject::GetParent() const
{
	return parent;
}


/* Position, Scale, Rotation */
Vector3 GameObject::GetPosition() const
{
	return Vector3(translation.m[0][3], translation.m[1][3], translation.m[2][3]);
}

void GameObject::SetPosition(const Vector3& vector)
{
	translation.m[0][3] = vector.x;
	translation.m[1][3] = vector.y;
	translation.m[2][3] = vector.z;
	MarkTransformAsChanged();
}

void GameObject::SetPosition(float x, float y, float z)
{
	translation.m[0][3] = x;
	translation.m[1][3] = y;
	translation.m[2][3] = z;
	MarkTransformAsChanged();
}

Vector3 GameObject::GetScale() const
{
	return Vector3(scale.m[0][0], translation.m[1][1], translation.m[2][2]);
}

void GameObject::SetScale(const Vector3& vector)
{
	scale.m[0][0] = vector.x;
	scale.m[1][1] = vector.y;
	scale.m[2][2] = vector.z;
	MarkTransformAsChanged();
}

void GameObject::SetScale(float x, float y, float z)
{
	scale.m[0][0] = x;
	scale.m[1][1] = y;
	scale.m[2][2] = z;
	MarkTransformAsChanged();
}

void GameObject::SetRotation(TQuaternion<float, Vector3> quat)
{
	rotationQuat = quat;
	const float* temp = rotationQuat.matrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			rotation.m[i][j] = temp[i * 4 + j];
		}
	}
	MarkTransformAsChanged();
}

void GameObject::RotateAround(const Vector3& axis, float degrees)
{
	degrees *= 0.0174533f;
	SetRotation(rotationQuat * TQuaternion<float, Vector3>(axis, degrees));
}

void GameObject::RotateAroundRadian(const Vector3& axis, float radian)
{
	SetRotation(rotationQuat * TQuaternion<float, Vector3>(axis, radian));
}

void GameObject::LookAt(const Vector3& destPoint)
{
	Vector3 newForward = Normalize(GetPosition() - destPoint);

	float dotValue = Dot(GetForwardVector(), newForward);
	if (dotValue > 1.0f)
		dotValue = 1.0f;

	if (abs(dotValue + 1.0f) < 0.000001f)
	{
		RotateAroundRadian(GetUpVector(), 3.14159265f);
		return;
	}

	if (abs(dotValue - 1.0f) < 0.000001f)
	{
		return;
	}

	float rotAngle = static_cast<float>(acos(dotValue));
	Vector3 rotAxis = Cross(GetForwardVector(), newForward);
	rotAxis = Normalize(rotAxis);
	RotateAroundRadian(rotAxis, -rotAngle);
}

void GameObject::LookAtUpVector(const Vector3& destPoint)
{
	Vector3 newForward = Normalize(GetPosition() - destPoint);

	float dotValue = Dot(-GetUpVector(), newForward);
	if (dotValue > 1.0f)
		dotValue = 1.0f;

	if (abs(dotValue + 1.0f) < 0.000001f)
	{
		RotateAroundRadian(GetRightVector(), 3.14159265f);
		return;
	}

	if (abs(dotValue - 1.0f) < 0.000001f)
	{
		return;
	}

	float rotAngle = static_cast<float>(acos(dotValue));
	Vector3 rotAxis = Cross(-GetUpVector(), newForward);
	rotAxis = Normalize(rotAxis);
	RotateAroundRadian(rotAxis, -rotAngle);
}


/* Transform */
Vector3 GameObject::GetRightVector()
{
	return rotation[0];
}

Vector3 GameObject::GetUpVector()
{
	return rotation[1];
}

Vector3 GameObject::GetForwardVector()
{
	return rotation[2];
}

Transform GameObject::GetTRS()
{
	return localTRS;
}

Transform GameObject::GetObjectToWorldMatrix()
{
	return objectToWorld;
}

void GameObject::MarkTransformAsChanged()
{
	transformNeedsToUpdate = true;
	for (int i = 0; i < children.size(); i++)
		children[i]->MarkTransformAsChanged();
}

void GameObject::UpdateTransformIfNeeded()
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

		if (parent != nullptr)
			objectToWorld = localTRS * parent->GetObjectToWorldMatrix();
		else
			objectToWorld = localTRS;
		transformNeedsToUpdate = false;
	}
}
