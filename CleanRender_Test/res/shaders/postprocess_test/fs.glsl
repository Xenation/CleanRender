#version 420

in vec2 uv;

layout (std140, binding = 1) uniform Camera {
	mat4x4 projectionMatrix;
	mat4x4 viewMatrix;
	ivec2 resolution;
};

layout (binding = 0) uniform sampler2D mainTex;
layout (binding = 1) uniform sampler2D depthTex;

layout (std140, binding = 10) uniform Material {
	float placeholder;
};

out vec4 fragColor;

void main() {
	vec2 pixelUV = 1.0 / resolution;
	vec4 col = vec4(0, 0, 0, 1);

	col = texture(depthTex, uv);
	vec4 colUp = texture(depthTex, uv + vec2(0, pixelUV.y));
	vec4 colDown = texture(depthTex, uv + vec2(0, -pixelUV.y));
	vec4 colRight = texture(depthTex, uv + vec2(pixelUV.x, 0));
	vec4 colLeft = texture(depthTex, uv + vec2(-pixelUV.x, 0));

	col.xyz = (colUp.xyz - col.xyz) + (colDown.xyz - col.xyz) + (colRight.xyz - col.xyz) + (colLeft.xyz - col.xyz);
	col.rgb *= 100;
	
	//col.xyz = texture(depthTex, uv).xyz;

	fragColor = col * placeholder;
}
