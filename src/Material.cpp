#include "..\include\Material.h"

Material::Material(const Shader& shader)
	: mShader(shader)
{
}

Material::Material(const Shader& shader, const Texture& baseTexture)
	: mShader(shader)
{
	mTextures[0] = { 0, baseTexture };
}

Material::Material(const Shader& shader, const std::vector<TextureMap>& textures)
	: mShader(shader),
	mTextures(textures)
{
}

Material::~Material()
{
}

void Material::SetTexture(const Texture& texture, unsigned int index)
{
	int existingIndex = GetTextureMapIndex(index);
	if (existingIndex >= 0)
	{
		mTextures[existingIndex].texture = texture;
	}
	else
	{
		mTextures.push_back({ index, texture });
	}
}

void Material::Use()
{
	mShader.Use();

	for (const auto& iter : mTextures)
	{
		iter.texture.Use(iter.index);
	}
}

int Material::GetTextureMapIndex(unsigned int index)
{
	for (unsigned int i = 0; i < mTextures.size(); ++i)
	{
		if (mTextures[i].index == index)
		{
			return i;
		}
	}

	return -1;
}
