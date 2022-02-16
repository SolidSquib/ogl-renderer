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

StaticMeshObject& StaticMeshObject::SetShader(std::shared_ptr<Shader> shader)
{
	mShader = shader;
	return *this;
}

StaticMeshObject& StaticMeshObject::SetMaterial(const Material& material)
{
	mMaterial = material;
	return *this;
}

StaticMeshObject& StaticMeshObject::SetAmbientColor(const glm::vec3& ambientColor)
{
	mMaterial.ambientColor = ambientColor;
	return *this;
}

StaticMeshObject& StaticMeshObject::SetDiffuseColor(const glm::vec3& diffuseColor)
{
	mMaterial.diffuseColor = diffuseColor;
	return *this;
}

StaticMeshObject& StaticMeshObject::SetSpecularColor(const glm::vec3& specularColor)
{
	mMaterial.specularColor = specularColor;
	return *this;
}

StaticMeshObject& StaticMeshObject::SetShininess(const float& shininess)
{
	mMaterial.shininess = shininess;
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
		mShader->Use();

		// Set transforms
		glm::mat4 meshTransform(1.0f);
		meshTransform = glm::translate(meshTransform, mPosition);
		meshTransform = glm::scale(meshTransform, mScale);

		mShader->SetMatrix4("model", meshTransform);
		mShader->SetMaterial("material", mMaterial);
	}	
}

void StaticMeshObject::Render()
{
	if (mMesh.get())
	{
		mMesh->Render();
	}	
}
