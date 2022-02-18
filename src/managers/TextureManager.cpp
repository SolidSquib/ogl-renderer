#include "..\..\include\managers\TextureManager.h"

std::unique_ptr<TextureManager> TextureManager::mManager;

TextureManager& TextureManager::Get()
{
	if (!mManager.get())
	{
		mManager = std::unique_ptr<TextureManager>(new TextureManager);
	}

	return *mManager.get();
}

std::shared_ptr<Texture> TextureManager::FromFile(const std::string& path, bool flipVertically)
{
	if (mLoadedTextures.contains(path))
	{
		std::cout << "WARNING::TEXTURE_MANAGER: " << path << " already loaded." << std::endl;
		return mLoadedTextures[path];
	}

	int width, height, numChannels;

	stbi_set_flip_vertically_on_load(flipVertically);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
	unsigned int textureChannels = 0;

	switch (numChannels)
	{
	case 1:
		textureChannels = GL_R;
		break;
	case 2:
		textureChannels = GL_RG;
		break;
	case 3:
		textureChannels = GL_RGB;
		break;
	case 4:
		textureChannels = GL_RGBA;
		break;
	default:
		std::cout << "ERROR::TEXTURE_MANAGER::FAILED_TO_LOAD: " << path << " unsupported channel format detected." << std::endl;
	}

	if (data)
	{
		std::shared_ptr<Texture> newTexture(new Texture);

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, textureChannels, width, height, 0, textureChannels, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		newTexture->mID = texture;
		newTexture->mPath = path;
		newTexture->mType = Texture::TEX_DIFFUSE;

		mLoadedTextures[path] = newTexture;
	}
	else
	{
		std::cout << "ERROR::TEXTURE_MANAGER::FAILED_TO_LOAD: " << path << " failed to load correctly." << std::endl;
	}

	stbi_image_free(data);

	if (mLoadedTextures.contains(path))
	{
		return mLoadedTextures[path];
	}

	return nullptr;
}

std::shared_ptr<Texture> TextureManager::RequestTexture(const std::string& path)
{
	if (!mLoadedTextures.contains(path))
	{
		return FromFile(path, false);
	}

	return mLoadedTextures[path];
}

void TextureManager::SetTextureType(const std::shared_ptr<Texture>& texture, Texture::ETextureType type)
{
	if (texture)
	{
		texture->mType = type;
	}
}

void TextureManager::SetTextureType(const std::string& path, Texture::ETextureType type)
{
	std::shared_ptr<Texture> texture = RequestTexture(path);
	SetTextureType(texture, type);
}

bool TextureManager::ReleaseTexture(Texture* texture)
{
	// TODO keep our own reference count somewhere so we can actually release textures instead of waiting until the manager is deleted.
	return false;
}
