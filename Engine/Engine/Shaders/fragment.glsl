#version 410

in vec2 uv;
flat in vec4 centerPosition;
in vec4 position;
flat in vec3 lightPos;


// interpolated colour received from vertex stage

// first output is mapped to the framebuffer's colour index by default
out vec4 fragmentColor;

uniform sampler2D tex;
uniform sampler2D normalMap;
uniform int normalMapEnabled;
uniform int diffuseEnabled;

uniform vec3 cameraPosition;

void main(void) {
	vec3 color = texture(tex, uv).rgb;
	if(diffuseEnabled != 0) {
		vec3 normal;
		// if(normalMapEnabled != 0) {
			// normal = normalize((position + ((2*texture(normalMap, uv)) - 1)) - centerPosition).xyz;
		// } else {
			normal = normalize(position - centerPosition).xyz;
		// }
		vec3 d = normalize(lightPos - position.xyz);

		vec3 cl = vec3(1);
		vec3 cp = vec3(1);
		vec3 diffuse = cl*max(0, dot(normal, d));
		vec3 specular = vec3(0);
		// vec3 direction = (position.xyz - cameraPosition);
		// specular = cl*texture(specularMap, uv).xyz*max(0, pow(dot(normalize(-direction + d), normal), 64));
		vec3 ambient = vec3(0.1);

		color = (color*(ambient + diffuse)) + specular;
	}
	
	fragmentColor = vec4(color,0);
}
