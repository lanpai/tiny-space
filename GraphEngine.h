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

    protected:
        virtual void OnPreInit() override;
        virtual void OnPostInit() override;

    private:
        GraphScreen *_graph;
};

#endif
