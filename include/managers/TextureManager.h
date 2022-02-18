#pragma once

#include <iostream>
#include <map>
#include "Texture.h"

class TextureManager
{
public:
	static TextureManager& Get();
	
	std::shared_ptr<Texture> FromFile(const std::string& path, bool flipVertically);
	std::shared_ptr<Texture> RequestTexture(const std::string& path);
	void SetTextureType(const std::shared_ptr<Texture>& texture, Texture::ETextureType type);
	void SetTextureType(const std::string& path, Texture::ETextureType type);

	bool ReleaseTexture(Texture* texture);

private:
	static std::unique_ptr<TextureManager> mManager;
	std::map<std::string, std::shared_ptr<Texture>> mLoadedTextures;

	TextureManager() {}
};