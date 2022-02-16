#include "Texture.h"

Texture::Texture(const char* texturePath, bool flipVertically = false)
	: Texture(texturePath, flipVertically, GL_RGB)
{	
}

Texture::Texture(const char* texturePath, bool flipVertically, unsigned int glTextureType)
{
	int width, height, numChannels;

	stbi_set_flip_vertically_on_load(flipVertically);
	unsigned char* data = stbi_load(texturePath, &width, &height, &numChannels, 0);

	if (data)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, glTextureType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		ID = texture;
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << std::endl;
	}

	stbi_image_free(data);
}

void Texture::SetTextureWrappingMode(unsigned int paramu, unsigned int paramv)
{
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, paramu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, paramv);
}

void Texture::SetTextureSamplingMode(unsigned int paramMin, unsigned int paramMax)
{
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, paramMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, paramMax);
}
