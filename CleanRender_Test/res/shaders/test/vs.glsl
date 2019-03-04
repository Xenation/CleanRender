#version 420
#pragma pass opaque

layout (location = 0) in vec3 vertexPos;

layout (std140, binding = 1) uniform CameraMatrices {
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
};

layout (std140, binding = 10) uniform Material {
	mat4x4 modelMatrix;
};

out vec4 worldPos;

void main() {
	worldPos = modelMatrix * vec4(vertexPos, 1.0);
	gl_Position = projectionMatrix * viewMatrix * worldPos;
}
