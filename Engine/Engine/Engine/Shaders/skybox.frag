#version 430

in vec3 fragmentUv;

uniform samplerCube skybox;

out vec4 fragmentColor;

void main() {
	fragmentColor = texture(skybox, fragmentUv);
}