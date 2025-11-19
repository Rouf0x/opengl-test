#version 330 core
out vec4 FragColor;

in vec3 currentPos;
in vec3 normal;
in vec3 color;
in vec2 textureCoordinates;

// ================== Textures ==================
uniform sampler2D diffuse0;
uniform sampler2D specular0;

// ================== Camera ==================
uniform vec3 camPos;

// ================== Light Settings ==================
uniform int lightType; // 1 = point, 2 = directional, 3 = spot
uniform vec3 lightPos;
uniform vec3 lookVector;
uniform vec4 lightColor;
uniform float lightAIntensity;
uniform float lightBIntensity;

// ================== Material Settings ==================
uniform float specularIntensity;
uniform float ambientIntensity;

// ================== Spotlight Settings ==================
const float SPOT_OUTER_CONE = 0.85;
const float SPOT_INNER_CONE = 0.95;

// ================== Fog Settings ==================
const vec3 FOG_COLOR = vec3(0.0, 0.0, 0.0);
const float NEAR_PLANE = 0.1;
const float FAR_PLANE = 1.0;

// ================== Helper Functions ==================
float linearizeDepth(float depth) {
    return (2.0 * NEAR_PLANE * FAR_PLANE) / (FAR_PLANE + NEAR_PLANE - (depth * 2.0 - 1.0) * (FAR_PLANE - NEAR_PLANE));
}

float logisticDepth(float depth) {
    float zVal = linearizeDepth(depth);
    return 1.0 / (1.0 + exp(-0.5 * (zVal - 2.0)));
}

// ================== Lighting Functions ==================
vec4 pointLight() {
    vec3 lightVec = lightPos - currentPos;
    float dist = length(lightVec);
    float inten = 2.0 / (lightAIntensity * dist * dist + lightBIntensity * dist + 1.0);

    vec3 normalFinal = normalize(normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normalFinal, lightDirection), 0.0);

    vec3 viewVector = normalize(camPos - currentPos);
    vec3 specularReflection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewVector, specularReflection), 0.0), 16);
    float specular = specularAmount * specularIntensity;

    return (texture(diffuse0, textureCoordinates) * (diffuse * inten + ambientIntensity) + texture(specular0, textureCoordinates).r * specular * inten) * lightColor;
}

vec4 directLight() {
    vec3 lightVec = vec3(0.0, 1.0, 1.0);
    float dist = length(lightVec);
    float inten = 1.0 / (lightAIntensity * dist * dist + lightBIntensity * dist + 1.0);

    vec3 normalFinal = normalize(normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normalFinal, lightDirection), 0.0);

    vec3 viewVector = normalize(camPos - currentPos);
    vec3 specularReflection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewVector, specularReflection), 0.0), 16);
    float specular = specularAmount * specularIntensity;

    return (texture(diffuse0, textureCoordinates) * (diffuse + ambientIntensity) + texture(specular0, textureCoordinates).r * specular) * lightColor;
}

vec4 spotLight() {
    vec3 lightVec = lightPos - currentPos;
    float dist = length(lightVec);

    vec3 normalFinal = normalize(normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normalFinal, lightDirection), 0.0);

    vec3 viewVector = normalize(camPos - currentPos);
    vec3 specularReflection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewVector, specularReflection), 0.0), 16);
    float specular = specularAmount * specularIntensity;

    float angle = dot(normalize(lookVector), -lightDirection);
    float inten = 1.0 / (lightAIntensity * dist * dist + lightBIntensity * dist + 1.0) *
    clamp((angle - SPOT_OUTER_CONE) / (SPOT_INNER_CONE - SPOT_OUTER_CONE), 0.0, 1.0);

    return (texture(diffuse0, textureCoordinates) * (diffuse * inten + ambientIntensity) + texture(specular0, textureCoordinates).r * specular * inten) * lightColor;
}

// ================== Main ==================
void main() {
    if (texture(diffuse0, textureCoordinates).a < 0.1)
    discard;

    float depth = logisticDepth(gl_FragCoord.z);

    switch(lightType) {
        case 1:
        FragColor = pointLight() * (1.0 - depth) + vec4(depth * FOG_COLOR, 1.0);
        break;
        case 2:
        FragColor = directLight() * (1.0 - depth) + vec4(depth * FOG_COLOR, 1.0);
        break;
        case 3:
        FragColor = spotLight() * (1.0 - depth) + vec4(depth * FOG_COLOR, 1.0);
        break;
        default:
        FragColor = pointLight() * (1.0 - depth) + vec4(depth * FOG_COLOR, 1.0);
        break;
    }
}
