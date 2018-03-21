#pragma once

class Camera
{
protected:
	int frameWidth;
	int frameHeight;
	float zNear;
	float zFar;
	float fov;

public:
	Camera();
	Camera(int width, int height, float near, float far, float fov=45.0f);

	void SetFrameWidth(int w);
	void SetFrameHeight(int h);
	int FrameWidth() const;
	int FrameHeight() const;
	void SetClippingPlanes(float n, float f);
};
