#version 450

layout (location = 0) in vec3 aVertexPosition;
layout (location = 1) in vec3 aVertexCouleur;
//uniform float uTranslationX;
uniform mat4 uTransformationMatrix, uViewMatrix, uProjectionMatrix;
out vec3 sortCouleur;

void main() 
{
	gl_Position =   uViewMatrix * uTransformationMatrix * vec4 (aVertexPosition.x, aVertexPosition.y, aVertexPosition.z, 1.f);
	sortCouleur = aVertexCouleur;
}
