#include "camera-standard.h"
#include "box2D.h"

/*!
\brief Create a default CameraBis.
*/
CameraStandard::CameraStandard()
{
	CameraStandard::eye = Vector3(0.0);
	CameraStandard::at = Vector3(0.0, 1.0, 0.0);
	CameraStandard::up = Vector3(0.0, 0.0, 1.0);
	CameraStandard::frameWidth = 1.0;
	CameraStandard::frameHeight = 1.0;
	CameraStandard::nearplane = 1.0;
	CameraStandard::farplane = 1000.0;
	CameraStandard::cah = 0.980;
	CameraStandard::fov = 35.0;
}

/*!
\brief This function creates CameraBis given its location and look at point.

If no upward vector is provided, it is internally defined as z-axis.

\image html CameraBis.png

The view vector is defined as the vector between the eye point and the look at point.
The right vector, which is always computed as a cross product between the view vector and the up vector.
\param eye Eye point.
\param at Look-at point.
\param up Up vector.
\param width, height Width and height of virtual screen.
\param near, far Near and far planes.
*/
CameraStandard::CameraStandard(const Vector3& eye, const Vector3& at, const Vector3& up, float frameWidth, float frameHeight, float near, float far)
{
	CameraStandard::eye = eye;
	CameraStandard::at = at;
	CameraStandard::up = up;
	CameraStandard::frameWidth = frameWidth;
	CameraStandard::frameHeight = frameHeight;
	CameraStandard::nearplane = near;
	CameraStandard::farplane = far;
	CameraStandard::cah = 0.980;
	CameraStandard::fov = 35.0;
}

/*!
\brief Steps forward or backward by a given distance.

\param r Stepping distance.
*/
void CameraStandard::MoveForward(float r)
{
	eye += Normalize(at - eye)*r;
	at  += Normalize(at - eye)*r;
}

/*!
\brief Rotates the CameraBis around the up vector by a given angle.
\param a Rotation angle.
*/
void CameraStandard::RotateAroundUp(float a)
{
	Vector3 z = at - eye;
	float length = Magnitude(z);
	z /= length;
	Vector3 left = Cross(up, z);
	left /= Magnitude(left);

	// Rotate
	z = z*cos(a) + left*sin(a);

	at = eye + z*length;
}

/*!
\brief Reset the CameraBis so that the up vector points to the sky.
*/
void CameraStandard::ResetVertical()
{
	up = Vector3(0, 0, 1.0);

	Vector3 z = at - eye;
	float length = Magnitude(z);

	Vector3 left = Cross(up, z);
	z = Cross(left, up);
	z /= Magnitude(z);

	at = eye + z*length;
}

/*!
\brief Steps forward or backward by a given distance.
\param a Rotation angle.
*/
void CameraStandard::UpDown(float a)
{
	Vector3 z = at - eye;
	float length = Magnitude(z);
	z /= length;
	Vector3 left = Cross(up, z);
	left /= Magnitude(left);

	// Rotate
	z = z*cos(a) + up*sin(a);

	// Update up Vector3
	up = Cross(z, left);

	at = eye + z*length;
}

/*!
\brief Moves the CameraBis sideway.
\param a Distance.
*/
void CameraStandard::MoveSideway(float a)
{
	Vector3 z = at - eye;
	float length = Magnitude(z);
	z /= length;
	Vector3 left = Cross(up, z);
	left /= Magnitude(left);

	eye += left * a;
	at  += left * a;
}

/*!
\brief Moves the CameraBis left or right in the horizontal plane.
\param a Distance.
*/
void CameraStandard::SlideHorizontal(float a)
{
	Vector3 z = at - eye;
	z.z = 0.0;
	float length = Magnitude(z);
	z /= length;
	Vector3 left = Cross(Vector3(0, 0, 1), z);
	left /= Magnitude(left);

	eye += left * a;
	at  += left * a;
}

/*!
\brief Moves the eye point towards or from the look at point.
\param a Distance.
*/
void CameraStandard::Zoom(float a)
{
	Vector3 z = at - eye;
	float length = Magnitude(z);
	z /= length;

	eye += z * a;
	at  += z * a;
}

/*!
\brief Rotates the CameraBis relatively to the look-at point.
\param a Distance.
*/
void CameraStandard::LeftRightRound(float a)
{
	Vector3 e = eye - at;
	Vector3 left = Cross(up, e);
	e = Vector3(e[0] * cos(a) - e[1] * sin(a), e[0] * sin(a) + e[1] * cos(a), e[2]);
	left = Vector3(left[0] * cos(a) - left[1] * sin(a), left[0] * sin(a) + left[1] * cos(a), 0.0);
	up = Normalize(Cross(left, -e));
	eye = at + e;
}

