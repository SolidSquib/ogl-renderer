#pragma once

#include "Shader.h"
#include "Texture.h"

#include <vector>
#include <map>

struct TextureMap
{
	unsigned int index;
	std::shared_ptr<Texture> texture;
};

class Material
{
public:
	Material(std::shared_ptr<Shader> shader);
	Material(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> baseTexture);
	Material(std::shared_ptr<Shader> shader, const std::vector<TextureMap>& textures);
	~Material();

	void SetTexture(std::shared_ptr<Texture> texture, unsigned int index);
	void Use();

	// functions for setting uniform variables
	void SetBool(const std::string& name, bool value) const { mShader->SetBool(name, value); }
	void SetInt(const std::string& name, int value) const { mShader->SetInt(name, value); }
	void SetFloat(const std::string& name, float value) const { mShader->SetFloat(name, value); }
	void SetVector2(const std::string& name, float x, float y) const { mShader->SetVector2(name, x, y); }
	void SetVector3(const std::string& name, float x, float y, float z) const { mShader->SetVector3(name, x, y, z); }
	void SetVector4(const std::string& name, float x, float y, float z, float w) const { mShader->SetVector4(name, x, y, z, w); }
	void SetColor(const std::string& name, float red, float green, float blue, float alpha) const { mShader->SetColor(name, red, green, blue, alpha); }
	void SetMatrix4(const std::string& name, glm::mat4 matrix) const { mShader->SetMatrix4(name, matrix); }

	// getters for uniform values
	bool GetBool(const std::string& name) const { return mShader->GetBool(name); }
	int GetInt(const std::string& name) const { return mShader->GetInt(name); }
	float GetFloat(const std::string& name) const { return mShader->GetFloat(name); }
	void GetVector2(const std::string& name, float& outX, float& outY) const { mShader->GetVector2(name, outX, outY); }
	void GetVector3(const std::string& name, float& outX, float& outY, float& outZ) const { mShader->GetVector3(name, outX, outY, outZ); }
	void GetVector4(const std::string& name, float& outX, float& outY, float& outZ, float& outW) const { mShader->GetVector4(name, outX, outY, outZ, outW); }
	void GetColor(const std::string& name, float& outR, float& outG, float& outB, float& outA) const { mShader->GetColor(name, outR, outG, outB, outA); }

	std::shared_ptr<Shader> GetShader() const { return mShader; }
	int GetTextureMapIndex(unsigned int index);

private:
	std::shared_ptr<Shader> mShader;
	std::vector<TextureMap> mTextures;
};