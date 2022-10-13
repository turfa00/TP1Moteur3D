#version 450

#define INDEX 0

layout (location = INDEX) out vec4 frag_color;

void main() 
{
	frag_color = vec4(255.f, 0.f, 0.f, 1.f);
}
