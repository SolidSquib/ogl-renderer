#pragma once

#include "IRenderable.h"
#include "Material.h"

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 col;
	glm::vec3 norm;
	glm::vec2 uv;
};

enum EVertexAttributes
{
	EVA_POSITION = 0x1,
	EVA_COLOR = 0x2,
	EVA_NORMAL = 0x4,
	EVA_UV = 0x8
};

class Mesh : IRenderable
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int vertexAttributes);
	~Mesh();

	void PreRender();
	void Render();

private:	
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	unsigned int mVertexAttributes;

	unsigned int VAO, VBO, EBO;

	void SetupMesh();
};