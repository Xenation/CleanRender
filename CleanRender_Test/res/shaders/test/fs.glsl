#version 420

layout (std140, binding = 2) uniform GlobalsVars {
	float time;
};

layout (std140, binding = 10) uniform Material {
	vec4 color;
};

in vec4 worldPos;

out vec4 fragColor;

void main() {
	fragColor = color;
}
