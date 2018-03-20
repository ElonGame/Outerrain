#pragma once
#include "transform.h"
#include "vec.h"

class Frame
{
protected:
	Transform translation = Identity();
	Transform rotation = Identity();
	Transform scale = Identity();

public:
	Frame();

	Vector3 GetPosition() const;
	void SetPosition(const Vector3& vector);
	void SetPosition(float x, float y, float z);
	Vector3 GetScale() const;
	void SetScale(const Vector3& vector);
	void SetScale(float x, float y, float z);

	Transform GetMatrix() const;
};
