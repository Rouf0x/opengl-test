#version 330 core
out vec4 FragColor;

in vec3 currentPos;
in vec3 normal;
in vec3 color;
in vec2 textureCoordinates;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform float specularIntensity;
uniform float ambientIntensity;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

uniform int lightType;

uniform vec3 lookVector;

vec4 pointLight() {
    vec3 lightVec = lightPos - currentPos;
    float dist = length(lightVec);
    float a = 0.20f;
    float b = 0.70f;
    float inten = 2.0f / (a * dist * dist + b * dist + 1.0f);

    vec3 normalFinal = normalize(normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normalFinal, lightDirection), 0.0f);
    //float ambientIntensityIntensity = 0.2f;

    //float specularIntensity = 2.00f;
    vec3 viewVector = normalize(camPos - currentPos);
    vec3 specularReflection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewVector, specularReflection), 0.0f), 16);
    float specular = specularAmount * specularIntensity;

    return (texture(diffuse0, textureCoordinates) * (diffuse * inten + ambientIntensity) + texture(specular0, textureCoordinates).r * specular * inten) * lightColor;
}

vec4 directLight() {
    vec3 lightVec = vec3(0.0f, 1.0f, 1.0f);
    float dist = length(lightVec);
    float a = 0.15f;
    float b = 0.0f;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    vec3 normalFinal = normalize(normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normalFinal, lightDirection), 0.0f);
    //float ambientIntensityIntensity = 0.2f;

    //float specularIntensity = 2.00f;
    vec3 viewVector = normalize(camPos - currentPos);
    vec3 specularReflection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewVector, specularReflection), 0.0f), 16);
    float specular = specularAmount * specularIntensity;

    return (texture(diffuse0, textureCoordinates) * (diffuse + ambientIntensity) + texture(specular0, textureCoordinates).r * specular) * lightColor;
}

vec4 spotLight() {
    vec3 lightVec = lightPos - currentPos;

    float outerCone = 0.97;
    float innerCone = 0.98;

    float dist = length(lightVec);
    float a = 0.80f;
    float b = 0.0f;

    vec3 normalFinal = normalize(normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normalFinal, lightDirection), 0.0f);
    //float ambientIntensityIntensity = 0.2f;

    //float specularIntensity = 2.00f;
    vec3 viewVector = normalize(camPos - currentPos);
    vec3 specularReflection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewVector, specularReflection), 0.0f), 16);
    float specular = specularAmount * specularIntensity;

    float angle = dot(normalize(lookVector), -lightDirection);
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f) * clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    return (texture(diffuse0, textureCoordinates) * (diffuse * inten + ambientIntensity) + texture(specular0, textureCoordinates).r * specular * inten) * lightColor;
}

void main() {
    switch(lightType) {
        case 1:
            FragColor = pointLight();
            break;
        case 2:
            FragColor = directLight();
            break;
        case 3:
            FragColor = spotLight();
            break;
        default:
            FragColor = pointLight();
            break;
    }

}