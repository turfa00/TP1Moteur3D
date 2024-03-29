#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;

out vec3 normalInterp, vertexPos;

uniform mat4 NormalMat;
uniform mat4 modelMatrix, viewMatrix; //Model
uniform mat4 uMVPMatrix; // Projection * View * Model

vec3 normalVec;


void main()
{
	vec4 vertPos4 =  viewMatrix * modelMatrix * vec4( aVertexPosition, 1.f);
	vertexPos = vec3(vertPos4) / vertPos4.w;
	normalInterp = vec3(NormalMat * vec4(aVertexNormal,0.f));
	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );
}
