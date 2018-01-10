#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;

out vec2 uv;
flat out vec4 centerPosition;
out vec4 position;
flat out vec3 lightPos;

// uniform vec4 shapeCenter;

uniform mat4 cameraMatrix;
uniform mat4 perspectiveMatrix;
uniform mat4 modelviewMatrix;
// output to be interpolated between vertices and passed to the fragment stage

void main() {
    // assign vertex position without modification
	gl_Position = perspectiveMatrix*cameraMatrix*modelviewMatrix*vec4(vertexPosition, 1.0);
	uv = vertexUV;
	// normal = (modelviewMatrix*vec4(surfaceNormal, 0.0)).xyz;
	centerPosition = perspectiveMatrix*cameraMatrix*modelviewMatrix*vec4(0, 0, 0, 1);
	position = gl_Position;
	lightPos = (perspectiveMatrix*cameraMatrix*vec4(0, 0, 0, 1)).xyz;
}
