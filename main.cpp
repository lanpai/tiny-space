#include "GraphEngine.h"

#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ratio>

#define G 6.674e-11

#define Sun_Radius 6378100
#define Sun_Density 1408
#define Body_SUN(x, y) Body(Sun_Density, Sun_Radius, x, y)

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
    float2 vel;
    float2 acc;
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

float2 CalcField(double x, double y)
{
    double fieldX = 0.0;
    double fieldY = 0.0;

    for (Body body : bodies) {
        if (body.x == x && body.y == y) continue;

        double deltaX = body.x - x;
        double deltaY = body.y - y;
        double distance = sqrt(deltaX * deltaX + deltaY * deltaY);

        double radius = std::min(body.radius, distance);

        double mass = body.density * (4.0 / 3.0) * PI * radius * radius * radius;

        double fieldStrength = (G * mass)/(distance * distance);

        fieldX += (deltaX / distance) * fieldStrength;
        fieldY += (deltaY / distance) * fieldStrength;
    }

    return float2(fieldX, fieldY);
}

double PlanetaryBodies(double x, double y, double t)
{
    x *= Sun_Radius / 3.5;
    y *= Sun_Radius / 3.5;

    //x += bodies[0].x;
    //y += bodies[0].y;

    float2 field = CalcField(x, y);
    return -1.0 * sqrt(field.x * field.x + field.y * field.y);
}

ColorRGBA8 PlanetaryBodiesColor(double x, double y, double t)
{
    return ColorHSVA8((int)std::fmod(-100.0 * PlanetaryBodies(x, y, t) + 190, 360.0), 0.75f, 1.0f, 1.0f).ConvertToRGBA8();
}

void PlanetaryUpdate(double delta)
{
    delta *= 60;
    double totalDelta = delta;
    double maxDeltaStep = 0.05;
    for (double deltaElapsed = 0.0;
            deltaElapsed < totalDelta;
            deltaElapsed += std::min(maxDeltaStep, totalDelta - deltaElapsed)) {
        delta = std::min(maxDeltaStep, totalDelta - deltaElapsed);

        for (Body &body : bodies) {
            float2 field = CalcField(body.x, body.y);
            body.acc = field;
        }

        for (Body &body : bodies) {
            body.vel.x = body.vel.x + body.acc.x * delta;
            body.vel.y = body.vel.y + body.acc.y * delta;
        }

        for (Body &body : bodies) {
            body.x = body.x + (body.vel.x * delta);
            body.y = body.y + (body.vel.y * delta);
        }
    }
}

int main(int argc, char *argv[]) {
    std::printf("tiny-space\n");

    bodies.push_back(Body_SUN(0, 0));
    bodies.push_back(Body(Sun_Density * 0.7, Sun_Radius * 0.7, 0, 6.0 * Sun_Radius));
    bodies.back().vel.x = 1.5e3;
    bodies.push_back(Body(Sun_Density * 0.7, Sun_Radius * 0.7, 0, -6.0 * Sun_Radius));
    bodies.back().vel.x = -1.5e3;
    bodies.push_back(Body(Sun_Density * 0.7, Sun_Radius * 0.7, -6.0 * Sun_Radius, 0));
    bodies.back().vel.y = -1.5e3;
    bodies.push_back(Body(Sun_Density * 0.7, Sun_Radius * 0.7, 6.0 * Sun_Radius, 0));
    bodies.back().vel.y = 1.5e3;

    GraphEngine engine(&PlanetaryBodies, &PlanetaryBodiesColor);
    engine.SetOnUpdate(&PlanetaryUpdate);
    engine.Run();

    return 0;
}
