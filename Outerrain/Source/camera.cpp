#include "camera-base.h"

CameraBase::CameraBase() : frameWidth(0), frameHeight(0), nearplane(0.1f), farplane(5000.0f), fov(45.0f)
{
}

CameraBase::CameraBase(int frameWidth, int frameHeight, float near, float far, float f) : frameWidth(frameWidth), frameHeight(frameHeight), nearplane(near), farplane(far), fov(f)
{

}

void CameraBase::SetFrameWidth(int w)
{
	frameWidth = w;
}

void CameraBase::SetFrameHeight(int h)
{
	frameHeight = h;
}

int CameraBase::FrameWidth() const
{
	return frameWidth;
}

int CameraBase::FrameHeight() const
{
	return frameHeight;
}

void CameraBase::SetClippingPlanes(float n, float f)
{
	nearplane = n;
	farplane = f;
}
