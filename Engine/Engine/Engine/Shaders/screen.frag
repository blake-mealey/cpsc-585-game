#version 430

in vec2 fragmentUv;

uniform sampler2D screen;

void main() {
	gl_FragColor = texture(screen, fragmentUv);
}
