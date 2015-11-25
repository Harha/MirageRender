// std includes
#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <map>

// mirage includes
#include "scene.h"
#include "../macros.h"
#include "transform.h"
#include "../accelerators/kdtree.h"
#include "../cameras/orthographic.h"
#include "../cameras/perspective.h"
#include "light.h"
#include "../lights/pointlight.h"

namespace mirage
{

Scene::Scene(std::string filePath) : m_filePath(filePath)
{
    /* Reset accelerator to point at nullptr first */
    m_accelerator = nullptr;

    /* Set some things to default values */
    m_radClamping = 2.5f;
    m_recMax = 5;

    /* Load scene from the provided filepath */
    if (m_filePath != "")
    {
        loadSceneFile();

        if (m_cameras.size() <= 0)
        {
            Camera *cam_persp = new CameraPersp(Transform(vec3(0, 0, 0)));
            m_cameras.push_back(cam_persp);
        }
    }
    else
    {
        ERR("Scene: Empty filepath for scene description file! Exiting...");
        std::exit(1);
    }

    LOG("Scene: a New Scene object was created.");
}

Scene::~Scene()
{
    /* Destroy accelerator if not pointing to null */
    if (m_accelerator)
    {
        delete m_accelerator;
    }

    /* Destroy cameras */
    for (Camera *c : m_cameras)
    {
        if (c)
            delete c;
    }

    /* Destroy lights */
    for (Light *l : m_lights)
    {
        if (l)
            delete l;
    }

    /* Destroy meshes */
    for (Mesh *m : m_meshes)
    {
        if (m)
            delete m;
    }
}

bool Scene::intersect(const Ray &ray, Intersection &iSect) const
{
    return m_accelerator->intersect(ray, iSect);
}

bool Scene::intersectP(const Ray &ray) const
{
    return m_accelerator->intersectP(ray);
}

void Scene::setAccelerator(Accelerator *accel)
{
    m_accelerator = accel;
}

void Scene::addLight(Light *l)
{
    m_lights.push_back(l);
}

Accelerator *Scene::getAccelerator() const
{
    return m_accelerator;
}

Camera *Scene::getCamera() const
{
    return m_cameras.back();
}

std::vector<Light *> Scene::getLights() const
{
    return m_lights;
}

float Scene::getRadClamping() const
{
    return m_radClamping;
}

int Scene::getRecMax() const
{
    return m_recMax;
}

void Scene::loadSceneFile()
{
    std::string type_accelerator;
    std::map<std::string, Transform> data_transforms;
    std::map<std::string, Film> data_films;
    std::map<std::string, Camera *> data_cameras;
    std::map<std::string, Material *> data_materials;
    std::map<std::string, Mesh *> data_meshes;
    std::map<std::string, Light *> data_lights;

    std::ifstream file;
    std::string line;

    LOG("Scene: Attempting to load: " << m_filePath);

    file.open("res/scenes/" + m_filePath);

    if (file.is_open())
    {
        while (file.good())
        {
            std::getline(file, line);

            // Trim any leading, trailing and extra spaces
            line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
            line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "");

            // Check for empty line, continue if true
            if (line.empty())
            {
                continue;
            }

            // Print comments
            /*if (line.substr(0, 1) == "#")
            {
                std::cout << line << std::endl;
            }*/

            // Accelerator type
            else if (line.substr(0, 12) == "Accelerator ")
            {
                std::istringstream s(line.substr(12));

                s >> type_accelerator;
            }

            // Radiance clamping
            else if (line.substr(0, 9) == "radclamp ")
            {
                std::istringstream s(line.substr(9));

                s >> m_radClamping;
            }

            // Max recursion
            else if (line.substr(0, 7) == "recmax ")
            {
                std::istringstream s(line.substr(7));

                s >> m_recMax;
            }

            // Transforms
            else if (line.substr(0, 10) == "Transform ")
            {
                std::istringstream s(line.substr(10));

                std::string key; // Name
                vec3 pos; // Position
                vec3 dst; // LookAt destination
                vec3 scl; // Scale

                s >> key;

                s >> pos.x;
                s >> pos.y;
                s >> pos.z;

                s >> dst.x;
                s >> dst.y;
                s >> dst.z;

                s >> scl.x;
                s >> scl.y;
                s >> scl.z;

                data_transforms.insert(std::pair<std::string, Transform>(key, Transform(pos, quaternion().identity().lookAt(pos, dst), scl)));
            }

            // Films
            else if (line.substr(0, 5) == "Film ")
            {
                std::istringstream s(line.substr(5));

                std::string key; // Name
                int wth; // Width
                int hth; // Height

                s >> key;
                s >> wth;
                s >> hth;

                data_films.insert(std::pair<std::string, Film>(key, Film(wth, hth)));
            }

            // Cameras
            else if (line.substr(0, 7) == "Camera ")
            {
                std::istringstream s(line.substr(7));

                std::string key; // Name
                std::string trf; // Transform
                std::string flm; // Film
                std::string tpe; // Type

                s >> key;
                s >> trf;
                s >> flm;
                s >> tpe;

                // Orthographic camera
                if (tpe == "ortho")
                {
                    float zoom; // Zoom

                    s >> zoom;

                    data_cameras.insert(std::pair<std::string, Camera *>(key, new CameraOrtho(data_transforms.at(trf), data_films.at(flm), 16, 32, zoom)));
                }

                // Perspective camera
                else if (tpe == "persp")
                {
                    float fov; // Field of view

                    s >> fov;

                    data_cameras.insert(std::pair<std::string, Camera *>(key, new CameraPersp(data_transforms.at(trf), data_films.at(flm), 16, 32, fov)));
                }

            }

            // Materials
            else if (line.substr(0, 9) == "Material ")
            {
                std::istringstream s(line.substr(9));

                std::string key; // Name
                std::string tpe; // Type

                s >> key;
                s >> tpe;

                // Diffuse material
                if (tpe == "diffuse")
                {
                    vec3 kd; // Diffuse color
                    vec3 ke; // Emissive color

                    s >> kd.x;
                    s >> kd.y;
                    s >> kd.z;

                    s >> ke.x;
                    s >> ke.y;
                    s >> ke.z;

                    data_materials.insert(std::pair<std::string, Material *>(key, m_objFactory.initDiffuseMaterial(kd, ke)));
                }

                // Unknown material type
                else
                {
                    ERR("Scene: Unknown material type (" << tpe << ")! Loading default diffuse material for (" << key << ")...");

                    data_materials.insert(std::pair<std::string, Material *>(key, m_objFactory.initDiffuseMaterial(vec3(1, 1, 1), vec3(0, 0, 0))));
                }
            }

            // Meshes
            else if (line.substr(0, 5) == "Mesh ")
            {
                std::istringstream s(line.substr(5));

                std::string key; // Name
                std::string trf; // Transform object
                std::string mat; // Material object
                std::string pth; // File path

                s >> key;
                s >> trf;
                s >> mat;
                s >> pth;

                data_meshes.insert(std::pair<std::string, Mesh *>(key, new Mesh(data_transforms.at(trf), data_materials.at(mat), &m_objFactory, pth)));
            }

            // Lights
            else if (line.substr(0, 6) == "Light ")
            {
                std::istringstream s(line.substr(5));

                std::string key; // Name
                std::string trf; // Transform object
                std::string tpe; // Type

                s >> key;
                s >> trf;
                s >> tpe;

                if (tpe == "point")
                {
                    vec3 col; // Emission
                    float aC; // Constant attenuation
                    float aL; // Linear attenuation
                    float aQ; // Quadratic attenuation

                    s >> col.x;
                    s >> col.y;
                    s >> col.z;
                    s >> aC;
                    s >> aL;
                    s >> aQ;

                    data_lights.insert(std::pair<std::string, Light *>(key, new PointLight(data_transforms.at(trf), col, aC, aL, aQ)));
                }
                else
                {
                    ERR("Scene: Unknown light type (" << tpe << ")! Failed to add light (" << key << ") to scene...");
                }
            }
        }
    }

    /* Combine all meshes together */
    std::vector<Shape *> shapes_main;

    // Insert shapes from meshes to the vector
    for (auto const &i : data_meshes)
    {
        Mesh *mesh = data_meshes.at(i.first.c_str());
        std::vector<Shape *> data_mesh_shapes = mesh->getShapes();

        for (size_t j = 0; j < data_mesh_shapes.size(); j++)
        {
            shapes_main.push_back(data_mesh_shapes[j]);
        }

        /* Add all loaded meshes into memory */
        m_meshes.push_back(mesh);
    }

    /* Create and initialize chosen acceleration tree structure */
    if (type_accelerator == "kd-tree")
    {
        m_accelerator = new KDTreeAccel(shapes_main, 1);
        m_accelerator->init();
    }

    /* Add all loaded cameras into memory */
    for (auto const &c : data_cameras)
    {
        m_cameras.push_back(data_cameras.at(c.first.c_str()));
    }

    /* Add all loaded lights into memory */
    for (auto const &l : data_lights)
    {
        m_lights.push_back(data_lights.at(l.first.c_str()));
    }

}

}
