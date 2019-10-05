#ifndef __GRID_SHADER_H
#define __GRID_SHADER_H

#include <tiny-piyo/IGLSLShader.h>
#include <tiny-piyo/Types.h>

#include <vector>

class GridShader : public IGLSLShader
{
    public:
        void Init();
        void Destroy();

        void End();

        void Render();

        void DrawTri(Vertex3D a, Vertex3D b, Vertex3D c);
        void DrawLine(Vertex3D a, Vertex3D b);
        void DrawGrid(
                double (*func)(double, double, double),
                ColorRGBA8 (*color)(double, double, double),
                double xmin, double xmax,
                double ymin, double ymax,
                double t);

    private:
        unsigned int _vaoID;
        unsigned int _vboID; 
        unsigned int _iboID;

        int _numIndices;
        std::vector<Vertex3D> _vertices;
        std::vector<unsigned int> _indices;

        double _deltaX = 0.5;
        double _deltaY = 0.5;
};

#endif
