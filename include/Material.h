#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Texture.h"

struct Material
{
	std::vector<std::shared_ptr<Texture>> textures;

	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 emissionColor;

	float shininess;
	bool usesTransparency = false;
};