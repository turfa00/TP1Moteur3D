#version 450

layout (location = 0) out vec4 frag_color;

in vec3 sortCouleur;
uniform float luminosite = 1.f;

void main() 
{
	frag_color = vec4(sortCouleur * luminosite, 1.f);
}
