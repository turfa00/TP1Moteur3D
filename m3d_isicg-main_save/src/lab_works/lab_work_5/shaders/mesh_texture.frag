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

uniform vec3 ambientColor, diffuseColor, specularColor, lightPosition;
uniform bool uHasDiffuseMap, uHasAmbientMap,uHasSpecularMap, uHasShininess;

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
	if(uHasDiffuseMap || uHasAmbientMap || uHasSpecularMap){
		vec4 color;
		if(uHasDiffuseMap){
			color += texture(uDiffuseMap, textureCoords);
		}
		if(uHasAmbientMap){
			color += texture(uAmbientMap, textureCoords);
		}
		if(uHasSpecularMap){
			//vec3 specularMap = vec3(uSpecularMap);
			color += texture(uSpecularMap, textureCoords);
		}
		if(uHasShininess){
			color += texture(uShininessMap, textureCoords);
		}
		fragColor = color;
	}	
	else{
	fragColor = vec4(ambientColor + diffuseColor * lambertian + specularColor * specular, 1.0);
	}
}
