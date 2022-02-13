#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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
	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetColor(const std::string& name, float red, float green, float blue, float alpha);

	bool IsValid() const { return ID != 0; }
};