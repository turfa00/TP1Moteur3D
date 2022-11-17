#version 450

precision mediump float;

layout( location = 0 ) out vec4 fragColor;
in vec3 normalInterp, vertexPos;

uniform float shininessVal;
uniform vec3 ambientColor, diffuseColor, specularColor, lightPos;

void main()
{
	vec3 N = normalize(normalInterp);
	vec3 L = normalize(/*lightPos*/ - vertexPos);
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
	//fragColor = vec4(diffuseColor + ambientColor + specularColor, 1.0);
	fragColor = vec4(ambientColor + diffuseColor * lambertian + specularColor * specular, 1.0);
}
