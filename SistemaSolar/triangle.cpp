#include "triangle.h"

#include <iostream>

#ifdef _WIN32
//#define GLAD_GL_IMPLEMENTATION // Necessary for headeronly version.
#include <glad/glad.h>
#elif __APPLE__
#include <OpenGL/gl3.h>
#endif

TrianglePtr Triangle::Make ()
{
  return TrianglePtr(new Triangle());
}

Triangle::Triangle ()
{
  float coord[] = {-1.0f,0.0f,1.0f,0.0f,0.0f,1.0f};
  // create VAO
  glGenVertexArrays(1,&m_vao);
  glBindVertexArray(m_vao);
  // create coord buffer
  GLuint id;
  glGenBuffers(1,&id);
  glBindBuffer(GL_ARRAY_BUFFER,id);
  glBufferData(GL_ARRAY_BUFFER,sizeof(coord),coord,GL_STATIC_DRAW);
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);  // coord
  glEnableVertexAttribArray(0);
}

Triangle::~Triangle () 
{
}

void Triangle::Draw (StatePtr st)
{
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES,0,3);
}