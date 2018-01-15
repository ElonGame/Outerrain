#ifndef _ORBITER_H
#define _ORBITER_H

#include "vec.h"
#include "transform.h"

class CameraOrbiter
{
public:
	CameraOrbiter();
	CameraOrbiter(const Vector3& center, const float size, const float zNear, const float zFar);
	CameraOrbiter(const Vector3& pmin, const Vector3& pmax);

	void LookAt(const Vector3& center, const float size);
	void LookAt(const Vector3& pmin, const Vector3& pmax);
	void LookAt(const Bounds&);

	void Rotation(const float x, const float y);
	void Translation(const float x, const float y);
	void Move(const float z);
	Transform View() const;
	Transform Projection(const float width, const float height, const float fov) const;
	void Frame(const float width, const float height, const float z, const float fov, Vector3& dO, Vector3& dx, Vector3& dy) const;
	 
	Vector3 Position() const;
	void SetFrameWidth(int w);
	void SetFrameHeight(int h);
	int FrameWidth() const;
	int FrameHeight() const;
	void SetClippingPlanes(float n, float f);

protected:
	Vector3 center;
	Vector2 position;
	Vector2 rotation;
	float size;
	int frameWidth, frameHeight;
	float zNear = 1.0f;
	float zFar = 1000.0f;
};

#endif
