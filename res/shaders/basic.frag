#version 460 core

struct BasicLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
  float shininess;
};

in vec3 fragPos;
in vec3 fragNormal;
in vec3 fragColor;

out vec4 outColor;

uniform vec3 viewerPos;
uniform BasicLight basicLight;
uniform Material material;

void main() {
  vec3 normal = normalize(fragNormal);
  vec3 lightDirection = normalize(basicLight.position - fragPos);
  float diffuse = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuseLight = basicLight.diffuse * (diffuse * material.diffuse);

  vec3 viewDir = normalize(viewerPos - fragPos);
  vec3 reflectDir = normalize(reflect(-lightDirection, normal));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specularLight = basicLight.specular * (spec * material.specular);

  vec3 ambientLight = material.ambient * basicLight.ambient;

  vec3 lightColor = ambientLight + specularLight + diffuseLight;
  outColor = vec4(lightColor, 1.0f);
}