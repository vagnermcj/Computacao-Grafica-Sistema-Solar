#version 410

in vec3 veye;
in vec3 neye;

uniform vec4 lpos;
uniform vec4 lamb;
uniform vec4 ldif;
uniform vec4 lspe;

uniform vec4 mamb;
uniform vec4 mdif;
uniform vec4 mspe;
uniform float mshi;

out vec4 fcolor;

void main(void)
{
    vec3 v = normalize(-veye);
    vec3 n = normalize(neye);
    vec3 light = normalize((lpos.w == 0) ? vec3(lpos) : vec3(lpos) - veye);

    // Difusa + Ambiente
    float ndotl = max(dot(n, light), 0.0);
    vec4 color = mamb * lamb + mdif * ldif * ndotl;

    // Especular
    if (ndotl > 0.0) {
        vec3 refl = normalize(reflect(-light, n));
        color += pow(max(dot(refl, v), 0.0), mshi) * mspe * lspe;
    }
    
    fcolor = color;
  
}
