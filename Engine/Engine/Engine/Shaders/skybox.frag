#version 430

in vec3 fragmentUv;

uniform vec3 colorAdjust;
uniform samplerCube skybox;

out vec4 fragmentColor;

void main() {
	vec4 mult = vec4(colorAdjust, 1);
	fragmentColor = texture(skybox, fragmentUv)*mult;
}