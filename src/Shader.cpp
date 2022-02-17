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

void Shader::SetTexture(std::shared_ptr<Texture> texture, unsigned int index)
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

	unsigned int useTexture = GL_TEXTURE0 + index;
	glActiveTexture(useTexture);
	glBindTexture(GL_TEXTURE_2D, texture.get() ? texture->GetID() : 0);
}

void Shader::SetMaterial(const std::string& name, const Material& material)
{
	SetTexture(material.diffuseMap, 0);
	SetTexture(material.specularMap, 1);
	SetTexture(material.emissionMap, 2);

	SetInt(name + ".diffuseMap", 0);
	SetInt(name + ".specularMap", 1);
	SetInt(name + ".emissionMap", 2);
	SetVector3(name + ".ambient", glm::vec3(material.ambientColor));
	SetVector3(name + ".diffuse", glm::vec3(material.diffuseColor));
	SetVector3(name + ".specular", glm::vec3(material.specularColor));
	SetFloat(name + ".shininess", material.shininess);
}

void Shader::SetDirectionalLight(const std::string& name, const DirectionalLight& light, unsigned int lightIndex)
{
	// only support for one dir light currently
	SetVector3(name + ".direction", light.direction);
	SetVector3(name + ".ambient", glm::vec3(light.ambient));
	SetVector3(name + ".diffuse", glm::vec3(light.diffuse));
	SetVector3(name + ".specular", glm::vec3(light.specular));
}

void Shader::SetPointLight(const std::string& name, const PointLight& light, unsigned int lightIndex)
{
	char buffer[512];
	const char* formatString = "%s[%u].%s";
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "position");	
	SetVector3(buffer, light.position);
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "ambient");
	SetVector3(buffer, glm::vec3(light.ambient));
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "diffuse");
	SetVector3(buffer, glm::vec3(light.diffuse));
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "specular");
	SetVector3(buffer, glm::vec3(light.specular));
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "constant");
	SetFloat(buffer, light.constant);
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "linear");
	SetFloat(buffer, light.linear);
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "quadratic");
	SetFloat(buffer, light.quadratic);
}

void Shader::SetSpotLight(const std::string& name, const SpotLight& light, unsigned int lightIndex)
{
	char buffer[512];
	const char* formatString = "%s[%u].%s";
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "position");
	SetVector3(buffer, light.position);
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "direction");
	SetVector3(buffer, light.direction);
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "ambient");
	SetVector3(buffer, glm::vec3(light.ambient));
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "diffuse");
	SetVector3(buffer, glm::vec3(light.diffuse));
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "specular");
	SetVector3(buffer, glm::vec3(light.specular));
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "constant");
	SetFloat(buffer, light.constant);
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "linear");
	SetFloat(buffer, light.linear);
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "quadratic");
	SetFloat(buffer, light.quadratic);
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "innerCutoff");
	SetFloat(buffer, light.innerCutoff);
	sprintf_s(buffer, formatString, name.c_str(), lightIndex, "outerCutoff");
	SetFloat(buffer, light.outerCutoff);
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

int Shader::GetTextureMapIndex(unsigned int index)
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

void Shader::Unbind()
{
	glUseProgram(0);
}