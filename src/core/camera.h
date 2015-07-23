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

    virtual void calcCamRay(const int x, const int y, Ray *ray) const;

    Transform &getTransform()
    {
        return m_transform;
    }
protected:
    Transform m_transform;
    Film m_film;
};

class CameraOrtho : public Camera
{
public:
    CameraOrtho(Transform transform = Transform(), Film film = Film(), float zoom = 0.1f);

    virtual void calcCamRay(const int x, const int y, Ray *ray) const override;
private:
    float m_zoom;
};

}

#endif // CAMERA_H