/*!
\brief Rotates the CameraBis relatively to the look-at point.
\param a Distance.
*/
void CameraStandard::UpDownRound(float a)
{
	Vector3 z = at - eye;
	float length = Magnitude(z);
	z /= length;
	Vector3 left = Cross(up, z);
	left /= Magnitude(left);

	// Rotate
	z = z*cos(a) + up*sin(a);

	// Update Vector3
	up = Cross(z, left);
	eye = at - z*length;
}

/*!
\brief Moves the CameraBis left or right, while preserving height.
\param a Distance.
*/
void CameraStandard::LeftRightHorizontal(float a)
{
	Vector3 z = at - eye;
	z.z = 0.0;
	float length = Magnitude(z);
	z /= length;
	Vector3 left = Cross(Vector3(0, 0, 1), z);
	left /= Magnitude(left);

	eye += left * a;
	at  += left * a;
}

/*!
\brief Moves the CameraBis vertically.

This function keeps the left Vector3 horizontal.
\param a Distance.
*/
void CameraStandard::UpDownVertical(float a)
{
	Vector3 z = at - eye;
	float length = Magnitude(z);
	z /= length;
	Vector3 left = Cross(Vector3(0, 0, 1), z);
	left /= Magnitude(left);

	eye += Cross(z * a, left);
	at  += Cross(z * a, left);
}

/*!
\brief Returns the Horizontal angle of view.

Angle is in radian.

\param w, h Width and height of the screen
*/
float CameraStandard::GetAngleOfViewH() const
{
	// Calcul de param�tres de cam�ras (A garder)
	// http://www.scratchapixel.com/lessons/3d-advanced-lessons/CameraBiss-advanced-techniques/film-aperture-focal-length/

	// Horizontal angle of view in degrees 
	return 2.0 * atan(cah * 25.4 * 0.5 / fov);
}

/*!
\brief Returns the vertical angle of view.

Angle is in radian.

\param w, h Width and height of the screen
*/
float CameraStandard::GetAngleOfViewV(float w, float h) const
{
	// Horizontal angle of view  
	float avh = GetAngleOfViewH();

	float avv = 2.0*atan(tan(avh / 2.0)*float(h) / float(w));

	// Vertical angle of view
	return avv;
}

/*!
\brief Compute the equation of a ray given a pixel in the CameraBis plane.
\param px,py Pixel coordinates.
\param w,h Size of the viewing window.
*/
Ray CameraStandard::PixelToRay(int px, int py, int w, int h) const
{
	// Get coordinates
	Vector3 view = Normalize(At() - Eye());
	Vector3 horizontal = Normalize(Cross(view, Up()));
	Vector3 vertical = Normalize(Cross(horizontal, view));

	float length = 1.0;

	// Convert to radians 
	float rad = GetAngleOfViewV(w, h);  // fov

	float vLength = tan(rad / 2.0) * length;
	float hLength = vLength * (float(w) / float(h));

	vertical *= vLength;
	horizontal *= hLength;

	// Translate mouse coordinates so that the origin lies in the center of the view port
	float x = px - w / 2.0;
	float y = h / 2.0 - py;

	// Scale mouse coordinates so that half the view port width and height becomes 1.0
	x /= w / 2.0;
	y /= h / 2.0;

	// Direction is a linear combination to compute intersection of picking ray with view port plane
	return Ray(eye, Normalize(view*length + horizontal*x + vertical*y));
}

/*!
\brief Compute coordinates of a point in the CameraBis plane.
\param p Point.
\param u, v Coordinates in the screen
\param w, h Size of the viewing window.
*/
bool CameraStandard::VectorToPixel(const Vector3& p, float& u, float& v, int w, int h) const
{
	// Get coordinates
	Vector3 view = Normalize(At() - Eye());
	Vector3 horizontal = Normalize(Cross(view, Up()));
	Vector3 vertical = Normalize(Cross(horizontal, view));

	// Convert to radians 
	float rad = GetAngleOfViewV(w, h);  // fov

	float vLength = tan(rad / 2);
	float hLength = vLength * (float(w) / float(h));

	// Direction
	Vector3 ep = p - Eye();
	u = Dot(horizontal, ep) / vLength;
	v = Dot(vertical, ep) / hLength;
	float z = Dot(view, ep);

	u /= z;
	v /= z;

	// Check if point lies outside of frustum
	if ((u < -1.0) || (u > 1.0) || (v < -1.0) || (v > 1.0) || (z < nearplane) || (z > farplane))
		return false;

	return true;
}
