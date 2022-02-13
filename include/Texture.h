#pragma once

#include <glad/glad.h>
#include <iostream>
#include "stb_image/stb_image.h"

class Texture
{
private:
	unsigned int ID = 0;

public:
	Texture(const char* texturePath);

	void Use();

	unsigned int GetID() const { return ID; }
};