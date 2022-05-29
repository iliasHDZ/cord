#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec3 viewPos;

in vec2 texCoord;

in float depth;

#define LIGHT_NONE 0
#define LIGHT_DIRECTIONAL 1
#define LIGHT_POINT 2
#define LIGHT_SPOT 3

#define LIGHT_COUNT 10

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    sampler2D diffuseMap;
    sampler2D specularMap;
    bool hasDiffuseMap;
    bool hasSpecularMap;

    float shininess;
};

struct Light {
    int  type;

    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;

uniform float near;
uniform float far;

uniform Light lights[LIGHT_COUNT];

float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

float calcAttenuation(Light light, vec3 fragPos) {
    float d = length(fragPos - light.position);
    return 1.0 / (light.constant + light.linear * d + light.quadratic * d * d);
}

vec3 calcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambientColor  = material.ambient;
    vec3 diffuseColor  = material.diffuse;
    vec3 specularColor = material.specular;

    if (material.hasDiffuseMap) {
        ambientColor = texture(material.diffuseMap, texCoord).xyz;
        diffuseColor = ambientColor;

        if (texture(material.diffuseMap, texCoord).a < 0.5)
            discard;
    }

    if (material.hasSpecularMap)
        specularColor = texture(material.specularMap, texCoord).xyz;

    // ambient
    vec3 ambient = light.ambient * ambientColor;

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseColor;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specularColor;

    // attenuation
    float attenuation = calcAttenuation(light, fragPos);
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0, 1.0);

    vec3 ambientColor  = material.ambient;

    if (material.hasDiffuseMap)
        ambientColor = texture(material.diffuseMap, texCoord).xyz;

    vec3 offColor = calcAttenuation(light, fragPos) * light.ambient * ambientColor;

    if (intensity == 0.0) {
        return offColor;
    } else if (intensity < 1.0) {
        return mix(offColor, calcPointLight(light, normal, fragPos, viewDir), intensity);
    }

    return calcPointLight(light, normal, fragPos, viewDir) * intensity;
}

void main() {
    vec3 result = vec3(0.0);
    vec3 viewDir = normalize(viewPos - fragPos);

    for (int i = 0; i < LIGHT_COUNT; i++) {
        Light light = lights[i];

        if (light.type == LIGHT_POINT)
            result += calcPointLight(light, normalize(normal), fragPos, viewDir);
        else if (light.type == LIGHT_SPOT)
            result += calcSpotLight(light, normalize(normal), fragPos, viewDir);
    }

    float depth = linearizeDepth(gl_FragCoord.z);

    FragColor = vec4(result, 1.0);
}
