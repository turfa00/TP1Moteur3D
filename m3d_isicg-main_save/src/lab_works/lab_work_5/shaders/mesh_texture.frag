#version 450

precision mediump float;

layout( location = 0 ) out vec4 fragColor;
layout (binding = 1) uniform sampler2D uDiffuseMap;
in vec3 normalInterp, vertexPos;
in vec2 diffuseTexCoords;

uniform float shininessVal;
uniform vec3 ambientColor, diffuseColor, specularColor, lightPosition;

uniform bool uHasDiffuseMap;

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
	if(uHasDiffuseMap){
		vec4 color = texture(uDiffuseMap, diffuseTexCoords);
		fragColor = color;
	}	
	else{
	fragColor = vec4(ambientColor + diffuseColor * lambertian + specularColor * specular, 1.0);
	}
}
