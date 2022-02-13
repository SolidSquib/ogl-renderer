#include "Texture.h"

Texture::Texture(const char* texturePath)
{
	int width, height, numChannels;
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

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		ID = texture;
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << std::endl;
	}

	stbi_image_free(data);
}

void Texture::Use()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}
