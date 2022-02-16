#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inCol;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

out vec4 finalLightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec4 lightColor;
uniform vec4 ambientLightColor;

void main()
{
	vec4 worldPosition = model * vec4(inPos, 1.0);
	vec3 normal = mat3(transpose(inverse(model))) * inNormal;

	vec3 lightDirection = normalize(worldPosition.xyz - lightPosition);
	vec3 viewDirection = normalize(worldPosition.xyz - viewPosition);
	vec3 reflectDirection = reflect(lightDirection, normal);

	const float ambientStrength = 0.35f;
	vec4 ambientColor = ambientStrength * ambientLightColor;

	vec4 diffuse = max(dot(-lightDirection, normal), 0.0) * lightColor;

	float specularStrength = 0.5;
	float specValue = pow(max(dot(-viewDirection, reflectDirection), 0.0), 32); // 32 is the specular component. Higher means shinier.
	vec4 specular = specularStrength * specValue * lightColor;

	finalLightColor = ambientColor + diffuse + specular;
	gl_Position = projection * view * worldPosition;
}