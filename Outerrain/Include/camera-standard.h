#pragma once

#include "camera-base.h"
#include "box.h"

class CameraStandard : public CameraBase
{
protected:
	Vector3 eye;
	Vector3 at;
	Vector3 up;
	float cah;

public:
	CameraStandard();
	CameraStandard(const Vector3&, const Vector3&, const Vector3& = Vector3(0.0f, 0.0f, 1.0f), float = 1.0, float = 1.0, float = 1.0, float = 100000.0);

	Vector3 At() const;
	Vector3 Eye() const;
	Vector3 Up() const;
	Vector3 ViewDirection() const;

	float GetCameraApertureH() const;
	float GetAngleOfViewH(float, float) const;
	float GetAngleOfViewV(float, float) const;

	void MoveForward(float);
	void RotateAroundUp(float);
	void ResetVertical();
	void UpDown(float);
	void MoveSideway(float);
	void Zoom(float);
	void LeftRightRound(float);
	void UpDownRound(float);

	void SlideHorizontal(float);
	void SetAt(const Vector3&);
	void SetEye(const Vector3&);

	// Move camera in a plane
	void UpDownVertical(float);
	void LeftRightHorizontal(float);

	Ray PixelToRay(int, int, int, int) const;
	bool VectorToPixel(const Vector3&, float&, float&, int, int) const;
};

inline Vector3 CameraStandard::At()const {
	return at;
}

inline Vector3 CameraStandard::Eye()const {
	return eye;
}

inline Vector3 CameraStandard::Up()const {
	return up;
}

inline Vector3 CameraStandard::ViewDirection() const {
	return at - eye;
}

inline void CameraStandard::SetAt(const Vector3& p) {
	at = p;
	up = Vector3(0.0, 0.0, 1.0);
}

inline void CameraStandard::SetEye(const Vector3& p) {
	eye = p;
}

inline float CameraStandard::GetCameraApertureH() const {
	return cah;
}
