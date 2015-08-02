// std includes
#include <iostream>

// mirage includes
#include "orthographic.h"
#include "../macros.h"
#include "../math/vec3.h"
#include "../math/quaternion.h"

namespace mirage
{

CameraOrtho::CameraOrtho(Transform transform, Film film, float zoom) : Camera(transform, film), m_zoom(zoom)
{
    LOG("Created a new orthographic camera.");
}

void CameraOrtho::calcCamRay(const int x, const int y, Ray &ray) const
{
    // Construct the ray's origin vector
    vec3 e = m_transform.getPosition();
    vec3 s = e + vec3(x - m_film.getResolutionX() * 0.5f + 0.5f, m_film.getResolutionY() * 0.5f - y + 0.5f, 5.0f) * m_zoom;

    // Get the ray's direction vector
    vec3 p = m_transform.getOrientation().getForwardVector();

    // Rotate the ray origin based on camera orientation
    quaternion q = m_transform.getOrientation();
    quaternion q_inv = q.conjugate();
    quaternion w(0.0f, s.x, s.y, s.z);
    quaternion r = q * w * q_inv;

    // Assign final parameters to the output &ray
    ray.setOrigin(vec3(r.x, r.y, r.z));
    ray.setDirection(p);
}

}
