#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D myTextureSampler;
uniform float alphaChannel;

void main() {
	float a = alphaChannel;
	color =
	texture2D(myTextureSampler, UV) * vec4(1, 1, 1, a);
}