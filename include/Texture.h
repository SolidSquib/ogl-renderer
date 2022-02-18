#pragma once

#include <glad/glad.h>
#include <iostream>
#include "stb_image/stb_image.h"

class Texture
{
	friend class TextureManager;
public:
	enum ETextureType {
		TEX_DIFFUSE,
		TEX_SPECULAR,
		TEX_NORMAL,
		TEX_EMISSION
	};

public:	
	~Texture();

	void SetTextureWrappingMode(unsigned int paramu, unsigned int paramv);
	void SetTextureSamplingMode(unsigned int paramMin, unsigned int paramMag);

	unsigned int GetID() const { return mID; }
	unsigned int GetType() const { return mType; }
	std::string GetPath() const { return mPath; }

private:
	unsigned int mID = 0;
	ETextureType mType = TEX_DIFFUSE;
	std::string mPath = "";

	Texture() {}
};