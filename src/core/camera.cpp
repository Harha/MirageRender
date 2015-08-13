// std includes
#include <iostream>

// mirage includes
#include "camera.h"
#include "input.h"
#include "../macros.h"
#include "../math/vec3.h"
#include "../math/quaternion.h"

namespace mirage
{

Camera::Camera(Transform transform, Film film, float speed, float sensitivity) : m_transform(transform), m_film(film), m_speed(speed), m_sensitivity(sensitivity)
{

}

void Camera::update(float dt, bool keys[256])
{

}

void Camera::calcCamRay(const int x, const int y, Ray &ray) const
{
    ERR("Purely virtual function Camera::calcCamRay called!");
}

void Camera::move(const vec3 &dir, float delta)
{
    m_transform.setPosition(m_transform.getPosition() + dir * delta);
    m_film.clearSamples();
}

void Camera::rotate(const vec3 &axis, float delta)
{
    quaternion q;
    q.euler(axis.x, axis.y, axis.z, delta);
    m_transform.setOrientation((q * m_transform.getOrientation()).normalize());
    m_film.clearSamples();
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
