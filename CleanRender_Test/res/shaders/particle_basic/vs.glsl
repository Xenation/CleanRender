#version 420
#pragma pass opaque

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 vertexUV;

layout (std140, binding = 1) uniform Camera {
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
	ivec2 resolution;
};

uniform mat4x4 modelMatrix;

layout (std140, binding = 10) uniform Material {
	vec4 color;
};

out vec4 worldPos;
out vec2 uv;

void main() {
	uv = vertexUV;
	worldPos = modelMatrix * vec4(vertexPos, 1.0);
	gl_Position = projectionMatrix * viewMatrix * worldPos;
}