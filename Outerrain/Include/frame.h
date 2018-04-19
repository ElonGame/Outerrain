#pragma once
#include "transform.h"
#include "vec.h"
#include "quaternion.h"

class Frame
{
protected:
	Transform translation = Identity();
	Transform rotation = Identity();
	Transform scale = Identity();

public:
	Frame();
	~Frame();

	void SetRotation(const TQuaternion<float, Vector3>& quat);

	Vector3 GetPosition() const;
	void SetPosition(const Vector3& vector);

	Vector3 GetScale() const;
	void SetScale(const Vector3& vector);

	Transform GetTRS() const;
};
