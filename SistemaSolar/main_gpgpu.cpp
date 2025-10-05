
#ifdef _WIN32
#define GLAD_GL_IMPLEMENTATION // Necessary for headeronly version.
#include <glad/glad.h>
#elif __APPLE__
#include <OpenGL/gl3.h>
#endif
#include <GLFW/glfw3.h>

#include "computeshader.h"
#include "texbuffer.h"

#include <iostream>
#include <cassert>
#include <vector>


static void initialize (void)
{
    std::vector<float> vet = { 1, 2, 3, 4 };
    auto buf = TexBuffer::Make("data", vet);
    auto cs = ComputeShader::Make("../shaders/cs/compute_shader.glsl");
    cs->AttachTexBuffer(buf);
    cs->Dispatch(1);
    vet = buf->GetData();
    for (auto x: vet)
      std::cout << x << std::endl;
}

static void error (int code, const char* msg)
{
  printf("GLFW error %d: %s\n", code, msg);
  glfwTerminate();
  exit(0);
}

int main ()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);       // required for mac os
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER,GLFW_TRUE);  // option for mac os
#endif

  glfwSetErrorCallback(error);

  GLFWwindow* win = glfwCreateWindow(600,400,"Window title",nullptr,nullptr);
  assert(win);

  glfwMakeContextCurrent(win);
#ifdef __glad_h_
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      printf("Failed to initialize GLAD OpenGL context\n");
      exit(1);
  }
#endif
  printf("OpenGL version: %s\n", glGetString(GL_VERSION));

  initialize();

  glfwTerminate();
  return 0;
}

