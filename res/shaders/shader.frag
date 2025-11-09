#version 460 core
out vec4 outColor;

// in vec3 ourColor;
in vec2 texCoords;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main() {
  outColor = 
  mix(
    texture(ourTexture1, texCoords),
    texture(ourTexture2, texCoords),
    0.2);
}