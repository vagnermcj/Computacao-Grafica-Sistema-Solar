#include <memory>
class TexBuffer;
using TexBufferPtr = std::shared_ptr<TexBuffer>; 

#ifndef TEXBUFFER_H
#define TEXBUFFER_H

#include "appearance.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class TexBuffer : public Appearance {
  unsigned int m_tex;
  unsigned int m_buffer;
  std::string m_varname;
protected:
  TexBuffer (const std::string& varname, const std::vector<float>& data);
public:
  static TexBufferPtr Make (const std::string& varname, const std::vector<float>& data);
  void SetData (const std::vector<float>& data);
  std::vector<float> GetData () const;
  virtual ~TexBuffer ();
  unsigned int GetTexId () const;
  const std::string& GetName () const;
  virtual void Load (StatePtr st);
  virtual void Unload (StatePtr st);
};

#endif