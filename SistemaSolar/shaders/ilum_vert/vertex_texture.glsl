#version 410

layout(location = 0) in vec4 coord;
layout(location = 1) in vec3 normal;
layout(location = 3) in vec2 texcoord;

uniform mat4 Mv; 
uniform mat4 Mn; 
uniform mat4 Mvp;

uniform vec4 lpos;  // light pos in eye space
uniform vec4 lamb;
uniform vec4 ldif;
uniform vec4 lspe;

uniform vec4 mamb;
uniform vec4 mdif;
uniform vec4 mspe;
uniform float mshi;

out data {
  vec4 color;
  vec2 texcoord;
} v;

void main (void) 
{
  vec3 veye = vec3(Mv*coord);
  vec3 light;
  if (lpos.w == 0) 
    light = normalize(vec3(lpos));
  else 
    light = normalize(vec3(lpos)-veye); 
  vec3 neye = normalize(vec3(Mn*vec4(normal,0.0f)));
  float ndotl = dot(neye,light);
  v.color = mamb*lamb + mdif * ldif * max(0,ndotl); 
  if (ndotl > 0) {
    vec3 refl = normalize(reflect(-light,neye));
    v.color += mspe * lspe * pow(max(0,dot(refl,normalize(-veye))),mshi); 
  }
  v.texcoord = texcoord;
  gl_Position = Mvp*coord; 
}

