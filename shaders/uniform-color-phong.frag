#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light 
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 position;
in vec3 normal;

uniform Material material;
uniform Light light;

out vec4 fragColor;

void main()
{
	vec3 viewPosition = vec3(0.0); // we're calculating in view space, so the view will always be here.

	vec3 lightDirection = normalize(position.xyz - light.position);
	vec3 viewDirection = normalize(position.xyz - viewPosition);
	vec3 reflectDirection = reflect(lightDirection, normal);

	float diffFactor = max(dot(-lightDirection, normal), 0.0);
	float specFactor = pow(max(dot(-viewDirection, reflectDirection), 0.0), material.shininess); // 32 is the specular component. Higher means shinier.
	
	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * material.diffuse * diffFactor;
	vec3 specular = light.specular * material.specular * specFactor;

	fragColor = vec4(ambient + diffuse + specular, 1.0);
}