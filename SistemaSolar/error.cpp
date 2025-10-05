#include "error.h"
#ifdef _WIN32
//#define GLAD_GL_IMPLEMENTATION // Necessary for headeronly version.
#include <glad/glad.h>
#elif __APPLE__
#include <OpenGL/gl3.h>
#endif
#include <iostream>
#include <cstdlib>

void Error::Check (const std::string& msg)
{
  GLenum err = glGetError();
  if (err == GL_NO_ERROR) 
    return;
  switch(err) {
    case GL_INVALID_ENUM: std::cerr << "GL error: GL_INVALID_ENUM (" << msg << ")\n"; break;
    case GL_INVALID_VALUE: std::cerr << "GL error: GL_INVALID_VALUE (" << msg << ")\n"; break;
    case GL_INVALID_OPERATION: std::cerr << "GL error: GL_INVALID_OPERATION (" << msg << ")\n"; break;
    case GL_OUT_OF_MEMORY: std::cerr << "GL error: GL_OUT_OF_MEMORY (" << msg << ")\n"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: std::cerr << "GL error: GL_INVALID_FRAMEBUFFER_OPERATION (" << msg << ")\n"; break;
    default: std::cerr << "GL error: unknown error (" << msg << ")\n"; break;
  }
  exit(1);
}