// std includes
#include <iostream>

// mirage includes
#include "perspective.h"
#include "../macros.h"
#include "../math/vec3.h"
#include "../math/quaternion.h"

namespace mirage
{

CameraPersp::CameraPersp(Transform transform, Film film, float fov) : Camera(transform, film), m_fov(std::tan((fov * 180.0f / PI) / 2.0f))
{
    LOG("Created a new perspective camera.");
}

void CameraPersp::calcCamRay(const int x, const int y, Ray &ray) const
{
    // Construct the ray's direction vector and aim it towards the virtual screen's pixel
    auto x_norm = ((x - m_film.getResolutionX() * 0.5f) / m_film.getResolutionX() * m_film.getAspectRatio()) * m_fov;
    auto y_norm = ((m_film.getResolutionY() * 0.5f - y) / m_film.getResolutionY()) * m_fov;
    auto v_norm = vec3(x_norm, y_norm, 1.0f);

    // Rotate the ray direction based on camera orientation
    quaternion q = m_transform.getOrientation();
    quaternion q_inv = q.conjugate();
    quaternion w(0.0f, v_norm.x, v_norm.y, v_norm.z);
    quaternion r = q * w * q_inv;

    // Assign final parameters to the output &ray
    ray.setOrigin(m_transform.getPosition());
    ray.setDirection(vec3(r.x, r.y, r.z));
}

void CameraPersp::setFoV(float fov)
{
    m_fov = std::tan((fov * 180.0f / PI) / 2.0f);
}

float CameraPersp::getFoV() const
{
    return m_fov;
}

}
