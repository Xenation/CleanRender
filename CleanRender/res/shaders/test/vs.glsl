#version 400

layout (location = 0) in vec3 vertexPos;

uniform mat4x4 projectionMatrix;
uniform mat4x4 viewMatrix;
uniform mat4x4 modelMatrix;

void main() {
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPos, 1.0);
}
