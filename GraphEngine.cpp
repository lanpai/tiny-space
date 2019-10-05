#include "GraphEngine.h"

GraphEngine::GraphEngine(
        double (*func)(double, double, double),
        ColorRGBA8 (*color)(double, double, double))
{
    this->_graph = new GraphScreen;
    this->_graph->SetFunc(func);
    this->_graph->SetColor(color);
}

void GraphEngine::OnPreInit()
{
    this->name = "tiny-space";
}

void GraphEngine::OnPostInit()
{
    this->AddScreen("main", this->_graph);
    this->ChangeScreen("main");
}
