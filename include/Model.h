#pragma once

#include "Mesh.h"
#include "Shader.h"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

class Model : public IRenderable
{
public:
	Model(std::shared_ptr<Mesh> mesh);
	Model(const char* loadPath);

	void SetShader(std::shared_ptr<Shader> shader);
	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);
	void SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	
	std::shared_ptr<Mesh> GetMesh(unsigned int index) const { return mMeshes.size() < index && index >= 0 ? mMeshes[index] : nullptr; }
	std::shared_ptr<Shader> GetShader() const { return mShader; }
	glm::vec3 GetPosition() const { return mPosition; }
	glm::vec3 GetRotation() const { return mRotation; }
	glm::vec3 GetScale() const { return mScale; }
	bool UsesTransparency() const;

	virtual void Render(Shader* shader);

private:
	std::vector<std::shared_ptr<Mesh>> mMeshes;
	std::shared_ptr<Shader> mShader;

	glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 mRotation = glm::vec3(0.0f, 0.0f, 0.0f); // roll, pitch, yaw
	glm::vec3 mScale = glm::vec3(1.0f, 1.0f, 1.0f);

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene, const std::string& directory);
	std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);
	std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* material, unsigned int type, const std::string& directory);
};