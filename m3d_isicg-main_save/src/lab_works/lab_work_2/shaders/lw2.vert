#version 450

#define INDEX 0

layout (location = INDEX) in vec2 aVertexPosition;
layout (location = 1) in vec3 aVertexCouleur;

out vec3 sortCouleur;

void main() 
{
	gl_Position =  vec4 (aVertexPosition, 0.f, 1.f);
	sortCouleur = aVertexCouleur;
}
