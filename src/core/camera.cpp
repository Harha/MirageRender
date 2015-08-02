// std includes
#include <iostream>

// mirage includes
#include "camera.h"
#include "../macros.h"
#include "../math/vec3.h"
#include "../math/quaternion.h"

namespace mirage
{

Camera::Camera(Transform transform, Film film) : m_transform(transform), m_film(film)
{

}

void Camera::calcCamRay(const int x, const int y, Ray &ray) const
{
    ERR("Illegal call to Camera::calcCameraRay!\nThis is an abstract base class method and should never be called!");
}

void Camera::move(const vec3 &dir, float delta)
{
    m_transform.setPosition(m_transform.getPosition() + dir * delta);
}

void Camera::rotate(const vec3 &axis, float delta)
{
    quaternion q;
    q.euler(axis.x, axis.y, axis.z, delta);
    m_transform.setOrientation((q * m_transform.getOrientation()).normalize());
}

Transform &Camera::getTransform()
{
    return m_transform;
}

Film &Camera::getFilm()
{
    return m_film;
}

}
