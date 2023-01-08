#version 450

precision mediump float;

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform sampler2D gPosition;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D gAmbiant;
layout (binding = 3) uniform sampler2D gDiffuse;
layout (binding = 4) uniform sampler2D gSpecular;
layout (binding = 5) uniform sampler2D gDepth;

/*in vec3 normalInterp, vertexPos;
in vec2 textureCoords;
in vec3 vertexTangentSpace, lightTangentSpace;
in mat3 inv_TBN;*/

uniform vec3 ambientColor, diffuseColor, specularColor, lightPosition;
uniform bool uHasAmbientMap, uHasDiffuseMap,uHasSpecularMap, uHasShininessMap, uHasNormalMap;
uniform float shininessVal;

vec3 changerNormale(vec3 normal, vec3 lightPosition){
	normal = normalize(normal);
	lightPosition = normalize(lightPosition);
	if(dot(lightPosition, normal) < 0){
		return normal * -1;
	}
	return normal;
}

void main()
{
	vec3 N, L, Lo;

	//Pixel Courant
	ivec2 texCoord = ivec2(gl_FragCoord.xy);
	//Textures
	vec4 fragPos = texelFetch(gPosition, texCoord, 0);
	vec4 normal = texelFetch(gNormal, texCoord, 0);
	vec4 ambient = texelFetch(gAmbiant, texCoord, 0);
	vec4 diffuse = texelFetch(gDiffuse, texCoord, 0);
	vec4 specular = texelFetch(gSpecular, texCoord, 0);
	vec4 depth = texelFetch(gDepth, texCoord, 0);
	
	vec3 lightDir = normalize (- fragPos.xyz);
	L = normalize(lightPosition - fragPos.xyz);
	N = changerNormale(normal.xyz, lightPosition);
	float specAngle = dot(normal.xyz, normalize(-fragPos.xyz));
	float	spec = pow(specAngle, shininessVal);
	float lambertian = max(dot(N, L), 0.0);
	//Out
	FragColor = vec4(ambient + (diffuse) + (specular*spec));
}