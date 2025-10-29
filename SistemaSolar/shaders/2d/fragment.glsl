#version 410

uniform sampler2D face;

out vec4 outcolor;
in vec2 vTexcoord;

void main (void)
{
  outcolor = texture(face, vTexcoord);
}
