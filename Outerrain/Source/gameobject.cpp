#include "gameobject.h"


void GameObject::CallStartOnComponents()
{
	for (int i = 0; i < components.size(); i++)
		components[i]->Start();
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

void GameObject::SetName(std::string s)
{
	name = s;
}

std::string GameObject::GetName()
{
	return name;
}


/*-------------Components-------------*/
void GameObject::AddComponent(Component* component)
{
	component->SetGameObject(this);
	this->components.push_back(component);
}

std::vector<Component*> GameObject::GetAllComponents()
{
	return components;
}
/*-------------Components-------------*/


/*-------------Children-------------*/
void GameObject::AddChild(GameObject* child)
{
	child->SetParent(this);
	this->children.push_back(child);
}

void GameObject::RemoveChildAt(int i)
{
	children.erase(children.begin() + i);
}

GameObject* GameObject::GetChildAt(int i)
{
	return children[i];
}

std::vector<GameObject*> GameObject::GetAllChildren()
{
	return children;
}

void GameObject::SetParent(GameObject* parent)
{
	this->parent = parent;
}

GameObject* GameObject::GetParent()
{
	return this->parent;
}
/*-------------Components-------------*/


/*-------------Position Functions-------------*/
Vector3 GameObject::GetPosition()
{
	return Vector3(translation.m[0][3], translation.m[1][3], translation.m[2][3]);
}

void GameObject::SetPosition(Vector3 vector)
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
/*-------------Position Functions-------------*/


/*-------------Scale Functions-------------*/
Vector3 GameObject::GetScale()
{
	return Vector3(scale.m[0][0], translation.m[1][1], translation.m[2][2]);
}

void GameObject::SetScale(Vector3 vector)
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
/*-------------Scale Functions-------------*/


/*-------------Rotation Functions-------------*/
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
	/*rotation.m[2][0] = -rotation.m[2][0];
	rotation.m[2][1] = -rotation.m[2][1];
	rotation.m[2][2] = -rotation.m[2][2];*/
	MarkTransformAsChanged();
}

void GameObject::RotateAround(Vector3 axis, float degrees)
{
	degrees *= 0.0174533f;
	TQuaternion<float, Vector3> quat = TQuaternion<float, Vector3>(axis, degrees);
	SetRotation(rotationQuat * quat);
}

void GameObject::RotateAroundRadian(Vector3 axis, float radian)
{
	TQuaternion<float, Vector3> quat = TQuaternion<float, Vector3>(axis, radian);
	SetRotation(rotationQuat * quat);
}

void GameObject::LookAt(Vector3 destPoint)
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

	float rotAngle = (float)acos(dotValue);
	Vector3 rotAxis = Cross(GetForwardVector(), newForward);
	rotAxis = Normalize(rotAxis);
	RotateAroundRadian(rotAxis, -rotAngle);
}

void GameObject::LookAtUpVector(Vector3 destPoint)
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

	float rotAngle = (float)acos(dotValue);
	Vector3 rotAxis = Cross(-GetUpVector(), newForward);
	rotAxis = Normalize(rotAxis);
	RotateAroundRadian(rotAxis, -rotAngle);
}
/*-------------Rotation Functions-------------*/


/*-------------Transform Management-------------*/
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

		if (this->parent != nullptr)
			objectToWorld = localTRS * this->parent->GetObjectToWorldMatrix();
		else
			objectToWorld = localTRS;
		transformNeedsToUpdate = false;
	}
}
/*-------------Transform Management-------------*/