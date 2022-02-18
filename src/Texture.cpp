#include "Texture.h"

Texture::~Texture()
{
	glDeleteTextures(1, &mID);
	mID = 0;
}

void Texture::SetTextureWrappingMode(unsigned int paramu, unsigned int paramv)
{
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, paramu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, paramv);
}

void Texture::SetTextureSamplingMode(unsigned int paramMin, unsigned int paramMax)
{
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, paramMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, paramMax);
}
