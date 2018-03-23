#include "frame.h"

Frame::Frame()
{
	SetRotation(TQuaternion<float, Vector3>(0, 0, 0, 1));
	SetPosition(Vector3(0));
}

Frame::~Frame()
{

}

void Frame::SetRotation(const TQuaternion<float, Vector3>& quat)
{
	const float* temp = quat.matrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			rotation.m[i][j] = temp[i * 4 + j];
		}
	}
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

Transform Frame::GetMatrix() const
{
	return translation * rotation * scale;
}
