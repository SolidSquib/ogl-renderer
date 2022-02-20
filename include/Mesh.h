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
	Mesh(const std::vector<Vertex>& vertices, unsigned int vertexAttributes);
	Mesh(const float* vertices, unsigned int numVertices, const std::vector<unsigned int>& indices, unsigned int vertexAttributes);
	Mesh(const float* vertices, unsigned int numVertices, unsigned int vertexAttributes);
	~Mesh();

	// only copy explicitly
	std::shared_ptr<Mesh> Copy();

	void Render(class Shader* shader);

	Material GetMaterial() const { return mDefaultMaterial; }
	Material& GetMaterialRef() { return mDefaultMaterial; }

	void SetMaterial(const Material& material) { mDefaultMaterial = material; }

private:
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	unsigned int mVertexAttributes;
	bool mUseEBO = true;

	Material mDefaultMaterial;

	unsigned int VAO, VBO, EBO;

	Mesh(const Mesh& other); // copy constructor
	void SetupMesh();
	std::vector<Vertex> ProcessFloatArray(const float* vertices, unsigned int numVertices, unsigned int vertexAttributes);
};