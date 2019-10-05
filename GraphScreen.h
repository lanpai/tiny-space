#ifndef __GRAPH_SCREEN_H
#define __GRAPH_SCREEN_H

#include <tiny-piyo/IScreen.h>
#include <tiny-piyo/Camera.h>

#include "GridShader.h"

class GraphScreen : public IScreen
{
    public:
        void SetFunc(double (*func)(double, double, double)) { this->_func = func; }
        void SetColor(ColorRGBA8 (*color)(double, double, double)) { this->_color = color; }

    protected:
        virtual void OnInit() override;
        virtual void OnDestroy() override;
        virtual void OnUpdate() override;
        virtual void OnDraw() override;

    private:
        GridShader _shader;
        Camera _camera;
        float2 _prevCursorPos;

        double _time;

        double (*_func)(double, double, double);
        ColorRGBA8 (*_color)(double, double, double);
};

#endif
