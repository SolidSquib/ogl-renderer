#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode, fragmentCode;
	std::ifstream vertexFile, fragmentFile;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Read the contents of the shader files and save them to strings
	try
	{
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		std::stringstream vertexStream, fragmentStream;

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();
	}
	catch (std::ifstream::failure error)
	{
		std::cout << "ERROR::SHADER::FILE_READ_FAILED" << std::endl;
	}

	const char* cVertexCode = vertexCode.c_str();
	const char* cFragmentCode = fragmentCode.c_str();

	// Create and compile shaders, link to shader program
	unsigned int vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);	
	glShaderSource(vertex, 1, &cVertexCode, NULL);
	glCompileShader(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &cFragmentCode, NULL);
	glCompileShader(fragment);

	if (CheckShaderCompilation(vertex) && CheckShaderCompilation(fragment))
	{
		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertex);
		glAttachShader(shaderProgram, fragment);
		glLinkProgram(shaderProgram);

		if (CheckProgramLinkage(shaderProgram))
		{
			ID = shaderProgram;
		}
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

bool Shader::CheckShaderCompilation(unsigned int shader) const
{
	int success;
	char infoLog[512];
	char shaderSource[1024];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		glGetShaderSource(shader, 1024, NULL, shaderSource);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED: " << infoLog << std::endl
			<< shaderSource << std::endl;
	}

	return success;
}

bool Shader::CheckProgramLinkage(unsigned int program) const
{
	int success;
	char infoLog[512];

	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED" << infoLog << std::endl;
	}

	return success;
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVector2(const std::string& name, float x, float y, float z)
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::SetVector3(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::SetVector4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::SetColor(const std::string& name, float red, float green, float blue, float alpha)
{
	SetVector4(name, red, green, blue, alpha);
}

void Shader::Unbind()
{
	glUseProgram(0);
}
