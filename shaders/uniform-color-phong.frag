#version 330 core

struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D emissionMap;
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
in vec2 tex_coords;

uniform Material material;
uniform Light light;
uniform float time;

out vec4 fragColor;

vec3 DiffuseLighting(vec3 color, vec3 lightDirection)
{	
	float diffFactor = max(dot(-lightDirection, normal), 0.0);
	vec3 diffuseColor = texture(material.diffuseMap, tex_coords).xyz;
	return light.diffuse * color * diffFactor;
}

vec3 GetSpecularHighlight(vec3 specularAmount, vec3 lightDirection)
{
	vec3 viewDirection = normalize(position.xyz); // because view location is 0 (view space)
	vec3 reflectDirection = reflect(lightDirection, normal);
	float specFactor = pow(max(dot(-viewDirection, reflectDirection), 0.0), material.shininess); 
	return light.specular * specularAmount * specFactor;
}

vec2 DistortUVOverTime(vec2 uv, float amplitude, float xfrequency, float yfrequency, float xydependency = 0.0, float yxdependency = 0.0)
{
	float x = sin(xfrequency * uv.x + (xydependency * yfrequency * uv.y) + time) * amplitude;
	float y = sin(yfrequency * uv.y + (yxdependency * xfrequency * uv.x) + time) * amplitude;
	return vec2(uv.x+x, uv.y+y);
}

void main()
{
	vec3 lightDirection = normalize(position.xyz - light.position);
	vec3 diffuseColor = texture(material.diffuseMap, tex_coords).xyz;
	vec3 specularAmount = texture(material.specularMap, tex_coords).xyz;
	vec3 emissionColor = texture(material.emissionMap, DistortUVOverTime(tex_coords, 0.01, 5.0, 5.0, 0.5)).xyz;

	vec3 ambient = light.ambient * diffuseColor;
	vec3 diffuse = DiffuseLighting(diffuseColor, lightDirection);
	vec3 specular = GetSpecularHighlight(specularAmount, lightDirection);
	vec3 emission = vec3(0.0);
	
	if (specularAmount == vec3(0.0))
	{
		emission = emissionColor;
	}	

	fragColor = vec4(ambient + diffuse + specular + emission, 1.0);
}