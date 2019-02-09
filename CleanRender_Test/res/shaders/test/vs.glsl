#version 400

layout (location = 0) in vec3 vertexPos;

uniform mat4x4 projectionMatrix;
uniform mat4x4 viewMatrix;
uniform mat4x4 modelMatrix;

out vec4 worldPos;

void main() {
	worldPos = modelMatrix * vec4(vertexPos, 1.0);
	gl_Position = projectionMatrix * viewMatrix * worldPos;
}
