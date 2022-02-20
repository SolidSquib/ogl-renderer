#include "Mesh.h"
#include "Shader.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int vertexAttributes)
	: mVertices(vertices),
	mIndices(indices),
	mVertexAttributes(vertexAttributes),
	mUseEBO(true)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	SetupMesh();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, unsigned int vertexAttributes)
	: mVertices(vertices),
	mIndices(),
	mVertexAttributes(vertexAttributes),
	mUseEBO(false)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	SetupMesh();
}

Mesh::Mesh(const float* vertices, unsigned int numVertices, const std::vector<unsigned int>& indices, unsigned int vertexAttributes)
	: mVertices(ProcessFloatArray(vertices, numVertices, vertexAttributes)),
	mIndices(indices),
	mVertexAttributes(vertexAttributes),
	mUseEBO(true)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	SetupMesh();
}

Mesh::Mesh(const float* vertices, unsigned int numVertices, unsigned int vertexAttributes)
	: mVertices(ProcessFloatArray(vertices, numVertices, vertexAttributes)),
	mIndices(),
	mVertexAttributes(vertexAttributes),
	mUseEBO(false)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	SetupMesh();
}

/* When copying a mesh, we should also create new buffers so that the destructor doesn't
 * destroy the buffer objects for both */
Mesh::Mesh(const Mesh& other)
	: Mesh(other.mVertices, other.mIndices, other.mVertexAttributes)
{
	mDefaultMaterial = other.mDefaultMaterial;
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	VAO = VBO = EBO = 0;
}

std::shared_ptr<Mesh> Mesh::Copy()
{
	return std::shared_ptr<Mesh>(new Mesh(*this));
}

void Mesh::Render(Shader* shader)
{
	if (shader)
	{
		shader->SetMaterial("material", mDefaultMaterial);

		glBindVertexArray(VAO);
		if (mUseEBO)
		{
			glDrawElements(GL_TRIANGLES, static_cast<int>(mIndices.size()), GL_UNSIGNED_INT, (void*)NULL);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
		}
		glBindVertexArray(0);
	}
}

void Mesh::SetupMesh()
{
	glBindVertexArray(VAO);	

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

	if ((mVertexAttributes & EVA_POSITION) == EVA_POSITION)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
		glEnableVertexAttribArray(0);
	}	

	if ((mVertexAttributes & EVA_COLOR) == EVA_COLOR)
	{
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(1);
	}

	if ((mVertexAttributes & EVA_NORMAL) == EVA_NORMAL)
	{
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(2);
	}

	if ((mVertexAttributes & EVA_UV) == EVA_UV)
	{
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(glm::vec3)));
		glEnableVertexAttribArray(3);	
	}

	if (mUseEBO)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

std::vector<Vertex> Mesh::ProcessFloatArray(const float* vertices, unsigned int numVertices, unsigned int vertexAttributes)
{
	std::vector<Vertex> processedVerts;

	int currentIndex = 0;
	for (unsigned int i = 0; i < numVertices; ++i)
	{
		Vertex vert;

		if ((vertexAttributes & EVA_POSITION) == EVA_POSITION)
		{
			vert.pos = glm::vec3(vertices[currentIndex++], vertices[currentIndex++], vertices[currentIndex++]);
		}

		if ((vertexAttributes & EVA_COLOR) == EVA_COLOR)
		{
			vert.col = glm::vec3(vertices[currentIndex++], vertices[currentIndex++], vertices[currentIndex++]);
		}

		if ((vertexAttributes & EVA_NORMAL) == EVA_NORMAL)
		{
			vert.norm = glm::vec3(vertices[currentIndex++], vertices[currentIndex++], vertices[currentIndex++]);
		}

		if ((vertexAttributes & EVA_UV) == EVA_UV)
		{
			vert.uv = glm::vec2(vertices[currentIndex++], vertices[currentIndex++]);
		}

		processedVerts.push_back(vert);
	}

	return processedVerts;
}
