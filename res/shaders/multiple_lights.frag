#version 460 core

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  bool ambientTexturePresent;
  sampler2D ambientTexture;
  bool diffuseTexturePresent;
  sampler2D diffuseTexture;
  bool specularTexturePresent;
  sampler2D specularTexture;

  float shininess;
};

struct Light {
  int type;

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

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTex;

out vec4 outColor;

uniform vec3 viewerPos;
uniform Material material;
uniform Light light;

vec3 calculateAmbientLight() {
  vec3 ambientLight = vec3(0.0f);
  if (material.ambientTexturePresent) {
    ambientLight = vec3(texture(material.ambientTexture, fragTex)) * light.ambient;
  } else {
    if (material.diffuseTexturePresent) {
      ambientLight = vec3(texture(material.diffuseTexture, fragTex)) * light.ambient;
    } else {
      ambientLight = material.ambient * light.ambient;
    }
  }
  return ambientLight;
}

vec3 calculateDiffuseLight(vec3 normal, vec3 lightDirection) {
  float diffuse = max(dot(normal, lightDirection), 0.0f);

  vec3 diffuseLight;
  if (material.diffuseTexturePresent) {
    diffuseLight = light.diffuse * (diffuse * vec3(texture(material.diffuseTexture, fragTex)));
  } else {
    diffuseLight = light.diffuse * (diffuse * material.diffuse);
  }
  return diffuseLight;
}

vec3 calculateSpecularLight(vec3 normal, vec3 lightDirection) {
  vec3 viewDir = normalize(viewerPos - fragPos);
  vec3 reflectDir = normalize(reflect(-lightDirection, normal));
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

  vec3 specularLight;
  if (material.specularTexturePresent) {
    specularLight = light.specular * (spec * vec3(texture(material.specularTexture, fragTex)));
  } else {
    specularLight = light.specular * (spec * material.specular);
  }
  return specularLight;
}

vec3 directionalLight() {
  vec3 color = vec3(0.0f);

  vec3 normal = normalize(fragNormal);
  vec3 lightDirection = normalize(-light.direction);

  vec3 diffuseLight = calculateDiffuseLight(normal, lightDirection);
  vec3 specularLight = calculateSpecularLight(normal, lightDirection);
  vec3 ambientLight = calculateAmbientLight();

  color = (ambientLight + specularLight + diffuseLight);

  return color;
}

vec3 pointLight() {
  vec3 color = vec3(0.0f);

  vec3 normal = normalize(fragNormal);
  vec3 lightDirection = normalize(light.position - fragPos);

  vec3 diffuseLight = calculateDiffuseLight(normal, lightDirection);
  vec3 specularLight = calculateSpecularLight(normal, lightDirection);
  vec3 ambientLight = calculateAmbientLight();

  float dist = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

  color = (ambientLight + specularLight + diffuseLight) * attenuation;

  return color;
}

vec3 spotLight() {
  vec3 color = vec3(0.0f);

  vec3 normal = normalize(fragNormal);
  vec3 lightDirection = normalize(light.position - fragPos);

  vec3 diffuseLight = calculateDiffuseLight(normal, lightDirection);
  vec3 specularLight = calculateSpecularLight(normal, lightDirection);
  vec3 ambientLight = calculateAmbientLight();

  float theta = dot(lightDirection, normalize(-light.direction));
  float epsilon = light.innerCutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0f);

  float dist = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

  color = (ambientLight + specularLight + diffuseLight) * intensity * attenuation;

  return color;
}

void main() {
  vec3 color = vec3(0.0f);
  if (light.type == 0) {
    color = directionalLight();
  } else if (light.type == 1) {
    color = pointLight();
  } else if (light.type == 2) {
    color = spotLight();
  }
  outColor = vec4(color, 1.0f);
}