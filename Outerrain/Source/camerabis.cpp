#include "camerabis.h"

/*!
\brief Create a default CameraBis.
*/
CameraBis::CameraBis()
{
  CameraBis::eye = Vector(0.0);
  CameraBis::at = Vector(0.0, 1.0, 0.0);
  CameraBis::up = Vector(0.0, 0.0, 1.0);
  CameraBis::width = 1.0;
  CameraBis::height = 1.0;

  // Near and far planes 
  CameraBis::nearplane = 1.0;
  CameraBis::farplane = 1000.0;

  // Aperture
  CameraBis::cah = 0.980;
  CameraBis::cav = 0.735;
  CameraBis::fl = 35.0;
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
CameraBis::CameraBis(const Vector& eye, const Vector& at, const Vector& up, float width, float height, float near, float far)
{
  CameraBis::eye = eye;
  CameraBis::at = at;
  CameraBis::up = up;
  CameraBis::width = width;
  CameraBis::height = height;

  // Near and far planes 
  CameraBis::nearplane = near;
  CameraBis::farplane = far;

  // Aperture
  CameraBis::cah = 0.980;
  CameraBis::cav = 0.735;
  CameraBis::fl = 35.0;
}

/*!
\brief Create a CameraBis given its location and look at point.
\param eye Eye point.
\param at Look-at point.
\param up Up vector.
\param field Field of view, should be in [0,Math::Pi/2.0].
\param near, far Near and far planes.
*/
CameraBis::CameraBis(const Vector& eye, const Vector& at, const Vector& up, float field, float near, float far)
{
  CameraBis::eye = eye;
  CameraBis::at = at;
  CameraBis::up = up;
  CameraBis::width = sin(field / 2.0);
  CameraBis::height = sin(field / 2.0);

  // Near and far planes 
  CameraBis::nearplane = near;
  CameraBis::farplane = far;

  // Aperture
  CameraBis::cah = 0.980;
  CameraBis::cav = 0.735;
  CameraBis::fl = 35.0;
}

/*!
\brief Translates a CameraBis by a given vector.
\param t %Translation vector.
*/
void CameraBis::Translate(const Vector& t)
{
  eye += t;
  at += t;
}

/*!
\brief Rotates the CameraBis with a rotation matrix.

The rotation is centered at the eye, so the eye location is preserved. Other parameters are preserved.

\param r Rotation matrix.
*/
void CameraBis::Rotate(const Matrix& r)
{
  // Eye is unchanged
  // Up vector
  up = r*up;
  // Look at point
  at = eye + r*(at - eye);
}

/*!
\brief Steps forward or backward by a given distance.

\param r Stepping distance.
*/
void CameraBis::Step(float r)
{
  eye += Normalized(at - eye)*r;
  at += Normalized(at - eye)*r;
}

/*!
\brief Get the frame of the CameraBis.
*/
Frame CameraBis::GetFrameOWI() const
{
  // Viewing direction
  Vector z = at - eye;
  Normalize(z);

  // Set right using cross product
  Vector x = z / up;
  Normalize(x);

  // Set implicitly normalized up    
  Vector y = x / z;
  Normalize(y);

  return Frame(eye, x, y, z);
}

/*!
\brief Overloaded.
\param s Stream.
\param CameraBis The CameraBis.
*/
ostream& operator<<(ostream& s, const CameraBis& CameraBis)
{
  s << "CameraBis(" << CameraBis.eye << ',' << CameraBis.at << ',' << CameraBis.width << ',' << CameraBis.height << ',' << CameraBis.up << ')' << endl;
  return s;
}

/*!
\brief Rotates the CameraBis around the up vector by a given angle.
\param a Rotation angle.
*/
void CameraBis::LeftRight(float a)
{
  Vector z = at - eye;
  float length = Norm(z);
  z /= length;
  Vector left = up / z;
  left /= Norm(left);

  // Rotate
  z = z*cos(a) + left*sin(a);

  at = eye + z*length;
}

/*!
\brief Reset the CameraBis so that the up vector points to the sky.
*/
void CameraBis::Vertical()
{
  up = Vector(0, 0, 1.0);

  Vector z = at - eye;
  float length = Norm(z);

  Vector left = up / z;
  z = left / up;
  z /= Norm(z);

  at = eye + z*length;
}

/*!
\brief Steps forward or backward by a given distance.
\param a Rotation angle.
*/
void CameraBis::UpDown(float a)
{
  Vector z = at - eye;
  float length = Norm(z);
  z /= length;
  Vector left = up / z;
  left /= Norm(left);

  // Rotate
  z = z*cos(a) + up*sin(a);

  // Update up vector
  up = z / left;

  at = eye + z*length;
}

/*!
\brief Moves the CameraBis sideway.
\param a Distance.
*/
void CameraBis::SideWay(float a)
{
  Vector z = at - eye;
  float length = Norm(z);
  z /= length;
  Vector left = up / z;
  left /= Norm(left);

  eye += a*left;
  at += a*left;
}

/*!
\brief Moves the CameraBis left or right in the horizontal plane.
\param a Distance.
*/
void CameraBis::SlideHorizontal(float a)
{
  Vector z = at - eye;
  z[2] = 0.0;
  float length = Norm(z);
  z /= length;
  Vector left = Vector(0, 0, 1) / z;
  left /= Norm(left);

  eye += a*left;
  at += a*left;
}

/*!
\brief Moves the eye point towards or from the look at point.
\param a Distance.
*/
void CameraBis::BackForth(float a)
{
  Vector z = at - eye;
  float length = Norm(z);
  z /= length;

  eye += a*z;
  //at+=a*z;
}

/*!
\brief Rotates the CameraBis relatively to the look-at point.
\param a Distance.
*/
void CameraBis::LeftRightRound(float a)
{
  Vector e = eye - at;
  Vector left = up / e;
  e = Vector(e[0] * cos(a) - e[1] * sin(a), e[0] * sin(a) + e[1] * cos(a), e[2]);
  left = Vector(left[0] * cos(a) - left[1] * sin(a), left[0] * sin(a) + left[1] * cos(a), 0.0);
  up = Normalized(left / -e);
  eye = at + e;
}

/*!
\brief Rotates the CameraBis relatively to the look-at point.
\param a Distance.
*/
void CameraBis::UpDownRound(float a)
{
  Vector z = at - eye;
  float length = Norm(z);
  z /= length;
  Vector left = up / z;
  left /= Norm(left);

  // Rotate
  z = z*cos(a) + up*sin(a);

  // Update Vector
  up = z / left;
  eye = at - z*length;
}

/*!
\brief Check if the argument box lies in the view frustum.
\param box The box.
*/
bool CameraBis::InsideFrustum(const Box& box) const
{
  Vector z = at - eye;
  z /= Norm(z);
  if ((box.Center() - eye)*z < -box.Radius())
  {
    return false;
  }
  return true;
}

/*!
\brief Check if the point lies in the view frustum.
\param p The point.
*/
bool CameraBis::InsideFrustum(const Vector& p) const
{
  cout << "warning: should be modified" << endl;
  return true;
}

/*!
\brief Moves the CameraBis left or right, while preserving height.
\param a Distance.
*/
void CameraBis::LeftRightHorizontal(float a)
{
  Vector z = at - eye;
  z[2] = 0.0;
  float length = Norm(z);
  z /= length;
  Vector left = Vector(0, 0, 1) / z;
  left /= Norm(left);

  eye += a*left;
  at += a*left;
}

/*!
\brief Moves the CameraBis vertically.

This function keeps the left vector horizontal.
\param a Distance.
*/
void CameraBis::UpDownVertical(float a)
{
  Vector z = at - eye;
  float length = Norm(z);
  z /= length;
  Vector left = Vector(0, 0, 1) / z;
  left /= Norm(left);

  eye += a*z / left;
  at += a*z / left;
}

/*!
\brief Returns the Horizontal angle of view.

Angle is in radian.

\param w, h Width and height of the screen
*/
float CameraBis::GetAngleOfViewH(float w, float h) const
{
  // Calcul de param�tres de cam�ras (A garder)
  // http://www.scratchapixel.com/lessons/3d-advanced-lessons/CameraBiss-advanced-techniques/film-aperture-focal-length/

  // Horizontal angle of view in degrees 
  return 2.0 * atan(cah * 25.4 * 0.5 / fl);
}

/*!
\brief Returns the vertical angle of view.

Angle is in radian.

\param w, h Width and height of the screen
*/
float CameraBis::GetAngleOfViewV(float w, float h) const
{
  // Horizontal angle of view  
  float avh = GetAngleOfViewH(w, h);

  float avv = 2.0*atan(tan(avh / 2.0)*float(h) / float(w));

  // Vertical angle of view
  return avv;
}

/*!
\brief Create a QString form a CameraBis.
\param n Number of digits used in the representation of reals.
*/
QString CameraBis::ToString(int n) const
{
  return QString("Eye(%1,%2,%3)\n At(%4,%5,%6)").arg(eye[0], 0, 'f', n).arg(eye[1], 0, 'f', n).arg(eye[2], 0, 'f', n).arg(at[0], 0, 'f', n).arg(at[1], 0, 'f', n).arg(at[2], 0, 'f', n);
}

/*!
\brief Compute the equation of a ray given a pixel in the CameraBis plane.
\param px,py Pixel coordinates.
\param w,h Size of the viewing window.
*/
Ray CameraBis::PixelToRay(int px, int py, int w, int h) const
{
  // Get coordinates
  Vector view = Normalized(At() - Eye());
  Vector horizontal = Normalized(view / Up());
  Vector vertical = Normalized(horizontal / view);

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
  return Ray(eye, Normalized(view*length + horizontal*x + vertical*y));
}

/*!
\brief Compute coordinates of a point in the CameraBis plane.
\param p Point.
\param u, v Coordinates in the screen
\param w, h Size of the viewing window.
*/
bool CameraBis::VectorToPixel(const Vector& p, float& u, float& v, int w, int h) const
{
  // Get coordinates
  Vector view = Normalized(At() - Eye());
  Vector horizontal = Normalized(view / Up());
  Vector vertical = Normalized(horizontal / view);

  // Convert to radians 
  float rad = GetAngleOfViewV(w, h);  // fov

  float vLength = tan(rad / 2);
  float hLength = vLength * (float(w) / float(h));

  // Direction
  Vector ep = p - Eye();
  u = horizontal*ep / vLength;
  v = vertical*ep / hLength;
  float z = view*ep;

  u /= z;
  v /= z;

  // Check if point lies outside of frustum
  if ((u < -1.0) || (u > 1.0) || (v < -1.0) || (v > 1.0) || (z < nearplane) || (z > farplane))
    return false;

  return true;
}

/*!
\brief Create a default CameraBis focusing on a given box.

THe CameraBis is located outside of the box and focuses on the center of the target box.

\param box The box.
*/
CameraBis CameraBis::View(const Box& box)
{
  // Planar rectangle
  Box2 r = Box2(box);

  // Viewing diagonal
  Vector2 v = 0.5*r.Diagonal();

  return CameraBis(box.Center() - (2.0*v).ToVector(-Norm(v)), box.Center(), Vector(0.0, 0.0, 1.0), 1.0, 1.0, 5.0, 10000);
}