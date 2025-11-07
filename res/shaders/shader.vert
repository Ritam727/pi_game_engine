#version 460 core
layout(location = 0) in vec3 inPos;
// layout(location = 1) in vec3 inColor;
layout(location = 1) in vec2 inTex;

// out vec3 ourColor;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(inPos, 1.0f);
  // ourColor = inColor;
  texCoords = inTex;
}