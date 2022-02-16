#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader 
{
private:
	unsigned int ID = 0;

	bool CheckShaderCompilation(unsigned int shader) const;
	bool CheckProgramLinkage(unsigned int shaderProgram) const;
	
public:
	// Construct a shader from shader files
	Shader(const char* vertexPath, const char* fragmentPath);

	// Bind this shader to the active OGL state
	void Use();

	// functions for setting uniform variables
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVector2(const std::string& name, float x, float y) const;
	void SetVector3(const std::string& name, float x, float y, float z) const;
	void SetVector4(const std::string& name, float x, float y, float z, float w) const;
	void SetColor(const std::string& name, float red, float green, float blue, float alpha) const;
	void SetMatrix4(const std::string& name, glm::mat4 matrix) const;

	// getters for uniform values
	bool GetBool(const std::string& name) const;
	int GetInt(const std::string& name) const;
	float GetFloat(const std::string& name) const;
	void GetVector2(const std::string& name, float& outX, float& outY) const;
	void GetVector3(const std::string& name, float& outX, float& outY, float& outZ) const;
	void GetVector4(const std::string& name, float& outX, float& outY, float& outZ, float& outW) const;
	void GetColor(const std::string& name, float& outR, float& outG, float& outB, float& outA) const;

	bool IsValid() const { return ID != 0; }

	static void Unbind();
};