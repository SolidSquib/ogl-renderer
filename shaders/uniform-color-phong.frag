#version 330 core

#define MAX_DIFFUSE_MAPS 1
#define MAX_SPECULAR_MAPS 1
#define MAX_EMISSION_MAPS 1
#define MAX_NORMAL_MAPS 1
struct Material
{
	sampler2D diffuseMaps[MAX_DIFFUSE_MAPS];
	sampler2D specularMaps[MAX_SPECULAR_MAPS];
	sampler2D emissionMaps[MAX_EMISSION_MAPS];
	sampler2D normalMaps[MAX_NORMAL_MAPS];

	bool useDiffuseColor;
	bool useSpecularValue;
	bool useEmissionColor;
	bool useVertexNormals;

	vec3 diffuseColor;
	vec3 specularValue;
	vec3 emissionColor;

	float shininess;
};

struct PointLight 
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
#define NUM_POINT_LIGHTS 4
uniform PointLight pointLights[NUM_POINT_LIGHTS];

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
}; 
uniform DirectionalLight directionalLight;

struct SpotLight
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	highp float innerCutoff;
	highp float outerCutoff;
};
#define NUM_SPOT_LIGHTS 4
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_tex_coords;

uniform Material material;
uniform float time;

out vec4 fragColor;

vec2 DistortUVOverTime(vec2 uv, float amplitude, float xfrequency, float yfrequency, float xydependency = 0.0, float yxdependency = 0.0)
{
	float x = sin(xfrequency * uv.x + (xydependency * yfrequency * uv.y) + time) * amplitude;
	float y = sin(yfrequency * uv.y + (yxdependency * xfrequency * uv.x) + time) * amplitude;
	return vec2(uv.x+x, uv.y+y);
}

vec4 CalculateDirectionalLight(DirectionalLight light, vec4 baseColor, vec4 specularValue, vec4 normal, vec4 viewDirection)
{
	vec4 lightDir = vec4(normalize(light.direction), 0.0);
	vec4 reflectDir = normalize(reflect(lightDir, normal));

	vec4 ambient = vec4(light.ambient, 1.0) * baseColor;

	float diffuseFactor = max(dot(normal, -lightDir), 0.0);
	vec4 diffuse = baseColor * vec4(light.diffuse, 1.0) * diffuseFactor;

	float specularFactor = max(pow(dot(viewDirection, -reflectDir), max(material.shininess, 0.1)), 0.0);
	vec4 specular = specularValue * vec4(light.specular, 1.0) * specularFactor;
	
	return max(ambient + diffuse + specular, 0.0);
}

vec4 CalculatePointLight(PointLight light, vec4 baseColor, vec4 specularValue, vec4 position, vec4 normal, vec4 viewDirection)
{
	vec4 lightDir = normalize(position - vec4(light.position, 1.0));
	vec4 reflectDir = reflect(lightDir, normal);

	float distance = length(position - vec4(light.position, 1.0));
	float attenuationFactor = max(1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)), 0.0);

	vec4 ambient = vec4(light.ambient, 1.0) * baseColor * attenuationFactor;

	float diffuseFactor = max(dot(normal, -lightDir), 0.0);
	vec4 diffuse = baseColor * vec4(light.diffuse, 1.0) * diffuseFactor * attenuationFactor;

	float specularFactor = max(pow(dot(viewDirection, -reflectDir), max(material.shininess, 0.1)), 0.0);
	vec4 specular = specularValue * vec4(light.specular, 1.0) * specularFactor * attenuationFactor;
	
	return max(ambient + diffuse + specular, 0.0);
}

vec4 CalculateSpotLight(SpotLight light, vec4 baseColor, vec4 specularValue, vec4 position, vec4 normal, vec4 viewDirection)
{
	vec4 centerLightDir = vec4(normalize(light.direction), 0.0);
	vec4 lightDir = normalize(position - vec4(light.position, 1.0));
	vec4 reflectDir = reflect(lightDir, normal);

	float theta = dot(lightDir, centerLightDir);
	float epsilon = light.innerCutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	float distance = length(position - vec4(light.position, 1.0));
	float attenuationFactor = max(1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)), 0.0);

	vec4 ambient = vec4(light.ambient, 1.0) * baseColor * attenuationFactor;

	float diffuseFactor = max(dot(normal, -lightDir), 0.0);
	vec4 diffuse = baseColor * vec4(light.diffuse, 1.0) * diffuseFactor * attenuationFactor * intensity;

	float specularFactor = max(pow(dot(viewDirection, -reflectDir), max(material.shininess, 0.1)), 0.0);
	vec4 specular = specularValue * vec4(light.specular, 1.0) * specularFactor * attenuationFactor * intensity;
	
	return max(ambient + diffuse + specular, 0.0);
}

void main()
{
	vec4 diffuseColor = material.useDiffuseColor 
		? vec4(material.diffuseColor, 1.0)
		: texture(material.diffuseMaps[0], frag_tex_coords);

	vec4 specularAmount = material.useSpecularValue 
		? vec4(material.specularValue, 1.0)
		: texture(material.specularMaps[0], frag_tex_coords);

	vec4 emissionColor = material.useEmissionColor 
		? vec4(material.emissionColor, 1.0)
		: texture(material.emissionMaps[0], DistortUVOverTime(frag_tex_coords, 0.01, 5.0, 5.0, 1.5));
	
	vec4 normal = vec4(normalize(frag_normal), 0.0);

	vec4 emission = vec4(0.0);
	if (specularAmount.xyz == vec3(0.0))
	{
		emission = emissionColor;
	}
		
	vec4 viewDirection = vec4(0.0, 0.0, -1.0, 0.0);
	vec4 position = vec4(frag_position, 1.0);

	vec4 resultColor = CalculateDirectionalLight(directionalLight, diffuseColor, specularAmount, normal, viewDirection);
	for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
	{
		resultColor += CalculatePointLight(pointLights[i], diffuseColor, specularAmount, position, normal, viewDirection);
	}
	for (int i = 0; i < NUM_SPOT_LIGHTS; ++i)
	{
		resultColor += CalculateSpotLight(spotLights[i], diffuseColor, specularAmount, position, normal, viewDirection);
	}
	
	fragColor = resultColor + emission;
}