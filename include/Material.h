#pragma once

#include "Shader.h"
#include "Texture.h"

#include <vector>
#include <map>

struct TextureMap
{
	unsigned int index;
	Texture texture;
};

class Material
{
public:
	Material(const Shader& shader);
	Material(const Shader& shader, const Texture& baseTexture);
	Material(const Shader& shader, const std::vector<TextureMap>& textures);
	~Material();

	void SetTexture(const Texture& texture, unsigned int index);
	void Use();

	Shader GetShader() const { return mShader; }
	int GetTextureMapIndex(unsigned int index);

private:
	Shader mShader;
	std::vector<TextureMap> mTextures;
};