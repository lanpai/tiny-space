#include "GraphEngine.h"

#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>

#define G 6.674 * 0.00000000001

struct Body
{
    Body(double density, double radius, double x, double y)
    {
        this->density = density;
        this->radius = radius;
        this->x = x;
        this->y = y;
    }
    double density;
    double radius;
    double x, y;
};

std::vector<Body> bodies;

double Spiral(double x, double y, double t)
{
    return (cos(sqrt(x * x + y * y) + t) + 1.0) / 2.0;
}

ColorRGBA8 SpiralColor(double x, double y, double t)
{
    return ColorRGBA8(
            (1 - ((cos(sqrt(x * x + y * y) + t) + 1.0) / 2.0)) * 255,
            127,
            ((cos(sqrt(x * x + y * y) + t) + 1.0) / 2.0) * 255,
            255);
}

double Exp(double x, double y, double t)
{
    return x * x + y * y;
}

ColorRGBA8 ExpColor(double x, double y, double t)
{
    return ColorRGBA8(
            127,
            127,
            255 * (-(1/(Exp(x, y, t) + 1)) + 1),
            255);
}

double PlanetaryBodies(double x, double y, double t)
{
    double fieldX = 0.0f;
    double fieldY = 0.0f;

    for (Body body : bodies) {
        double deltaX = body.x - x;
        double deltaY = body.y - y;
        double distance = sqrt(deltaX * deltaX + deltaY * deltaY);

        double radius = std::min(body.radius, distance);

        double mass = body.density * (4.0 / 3.0) * PI * radius * radius * radius;

        double fieldStrength = (G * mass)/(distance * distance);

        fieldX += (deltaX / distance) * fieldStrength;
        fieldY += (deltaY / distance) * fieldStrength;
    }

    return -1.0 * sqrt(fieldX * fieldX + fieldY * fieldY);


    /*double mass = 0.0;
    double sumX = 0.0f;
    double sumY = 0.0f;
    for (Body body : bodies)
    {
        double deltaX = std::abs(body.x - x);
        double deltaY = std::abs(body.y - y);

        sumX += body.x;
        sumY += body.y;

        double radius = std::min(body.radius, sqrt(deltaX * deltaX + deltaY * deltaY));
        mass += body.density * (4.0/3.0) * PI * radius * radius * radius;
    }

    double avgX = sumX / bodies.size();
    double avgY = sumY / bodies.size();

    double deltaX = std::abs(avgX - x);
    double deltaY = std::abs(avgY - y);

    double denom = sqrt(deltaX * deltaX + deltaY * deltaY);
    denom *= denom;
    return -1 * (G * mass)/denom;*/
}

ColorRGBA8 PlanetaryBodiesColor(double x, double y, double t)
{
    //return ColorRGBA8(std::min(-1.0 * PlanetaryBodies(x, y, t), 1.0) * 255, 127, 255, 255);
    return ColorHSVA8((int)std::fmod(-50.0 * PlanetaryBodies(x, y, t) + 190, 360.0), 1.0f, 1.0f, 1.0f).ConvertToRGBA8();
}

int main(int argc, char *argv[]) {
    std::printf("tiny-space\n");

    ColorRGBA8 test = ColorHSVA8(164, 1.0, 0.6, 1.0).ConvertToRGBA8();
    std::printf("(%d, %d, %d)\n", test.r, test.g, test.b);

    bodies.push_back(Body(2000000000.0, 10.0, 0.0, 0.0));
    bodies.push_back(Body(1000000000.0, 10.0, 20.0, 10.0));

    GraphEngine engine(&PlanetaryBodies, &PlanetaryBodiesColor);
    engine.Run();

    return 0;
}
