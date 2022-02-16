#include "..\include\Material.h"

Material::Material(std::shared_ptr<Shader> shader)
	: mShader(shader)
{
}

Material::Material(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> baseTexture)
	: mShader(shader)
{
	mTextures[0] = { 0, baseTexture };
}

Material::Material(std::shared_ptr<Shader> shader, const std::vector<TextureMap>& textures)
	: mShader(shader),
	mTextures(textures)
{
}

Material::~Material()
{
}

void Material::SetTexture(std::shared_ptr<Texture> texture, unsigned int index)
{
	int existingIndex = GetTextureMapIndex(index);
	if (existingIndex >= 0)
	{
		mTextures[existingIndex].texture = texture;
	}
	else if (texture.get())
	{
		mTextures.push_back({ index, texture });
	}
}

void Material::Use()
{
	if (mShader.get())
	{
		mShader->Use();

		for (const auto& iter : mTextures)
		{
			if (iter.texture.get())
			{
				iter.texture->Use(iter.index);
			}			
		}
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
