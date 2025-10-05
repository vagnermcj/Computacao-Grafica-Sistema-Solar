#include "texbuffer.h"
#include "state.h"

#include <glm/gtc/type_ptr.hpp>
#ifdef _WIN32
//#define GLAD_GL_IMPLEMENTATION // Necessary for headeronly version.
#include <glad/glad.h>
#elif __APPLE__
#include <OpenGL/gl3.h>
#endif

#include <iostream>

TexBufferPtr TexBuffer::Make (const std::string& varname, const std::vector<float>& data)
{
  return TexBufferPtr(new TexBuffer(varname,data));
}

TexBuffer::TexBuffer (const std::string& varname, const std::vector<float>& data)
: m_varname(varname)
{
  glGenBuffers(1,&m_buffer);
  glGenTextures(1,&m_tex);
  SetData(data);
}

void TexBuffer::SetData (const std::vector<float>& data)
{
  glBindTexture(GL_TEXTURE_BUFFER,m_tex);
  glBindBuffer(GL_TEXTURE_BUFFER,m_buffer);
  glBufferData(GL_TEXTURE_BUFFER,
               data.size()*sizeof(float),
               data.data(),
               GL_DYNAMIC_DRAW);
  glTexBuffer(GL_TEXTURE_BUFFER,GL_R32F,m_buffer);
  glBindTexture(GL_TEXTURE_2D,0);
}

std::vector<float> TexBuffer::GetData () const
{
  std::vector<float> data;
  glBindBuffer(GL_TEXTURE_BUFFER,m_buffer);
  int size = 0;
  glGetBufferParameteriv(GL_TEXTURE_BUFFER,GL_BUFFER_SIZE,&size);
  data.resize(size/sizeof(float));
  glGetBufferSubData(GL_TEXTURE_BUFFER,0,size,data.data());
  glBindBuffer(GL_TEXTURE_BUFFER,0);
  return data;
} 

TexBuffer::~TexBuffer ()
{
}

unsigned int TexBuffer::GetTexId () const
{
  return m_tex;
}

const std::string& TexBuffer::GetName () const
{
  return m_varname;
} 

void TexBuffer::Load (StatePtr st)
{
  ShaderPtr shd = st->GetShader();
  shd->ActiveTexture(m_varname.c_str());
  glBindTexture(GL_TEXTURE_BUFFER,m_tex);
}

void TexBuffer::Unload (StatePtr st)
{
  ShaderPtr shd = st->GetShader();
  shd->DeactiveTexture();
}

