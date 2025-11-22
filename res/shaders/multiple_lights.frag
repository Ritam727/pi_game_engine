#version 460 core

struct DirectionalLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;

  float innerCutOff;
  float outerCutOff;
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
uniform DirectionalLight directionalLight;
uniform int pointLightCount;
uniform PointLight pointLights[4];
uniform SpotLight spotLight;
uniform TextureMaterial textureMaterial;

vec3 directionalLightContribution() {
  vec3 normal = normalize(fragNormal);
  vec3 lightDirection = normalize(-directionalLight.direction);
  float diffuse = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuseLight = directionalLight.diffuse * (diffuse * vec3(texture(textureMaterial.diffuse, fragTex)));

  vec3 viewDir = normalize(viewerPos - fragPos);
  vec3 reflectDir = normalize(reflect(-lightDirection, normal));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), textureMaterial.shininess);
  vec3 specularLight = directionalLight.specular * (spec * vec3(texture(textureMaterial.specular, fragTex)));

  vec3 ambientLight = vec3(texture(textureMaterial.diffuse, fragTex)) * directionalLight.ambient;

  return ambientLight + specularLight + diffuseLight;
}

vec3 pointLightContribution(int i) {
  vec3 normal = normalize(fragNormal);
  vec3 lightDirection = normalize(pointLights[i].position - fragPos);
  float diffuse = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuseLight = pointLights[i].diffuse * (diffuse * vec3(texture(textureMaterial.diffuse, fragTex)));

  vec3 viewDir = normalize(viewerPos - fragPos);
  vec3 reflectDir = normalize(reflect(-lightDirection, normal));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), textureMaterial.shininess);
  vec3 specularLight = pointLights[i].specular * (spec * vec3(texture(textureMaterial.specular, fragTex)));

  vec3 ambientLight = vec3(texture(textureMaterial.diffuse, fragTex)) * pointLights[i].ambient;

  float dist = length(pointLights[i].position - fragPos);
  float attenuation = 1.0f / (pointLights[i].constant + pointLights[i].linear * dist + pointLights[i].quadratic * dist * dist);

  return (ambientLight + specularLight + diffuseLight) * attenuation;
}

vec3 spotLightContribution() {
  vec3 color = vec3(0.0f);

  vec3 normal = normalize(fragNormal);
  vec3 lightDirection = normalize(spotLight.position - fragPos);
  float theta = dot(lightDirection, normalize(-spotLight.direction));
  float epsilon = spotLight.innerCutOff - spotLight.outerCutOff;
  float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0f);

  float diffuse = max(dot(normal, lightDirection), 0.0f);
  vec3 diffuseLight = intensity * spotLight.diffuse * (diffuse * vec3(texture(textureMaterial.diffuse, fragTex)));

  vec3 viewDir = normalize(viewerPos - fragPos);
  vec3 reflectDir = normalize(reflect(-lightDirection, normal));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), textureMaterial.shininess);
  vec3 specularLight = intensity * spotLight.specular * (spec * vec3(texture(textureMaterial.specular, fragTex)));

  vec3 ambientLight = vec3(texture(textureMaterial.diffuse, fragTex)) * spotLight.ambient;

  float dist = length(spotLight.position - fragPos);
  float attenuation = 1.0f / (spotLight.constant + spotLight.linear * dist + spotLight.quadratic * dist * dist);
  color = (ambientLight + specularLight + diffuseLight) * attenuation;

  return color;
}

void main() {
  vec3 color = directionalLightContribution();
  for (int i = 0; i < pointLightCount; i++) {
    color += pointLightContribution(i);
  }
  color += spotLightContribution();
  outColor = vec4(color, 1.0f);
}