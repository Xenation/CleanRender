#version 420

layout (std140, binding = 2) uniform GlobalsVars {
	float time;
};

layout (std140, binding = 10) uniform Material {
	vec4 color;
};

in vec4 worldPos;
in vec2 uv;

out vec4 fragColor;

void main() {
	fragColor = vec4(uv, 0.0, 1.0);
}
