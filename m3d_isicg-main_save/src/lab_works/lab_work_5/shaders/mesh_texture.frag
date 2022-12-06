#version 450

precision mediump float;

layout( location = 0 ) out vec4 fragColor;
layout (binding = 1) uniform sampler2D uDiffuseMap;
layout (binding = 2) uniform sampler2D uAmbientMap;
layout (binding = 3) uniform sampler2D uSpecularMap;
layout (binding = 4) uniform sampler2D uShininessMap;
uniform float shininessVal;

in vec3 normalInterp, vertexPos;
in vec2 textureCoords;

uniform vec3 ambientColor, diffuseColor, specularColor, lightDirection;
uniform bool uHasDiffuseMap,uHasSpecularMap, uHasShininessMap;

vec3 changerNormale(vec3 normal, vec3 lightDirection){
	normal = normalize(normal);
	lightDirection = normalize(lightDirection);
	if(dot(lightDirection, normal) < 0){
		return normal * -1;
	}
	return normal;
}

void main()
{
	vec3 N = changerNormale(normalInterp, lightDirection);
	//vec3 N = normalize(normalInterp);
	vec3 L = normalize(lightDirection - vertexPos);
	vec3 Lo = reflect(-L, N);
	//Lambert Cosine Law
	float lambertian = max(dot(N, L), 0.0);
	float specular = 0.0, specular2 = 0.0;
	float specAngle, shinVal2;
	//shinVal2 = texture(uShininessMap, textureCoords).x;
	if(lambertian > 0.0){
		vec3 V = normalize(-vertexPos);
		//specular
		specAngle = max(dot(Lo, V), 0.0);
		specular = pow(specAngle, shininessVal);
	}
	vec4 color;
	if(uHasDiffuseMap || uHasSpecularMap || uHasShininessMap){
		if(uHasDiffuseMap){
			color += texture(uDiffuseMap, textureCoords);
		}
		if(uHasSpecularMap || uHasShininessMap){
			vec3 spec = (texture(uSpecularMap, textureCoords).xxx);
			float shinVal = texture(uShininessMap, textureCoords).x;
			/*if(uHasShininessMap){
				specular = pow(specAngle, shinVal);
			}
			else{
				specular = 1.f;
			}*/
			color += vec4(spec * specular, 0.f);
		}
		fragColor = color;
		//fragColor = vec4(vec3(specAngle), 1.0); //FOR DEBUGGING
	}	
	else{
		color += vec4(ambientColor + diffuseColor * lambertian + specularColor * specular, 1.0);
		fragColor = color;
	}
}
