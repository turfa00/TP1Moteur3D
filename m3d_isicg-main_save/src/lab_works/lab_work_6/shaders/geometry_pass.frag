#version 450

precision mediump float;

//layout (location = 0) out vec4 fragColor;
layout (location = 0) out vec3 fragPosition;
layout (location = 1) out vec3 fragNormal;
layout (location = 2) out vec3 fragAmbiante;
layout (location = 3) out vec3 fragDiffuse;
layout (location = 4) out vec4 fragSpeculaire;
layout (location = 5) out vec3 fragDepth;

layout (binding = 1) uniform sampler2D uDiffuseMap;
layout (binding = 2) uniform sampler2D uAmbientMap;
layout (binding = 3) uniform sampler2D uSpecularMap;
layout (binding = 4) uniform sampler2D uShininessMap;
layout (binding = 5) uniform sampler2D uNormalMap;

in vec3 normalInterp, vertexPos;
in vec2 textureCoords;
in vec3 vertexTangentSpace, lightTangentSpace;
in mat3 inv_TBN;

uniform vec3 ambientColor, diffuseColor, specularColor, lightPosition;
uniform bool uHasDiffuseMap,uHasSpecularMap, uHasShininessMap, uHasNormalMap;
uniform float shininessVal;

vec3 changerNormale(vec3 normal, vec3 lightPosition){
	normal = normalize(normal);
	lightPosition = normalize(lightPosition);
	if(dot(lightPosition, normal) < 0){
		return normal * -1;
	}
	return normal;
}


void main(){
	fragPosition = vertexPos;
	fragNormal = normalize(normalInterp);
	fragAmbiante = ambientColor;
	fragDiffuse = (texture(uDiffuseMap, textureCoords).xyz);
	fragSpeculaire = texture(uShininessMap, textureCoords);
	//fragDepth = ;
}

/*void main()
{
	vec3 N, L, Lo;
	vec4 color;
	if(uHasNormalMap){
			N = (texture(uNormalMap, textureCoords).xyz); //https://geeks3d.developpez.com/normal-mapping-glsl/ reference
			N = normalize(inv_TBN * N);
			L = normalize(lightTangentSpace - vertexTangentSpace);
			
			Lo = reflect(-L, N);

			//color = texture(uNormalMap, textureCoords); //TO WORK ON
			//vec3 L2 = normalize(lightTangentSpace - vertexTangentSpace);
			//color += vec4(normal * max(dot(normal, L2), 0.0), 0.f);
	}
	if(!uHasNormalMap){
		N = changerNormale(normalInterp, lightPosition);
		//N = normalize(texture(uNormalMap, textureCoords).xyz);
		//vec3 N = normalize(normalInterp);
		L = normalize(lightPosition - vertexPos);
		Lo = reflect(-L, N);
	}
	
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
	if(uHasDiffuseMap || uHasSpecularMap || uHasShininessMap || uHasNormalMap){
		if(uHasDiffuseMap){
			vec4 diffuseTex = texture(uDiffuseMap, textureCoords);
			if(diffuseTex.a < 0.5){
				discard;
			}
			color += diffuseTex;
		}
		if(uHasSpecularMap || uHasShininessMap){
			vec3 spec = (texture(uSpecularMap, textureCoords).xxx);
			float shinVal = texture(uShininessMap, textureCoords).x;
			color += vec4(spec * specular, 0.f);
		}
		if(uHasNormalMap){
			//color += texture(uDiffuseMap, textureCoords) * lambertian;
		}
		fragColor = color;
		//fragColor = vec4(vec3(specAngle), 1.0); //FOR DEBUGGING
	}	
	else{
		//color += vec4(ambientColor + diffuseColor * lambertian + specularColor * specular, 1.0);
		//fragColor = color;
	}
	color += vec4(diffuseColor * lambertian + specularColor * specular, 0.0);
	fragColor = vec4(color.xyz, 1.0f);
}
*/