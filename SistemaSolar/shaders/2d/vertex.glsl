#version 410

layout (location=0) in vec4 vertex;
layout (location=1) in vec2 aTexcoord;

uniform mat4 Mvp;
out vec2 vTexcoord;

void main (void)
{
  vTexcoord = aTexcoord;
  gl_Position = Mvp * vertex;
}

