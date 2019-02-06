#version 400

in vec4 worldPos;

out vec4 fragColor;

void main() {
	fragColor = vec4(abs(worldPos.xyz), 1.0);
}
