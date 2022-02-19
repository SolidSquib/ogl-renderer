#include "Model.h"
#include "managers/TextureManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(std::shared_ptr<Mesh> mesh)
{
	mMeshes.push_back(mesh);
}

Model::Model(const char* loadPath)
{
	LoadModel(loadPath);
}

Model& Model::SetShader(std::shared_ptr<Shader> shader)
{
	mShader = shader;
	return *this;
}

Model& Model::SetPosition(const glm::vec3& position)
{
	mPosition = position;
	return *this;
}

Model& Model::SetRotation(const glm::vec3& rotation)
{
	mRotation = rotation;
	return *this;
}

Model& Model::SetScale(const glm::vec3& scale)
{
	mScale = scale;
	return *this;
}

Model& Model::SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	return *this;
}

void Model::Render(Shader& shader)
{
	mShader->Use();
	for (auto& mesh : mMeshes)
	{
		if (mesh.get())
		{
			// Set transforms
			glm::mat4 meshTransform(1.0f);
			meshTransform = glm::translate(meshTransform, mPosition);
			meshTransform = glm::scale(meshTransform, mScale);
			meshTransform = glm::rotate(meshTransform, mRotation.x, glm::vec3(0.0f, 0.0f, -1.0f)); // roll
			meshTransform = glm::rotate(meshTransform, mRotation.y, glm::vec3(1.0f, 0.0f, 0.0f));  // pitch
			meshTransform = glm::rotate(meshTransform, mRotation.z, glm::vec3(0.0f, 1.0f, 0.0f));  // yaw
			mShader->SetMatrix4("model", meshTransform);

			mesh->Render(shader);
		}
	}
		
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;
	unsigned int attributeFlags = 0;
	Material mat;
		
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;

		if (mesh->HasPositions())
		{
			attributeFlags |= EVA_POSITION;
			const aiVector3D& location = mesh->mVertices[i];
			vertex.pos = glm::vec3(location.x, location.y, location.z);
		}
		
		if (mesh->HasVertexColors(0))
		{
			attributeFlags |= EVA_COLOR;
			const aiVector3D& color = mesh->mNormals[i];
			vertex.col = glm::vec3(color.x, color.y, color.z);
		}

		if (mesh->HasNormals())
		{
			attributeFlags |= EVA_NORMAL;
			const aiVector3D& normal = mesh->mNormals[i];
			vertex.norm = glm::vec3(normal.x, normal.y, normal.z);
		}		

		if (mesh->HasTextureCoords(0))
		{
			attributeFlags |= EVA_UV;
			const aiVector3D& uv = mesh->mTextureCoords[0][i];
			vertex.uv = glm::vec2(uv.x, uv.y);
		}
		else
		{
			vertex.uv = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		auto diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
		mat.textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		auto specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
		mat.textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		auto emissiveMaps = LoadMaterialTextures(material, aiTextureType_EMISSIVE);
		mat.textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
	}

	std::shared_ptr<Mesh> generatedMesh(new Mesh(vertices, indices, attributeFlags));

	return generatedMesh;
}

std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures(aiMaterial* material, unsigned int type)
{
    aiTextureType eTextureType = (aiTextureType)type;
    
	std::vector<std::shared_ptr<Texture>> loadedTextures;
	for (unsigned int i = 0; i < material->GetTextureCount(eTextureType); ++i)
	{
		aiString texturePath;
		material->GetTexture(eTextureType, i, &texturePath);
		
		if (std::shared_ptr<Texture> texture = TextureManager::Get().RequestTexture(texturePath.C_Str()))
		{
			switch (type)
			{
			case aiTextureType_DIFFUSE:
				TextureManager::Get().SetTextureType(texture, Texture::TEX_DIFFUSE);
				break;

			case aiTextureType_SPECULAR:
				TextureManager::Get().SetTextureType(texture, Texture::TEX_SPECULAR);
				break;

			case aiTextureType_NORMALS:
				TextureManager::Get().SetTextureType(texture, Texture::TEX_NORMAL);
				break;

			case aiTextureType_EMISSIVE:
				TextureManager::Get().SetTextureType(texture, Texture::TEX_EMISSION);
				break;
            default:
                break;
            }

			loadedTextures.push_back(texture);
		}		
	}

	return loadedTextures;
}
