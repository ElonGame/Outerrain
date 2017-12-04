
#ifndef _ORBITER_H
#define _ORBITER_H

#include "vec.h"
#include "transform.h"

//! representation de la camera, type orbiter, place sur une sphere autour du centre de l'objet.
class CameraOrbiter
{
public:
	//! cree une camera par defaut. observe le centre (0, 0, 0) a une distance 5.
	CameraOrbiter() : m_center(), m_position(), m_rotation(), m_size(5.f), m_radius(5.f) {}
	//! cree une camera. observe le point center a une distance size.
	CameraOrbiter(const Vector3& center, const float size) : m_center(center), m_position(), m_rotation(), m_size(size), m_radius(size) {}
	//! cree une camera. observe une boite alignee sur les axes.
	CameraOrbiter(const Vector3& pmin, const Vector3& pmax) : m_center(Center(pmin, pmax)), m_position(), m_rotation(), m_size(Magnitude(pmin - pmax)), m_radius(m_size) {}

	//! observe le point center a une distance size.
	void lookat(const Vector3& center, const float size);
	//! observe le centre d'une boite englobante.
	void lookat(const Vector3& pmin, const Vector3& pmax);
	void LookAt(const Bounds&);

	//! change le point de vue / la direction d'observation.
	void rotation(const float x, const float y);
	//! deplace le centre / le point observe.
	void translation(const float x, const float y);
	//! rapproche / eloigne la camera du centre.
	void move(const float z);

	//! renvoie la transformation vue.
	Transform view() const;

	//! renvoie la projection reglee pour une image d'aspect width / height, et une ouverture de fov degres.
	Transform projection(const float width, const float height, const float fov) const;

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
	void frame(const float width, const float height, const float z, const float fov, Vector3& dO, Vector3& dx, Vector3& dy) const;

	//! renvoie la position de la camera dans le repere du monde.
	Vector3 position();

	//! renvoie le rayon de la scene.
	float radius() const { return m_radius; }

	void SetFrameWidth(float w) { frameWidth = w; }
	void SetFrameHeight(float h) { frameHeight = h; }

	float FrameWidth() const { return frameWidth; }
	float FrameHeight() const { return frameHeight; }

protected:
	Vector3 m_center;
	Vector2 m_position;
	Vector2 m_rotation;
	float m_size;
	float m_radius;

	float frameWidth, frameHeight;
};

///@}
#endif
