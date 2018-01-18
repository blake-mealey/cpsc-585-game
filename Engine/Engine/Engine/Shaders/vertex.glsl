#version 410

layout(location = 0) in vec3 vertexPosition_model;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec3 vertexNormal_model;

uniform vec3 lightPosition_world;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewProjectionMatrix;

out vec2 fragmentUv;
out vec3 surfaceNormal_camera;
out vec3 lightDirection_camera;
out vec3 eyeDirection_camera;


void main() {
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosition_model, 1);

	vec3 vertexPosition_camera = (viewMatrix * modelMatrix * vec4(vertexPosition_model, 1)).xyz;
	eyeDirection_camera = vec3(0, 0, 0) - vertexPosition_camera;

	vec3 lightPosition_camera = (viewMatrix * vec4(lightPosition_world, 1)).xyz;
	lightDirection_camera = lightPosition_camera - vertexPosition_camera;
	lightDirection_camera = lightPosition_camera + eyeDirection_camera;
	
	surfaceNormal_camera = (viewMatrix * modelMatrix * vec4(vertexNormal_model, 0)).xyz;

	fragmentUv = vertexUv;
}
