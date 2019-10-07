#ifndef __GRAPH_ENGINE_H
#define __GRAPH_ENGINE_H

#include <tiny-piyo/IEngine.h>

#include "GraphScreen.h"

class GraphEngine : public IEngine
{
    public:
        GraphEngine(
                double (*func)(double, double, double),
                ColorRGBA8 (*color)(double, double, double));
        void SetOnUpdate(void (*update)(double)) { this->_update = update; };

    protected:
        virtual void OnPreInit() override;
        virtual void OnPostInit() override;
        virtual void OnUpdate() override;

    private:
        static void _DefaultUpdate(double) {};
        GraphScreen *_graph;
        void (*_update)(double);
};

#endif
