#version 410

uniform vec3 lightColor;
uniform float lightPower;

in vec3 surfaceNormal_camera;
in vec3 lightDirection_camera;
in vec3 eyeDirection_camera;

out vec3 fragmentColor;


void main(){
	vec3 materialDiffuseColor = vec3(.5, 0, 0);
	vec3 materialAmbientColor = vec3(0.1, 0.1, 0.1) * materialDiffuseColor;
	vec3 materialSpecularColor = vec3(1, 1, 1);

	vec3 n = normalize(surfaceNormal_camera);
	vec3 l = normalize(lightDirection_camera);
	vec3 E = normalize(eyeDirection_camera);
	vec3 R = reflect(-l, n);

	float cosTheta = clamp(dot(n, l), 0, 1);
	float cosAlpha = clamp(dot(E, R), 0, 1);
	
	float distanceToLight = length(lightDirection_camera);
	float distanceSquaredInverse = 1.f/(distanceToLight*distanceToLight);
	fragmentColor = materialAmbientColor +																// Ambient
		(materialDiffuseColor * lightColor * lightPower * cosTheta * distanceSquaredInverse) +			// Diffuse
		(materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 50) * distanceSquaredInverse);	// Specular
}
