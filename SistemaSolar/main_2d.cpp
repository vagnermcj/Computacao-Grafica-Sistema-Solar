
#ifdef _WIN32
//#define GLAD_GL_IMPLEMENTATION // Necessary for headeronly version.
#include <glad/glad.h>
#elif __APPLE__
#include <OpenGL/gl3.h>
#endif
#include <GLFW/glfw3.h>

#include "scene.h"
#include "state.h"
#include "camera2d.h"
#include "color.h"
#include "transform.h"
#include "error.h"
#include "shader.h"
#include "triangle.h"
#include "disk.h"
#include "texture.h"
#include "quad.h"

#include <iostream>

static ScenePtr scene;
static CameraPtr camera;

class OrbitTranslation;
using OrbitTranslationPtr = std::shared_ptr<OrbitTranslation>;

class OrbitTranslation : public Engine
{
    TransformPtr m_trf;
    float m_radius;
    float m_angle;
    float m_speed;

protected:
    OrbitTranslation(TransformPtr trf, float radius, float speed)
        : m_trf(trf), m_radius(radius), m_speed(speed), m_angle(0.0f)
    {
    }

public:
    static OrbitTranslationPtr Make(TransformPtr trf, float radius, float speed)
    {
        return OrbitTranslationPtr(new OrbitTranslation(trf, radius, speed));
    }

    virtual void Update(float dt)
    {
        m_angle += m_speed * -dt;

        m_trf->LoadIdentity();
        m_trf->Rotate(m_angle, 0, 0, 1);
        m_trf->Translate(m_radius, 0.0f, 0.0f);
    }
};

class PlanetRotation;
using PlanetRotationPtr = std::shared_ptr<PlanetRotation>;

class PlanetRotation : public Engine
{
    TransformPtr m_trf;
    float m_speed;

protected:
    PlanetRotation(TransformPtr trf, float speed)
        : m_trf(trf), m_speed(speed)
    {
    }

public:
    static PlanetRotationPtr Make(TransformPtr trf, float speed)
    {
        return PlanetRotationPtr(new PlanetRotation(trf, speed));
    }

    virtual void Update(float dt)
    {
        m_trf->Rotate(m_speed * -dt, 0, 0, 1);
    }
};


static void initialize (void)
{
  // set background color: white 
  glClearColor(0, 0, 0, 0);
  // enable depth test 
  glEnable(GL_DEPTH_TEST);

  auto shader = Shader::Make();
  shader->AttachVertexShader("./shaders/2d/vertex.glsl");
  shader->AttachFragmentShader("./shaders/2d/fragment.glsl");
  shader->Link();

  // create objects
  camera = Camera2D::Make(0,10,0,10);

  //Moon setup
  auto moonSpriteTex = Texture::Make("face", "images/Moon.png");
  auto moonTrf = Transform::Make();
  moonTrf->Scale(0.1f, 0.1f, 1.0f);
  auto moon = Node::Make(moonTrf, { moonSpriteTex },{ Disk::Make()}); //General and Sprite Node

  //Earth Setup
  auto earthSpriteTex = Texture::Make("face", "images/Earth.png");
  auto earthSpriteTrf = Transform::Make();
  auto moonOrbitTrf = Transform::Make();
  earthSpriteTrf->Scale(0.4f, 0.4f, 1.0f);

  auto earthSprite = Node::Make(earthSpriteTrf, { earthSpriteTex }, { Disk::Make() }); //Earth Sprite Node
  auto moonOrbit = Node::Make(moonOrbitTrf, { moon });
  auto earth = Node::Make({earthSprite, moonOrbit}); //General Earth Node

  //Mercury Setup
  auto mercurySpriteTex = Texture::Make("face", "images/Mercury.png");
  auto mercurySpriteTrf = Transform::Make();
  mercurySpriteTrf->Scale(0.2f, 0.2f, 1.0f);
  
  auto mercurySprite = Node::Make(mercurySpriteTrf, { mercurySpriteTex }, { Disk::Make() }); //Mercury Sprite Node
  auto mercury = Node::Make({mercurySprite}); //General Mercury Node
  
  //Sun Setup
  auto sunSpriteTex = Texture::Make("face", "images/Sun.png");
  auto sunTrf = Transform::Make();
  auto earthOrbitTrf = Transform::Make();
  auto mercuryOrbitTrf = Transform::Make();
  sunTrf->Translate(5.0f,5.0f,1.0f);

  auto sunSprite = Node::Make({ sunSpriteTex }, { Disk::Make() }); //Sprite Node
  auto earthOrbit = Node::Make(earthOrbitTrf, {earth}); //Orbit Node
  auto mercuryOrbit = Node::Make(mercuryOrbitTrf, {mercury}); //Orbit Node

  auto sun = Node::Make(sunTrf, {sunSprite, earthOrbit, mercuryOrbit}); //General Sun Node

  //Space Setup
  auto spaceSpriteTex = Texture::Make("face", "images/space.jpg");
  auto spaceTrf = Transform::Make();
  spaceTrf->Translate(-5.0f, 0.0f, 0.0f);
  spaceTrf->Scale(20.0f, 10.0f, 1.0f);
  auto spacePrite = Node::Make(spaceTrf, { spaceSpriteTex }, { Quad::Make() }); //Space Sprite Node
  auto space = Node::Make( { sun, spacePrite } ); //Space Node

  // build scene
  auto root = Node::Make(shader, { space });
  scene = Scene::Make(root);
  scene->AddEngine(OrbitTranslation::Make(earthOrbitTrf, 3.5f, 10.0f));
  scene->AddEngine(OrbitTranslation::Make(moonOrbitTrf, 0.8f, 20.0f));
  scene->AddEngine(OrbitTranslation::Make(mercuryOrbitTrf, 2.0f, 55.0f));
  scene->AddEngine(PlanetRotation::Make(earthSpriteTrf, 100.0f));
}

static void display (GLFWwindow* win)
{ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear window 
  Error::Check("before render");
  scene->Render(camera);
  Error::Check("after render");
}

static void error (int code, const char* msg)
{
  printf("GLFW error %d: %s\n", code, msg);
  glfwTerminate();
  exit(0);
}

static void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize (GLFWwindow* win, int width, int height)
{
  glViewport(0,0,width,height);
}

static void update (float dt)
{
  scene->Update(dt);
}

int main ()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);       // required for mac os
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);  // option for mac os
#endif

    glfwSetErrorCallback(error);

    GLFWwindow* win = glfwCreateWindow(1024, 600, "Window title", nullptr, nullptr);
    assert(win);
    glfwSetFramebufferSizeCallback(win, resize);  // resize callback
    glfwSetKeyCallback(win, keyboard);            // keyboard callback

    glfwMakeContextCurrent(win);
#ifdef __glad_h_
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD OpenGL context\n");
        exit(1);
    }
#endif
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

  initialize();

  float t0 = float(glfwGetTime());
  while(!glfwWindowShouldClose(win)) {
    float t = float(glfwGetTime());
    update(t-t0);
    t0 = t;
    display(win);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

