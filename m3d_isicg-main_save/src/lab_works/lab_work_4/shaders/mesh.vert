#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;

//out vec3 diffuseColor;
//out vec3 FragPos;
uniform mat4 NormalMat;
varying vec3 normalInterp, vertPos;
uniform mat4 modelMatrix, viewMatrix; //Model
uniform mat4 uMVPMatrix; // Projection * View * Model


void main()
{
	vec4 vertPos4 = modelMatrix * vec4( aVertexPosition, 1.0);
	vertPos = vec3(vertPos4) / vertPos4.w;
	normalInterp = vec3(NormalMat * vec4(aVertexNormal,0.0));
	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );
	//FragPos = vec3(modelMatrix * vec4(aVertexPosition, 1.0));
	//NormalMat = aVertexNormal;

}
