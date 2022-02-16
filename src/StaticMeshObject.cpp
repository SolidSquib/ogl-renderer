#include "StaticMeshObject.h"

StaticMeshObject::StaticMeshObject(std::shared_ptr<Mesh> mesh)
	: mMesh(mesh)
{
}

StaticMeshObject& StaticMeshObject::SetMesh(std::shared_ptr<Mesh> mesh)
{
	mMesh = mesh;
	return *this;
}

StaticMeshObject& StaticMeshObject::SetMaterial(std::shared_ptr<Material> material)
{
	mMaterial = material;
	return *this;
}

StaticMeshObject& StaticMeshObject::SetPosition(const glm::vec3& position)
{
	mPosition = position;
	return *this;
}

StaticMeshObject& StaticMeshObject::SetRotation(const glm::vec3& rotation)
{
	mRotation = rotation;
	return *this;
}

StaticMeshObject& StaticMeshObject::SetScale(const glm::vec3& scale)
{
	mScale = scale;
	return *this;
}

StaticMeshObject& StaticMeshObject::SetTransform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	return *this;
}

void StaticMeshObject::PreRender()
{
	if (mMesh.get())
	{
		mMesh->PreRender();
		mMaterial->Use();

		// Set transforms
		glm::mat4 meshTransform(1.0f);
		meshTransform = glm::translate(meshTransform, mPosition);
		// todo rotation
		meshTransform = glm::scale(meshTransform, mScale);
		mMaterial->SetMatrix4("model", meshTransform);
	}	
}

void StaticMeshObject::Render()
{
	if (mMesh.get())
	{
		mMesh->Render();
	}	
}
