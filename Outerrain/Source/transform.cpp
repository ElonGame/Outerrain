#include <cassert>
#include <cstdio>
#include <cmath>
#include <algorithm>

#include "transform.h"


double radians(const double deg)
{
	return ((double)3.14 / 180.f) * deg;
}

double degrees(const double rad)
{
	return (180.f / (double)3.14) * rad;
}

Transform::Transform(
	const double t00, const double t01, const double t02, const double t03,
	const double t10, const double t11, const double t12, const double t13,
	const double t20, const double t21, const double t22, const double t23,
	const double t30, const double t31, const double t32, const double t33)
{
	m[0][0] = t00; m[0][1] = t01; m[0][2] = t02; m[0][3] = t03;
	m[1][0] = t10; m[1][1] = t11; m[1][2] = t12; m[1][3] = t13;
	m[2][0] = t20; m[2][1] = t21; m[2][2] = t22; m[2][3] = t23;
	m[3][0] = t30; m[3][1] = t31; m[3][2] = t32; m[3][3] = t33;
}

Transform::Transform(const Vector3& x, const Vector3& y, const Vector3& z, const Vector3& w)
{
	m[0][0] = x.x;	m[0][1] = y.x;	m[0][2] = z.x;	m[0][3] = w.x;
	m[1][0] = x.y;	m[1][1] = y.y;	m[1][2] = z.y;	m[1][3] = w.y;
	m[2][0] = x.z;	m[2][1] = y.z;	m[2][2] = z.z;	m[2][3] = w.z;
	m[3][0] = 0;	m[3][1] = 0;	m[3][2] = 0;	m[3][3] = 1;
}

Vector3 Transform::operator[](int c) const
{
	assert(c >= 0 && c <= 3);
	return Vector3(m[0][c], m[1][c], m[2][c]);
}


//! renvoie le Vector3 transforme.
Vector3 Transform::operator() (const Vector3& p) const
{
	double x = p.x;
	double y = p.y;
	double z = p.z;

	double xt = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];        // dot(Vector4(m[0]), Vector4(p, 1))
	double yt = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];        // dot(Vector4(m[1]), Vector4(p, 1))
	double zt = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];        // dot(Vector4(m[2]), Vector4(p, 1))
	double wt = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3];        // dot(Vector4(m[3]), Vector4(p, 1))

	assert(wt != 0);
	double w = 1.f / wt;
	if (wt == 1.f)
		return Vector3(xt, yt, zt);
	else
		return Vector3(xt*w, yt*w, zt*w);
}

//! renvoie le Vector3/vecteur homogene transforme.
Vector4 Transform::operator() (const Vector4& v) const
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	double w = v.w;

	double xt = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3] * w;    // dot(Vector4(m[0]), v)
	double yt = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3] * w;    // dot(Vector4(m[1]), v)
	double zt = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3] * w;    // dot(Vector4(m[2]), v)
	double wt = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3] * w;    // dot(Vector4(m[3]), v)

	return Vector4(xt, yt, zt, wt);
}

//! renvoie la transposee de la matrice.
Transform Transform::transpose() const
{
	return Transform(
		m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3]);
}


Transform Transform::operator() (const Transform& b) const
{
	return compose_transform(*this, b);
}

//! renvoie la transformation a appliquer aux normales d'un objet transforme par la matrice m.
Transform Transform::normal() const
{
	return inverse().transpose();
}


Transform Identity()
{
	return Transform();
}

Transform Transpose(const Transform& m)
{
	return m.transpose();
}

Transform Inverse(const Transform& m)
{
	return m.inverse();
}

Transform Normal(const Transform& m)
{
	return m.normal();
}

Transform Scale(const double x, const double y, const double z)
{
	return Transform(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);
}

Transform Translation(const Vector3& v)
{
	return Transform(
		1, 0, 0, v.x,
		0, 1, 0, v.y,
		0, 0, 1, v.z,
		0, 0, 0, 1);
}

Transform Translation(const double x, const double y, const double z)
{
	return Translation(Vector3(x, y, z));
}

Transform RotationX(const double a)
{
	double sin_t = sinf(radians(a));
	double cos_t = cosf(radians(a));

	return Transform(
		1, 0, 0, 0,
		0, cos_t, -sin_t, 0,
		0, sin_t, cos_t, 0,
		0, 0, 0, 1);
}

Transform RotationY(const double a)
{
	double sin_t = sinf(radians(a));
	double cos_t = cosf(radians(a));

	return Transform(
		cos_t, 0, sin_t, 0,
		0, 1, 0, 0,
		-sin_t, 0, cos_t, 0,
		0, 0, 0, 1);
}

