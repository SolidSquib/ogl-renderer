#pragma once

#include <glad/glad.h>
#include <iostream>
#include "stb_image/stb_image.h"

class Texture
{
private:
	unsigned int ID = 0;

public:
	Texture(const char* texturePath, bool flipVertically);
	Texture(const char* texturePath, bool flipVertically, unsigned int glTextureType);

	void Use(unsigned int index = 0) const;
	void SetTextureWrappingMode(unsigned int paramu, unsigned int paramv);
	void SetTextureSamplingMode(unsigned int paramMin, unsigned int paramMag);

	unsigned int GetID() const { return ID; }
};