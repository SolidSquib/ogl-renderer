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

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVector2(const std::string& name, const glm::vec2& vector) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y);
}

void Shader::SetVector3(const std::string& name, const glm::vec3& vector) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::SetVector4(const std::string& name, const glm::vec4& vector) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetColor(const std::string& name, const glm::vec4& vector) const
{
	SetVector4(name, vector);
}

void Shader::SetMatrix4(const std::string& name, glm::mat4 matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetMaterial(const std::string& name, const Material& material)
{
	int numTexturesMapped = 0;
	int numDiffuse = 0, numSpecular = 0, numEmission = 0, numNormal = 0;
	bool diffuseFound = false, specularFound = false, emissionFound = false, normalFound = false;

	auto GetShaderParameterName = [](unsigned int type) -> std::string {
		switch (type)
		{		
		case Texture::TEX_SPECULAR: return "material.specularMaps";
		case Texture::TEX_EMISSION: return "material.emissionMaps";
		case Texture::TEX_NORMAL: return "material.normalMaps";
		case Texture::TEX_DIFFUSE: 
		default: 
			return "material.diffuseMaps";
		}
	};

	auto SetFlagForType = [&](unsigned int type) {
		switch (type)
		{
		case Texture::TEX_DIFFUSE: diffuseFound = true; return;
		case Texture::TEX_SPECULAR: specularFound = true; return;
		case Texture::TEX_EMISSION: emissionFound = true; return;
		case Texture::TEX_NORMAL: normalFound = true; return;
		default: return;
		}
	};
		
	auto GetCountForType = [&](unsigned int type) -> int& {
		switch (type)
		{		
		case Texture::TEX_SPECULAR: return numSpecular;
		case Texture::TEX_EMISSION: return numEmission;
		case Texture::TEX_NORMAL: return numNormal;
		case Texture::TEX_DIFFUSE: 
		default: 
			return numDiffuse;
		}
	};

	for (unsigned int i = 0; i < material.textures.size(); ++i)
	{
		const std::shared_ptr<Texture>& texture = material.textures[i];
		if (texture)
		{
			glActiveTexture(GL_TEXTURE0 + numTexturesMapped);
			SetInt(GetShaderParameterName(texture->GetType()) + "[" + std::to_string(GetCountForType(texture->GetType())++) + "]", numTexturesMapped++);
			glBindTexture(GL_TEXTURE_2D, texture->GetID());
			SetFlagForType(texture->GetType());
		}
	}

	SetBool("material.useDiffuseColor", !diffuseFound);
	SetBool("material.useSpecularValue", !specularFound);
	SetBool("material.useEmissionColor", !emissionFound);
	SetBool("material.useVertexNormals", !normalFound);

	glActiveTexture(GL_TEXTURE0);

	SetVector3(name + ".diffuseColor", glm::vec3(material.diffuseColor));
	SetVector3(name + ".specularValue", glm::vec3(material.specularColor));
	SetVector3(name + ".emissionColor", glm::vec3(material.emissionColor));
	SetFloat(name + ".shininess", 32.0f);
}

void Shader::SetDirectionalLight(const std::string& name, const DirectionalLight& light, unsigned int lightIndex)
{
	// only support for one dir light currently
	SetVector3(name + "[" + std::to_string(lightIndex) + "].direction", light.GetDirection());
	SetVector3(name + "[" + std::to_string(lightIndex) + "].ambient", glm::vec3(light.GetAmbientColor()));
	SetVector3(name + "[" + std::to_string(lightIndex) + "].diffuse", glm::vec3(light.GetDiffuseColor()));
	SetVector3(name + "[" + std::to_string(lightIndex) + "].specular", glm::vec3(light.GetSpecularColor()));
}

void Shader::SetPointLight(const std::string& name, const PointLight& light, unsigned int lightIndex)
{
	SetVector3(name + "[" + std::to_string(lightIndex) + "].position", light.GetPosition());
	SetVector3(name + "[" + std::to_string(lightIndex) + "].ambient", glm::vec3(light.GetAmbientColor()));
	SetVector3(name + "[" + std::to_string(lightIndex) + "].diffuse", glm::vec3(light.GetDiffuseColor()));
	SetVector3(name + "[" + std::to_string(lightIndex) + "].specular", glm::vec3(light.GetSpecularColor()));
	SetFloat(name + "[" + std::to_string(lightIndex) + "].constant", light.GetConstantComponent());
	SetFloat(name + "[" + std::to_string(lightIndex) + "].linear", light.GetLinearComponent());
	SetFloat(name + "[" + std::to_string(lightIndex) + "].quadratic", light.GetQuadraticComponent());
}

void Shader::SetSpotLight(const std::string& name, const SpotLight& light, unsigned int lightIndex)
{
	SetVector3(name + "[" + std::to_string(lightIndex) + "].position", light.GetPosition());
	SetVector3(name + "[" + std::to_string(lightIndex) + "].direction", light.GetDirection());
	SetVector3(name + "[" + std::to_string(lightIndex) + "].ambient", glm::vec3(light.GetAmbientColor()));
	SetVector3(name + "[" + std::to_string(lightIndex) + "].diffuse", glm::vec3(light.GetDiffuseColor()));
	SetVector3(name + "[" + std::to_string(lightIndex) + "].specular", glm::vec3(light.GetSpecularColor()));
	SetFloat(name + "[" + std::to_string(lightIndex) + "].constant", light.GetConstantComponent());
	SetFloat(name + "[" + std::to_string(lightIndex) + "].linear", light.GetLinearComponent());
	SetFloat(name + "[" + std::to_string(lightIndex) + "].quadratic", light.GetQuadraticComponent());
	SetFloat(name + "[" + std::to_string(lightIndex) + "].innerCutoff", glm::cos(light.GetInnerCutoffRadians()));
	SetFloat(name + "[" + std::to_string(lightIndex) + "].outerCutoff", glm::cos(light.GetOuterCutoffRadians()));
}

bool Shader::GetBool(const std::string& name) const
{
	int outValue;
	glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &outValue);
	return (bool)outValue;
}

int Shader::GetInt(const std::string& name) const
{
	int outValue;
	glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &outValue);
	return outValue;
}

float Shader::GetFloat(const std::string& name) const
{
	float outValue;
	glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &outValue);
	return outValue;
}

glm::vec2 Shader::GetVector2(const std::string& name) const
{
	float value[2];
	glGetnUniformfv(ID, glGetUniformLocation(ID, name.c_str()), 2, value);
	return glm::vec2(value[0], value[1]);
}

glm::vec3 Shader::GetVector3(const std::string& name) const
{
	float value[3];
	glGetnUniformfv(ID, glGetUniformLocation(ID, name.c_str()), 3, value);
	return glm::vec3(value[0], value[1], value[2]);
}

glm::vec4 Shader::GetVector4(const std::string& name) const
{
	float value[4];
	glGetnUniformfv(ID, glGetUniformLocation(ID, name.c_str()), 4, value);
	return glm::vec4(value[0], value[1], value[2], value[3]);
}

glm::vec4 Shader::GetColor(const std::string& name) const
{
	return GetVector4(name);
}

void Shader::Unbind()
{
	glUseProgram(0);
}
