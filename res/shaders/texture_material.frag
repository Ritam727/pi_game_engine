#version 460 core

struct BasicLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct TextureMaterial {
  sampler2D diffuse;
  sampler2D specular;

  float shininess;
};

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTex;

out vec4 outColor;

uniform vec3 viewerPos;
uniform BasicLight basicLight;
uniform TextureMaterial textureMaterial;

void main() {
  vec3 normal = normalize(fragNormal);
  vec3 lightDirection = normalize(basicLight.position - fragPos);
  float diffuse = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuseLight = basicLight.diffuse * (diffuse * vec3(texture(textureMaterial.diffuse, fragTex)));

  vec3 viewDir = normalize(viewerPos - fragPos);
  vec3 reflectDir = normalize(reflect(-lightDirection, normal));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), textureMaterial.shininess);
  vec3 specularLight = basicLight.specular * (spec * vec3(texture(textureMaterial.specular, fragTex)));

  vec3 ambientLight = vec3(texture(textureMaterial.diffuse, fragTex)) * basicLight.ambient;

  vec3 lightColor = ambientLight + specularLight + diffuseLight;
  outColor = vec4(lightColor, 1.0f);
}