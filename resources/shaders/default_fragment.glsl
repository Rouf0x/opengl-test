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
    // sample textures (assume engine binds 1x1 defaults for untextured meshes)
    vec4 diff = texture(diffuse0, textureCoordinates);
    float specTex = texture(specular0, textureCoordinates).r;

    vec3 lightVec = lightPos - currentPos;
    float dist = length(lightVec);
    float inten = 2.0 / (lightAIntensity * dist * dist + lightBIntensity * dist + 1.0);

    vec3 normalFinal = normalize(normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normalFinal, lightDirection), 0.0);

    vec3 viewVector = normalize(camPos - currentPos);
    vec3 specularReflection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewVector, specularReflection), 0.0), 16.0);
    float specular = specularAmount * specularIntensity;

    vec3 lit = diff.rgb * (diffuse * inten + ambientIntensity) + vec3(specTex) * specular * inten;
    lit *= lightColor.rgb;

    return vec4(lit, diff.a);
}

vec4 directLight() {
    vec4 diff = texture(diffuse0, textureCoordinates);
    float specTex = texture(specular0, textureCoordinates).r;

    vec3 lightVec = vec3(0.0, 1.0, 1.0);
    float dist = length(lightVec);
    float inten = 1.0 / (lightAIntensity * dist * dist + lightBIntensity * dist + 1.0);

    vec3 normalFinal = normalize(normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normalFinal, lightDirection), 0.0);

    vec3 viewVector = normalize(camPos - currentPos);
    vec3 specularReflection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewVector, specularReflection), 0.0), 16.0);
    float specular = specularAmount * specularIntensity;

    vec3 lit = diff.rgb * (diffuse * inten + ambientIntensity) + vec3(specTex) * specular * inten;
    lit *= lightColor.rgb;

    return vec4(lit, diff.a);
}

vec4 spotLight() {
    vec4 diff = texture(diffuse0, textureCoordinates);
    float specTex = texture(specular0, textureCoordinates).r;

    vec3 lightVec = lightPos - currentPos;
    float dist = length(lightVec);

    vec3 normalFinal = normalize(normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normalFinal, lightDirection), 0.0);

    vec3 viewVector = normalize(camPos - currentPos);
    vec3 specularReflection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewVector, specularReflection), 0.0), 16.0);
    float specular = specularAmount * specularIntensity;

    float angle = dot(normalize(lookVector), -lightDirection);
    float inten = 1.0 / (lightAIntensity * dist * dist + lightBIntensity * dist + 1.0) *
    clamp((angle - SPOT_OUTER_CONE) / (SPOT_INNER_CONE - SPOT_OUTER_CONE), 0.0, 1.0);

    vec3 lit = diff.rgb * (diffuse * inten + ambientIntensity) + vec3(specTex) * specular * inten;
    lit *= lightColor.rgb;

    return vec4(lit, diff.a);
}

// ================== Main ==================
void main() {
    vec4 diffSample = texture(diffuse0, textureCoordinates);

    // alpha discard (works for grass quads with alpha)
    if (diffSample.a < 0.1)
    discard;

    float depth = logisticDepth(gl_FragCoord.z);

    vec4 shaded;
    switch(lightType) {
        case 1:
        shaded = pointLight();
        break;
        case 2:
        shaded = directLight();
        break;
        case 3:
        shaded = spotLight();
        break;
        default:
        shaded = pointLight();
        break;
    }

    FragColor = shaded * (1.0 - depth) + vec4(depth * FOG_COLOR, 1.0);
}
