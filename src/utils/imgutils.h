#ifndef IMGUTILS_H
#define IMGUTILS_H

// std includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// mirage includes
#include "../macros.h"

namespace mirage
{

namespace util
{

struct PPMPixel
{
    int r,g,b;

    PPMPixel(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}

    int operator[](int i) const
    {
        assert(i >= 0 && i <= 2);
        return (&r)[i];
    }

    int &operator[](int i)
    {
        assert(i >= 0 && i <= 2);
        return (&r)[i];
    }
};

struct PPMImage
{
    int width, height;
    std::vector<PPMPixel> data;
};

static PPMImage loadPPM(std::string fileName)
{
    PPMImage result;

    std::ifstream file;
    std::string line;
    std::vector<std::string> lines;

    LOG("IMGUtils: Attempting to load: " << fileName);

    file.open("res/" + fileName);

    // Read the file into memory
    if (file.is_open())
    {
        while (file.good())
        {
            std::getline(file, line);

            if (line.empty() || line.substr(0, 1) == "#")
            {
                continue;
            }

            lines.push_back(line);
        }
    }

    std::vector<int> components;

    // Load the image data into components
    for (size_t i = 0; i < lines.size(); i++)
    {
        if (i == 0)
        {
            if (lines[i] != "P3")
            {
                ERR("IMGUtils: Failed to load " << fileName << " correctly, invalid file header.");
                return result;
            }
        }
        else if (i == 1)
        {
            std::istringstream s(lines[i]);

            s >> result.width;
            s >> result.height;
        }
        else
        {
            if (result.width <= 0 && result.height <= 0)
            {
                ERR("IMGUtils: Failed to load " << fileName << " correctly, size is zero.");
                return result;
            }

            std::istringstream s(lines[i]);

            while (s.rdbuf()->in_avail() != 0)
            {
                int component;

                s >> component;

                components.push_back(component);
            }
        }
    }

    // Load the components into result <-- TODO: Add error checking, etc, later.. for all procedures
    for (size_t i = 0; i < components.size(); i += 3)
    {
        PPMPixel pixel(components[i], components[i + 1], components[i + 2]);
        result.data.push_back(pixel);
    }

    LOG("IMGUtils: " << fileName << " has been loaded succesfully. w: " << result.width << " h: " << result.height);

    return result;
}

}

}

#endif // IMGUTILS_H
