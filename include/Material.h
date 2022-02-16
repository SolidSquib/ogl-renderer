#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Texture.h"

struct Material
{
	std::shared_ptr<Texture> diffuseMap;
	std::shared_ptr<Texture> specularMap;
	std::shared_ptr<Texture> emissionMap;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
};