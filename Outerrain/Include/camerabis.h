#pragma once

#include "box.h"

class CameraBis
{
protected:
    Vector3 eye;
    Vector3 at;
    Vector3 up;

    float width;
    float height;

    float cah; 
    float cav;
    float fl;

    float nearplane;
    float farplane;

public:
    CameraBis(); 
    CameraBis(const Vector3&, const Vector3&, const Vector3& = Vector3(0.0f, 0.0f, 1.0f), float = 1.0, float = 1.0, float = 1.0, float = 100000.0);
    CameraBis(const Vector3&, const Vector3&, const Vector3&, float, float = 1.0, float = 100000.0);

    void Translate(const Vector3&);

    Vector3 At() const;
    Vector3 Eye() const;
    Vector3 Up() const;
    Vector3 View() const; 

    float GetNear() const; 
    float GetFar() const; 
    float GetCameraApertureH() const; 
    float GetCameraApertureV() const; 
    float GetFocalLength() const; 
    float GetAngleOfViewH(float, float) const; 
    float GetAngleOfViewV(float, float) const; 

    void Step(float); 

    bool InsideFrustum(const Vector3&) const; 
    bool InsideFrustum(const Box&) const; 

    void LeftRight(float);
    void Vertical();
    void UpDown(float);
    void SideWay(float);
    void BackForth(float);
    void LeftRightRound(float);
    void UpDownRound(float); 

    void SlideHorizontal(float); 
    void SetAt(const Vector3& ); 
    void SetEye(const Vector3& ); 
    void SetPlanes(float, float); 
    void SetFieldOfView(float); 

    // Move camera in a plane
    void UpDownVertical(float); 
    void LeftRightHorizontal(float); 
    Ray PixelToRay(int, int, int, int) const;
    bool VectorToPixel(const Vector3&, float, float, int, int) const; 
    static CameraBis View(const Box&); 
}; 

//! Returns the look-at point.
inline Vector3 CameraBis::At()const {
    return at; 
}

//! Returns the eye point.
inline Vector3 CameraBis::Eye()const {
    return eye; 
}

//! Returns the up point.
inline Vector3 CameraBis::Up()const {
    return up; 
}

/*!
\brief Returns the view direction.
*/
inline Vector3 CameraBis::View()const {
    return at - eye; 
}

/*!
\brief Sets the camera target vector.
\param p Look-at point.
*/
inline void CameraBis::SetAt(const Vector3& p) {
    at = p; 
    up = Vector3(0.0, 0.0, 1.0); 
}

/*!
\brief Sets the camera eye point.
\param p Eye point.
*/
inline void CameraBis::SetEye(const Vector3& p) {
    eye = p; 
}

/*!
\brief Get the near distance.
*/
inline float CameraBis::GetNear()const {
    return nearplane; 
}

/*!
\brief Get the far distance.
*/
inline float CameraBis::GetFar()const {
    return farplane; 
}

/*!
\brief Get the far distance.
*/
inline float CameraBis::GetCameraApertureH()const {
    return cah; 
}

/*!
\brief Get the far distance.
*/
inline float CameraBis::GetCameraApertureV()const {
    return cav; 
}

/*!
\brief Get the far distance.
*/
inline float CameraBis::GetFocalLength()const {
    return fl; 
}

/*!
\brief Set the near and far planes.
\param n, f Near and far planes distance to th eye.
*/
inline void CameraBis::SetPlanes(float n, float f) {
    nearplane = n; 
    farplane = f; 
}