Transform RotationZ(const double a)
{
	double sin_t = sinf(radians(a));
	double cos_t = cosf(radians(a));

	return Transform(
		cos_t, -sin_t, 0, 0,
		sin_t, cos_t, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Transform Rotation(const Vector3& axis, const double angle)
{
	Vector3 a = Normalize(axis);
	double s = sinf(radians(angle));
	double c = cosf(radians(angle));

	return Transform(
		a.x * a.x + (1 - a.x * a.x) * c,
		a.x * a.y * (1 - c) - a.z * s,
		a.x * a.z * (1 - c) + a.y * s,
		0,

		a.x * a.y * (1 - c) + a.z * s,
		a.y * a.y + (1 - a.y * a.y) * c,
		a.y * a.z * (1 - c) - a.x * s,
		0,

		a.x * a.z * (1 - c) - a.y * s,
		a.y * a.z * (1 - c) + a.x * s,
		a.z * a.z + (1 - a.z * a.z) * c,
		0,

		0, 0, 0, 1);
}


Transform Perspective(const double fov, const double aspect, const double znear, const double zfar)
{
	// perspective, openGL version
	double itan = 1 / tanf(radians(fov) * 0.5f);
	double id = 1 / (znear - zfar);

	return Transform(
		itan / aspect, 0, 0, 0,
		0, itan, 0, 0,
		0, 0, (zfar + znear)*id, 2.f*zfar*znear*id,
		0, 0, -1, 0);
}

Transform Viewport(const double width, const double height)
{
	double w = width / 2.f;
	double h = height / 2.f;

	return Transform(
		w, 0, 0, w,
		0, h, 0, h,
		0, 0, .5f, .5f,
		0, 0, 0, 1);
}

Transform Lookat(const Vector3& from, const Vector3& to, const Vector3& up)
{
	Vector3 dir = Normalize(from - to);
	Vector3 right = Normalize(Cross(dir, Normalize(up)));
	Vector3 newUp = Normalize(Cross(right, dir));

	Transform m(
		right.x, newUp.x, -dir.x, from.x,
		right.y, newUp.y, -dir.y, from.y,
		right.z, newUp.z, -dir.z, from.z,
		0, 0, 0, 1);

	return m.inverse();
}

Transform compose_transform(const Transform& a, const Transform& b)
{
	Transform m;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];

	return m;
}

Transform operator* (const Transform& a, const Transform& b)
{
	return compose_transform(a, b);
}

Transform Transform::inverse() const
{
	Transform minv = *this;

	int indxc[4], indxr[4];
	int ipiv[4] = { 0, 0, 0, 0 };

	for (int i = 0; i < 4; i++) {
		int irow = -1, icol = -1;
		double big = 0.f;

		// Choose pivot
		for (int j = 0; j < 4; j++) {
			if (ipiv[j] != 1) {
				for (int k = 0; k < 4; k++) {
					if (ipiv[k] == 0) {
						if (fabsf(minv.m[j][k]) >= big) {
							big = std::abs(minv.m[j][k]);
							irow = j;
							icol = k;
						}
					}
					else if (ipiv[k] > 1)
						printf("singular matrix in make_inverse()\n");
				}
			}
		}

		assert(irow >= 0 && irow < 4);
		assert(icol >= 0 && icol < 4);

		++ipiv[icol];
		// Swap rows _irow_ and _icol_ for pivot
		if (irow != icol) {
			for (int k = 0; k < 4; ++k)
				std::swap(minv.m[irow][k], minv.m[icol][k]);
		}

		indxr[i] = irow;
		indxc[i] = icol;
		if (minv.m[icol][icol] == 0.)
			printf("singular matrix in make_inverse()\n");

		// Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
		double pivinv = 1.f / minv.m[icol][icol];
		minv.m[icol][icol] = 1.f;
		for (int j = 0; j < 4; j++)
			minv.m[icol][j] *= pivinv;

		// Subtract this row from others to zero out their columns
		for (int j = 0; j < 4; j++) {
			if (j != icol) {
				double save = minv.m[j][icol];
				minv.m[j][icol] = 0;
				for (int k = 0; k < 4; k++)
					minv.m[j][k] -= minv.m[icol][k] * save;
			}
		}
	}

	// Swap columns to reflect permutation
	for (int j = 3; j >= 0; j--) {
		if (indxr[j] != indxc[j]) {
			for (int k = 0; k < 4; k++)
				std::swap(minv.m[k][indxr[j]], minv.m[k][indxc[j]]);
		}
	}

	return minv;
}
