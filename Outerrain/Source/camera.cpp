#include "camera.h"

Camera::Camera() : frameHeight(0), frameWidth(0), zNear(0.1f), zFar(5000.0f)
{
}

Camera::Camera(int width, int height, float near, float far) : frameWidth(width), frameHeight(height), zNear(near), zFar(far)
{

}

void Camera::SetFrameWidth(int w)
{
	frameWidth = w;
}

void Camera::SetFrameHeight(int h)
{
	frameHeight = h;
}

int Camera::FrameWidth() const
{
	return frameWidth;
}

int Camera::FrameHeight() const
{
	return frameHeight;
}

void Camera::SetClippingPlanes(float n, float f)
{
	zNear = n;
	zFar = f;
}
