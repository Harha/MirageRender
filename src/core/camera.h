#ifndef CAMERA_H
#define CAMERA_H

// mirage includes
#include "transform.h"
#include "film.h"
#include "ray.h"

namespace mirage
{

class Camera
{
public:
    Camera(Transform transform = Transform(), Film film = Film(), float speed = 16, float sensitivity = 32);
    virtual ~Camera();
    virtual void update(float dt, bool keys[256]) = 0;
    virtual void calcCamRay(const int x, const int y, Ray &ray) const = 0;
    void move(const vec3 &dir, float delta);
    void rotate(const vec3 &axis, float delta);
    Transform &getTransform();
    Film &getFilm();
protected:
    Transform m_transform;
    Film m_film;
    float m_speed;
    float m_sensitivity;
};

}

#endif // CAMERA_H
