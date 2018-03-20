#include "frame.h"

Frame::Frame()
{

}

Vector3 Frame::GetPosition() const
{
	return Vector3(translation.m[0][3], translation.m[1][3], translation.m[2][3]);
}

void Frame::SetPosition(const Vector3& vector)
{
	translation.m[0][3] = vector.x;
	translation.m[1][3] = vector.y;
	translation.m[2][3] = vector.z;
}

void Frame::SetPosition(float x, float y, float z)
{
	translation.m[0][3] = x;
	translation.m[1][3] = y;
	translation.m[2][3] = z;
}

Vector3 Frame::GetScale() const
{
	return Vector3(scale.m[0][0], translation.m[1][1], translation.m[2][2]);
}

void Frame::SetScale(const Vector3& vector)
{
	scale.m[0][0] = vector.x;
	scale.m[1][1] = vector.y;
	scale.m[2][2] = vector.z;
}

void Frame::SetScale(float x, float y, float z)
{
	scale.m[0][0] = x;
	scale.m[1][1] = y;
	scale.m[2][2] = z;
}

Transform Frame::GetMatrix() const
{
	return translation * rotation * scale;
}
