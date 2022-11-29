#version 450

precision mediump float;

layout( location = 0 ) out vec4 fragColor;
in vec3 normalInterp, vertexPos;

uniform float shininessVal;
uniform vec3 ambientColor, diffuseColor, specularColor, lightPosition;

uniform bool uHasDiffuseMap;
layout (binding = 1) uniform sampler2D uDiffuseMap;

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
	vec3 N = changerNormale(normalInterp, lightPosition);
	//vec3 N = normalize(normalInterp);
	vec3 L = normalize(lightPosition - vertexPos);
	vec3 Lo = reflect(-L, N);
	//Lambert Cosine Law
	float lambertian = max(dot(N, L), 0.0);
	float specular = 0.0;
	if(lambertian > 0.0){
		vec3 V = normalize(-vertexPos);
		//specular
		float specAngle = max(dot(Lo, V), 0.0);
		specular = pow(specAngle, shininessVal);
	}
	if(!uHasDiffuseMap){
		fragColor = vec4(ambientColor + specularColor * specular, 1.0);
	}	
	else{
	//fragColor = vec4(diffuseColor + ambientColor + specularColor, 1.0);
	fragColor = vec4(ambientColor + diffuseColor * lambertian + specularColor * specular, 1.0);
	}
}
