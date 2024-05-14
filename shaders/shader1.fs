#version 330 core
in vec3 ourColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	FragColor = texture(ourTexture, texCoord) * vec4(ourColor, 1.0);
}