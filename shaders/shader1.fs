#version 330 core
in vec2 texCoord;
in vec4 vertexColor;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	FragColor = vertexColor;
}