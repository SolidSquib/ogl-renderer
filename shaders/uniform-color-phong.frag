#version 330 core

in vec3 position;
in vec3 normal;
in vec3 lightPositionViewSpace;

uniform vec4 uniformColor;
uniform vec4 lightColor;
uniform vec4 ambientLightColor;

out vec4 fragColor;

void main()
{
	vec3 viewPosition = vec3(0.0); // we're calculating in view space, so the view will always be here.

	vec3 lightDirection = normalize(position.xyz - lightPositionViewSpace);
	vec3 viewDirection = normalize(position.xyz - viewPosition);
	vec3 reflectDirection = reflect(lightDirection, normal);

	const float ambientStrength = 0.35f;
	vec4 ambientColor = ambientStrength * ambientLightColor;

	vec4 diffuse = max(dot(-lightDirection, normal), 0.0) * lightColor;

	float specularStrength = 0.5;
	float specValue = pow(max(dot(-viewDirection, reflectDirection), 0.0), 32); // 32 is the specular component. Higher means shinier.
	vec4 specular = specularStrength * specValue * lightColor;

	fragColor = uniformColor * (ambientColor + diffuse + specular);
}