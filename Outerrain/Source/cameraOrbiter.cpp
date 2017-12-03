#include <cstdio>
#include <algorithm>

#include "CameraOrbiter.h"

#define max(a, b) a > b ? a : b

void CameraOrbiter::lookat(const Vector3& center, const double size)
{
	m_center = center;
	m_position = Vector2(0, 0);
	m_rotation = Vector2(0, 0);
	m_size = size;
	m_radius = size;
}

void CameraOrbiter::lookat(const Vector3& pmin, const Vector3& pmax)
{
	lookat(Center(pmin, pmax), Magnitude(pmin - pmax));
}

void CameraOrbiter::LookAt(const Bounds& b) 
{
	lookat(b.min, b.max);
}

void CameraOrbiter::rotation(const double x, const double y)
{
	m_rotation.x = m_rotation.x + y;
	m_rotation.y = m_rotation.y + x;
}

void CameraOrbiter::translation(const double x, const double y)
{
	m_position.x = m_position.x - m_size * x;
	m_position.y = m_position.y + m_size * y;
}

void CameraOrbiter::move(const double z)
{
	m_size = m_size - m_size * 0.01f * z;
	if (m_size < 0.01f)
		m_size = 0.01f;
}

Transform CameraOrbiter::view() const
{
	return Translation(-m_position.x, -m_position.y, -m_size)
		* RotationX(m_rotation.x) * RotationY(m_rotation.y)
		* Translation(-Vector3(m_center));
}

Transform CameraOrbiter::projection(const double width, const double height, const double fov) const
{
	// calcule la distance entre le centre de l'objet et la camera
	//~ Transform t= view();
	//~ Vector3 c= t(m_center);
	//~ double d= -c.z;
	double d = Magnitude(m_center - Vector3(m_position.x, m_position.y, m_size));     // meme resultat plus rapide a calculer

																				 // regle near et far en fonction du centre et du rayon englobant l'objet 
	return Perspective(fov, width / height, max(0.1f, d - m_radius), max(1.f, d + m_radius));
}

void CameraOrbiter::frame(const double width, const double height, const double z, const double fov, Vector3& dO, Vector3& dx, Vector3& dy) const
{
	Transform v = view();
	Transform p = projection(width, height, fov);
	Transform viewport = Viewport(width, height);
	Transform t = viewport * p * v;              // passage monde vers image
	Transform tinv = t.inverse();                // l'inverse, passage image vers monde

												 // origine du plan image
	dO = tinv(Vector3(0, 0, z));
	// axe x du plan image
	Vector3 d1 = tinv(Vector3(1, 0, z));
	// axe y du plan image
	Vector3 d2 = tinv(Vector3(0, 1, z));

	dx = Vector3(dO - d1);
	dy = Vector3(dO - d2);
}

Vector3 CameraOrbiter::position()
{
	Transform t = view(); // passage monde vers camera
	Transform tinv = t.inverse(); // l'inverse, passage camera vers monde
	return tinv(Vector3(0, 0, 0)); // la camera se trouve a l'origine, dans le repere camera...
}
