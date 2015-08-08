// std includes
#include <iostream>

// lib includes
#include "SDL2/SDL.h"

// mirage includes
#include "perspective.h"
#include "../macros.h"
#include "../math/vec3.h"
#include "../math/quaternion.h"

namespace mirage
{

CameraPersp::CameraPersp(Transform transform, Film film, float speed, float sensitivity, float fov) : Camera(transform, film, speed, sensitivity), m_fov(std::tan((fov * 180.0f / PI) / 2.0f))
{
    LOG("Created a new perspective camera.");
}

void CameraPersp::update(float dt, bool keys[256])
{
    if (keys[SDL_SCANCODE_W])
    {
        move(m_transform.getOrientation().getForwardVector(), dt * m_speed);
    }
    else if (keys[SDL_SCANCODE_S])
    {
        move(m_transform.getOrientation().getForwardVector(), -dt * m_speed);
    }
    if (keys[SDL_SCANCODE_A])
    {
        move(m_transform.getOrientation().getRightVector(), -dt * m_speed);
    }
    else if (keys[SDL_SCANCODE_D])
    {
        move(m_transform.getOrientation().getRightVector(), dt * m_speed);
    }

    if (keys[SDL_SCANCODE_UP])
    {
        rotate(m_transform.getOrientation().getRightVector(), -dt * m_sensitivity);
    }
    else if (keys[SDL_SCANCODE_DOWN])
    {
        rotate(m_transform.getOrientation().getRightVector(), dt * m_sensitivity);
    }
    if (keys[SDL_SCANCODE_LEFT])
    {
        rotate(m_transform.getOrientation().getUpVector(), -dt * m_sensitivity);
    }
    else if (keys[SDL_SCANCODE_RIGHT])
    {
        rotate(m_transform.getOrientation().getUpVector(), dt * m_sensitivity);
    }
    if (keys[SDL_SCANCODE_Q])
    {
        rotate(m_transform.getOrientation().getForwardVector(), -dt * m_sensitivity);
    }
    else if (keys[SDL_SCANCODE_E])
    {
        rotate(m_transform.getOrientation().getForwardVector(), dt * m_sensitivity);
    }

    if (keys[SDL_SCANCODE_KP_PLUS])
    {
        m_fov += 0.1f * dt;
        m_film.clearSamples();
    }
    else if (keys[SDL_SCANCODE_KP_MINUS])
    {
        m_fov -= 0.1f * dt;
        m_film.clearSamples();
    }
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
