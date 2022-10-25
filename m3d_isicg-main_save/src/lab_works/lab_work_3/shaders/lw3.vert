#version 450

#define INDEX 0

layout (location = INDEX) in vec2 aVertexPosition;
layout (location = 1) in vec3 aVertexCouleur;
uniform float uTranslationX;
out vec3 sortCouleur;

void main() 
{
	gl_Position =  vec4 (aVertexPosition.x+uTranslationX,aVertexPosition.y, 0.f, 1.f);
	sortCouleur = aVertexCouleur;
}
