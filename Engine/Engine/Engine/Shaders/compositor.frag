#version 430

#define MOD_COUNT 4

in vec2 fragmentUv;

uniform sampler2D base;
uniform sampler2D mod0;
uniform sampler2D mod1;
uniform sampler2D mod2;
uniform sampler2D mod3;

out vec4 fragmentColor;

void main() {
	vec4 baseC = texture(base, fragmentUv);
	vec4 mod0C = texture(mod0, fragmentUv);
	vec4 mod1C = texture(mod1, fragmentUv);
	vec4 mod2C = texture(mod2, fragmentUv);
	vec4 mod3C = texture(mod3, fragmentUv);
	fragmentColor = clamp(baseC + mod0C + mod1C + mod2C + mod3C, 0.0, 1.0);
	// fragmentColor = clamp(baseC + mod0C + mod1C + mod2C, 0.0, 1.0);
}
