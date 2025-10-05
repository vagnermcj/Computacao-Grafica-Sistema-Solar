#include "disk.h"
#define _USE_MATH_DEFINES
#include <cmath>

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <OpenGL/gl3.h>
#endif

#define PI 3.14159265f

DiskPtr Disk::Make(int nslices)
{
    return DiskPtr(new Disk(nslices));
}

Disk::Disk(int nslices)
{
    GLuint vao, vbo[2];
    this->vertexCount = nslices + 2; // centro + nslices + repetir 1º vértice

    float* vertices = new float[vertexCount * 2];
    float* texcoords = new float[vertexCount * 2];

    // centro
    vertices[0] = 0.0f; vertices[1] = 0.0f;
    texcoords[0] = 0.5f; texcoords[1] = 0.5f;

    // circunferência
    for (int i = 0; i <= nslices; i++) {
        float angle = 2.0f * PI * i / nslices;
        float x = cos(angle);
        float y = sin(angle);

        vertices[2 * (i + 1)] = x;
        vertices[2 * (i + 1) + 1] = y;

        // mapeamento de textura (de -1..1 para 0..1)
        texcoords[2 * (i + 1)] = 0.5f + x * 0.5f;
        texcoords[2 * (i + 1) + 1] = 0.5f + y * 0.5f;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // posição
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // textura
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), texcoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    this->m_vao = vao;

    delete[] vertices;
    delete[] texcoords;
}

void Disk::Draw(StatePtr st)
{
    glBindVertexArray(this->m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

Disk::~Disk()
{
    glDeleteVertexArrays(1, &m_vao);
}
