#version 450

precision mediump float;

//layout (location = 0) out vec4 fragColor;
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAmbiant;
layout (location = 3) out vec3 gDiffuse;
layout (location = 4) out vec4 gSpecular;
layout (location = 5) out vec3 gDepth;

layout (binding = 1) uniform sampler2D uDiffuseMap;
layout (binding = 2) uniform sampler2D uAmbientMap;
layout (binding = 3) uniform sampler2D uSpecularMap;
layout (binding = 4) uniform sampler2D uShininessMap;
layout(binding = 5) uniform sampler2D uNormalMap;

in vec3 normalInterp, vertexPos;
in vec2 textureCoords;
in vec3 vertexTangentSpace, lightTangentSpace;
in mat3 inv_TBN;

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
	
	if(uHasNormalMap){
			N = texture(uNormalMap, textureCoords).xyz; //https://geeks3d.developpez.com/normal-mapping-glsl/ reference
			N = N * 2.f - 1.0f;
			N = normalize(inv_TBN * N);
			L = normalize(lightTangentSpace - vertexTangentSpace);
			
			//L = normalize(lightPosition - vertexPos );
			Lo = reflect(-L, N);
	}
	if(!uHasNormalMap){
		//N = changerNormale(normalInterp, lightPosition);
		N = changerNormale(normalInterp, lightTangentSpace);
		L = normalize(lightPosition - vertexPos);
		Lo = reflect(-L, N);
	}
	
	//Lambert Cosine Law
	float lambertian = max(dot(N, L), 0.0);
	float specular = 0.0;
	float specAngle;
	vec3 diffuseC, specularC, ambientC;
	float shininess;
	if(lambertian < 0.0){
		lambertian = - lambertian;
	}
	if(lambertian > 0.0){
		vec3 V = normalize(-vertexPos);
		//specular
		specAngle = dot(N, V);
		if(uHasShininessMap){
			shininess = texture(uShininessMap, textureCoords).x;	
		}
		else{
			shininess = shininessVal;
		}
		specular = pow(specAngle, shininess);
	}
	
	if(uHasDiffuseMap){
		diffuseC = texture(uDiffuseMap, textureCoords).xyz;
		if(texture(uDiffuseMap, textureCoords).a < 0.5){
			discard;
		}
	}
	else{
		diffuseC = vec3(diffuseColor.xyz);
	}
	if(uHasAmbientMap){
		ambientC = vec3(texture(uAmbientMap, textureCoords).xyz);
	}
	else{
		ambientC = vec3(ambientColor.xyz);
	}
	if(uHasSpecularMap){
		specularC = (texture(uSpecularMap, textureCoords).xxx);
	}
	else{
		specularC = vec3(specularColor.xyz);
	}
	color = vec4(ambientC + (diffuseC * lambertian) + (specularC * specular), 0.0);
	//fragColor = vec4(color.xyz, 1.0f);

	gPosition = vertexTangentSpace;
	gNormal = N;
	gAmbiant = ambientC;
	gDiffuse = diffuseC * lambertian;
	gSpecular = vec4(specularC * specular, 0.0);
}
