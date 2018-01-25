#version 430

struct PointLight {
	vec3 color;
	float power;
	vec3 position_world;
};

struct DirectionLight {
	vec3 color;
	vec3 direction_world;
};

layout (std430, binding = 0) buffer pointLightData {
	PointLight pointLights[];
};

layout (std430, binding = 1) buffer directionLightData {
	DirectionLight directionLights[];
};

uniform mat4 viewMatrix;

uniform vec3 materialDiffuseColor;
uniform vec3 materialSpecularColor;
uniform float materialSpecularity;

uniform vec3 ambientColor;

uniform sampler2D diffuseTexture;
uniform bool diffuseTextureEnabled;

in vec3 fragmentPosition_camera;
in vec3 surfaceNormal_camera;
in vec3 eyeDirection_camera;
in vec2 fragmentUv;

out vec3 fragmentColor;

vec3 getColorFromLight(vec3 diffuseColor, vec3 lightDirection_camera, vec3 lightColor, float distanceMultiplier) {
	vec3 n = normalize(surfaceNormal_camera);
	vec3 l = normalize(lightDirection_camera);
	vec3 E = normalize(eyeDirection_camera);
	vec3 R = reflect(-l, n);

	float cosTheta = clamp(dot(n, l), 0, 1);
	float cosAlpha = clamp(dot(E, R), 0, 1);
	
	return (diffuseColor * cosTheta * lightColor * distanceMultiplier) +									// Diffuse
		   (materialSpecularColor * lightColor * pow(cosAlpha, materialSpecularity) * distanceMultiplier);	// Specular
}

void main() {
	vec3 diffuseColor = materialDiffuseColor;
	if (diffuseTextureEnabled)
		diffuseColor = texture(diffuseTexture, vec2(1.f - fragmentUv.x, fragmentUv.y)).rgb;
	vec3 materialAmbientColor = ambientColor * diffuseColor;

	fragmentColor = materialAmbientColor;
	
	for (int i = 0; i < pointLights.length(); i++) {
		PointLight light = pointLights[i];
		vec3 lightPosition_camera = (viewMatrix * vec4(light.position_world, 1)).xyz;
		vec3 lightDirection_camera = lightPosition_camera - fragmentPosition_camera;
		float distanceToLight = length(lightDirection_camera);
		float distanceSquaredInverse = min(1.f, light.power * 1.f/(distanceToLight*distanceToLight));
		fragmentColor += getColorFromLight(diffuseColor, lightDirection_camera, light.color, distanceSquaredInverse);
	}

	for (int i = 0; i < directionLights.length(); i++) {
		DirectionLight light = directionLights[i];
		vec3 lightDirection_camera = (viewMatrix * vec4(light.direction_world, 0)).xyz;
		fragmentColor += getColorFromLight(diffuseColor, lightDirection_camera, light.color, 1);
	}
}
