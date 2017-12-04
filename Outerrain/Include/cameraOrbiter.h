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

	//! change le point de vue / la direction d'observation.
	void Rotation(const float x, const float y);
	//! deplace le centre / le point observe.
	void Translation(const float x, const float y);
	//! rapproche / eloigne la camera du centre.
	void Move(const float z);

	//! renvoie la transformation vue.
	Transform View() const;

	//! renvoie la projection reglee pour une image d'aspect width / height, et une ouverture de fov degres.
	Transform Projection(const float width, const float height, const float fov) const;

	/*! renvoie les coordonnees de l'origine d0 et les axes dx, dy du plan image dans le repere du monde.
	permet de construire un rayon pour le pixel x, y :
	- l'extremite : un point dans le plan image avec z = 1 :
	\code
	Point d1;
	Vector dx1, dy1;
	camera.frame(width, height, 1, fov, d1, dx1, dy1);
	Point e= d1 + x*dx1 + y*dy1;
	\endcode
	- l'origine :
	\code
	Point o= camera.position();
	\endcode

	ou autre solution, reproduire la projection openGL :
	- extremite : un point dans le plan image avec z = 1 :
	\code
	Point d1;
	Vector dx1, dy1;
	camera.frame( width, height, 1, fov, d1, dx1, dy1);
	Point e= d1 + x*dx1 + y*dy1;
	\endcode
	- origine : un point dans le plan image avec z = 0 :
	\code
	Point d0;
	Vector dx0, dy0;
	camera.frame(width, height, 0, fov, d0, dx0, dy0);
	Point o= d0 + x*dx0 + y*dy0;
	\endcode
	*/
	void Frame(const float width, const float height, const float z, const float fov, Vector3& dO, Vector3& dx, Vector3& dy) const;

	Vector3 Position();
	void SetFrameWidth(float w) { frameWidth = w; }
	void SetFrameHeight(float h) { frameHeight = h; }
	float FrameWidth() const { return frameWidth; }
	float FrameHeight() const { return frameHeight; }

protected:
	Vector3 center;
	Vector2 position;
	Vector2 rotation;
	float size;
	float frameWidth, frameHeight;
	float zNear, zFar;
};

#endif
