
#include <memory>

class ComputeShader;
using ComputeShaderPtr = std::shared_ptr<ComputeShader>;

#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H

#include "texbuffer.h"
#include <string>
#include <vector>

class ComputeShader { 
  unsigned int m_sid;
  unsigned int m_pid = 0;
  std::vector<TexBufferPtr> m_texbuffers;

protected:
  ComputeShader(const std::string& filename);

public:
  static ComputeShaderPtr Make(const std::string& filename);
  ~ComputeShader();

  // Attach a texture buffer to the compute shader
  void AttachTexBuffer(const TexBufferPtr texbuf);

  // Create program if needed, bind images & uniforms, and dispatch
  void Dispatch(int nx, int ny = 1, int nz = 1);
};

#endif