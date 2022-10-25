#version 450

#define INDEX 0

layout (location = INDEX) out vec4 frag_color;

in vec3 sortCouleur;
uniform float luminosite = 1.f;

void main() 
{
	frag_color = vec4(sortCouleur * luminosite, 1.f);
}
