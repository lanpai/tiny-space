#include "GraphScreen.h"

#include <tiny-piyo/Input.h>

void GraphScreen::OnInit()
{
    this->_shader.Init();
    this->_camera.Init(640, 480, 90.0f, 0.01f, 1000.0f);
    this->GetInputManager()->LockMouse();
}

void GraphScreen::OnDestroy()
{
    this->_shader.Destroy();
}

void GraphScreen::OnUpdate()
{
    this->_time += this->engine->GetFrameTime();

    float2 cursorPos = this->GetInputManager()->GetCursorPos();
    this->_camera.Rotate(((cursorPos - this->_prevCursorPos) / 200.0f).x, -((cursorPos - this->_prevCursorPos) / 200.0f).y, 0.0f);
    this->_prevCursorPos = cursorPos;

    if (this->GetInputManager()->IsKeyPressed(GLFW_KEY_R))
        this->_camera.SetRotation(0.0f, 0.0f, 0.0f);

    this->_camera.SetRotation(
        this->_camera.GetRotation().x,
        std::fmin(std::fmax(this->_camera.GetRotation().y, -PI / 2.0f), PI / 2.0f),
        this->_camera.GetRotation().z
    );
    float delta = this->engine->GetDelta() / 7;
    if (this->GetInputManager()->IsKeyDown(GLFW_KEY_W))
        this->_camera.Translate(0.0f, 0.0f, delta);
    if (this->GetInputManager()->IsKeyDown(GLFW_KEY_A))
        this->_camera.Translate(-delta, 0.0f, 0.0f);
    if (this->GetInputManager()->IsKeyDown(GLFW_KEY_S))
        this->_camera.Translate(0.0f, 0.0f, -delta);
    if (this->GetInputManager()->IsKeyDown(GLFW_KEY_D))
        this->_camera.Translate(delta, 0.0f, 0.0f);
    if (this->GetInputManager()->IsKeyDown(GLFW_KEY_Q))
        this->_camera.Translate(0.0f, delta, 0.0f);
    if (this->GetInputManager()->IsKeyDown(GLFW_KEY_E))
        this->_camera.Translate(0.0f, -delta, 0.0f);
}

void GraphScreen::OnDraw()
{
    this->_camera.Update(this->_shader);
    this->_shader.DrawGrid(
            this->_func,
            this->_color,
            -25.0, 25.0,
            -25.0, 25.0,
            this->_time);
    /*this->_shader.DrawLine(
            Vertex3D(
                float3(-10.0, 0.0, 0.0),
                ColorRGBA8(127, 127, 127, 255)),
            Vertex3D(
                float3(10.0, 0.0, 0.0),
                ColorRGBA8(255, 255, 255, 255)));
    this->_shader.DrawLine(
            Vertex3D(
                float3(0.0, 0.0, -10.0),
                ColorRGBA8(127, 127, 127, 255)),
            Vertex3D(
                float3(0.0, 0.0, 10.0),
                ColorRGBA8(255, 255, 255, 255)));
    this->_shader.DrawLine(
            Vertex3D(
                float3(0.0, -10.0, 0.0),
                ColorRGBA8(127, 127, 127, 255)),
            Vertex3D(
                float3(0.0, 10.0, 0.0),
                ColorRGBA8(255, 255, 255, 255)));*/
    this->_shader.End();
    this->_shader.Render();
}
