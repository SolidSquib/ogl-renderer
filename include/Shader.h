#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "Material.h"
#include "Light.h"

class Shader 
{
public:
	struct TextureMap
	{
		unsigned int index;
		std::shared_ptr<Texture> texture;
	};

public:
	// Construct a shader from shader files
	Shader(const char* vertexPath, const char* fragmentPath);

	// Bind this shader to the active OGL state
	void Use();

	// functions for setting uniform variables
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVector2(const std::string& name, const glm::vec2& vector) const;
	void SetVector3(const std::string& name, const glm::vec3& vector) const;
	void SetVector4(const std::string& name, const glm::vec4& vector) const;
	void SetColor(const std::string& name, const glm::vec4& color) const;
	void SetMatrix4(const std::string& name, glm::mat4 matrix) const;
	void SetMaterial(const std::string& name, const Material& material);
	void SetDirectionalLight(const std::string& name, const DirectionalLight& light, unsigned int lightIndex);
	void SetPointLight(const std::string& name, const PointLight& light, unsigned int lightIndex);
	void SetSpotLight(const std::string& name, const SpotLight& light, unsigned int lightIndex);

	// getters for uniform values
	bool GetBool(const std::string& name) const;
	int GetInt(const std::string& name) const;
	float GetFloat(const std::string& name) const;
	glm::vec2 GetVector2(const std::string& name) const;
	glm::vec3 GetVector3(const std::string& name) const;
	glm::vec4 GetVector4(const std::string& name) const;
	glm::vec4 GetColor(const std::string& name) const;

	bool IsValid() const { return ID != 0; }

	static void Unbind();

private:
	unsigned int ID = 0;

	bool CheckShaderCompilation(unsigned int shader) const;
	bool CheckProgramLinkage(unsigned int shaderProgram) const;
};