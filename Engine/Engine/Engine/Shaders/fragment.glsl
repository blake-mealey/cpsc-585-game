#version 410

uniform vec3 materialDiffuseColor;
uniform vec3 materialSpecularColor;
uniform float materialSpecularity;

uniform vec3 ambientColor;

uniform vec3 lightColor;
uniform float lightPower;

uniform sampler2D diffuseTexture;

in vec3 surfaceNormal_camera;
in vec3 lightDirection_camera;
in vec3 eyeDirection_camera;
in vec2 fragmentUv;

out vec3 fragmentColor;

void main(){
	vec3 textureDiffuseColor = texture(diffuseTexture, vec2(1.f - fragmentUv.x, fragmentUv.y)).rgb;
	vec3 diffuseColor = materialDiffuseColor * textureDiffuseColor;		// TODO: Mix texture and material colors based on transparency?
	vec3 materialAmbientColor = ambientColor * diffuseColor;

	vec3 n = normalize(surfaceNormal_camera);
	vec3 l = normalize(lightDirection_camera);
	vec3 E = normalize(eyeDirection_camera);
	vec3 R = reflect(-l, n);

	float cosTheta = clamp(dot(n, l), 0, 1);
	float cosAlpha = clamp(dot(E, R), 0, 1);
	
	float distanceToLight = length(lightDirection_camera);
	float distanceSquaredInverse = 1.f/(distanceToLight*distanceToLight);
	fragmentColor = materialAmbientColor +																// Ambient
		(diffuseColor * lightColor * lightPower * cosTheta * distanceSquaredInverse) +			// Diffuse
		(materialSpecularColor * lightColor * lightPower * pow(cosAlpha, materialSpecularity) * distanceSquaredInverse);	// Specular
}
