#include "GridShader.h"

#include <cmath>

#include <tiny-piyo/Shaders.h>

void GridShader::Init()
{
    this->_programID = CreateShader(
        R"(
            #version 330 core

            layout(location = 0) in vec3 vertPosition;
            layout(location = 1) in vec4 vertColor;

            out vec3 fragPosition;
            out vec4 fragColor;
            out vec4 delta;

            uniform mat4 view;

            void main() {
                delta = view * vec4(vertPosition, 1.0f);
                gl_Position = delta;
                
                fragPosition = vertPosition;
                fragColor = vertColor;
            }
        )",
        R"(
            #version 330 core

            in vec3 fragPosition;
            in vec4 fragColor;
            in vec4 delta;

            out vec4 color;

            void main() {
                float grid = 2.0;
                float gridWidth = length(delta.xyz)/ 300;

                color = fragColor;
                if (fragPosition.x - (grid * floor(fragPosition.x / grid)) < gridWidth ||
                    fragPosition.z - (grid * floor(fragPosition.z / grid)) < gridWidth) {
                    color = fragColor + vec4(0.7, 0.7, 0.7, 0.0);
                }
            }
        )"
    );

    // Generating VAO, VBO, and IBO
    if (this->_vaoID == 0)
        glGenVertexArrays(1, &this->_vaoID);
    if (this->_vboID == 0)
        glGenBuffers(1, &this->_vboID);
    if (this->_iboID == 0)
        glGenBuffers(1, &this->_iboID);

    // Binding the VAO and VBO, and IBO
    glBindVertexArray(this->_vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, this->_vboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_iboID);

    // Pointing the VAO to the correct location in the given Vertex3D object
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, normal));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

    // Unbinding the VAO
    glBindVertexArray(this->_vaoID);
}

void GridShader::Destroy()
{
    if (this->_vaoID)
        glDeleteVertexArrays(1, &this->_vaoID);
    if (this->_vboID)
        glDeleteBuffers(1, &this->_vboID);
    if (this->_iboID)
        glDeleteBuffers(1, &this->_iboID);

    glDeleteProgram(this->_programID);
}

void GridShader::End()
{
    // Binding the VBO
    glBindBuffer(GL_ARRAY_BUFFER, this->_vboID);

    // Uploading vertex data to the VBO
    glBufferData(GL_ARRAY_BUFFER, this->_vertices.size() * sizeof(Vertex3D), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->_vertices.size() * sizeof(Vertex3D), this->_vertices.data());
    
    // Unbinding the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Binding the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_iboID);

    // Uploading index data to the IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_indices.size() * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, this->_indices.size() * sizeof(unsigned int), this->_indices.data());

    // Unbinding the IBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Clearing vertex and index vectors
    this->_numIndices = this->_indices.size();
    this->_vertices.clear();
    this->_indices.clear();
}

void GridShader::Render()
{
    // Using GLSL program and binding the VAO
    this->Use();
    glBindVertexArray(this->_vaoID);

    //glLineWidth(2.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    // Drawing vertices
    glDrawElements(this->drawQuads ? GL_QUADS : GL_LINES, this->_numIndices, GL_UNSIGNED_INT, 0);

    // Unbinding the GLSL program and unbinding the VAO
    glBindVertexArray(0);
    this->Unuse();
}

void GridShader::DrawTri(Vertex3D a, Vertex3D b, Vertex3D c)
{
    int firstIndex = this->_vertices.size();

    // Pushing vertices into vector
    this->_vertices.reserve(this->_vertices.size() + 3);
    this->_vertices.push_back(a);
    this->_vertices.push_back(b);
    this->_vertices.push_back(c);

    // Pushing corresponding indices into vector
    this->_indices.reserve(this->_indices.size() + 3);
    this->_indices.push_back(firstIndex);
    this->_indices.push_back(firstIndex + 1);
    this->_indices.push_back(firstIndex + 2);
}

void GridShader::DrawLine(Vertex3D a, Vertex3D b)
{
    int firstIndex = this->_vertices.size();

    // Pushing vertices into vector
    this->_vertices.reserve(this->_vertices.size() + 2);
    this->_vertices.push_back(a);
    this->_vertices.push_back(b);

    // Pushing corresponding indices into vector
    this->_indices.reserve(this->_indices.size() + 2);
    this->_indices.push_back(firstIndex);
    this->_indices.push_back(firstIndex + 1);
}

void GridShader::DrawGrid(
        double (*func)(double, double, double),
        ColorRGBA8 (*color)(double, double, double),
        double xmin, double xmax,
        double ymin, double ymax,
        double t)
{
    int gridX = 0;
    int gridY = 0;

    int row = std::ceil((xmax - xmin) / this->_deltaX) + 1;
    int col = std::ceil((ymax - ymin) / this->_deltaY) + 1;
    this->_vertices.reserve(
            this->_vertices.size() +
            row * col - 1);
    this->_indices.reserve(
            this->_indices.size() +
            row * col * 4 - row - col);

    for (int indexX = 0; indexX < row; indexX++)
    {
        double x = xmin + this->_deltaX * indexX;
        x = (x > xmax) ? xmax : x;
        for (int indexY = 0; indexY < col; indexY++ )
        {
            double y = ymin + this->_deltaY * indexY;
            y = (y > ymax) ? ymax : y;

            int firstIndex = this->_vertices.size();
            
            this->_vertices.push_back(Vertex3D(
                    float3(
                        x, func(x, y, t), y),
                    color(x, y, t)));

            if (this->drawQuads) {
                if (y != ymin && x != xmin) {
                    this->_indices.push_back(firstIndex);
                    this->_indices.push_back(firstIndex - 1);
                    this->_indices.push_back(firstIndex - row - 1);
                    this->_indices.push_back(firstIndex - row);
                }
            }
            else {
                if (y != ymin)
                {
                    this->_indices.push_back(firstIndex);
                    this->_indices.push_back(firstIndex - 1);
                }
                if (x != xmin) {
                    this->_indices.push_back(firstIndex);
                    this->_indices.push_back(firstIndex - row);
                }
            }

            gridY++;
        }
        gridX++;
    }
}
