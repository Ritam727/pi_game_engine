#version 460 core
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTex;

out vec3 fragPos;
out vec3 fragNormal;
out vec3 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(inPos, 1.0f);
  fragPos = vec3(model * vec4(inPos, 1.0f));
  fragNormal = mat3(transpose(inverse(model))) * inNormal;
  fragColor = inColor;
}