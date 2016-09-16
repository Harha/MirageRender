// std includes
#include <iostream>

// lib includes
#include <SDL2/SDL.h>

// mirage includes
#include "orthographic.h"
#include "../macros.h"
#include "../math/vec3.h"
#include "../math/quaternion.h"

namespace mirage
{

CameraOrtho::CameraOrtho(Transform transform, Film film, float speed, float sensitivity, float zoom) : Camera(transform, film, speed, sensitivity), m_zoom(zoom)
{
    LOG("Camera: Created a new orthographic camera.");
}

void CameraOrtho::update(float dt, bool keys[256])
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
        move(m_transform.getOrientation().getRightVector(), dt * m_speed);
    }
    else if (keys[SDL_SCANCODE_D])
    {
        move(m_transform.getOrientation().getRightVector(), -dt * m_speed);
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
        rotate(m_transform.getOrientation().getUpVector(), dt * m_sensitivity);
    }
    else if (keys[SDL_SCANCODE_RIGHT])
    {
        rotate(m_transform.getOrientation().getUpVector(), -dt * m_sensitivity);
    }
    if (keys[SDL_SCANCODE_Q])
    {
        rotate(m_transform.getOrientation().getForwardVector(), dt * m_sensitivity);
    }
    else if (keys[SDL_SCANCODE_E])
    {
        rotate(m_transform.getOrientation().getForwardVector(), -dt * m_sensitivity);
    }

    if (keys[SDL_SCANCODE_KP_PLUS])
    {
        m_zoom += 0.1f * dt;
        m_film.clearSamples();
    }
    else if (keys[SDL_SCANCODE_KP_MINUS])
    {
        m_zoom -= 0.1f * dt;
        m_film.clearSamples();
    }
}

void CameraOrtho::calcCamRay(const int x, const int y, Ray &ray) const
{
    // Tent filter for each ray's xy directions
    float r1 = 2.0f * pseudorand(), dx = r1 < 1.0f ? std::sqrt(r1) - 1.0f : 1.0f - std::sqrt(2.0f - r1);
    float r2 = 2.0f * pseudorand(), dy = r2 < 1.0f ? std::sqrt(r2) - 1.0f : 1.0f - std::sqrt(2.0f - r2);

    // Construct the ray's origin vector
    vec3 e = m_transform.getPosition();
    vec3 s = e + vec3(m_film.getResolutionX() * 0.5f - x + dx + 0.5f, m_film.getResolutionY() * 0.5f - y + dy + 0.5f, 5.0f) * m_zoom;

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
