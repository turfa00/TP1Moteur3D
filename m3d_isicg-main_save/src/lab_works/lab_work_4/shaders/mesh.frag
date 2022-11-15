#version 450

layout( location = 0 ) out vec4 fragColor;
uniform mat4 NormalMat;
varying vec3 vertPos, normalInterp;

uniform float Ka, Kd, Ks, shininessVal;
uniform vec3 ambientColor, diffuseColor, lightPos;

varying vec4 color;
in vec3 Normal, FragPos;

precision mediump float;

void main()
{
	vec3 N = normalize(normalInterp);
	vec3 L = normalize(lightPos - vertPos);

	//Lambert Cosine Law
	float lambertian = max(dot(N,L),0.0);
	float specular = 0.0;
	if(lambertian > 0.0){
		vec3 R = reflect(-L, N);
		vec3 V = normalize(-vertPos);
		//specular term
		float specAngle = max(dot(R, V), 0.0);
		specular = pow(specAngle, shininessVal);
	}

	fragColor = vec4(ambientColor + lambertian * diffuseColor, 1.0);




	//vec4 vertPos4 = modelMatrix * vec4( aVertexPosition, 1.0);
	//vertPos = vec3(vertPos4) / vertPos4.w;
	//normalInterp = vec3(NormalMat * vec4(aVertexNormal,0.0));
	//vec3 N = normalize(normalInterp);
	//vec3 L = normalize(lightPos - vertPos);

	//float lambertian = max(dot(N,L), 0.0);
	//float specular = 0.0;

	//if(lambertian > 0.0){
		//vec3 R = reflect(-L, N);
		//vec3 V = normalize(-vertPos);

		//float specAngle = max(dot(R,V), 0.0);
		//specular = pow(specAngle, shininessVal);	
	//}
	//color = vec4(Ka * ambientColor + Kd * diffuseColor, 1.0);
	
	//color = vec4(ambientColor + diffuseColor, 1.f);

	//fragColor = vec4( ambientColor * diffuseColor, 1.f );
	
	//fragColor = vec4( color);
}
