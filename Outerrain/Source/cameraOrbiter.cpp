#include <cstdio>
#include <algorithm>
#include "cameraOrbiter.h"
#include "mathUtils.h"


CameraOrbiter::CameraOrbiter() 
	: center(), position(), rotation(), size(5.0f), zNear(0.1f), zFar(1000.0f)
{
}

CameraOrbiter::CameraOrbiter(const Vector3& center, float size, float zNear, float zFar) 
	: center(center), position(), rotation(), size(size), zNear(zNear), zFar(zFar)
{
}

CameraOrbiter::CameraOrbiter(const Vector3& pmin, const Vector3& pmax) 
	: center(Math::Center(pmin, pmax)), position(), rotation(), size(Magnitude(pmin - pmax)), 
		zNear(0.1f), zFar(1000.0f)
{
}

void CameraOrbiter::LookAt(const Vector3& c, float s)
{
	center = c;
	position = Vector2(0, 0);
	rotation = Vector2(0, 0);
	size = s;
}

void CameraOrbiter::LookAt(const Vector3& pmin, const Vector3& pmax)
{
	LookAt(Math::Center(pmin, pmax), Magnitude(pmin - pmax));
}

void CameraOrbiter::LookAt(const Bounds& b)
{
	LookAt(b.a, b.b);
}

void CameraOrbiter::Rotation(float x, float y)
{
	rotation.x = rotation.x + y;
	rotation.y = rotation.y + x;
}

void CameraOrbiter::Translation(float x, float y)
{
	position.x = position.x - size * x;
	position.y = position.y + size * y;
}

void CameraOrbiter::Move(float z)
{
	size = size - size * 0.01f * z;
	if (size < 0.01f)
		size = 0.01f;
}

Transform CameraOrbiter::View() const
{
	return TranslationTransform(-position.x, -position.y, -size)
		* RotationX(rotation.x) * RotationY(rotation.y)
		* TranslationTransform(-Vector3(center));
}

Transform CameraOrbiter::Projection(float width, float height, float fov) const
{
	return Perspective(fov, width / height, zNear, zFar);
}

void CameraOrbiter::Frame(float width, float height, float z, float fov, Vector3& dO, Vector3& dx, Vector3& dy) const
{
	Transform v = View();
	Transform p = Projection(width, height, fov);
	Transform viewport = Viewport(width, height);
	Transform t = viewport * p * v;              // passage monde vers image
	Transform tinv = t.Inverse();                // l'inverse, passage image vers monde

	// origine du plan image
	dO = tinv(Vector3(0, 0, z));

	// axe x du plan image
	Vector3 d1 = tinv(Vector3(1, 0, z));

	// axe y du plan image
	Vector3 d2 = tinv(Vector3(0, 1, z));

	dx = Vector3(dO - d1);
	dy = Vector3(dO - d2);
}

Vector3 CameraOrbiter::Position() const
{
	Transform t = View(); // passage monde vers camera
	Transform tinv = t.Inverse(); // l'inverse, passage camera vers monde
	return tinv(Vector3(0, 0, 0)); // la camera se trouve a l'origine, dans le repere camera...
}

void CameraOrbiter::SetFrameWidth(int w)
{ 
	frameWidth = w;
}

void CameraOrbiter::SetFrameHeight(int h)
{ 
	frameHeight = h; 
}

int CameraOrbiter::FrameWidth() const
{ 
	return frameWidth; 
}

int CameraOrbiter::FrameHeight() const
{ 
	return frameHeight; 
}

void CameraOrbiter::SetClippingPlanes(float n, float f)
{
	zNear = n;
	zFar = f;
}
