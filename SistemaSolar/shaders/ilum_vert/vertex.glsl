#version 410

layout(location = 0) in vec4 coord;
layout(location = 1) in vec3 normal;

uniform mat4 Mv; 
uniform mat4 Mn; 
uniform mat4 Mvp;

out vec3 veye;
out vec3 neye;

void main(void)
{
    veye = vec3(Mv * coord);                      
    neye = normalize(vec3(Mn * vec4(normal, 0.0)));
    gl_Position = Mvp * coord;
}
