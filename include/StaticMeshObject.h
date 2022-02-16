#pragma once

#include "Mesh.h"

class StaticMeshObject : public IRenderable
{
public:
	StaticMeshObject(std::shared_ptr<Mesh> mesh);

	StaticMeshObject& SetMesh(std::shared_ptr<Mesh> mesh);
	StaticMeshObject& SetMaterial(std::shared_ptr<Material> material);
	StaticMeshObject& SetPosition(const glm::vec3& position);
	StaticMeshObject& SetRotation(const glm::vec3& rotation);
	StaticMeshObject& SetScale(const glm::vec3& scale);
	StaticMeshObject& SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	
	std::shared_ptr<Mesh> GetMesh() const { return mMesh; }
	std::shared_ptr<Material> GetMaterial() const { return mMaterial; }
	glm::vec3 GetPosition() const { return mPosition; }
	glm::vec3 GetRotation() const { return mRotation; }
	glm::vec3 GetScale() const { return mScale; }

	virtual void PreRender();
	virtual void Render();

private:
	std::shared_ptr<Mesh> mMesh;
	std::shared_ptr<Material> mMaterial;
	glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 mRotation = glm::vec3(0.0f, 0.0f, 0.0f); // roll, pitch, yaw
	glm::vec3 mScale = glm::vec3(1.0f, 1.0f, 1.0f);
};