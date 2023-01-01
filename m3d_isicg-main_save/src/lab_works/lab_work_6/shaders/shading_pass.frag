#version 450

precision mediump float;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAmbiant;
uniform sampler2D gDiffuse;
uniform sampler2D gSpecular;
uniform sampler2D gDepth;
layout (binding = 1) uniform sampler2D uDiffuseMap;
layout (binding = 2) uniform sampler2D uAmbientMap;
layout (binding = 3) uniform sampler2D uSpecularMap;
layout (binding = 4) uniform sampler2D uShininessMap;
layout(binding = 5) uniform sampler2D uNormalMap;
struct light{
	vec3 position;
	vec3 color;
};

in vec3 normalInterp, vertexPos;
in vec2 textureCoords;
in vec3 vertexTangentSpace, lightTangentSpace;
in mat3 inv_TBN;

out vec4 FragColor;
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
	vec4 color;

	//Pixel Courant
	ivec2 coord = ivec2(gl_FragCoord.xy);
	//Textures
	//vec4 data = texelFetch(gTexture, coord);
	vec3 fragPos = texture(gPosition, coord).xyz;
	vec3 normal = texture(gNormal, coord).xyz;
	vec3 ambient = texture(gAmbiant, coord).xyz;
	vec3 diffuse = texture(gDiffuse, coord).xyz;
	float specular = texture(gSpecular, coord).x;
	vec3 depth = texture(gDepth, coord).xyz;

	vec3 lighting = ambient + diffuse + specular * 0.1;
	vec3 viewDir = normalize(lightPosition - vertexPos);
	
	FragColor = vec4(lighting, 1.0);
}
