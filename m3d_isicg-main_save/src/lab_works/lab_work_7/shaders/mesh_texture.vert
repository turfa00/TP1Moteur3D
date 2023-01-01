#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;

out vec3 normalInterp, vertexPos;
out vec2 textureCoords;
out vec3 vertexTangentSpace, lightTangentSpace;
out mat3 inv_TBN;
uniform mat4 NormalMat; //Change to normalMat
uniform mat4 modelMatrix, viewMatrix; //Model
uniform mat4 uMVPMatrix; // Projection * View * Model
uniform vec3 lightPosition;
vec3 normalVec;


void main()
{
	vec4 vertPos4 =  viewMatrix * modelMatrix * vec4( aVertexPosition, 1.f);
	vertexPos = vec3(vertPos4) / vertPos4.w;
	normalInterp = vec3(NormalMat * vec4(aVertexNormal,0.f));
	textureCoords = aVertexTexCoords;
	//Normal mapping
	vec3 T = normalize(vec3(viewMatrix * modelMatrix  * vec4(aVertexTangent, 0.f)));
	vec3 B = normalize(vec3(viewMatrix * modelMatrix * vec4(aVertexBitagent, 0.f)));
	vec3 N = normalize(vec3(viewMatrix * modelMatrix * vec4(aVertexNormal, 0.f)));
	inv_TBN = transpose(mat3(T, B, N));
	vertexTangentSpace = inv_TBN * vec3(viewMatrix * modelMatrix * vec4(aVertexPosition, 1.f));
	//vertexTangentSpace = inv_TBN * normalize(lightPosition - vertexPos);
	lightTangentSpace = inv_TBN * lightPosition;
	//lightTangentSpace = inv_TBN * normalize(lightPosition - vertexPos);


	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );
}
