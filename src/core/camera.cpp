// std includes
#include <iostream>

// mirage includes
#include "camera.h"
#include "../macros.h"
#include "../math/vec4.h"
#include "../math/quaternion.h"

namespace mirage
{

/*
 * Camera base class definitions below
 */
Camera::Camera(Transform transform, Film film) : m_transform(transform), m_film(film)
{

}

void Camera::calcCamRay(const int x, const int y, Ray *ray) const
{
    ERR("Illegal call to Camera::calcCameraRay!\nThis is an abstract base class method and should never be called!");
}

Transform &Camera::getTransform()
{
    return m_transform;
}

Film &Camera::getFilm()
{
    return m_film;
}

/*
 * Camera child class CameraOrtho definitions below
 */
CameraOrtho::CameraOrtho(Transform transform, Film film, float zoom) : Camera(transform, film), m_zoom(zoom)
{
    LOG("Created a new orthographic camera.");
}

void CameraOrtho::calcCamRay(const int x, const int y, Ray *ray) const
{
    // Construct the ray's origin vector
    vec4 e = m_transform.getPosition();
    vec4 s = e + vec4(x - m_film.getResolutionX() * 0.5f + 0.5f, m_film.getResolutionY() * 0.5f - y + 0.5f, 5.0f);

    // Get the ray's direction vector
    vec4 p = m_transform.getOrientation().getForwardVector().negate3();

    // Rotate the ray origin based on camera orientation
    quaternion q = m_transform.getOrientation();
    quaternion q_inv = q.conjugate();
    quaternion w(0.0f, s.x, s.y, s.z);
    quaternion r = q * w * q_inv;

    // Finally get the rotated origin vector
    s = vec4(r.x, r.y, r.z);

    // Assign final parameters to the output *ray
    ray->setOrigin(s);
    ray->setDirection(p);
}

}
