#version 330 core
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform float transitionFactor;

void main() {
    // Use the fragment position to create a unique shade for each face
    vec3 absPos = abs(FragPos);

    // Calculate a grey shade based on the position
    float greyShade = fract(absPos.x * 0.1 + absPos.y * 0.15 + absPos.z * 0.2);

    // Adjust the range of the grey shade
    greyShade = 0.3 + greyShade * 0.5;
    vec4 baseColor = vec4(vec3(greyShade), 1.0);

    // Calculate the texture color
    vec4 textureColor = texture(ourTexture, TexCoord);

    // Blend between base color and texture color using transitionFactor
    FragColor = mix(baseColor, textureColor, transitionFactor);
}
