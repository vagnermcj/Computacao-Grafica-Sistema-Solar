#include <memory>
class Arcball;
using ArcballPtr = std::shared_ptr<Arcball>; 

#ifndef ARCBALL_H
#define ARCBALL_H

#include <glm/glm.hpp>

class Arcball {
  int m_x0, m_y0;
  float m_distance;
  glm::mat4 m_mat;
protected:
  Arcball (float distance);
public:
  static ArcballPtr Make (float distance);
  void InitMouseMotion (int x, int y);
  void AccumulateMouseMotion (int x, int y);
  const glm::mat4& GetMatrix () const;
};

#endif

