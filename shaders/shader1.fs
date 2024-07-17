#version 330 core
in vec3 FragPos;
out vec4 FragColor;

void main() {
    // Use the fragment position to create a unique shade for each face
    vec3 absPos = abs(FragPos);
    
    // Calculate a grey shade based on the position
    float greyShade = fract(absPos.x * 0.1 + absPos.y * 0.15 + absPos.z * 0.2);
    
    // Adjust the range of the grey shade
    greyShade = 0.3 + greyShade * 0.5; // This will give a range from 0.3 to 0.8
    
    FragColor = vec4(vec3(greyShade), 1.0);
}
