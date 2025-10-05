#version 410

in data {
  vec4 color;
  vec2 texcoord;
} f;

out vec4 color;

uniform sampler2D decal;

void main (void)
{
  color = f.color * texture(decal,f.texcoord);
}

