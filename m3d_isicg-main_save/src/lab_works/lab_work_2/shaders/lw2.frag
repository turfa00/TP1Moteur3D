#version 450

#define INDEX 0

layout (location = INDEX) out vec4 frag_color;

in vec3 sortCouleur;

void main() 
{
	frag_color = vec4(sortCouleur, 1.f);
}
