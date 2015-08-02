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
    Camera(Transform transform = Transform(), Film film = Film());
    virtual void calcCamRay(const int x, const int y, Ray &ray) const = 0;
    void move(const vec3 &dir, float delta);
    void rotate(const vec3 &axis, float delta);
    Transform &getTransform();
    Film &getFilm();
protected:
    Transform m_transform;
    Film m_film;
};

}

#endif // CAMERA_H
