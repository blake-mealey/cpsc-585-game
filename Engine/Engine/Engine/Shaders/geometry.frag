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

struct SpotLight {
	vec3 color;
	float power;
	vec3 position_world;
	float angle;
	vec3 direction_world;
};

layout (std430, binding = 0) buffer pointLightData { PointLight pointLights[]; };
layout (std430, binding = 1) buffer directionLightData { DirectionLight directionLights[]; };
layout (std430, binding = 2) buffer spotLightData { SpotLight spotLights[]; };

uniform mat4 viewMatrix;

uniform vec3 materialDiffuseColor;
uniform vec3 materialSpecularColor;
uniform float materialSpecularity;

uniform vec3 ambientColor;

uniform sampler2D shadowMap;
uniform sampler2D diffuseTexture;
uniform uint diffuseTextureEnabled;

in vec4 shadowCoord;
in vec3 fragmentPosition_camera;
in vec3 surfaceNormal_camera;
in vec3 eyeDirection_camera;
in vec2 fragmentUv;

out vec3 fragmentColor;

vec3 getColorFromLight(vec3 diffuseColor, vec3 lightDirection_camera, vec3 lightColor) {
	vec3 n = normalize(surfaceNormal_camera);
	vec3 l = normalize(lightDirection_camera);
	vec3 E = normalize(eyeDirection_camera);
	vec3 R = reflect(-l, n);

	float cosTheta = clamp(dot(n, l), 0, 1);
	float cosAlpha = clamp(dot(E, R), 0, 1);
	
	return (diffuseColor * cosTheta * lightColor) +										// Diffuse
		   (materialSpecularColor * lightColor * pow(cosAlpha, materialSpecularity));	// Specular
}

void main() {
	float visibility = 1;
	if (texture(shadowMap, shadowCoord.xy).z < shadowCoord.z) {
		visibility = 0.5;
	}

	vec3 diffuseColor = (1 - diffuseTextureEnabled) * materialDiffuseColor
		+ diffuseTextureEnabled * texture(diffuseTexture, vec2(1.f - fragmentUv.x, fragmentUv.y)).rgb;
	vec3 materialAmbientColor = ambientColor * diffuseColor;

	fragmentColor = materialAmbientColor;
	
	for (int i = 0; i < pointLights.length(); i++) {
		PointLight light = pointLights[i];
		vec3 lightPosition_camera = (viewMatrix * vec4(light.position_world, 1)).xyz;
		vec3 lightDirection_camera = lightPosition_camera - fragmentPosition_camera;
		float distanceToLight = length(lightDirection_camera);
		float attenuation = 1.0 / (1.0 * (1.0/light.power) * (distanceToLight*distanceToLight));
		fragmentColor += visibility * attenuation * getColorFromLight(diffuseColor, lightDirection_camera, light.color);
	}

	for (int i = 0; i < directionLights.length(); i++) {
		DirectionLight light = directionLights[i];
		vec3 lightDirection_camera = (viewMatrix * vec4(-light.direction_world, 0)).xyz;
		fragmentColor += visibility * getColorFromLight(diffuseColor, lightDirection_camera, light.color);
	}

	for (int i = 0; i < spotLights.length(); i++) {
		SpotLight light = spotLights[i];
		vec3 lightPosition_camera = (viewMatrix * vec4(light.position_world, 1)).xyz;
		vec3 lightDirection_camera = lightPosition_camera - fragmentPosition_camera;

		vec3 coneDirection_camera = (viewMatrix * vec4(normalize(light.direction_world), 0)).xyz;
		float lightAngle = acos(dot(-normalize(lightDirection_camera), coneDirection_camera));
		if (lightAngle < light.angle) {
			float distanceToLight = length(lightDirection_camera);
			float attenuation = 1.0 / (1.0 * (1.0/light.power) * (distanceToLight*distanceToLight));
			fragmentColor += visibility * attenuation * getColorFromLight(diffuseColor, lightDirection_camera, light.color);
		}
	}
}
