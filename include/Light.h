#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Light
{	
	glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
};

struct DirectionalLight : public Light
{
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
};

struct PointLight : public Light
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight : public PointLight
{
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

	float innerCutoff;
	float outerCutoff;
};