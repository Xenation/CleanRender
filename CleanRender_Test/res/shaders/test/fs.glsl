#version 420

layout (std140, binding = 2) uniform GlobalsVars {
	float time;
};

in vec4 worldPos;

out vec4 fragColor;

void main() {
	fragColor = vec4(abs(worldPos.xyz) * sin(time), 1.0);
}
