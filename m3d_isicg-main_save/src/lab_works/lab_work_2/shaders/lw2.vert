#version 450

#define INDEX 0

layout (location = INDEX) in vec3 aVertexPosition;

void main() 
{
	gl_Position =  vec4 (aVertexPosition, 1.f);

}
