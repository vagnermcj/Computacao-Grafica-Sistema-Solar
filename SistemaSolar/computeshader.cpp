#include "computeshader.h"
#include "shader.h"
#include <iostream>

#ifdef _WIN32
//#define GLAD_GL_IMPLEMENTATION // Necessary for headeronly version.
#include <glad/glad.h>
#elif __APPLE__
#include <OpenGL/gl3.h>
#endif

ComputeShader::ComputeShader(const std::string& filename)
  : m_pid(0), m_texbuffers()
{
  m_sid = Shader::CreateShader(GL_COMPUTE_SHADER, filename);
}

ComputeShaderPtr ComputeShader::Make(const std::string& filename)
{
  return ComputeShaderPtr(new ComputeShader(filename));
}
ComputeShader::~ComputeShader()
{
}

void ComputeShader::AttachTexBuffer(TexBufferPtr texbuf)
{
  m_texbuffers.push_back(texbuf);
}

void ComputeShader::Dispatch(int nx, int ny, int nz)
{
  // Lazily create program on first dispatch 
  if (m_pid == 0) {
    m_pid = glCreateProgram();
    glAttachShader(m_pid,m_sid);
    Shader::LinkProgram(m_pid);
  }

  glUseProgram(m_pid);

  // Bind each texture as an image 
  for (GLuint i = 0; i < m_texbuffers.size(); ++i) {
    TexBufferPtr buf = m_texbuffers[i];
    // Uniform location
    GLint loc = glGetUniformLocation(m_pid, buf->GetName().c_str());
    glUniform1i(loc, i); // bind unit index

    // Bind as image (read-write, layer=0, level=0)
    // Note: for buffer textures, pass GL_FALSE for layered, as in the Python code.
    glBindImageTexture(
      i,                      // image unit
      buf->GetTexId(),         // texture id
      0,                      // mip level
      GL_FALSE,               // layered
      0,                      // layer
      GL_READ_WRITE,          // access
      GL_R32F                 // format (e.g., GL_R32F, GL_RGBA32F)
    );
  }

  // Dispatch
  glDispatchCompute(nx, ny, nz);

  // Memory barriers (mirroring Python)
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT |
                  GL_TEXTURE_FETCH_BARRIER_BIT  |
                  GL_BUFFER_UPDATE_BARRIER_BIT);
}
