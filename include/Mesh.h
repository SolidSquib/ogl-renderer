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
	glm::vec2 uv;
};

class Mesh : IRenderable
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();

	void PreRender();
	void Render();

private:	
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;

	unsigned int VAO, VBO, EBO;

	void SetupMesh();
};