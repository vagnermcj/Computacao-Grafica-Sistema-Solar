#include "quad.h"
#include "error.h"
#include "grid.h"

#include <iostream>

#ifdef _WIN32
#include <glad/glad.h>
#elif __APPLE__
#include <OpenGL/gl3.h>
#endif

QuadPtr Quad::Make(int nx, int ny)
{
    return QuadPtr(new Quad(nx, ny));
}

Quad::Quad(int nx, int ny)
{
    GridPtr grid = Grid::Make(nx, ny);
    m_nind = grid->IndexCount();

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    GLuint vbo[2];
    glGenBuffers(2, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 2 * grid->VertexCount() * sizeof(float),
        grid->GetCoords(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // posição (x, y)
    glEnableVertexAttribArray(0);

    // --- Coordenadas de textura ---
    float* texcoords = new float[2 * grid->VertexCount()];
    for (int i = 0; i < grid->VertexCount(); ++i)
    {
        float x = grid->GetCoords()[2 * i];
        float y = grid->GetCoords()[2 * i + 1];
        texcoords[2 * i] = (x + 1.0f) / 2.0f;
        texcoords[2 * i + 1] = (y + 1.0f) / 2.0f;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 2 * grid->VertexCount() * sizeof(float),
        texcoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); // texcoord
    glEnableVertexAttribArray(1);

    delete[] texcoords;

    GLuint index;
    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        m_nind * sizeof(unsigned int),
        grid->GetIndices(),
        GL_STATIC_DRAW);
}

Quad::~Quad()
{
}

void Quad::Draw(StatePtr)
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_nind, GL_UNSIGNED_INT, 0);
}
