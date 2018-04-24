#pragma once

class CameraBase
{
protected:
	int frameWidth;
	int frameHeight;
	float nearplane;
	float farplane;
	float fov;

public:
	CameraBase();
	CameraBase(int frameWidth, int frameHeight, float near, float far, float fov = 45.0f);

	void SetFrameWidth(int w);
	void SetFrameHeight(int h);
	int FrameWidth() const;
	int FrameHeight() const;
	void SetClippingPlanes(float n, float f);
};
