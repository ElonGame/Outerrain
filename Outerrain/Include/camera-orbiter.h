#pragma once

#include "vec.h"
#include "transform.h"
#include "camera-base.h"
#include "ray.h"

class Box;

class CameraOrbiter : public CameraBase
{
protected:
	Vector3 center;
	Vector2 position;
	Vector2 rotation;
	float size;

public:
	CameraOrbiter();
	CameraOrbiter(const Vector3& center, const float size, const float nearplane, const float farplane);
	CameraOrbiter(const Vector3& pmin, const Vector3& pmax);

	void LookAt(const Vector3& center, const float size);
	void LookAt(const Vector3& pmin, const Vector3& pmax);
	void LookAt(const Box&);

	Ray PixelToRay(const Vector2i& pixel);
	Vector2i VectorToPixel(const Vector3& worldPoint);

	void Rotation(float x, float y);
	void Translation(float x, float y);
	void Move(float z);
	Transform ViewDirection() const;
	Transform Projection() const;
	void Frame(float frameWidth, float frameHeight, float z, Vector3& dO, Vector3& dx, Vector3& dy) const;
	Vector3 Position() const;
};
