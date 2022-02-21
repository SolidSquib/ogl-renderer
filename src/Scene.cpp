#include "Scene.h"
#include <map>
#include <glm/gtx/norm.hpp>
#include <GLFW/glfw3.h>

Scene::Scene(const std::shared_ptr<Camera>& sceneCamera)
	: mSceneCamera(sceneCamera)
{
}

Scene::~Scene()
{
}

void Scene::Render(Shader* shader)
{
	glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// sort the scene 
	mSortedTransparentActors.clear();
	mOpaqueMeshes.clear();
	for (auto& actor : mSceneActors)
	{
		if (actor->UsesTransparency())
		{
			float distanceSqr = glm::length2(mSceneCamera->GetLocation() - actor->GetPosition());
			mSortedTransparentActors.emplace(distanceSqr, actor);
		}
		else
		{
			mOpaqueMeshes.push_back(actor);
		}
	}

	Shader* lastUsedShader = nullptr;
	
	// render 
	for (auto& model : mOpaqueMeshes)
	{
		if (Shader* useShader = model->GetShader() ? model->GetShader().get() : shader)
		{
			if (useShader != lastUsedShader)
			{
				PrepareShader(useShader);
				lastUsedShader = useShader;
			}

			PrepareModel(shader, model);
			model->Render(shader);
		}
	}

	for (auto actor_iter = mSortedTransparentActors.rbegin(); actor_iter != mSortedTransparentActors.rend(); ++actor_iter)
	{
		if (Shader* useShader = actor_iter->second->GetShader() ? actor_iter->second->GetShader().get() : shader)
		{
			if (useShader != lastUsedShader)
			{
				PrepareShader(useShader);
				lastUsedShader = useShader;
			}

			PrepareModel(shader, actor_iter->second);
			actor_iter->second->Render(shader);
		}
	}

	Shader::Unbind();
}

void Scene::AddActor(Model* model)
{
	mSceneActors.push_back(model);
}

void Scene::AddLight(Light* light)
{
	mSceneLights.push_back(light);
}

void Scene::PrepareLights(Shader* shader)
{
	int directionalLights = 0, pointLights = 0, spotLights = 0;
	// Setup lights for shader
	for (auto light : mSceneLights)
	{
		// set the lights in view space and bind to shader.
		switch (light->GetLightType())
		{
		case ELightType::Directional:
		{
			DirectionalLight lightCopy = *static_cast<DirectionalLight*>(light);
			lightCopy.SetDirection(mSceneCamera->GetViewMatrix() * glm::vec4(lightCopy.GetDirection(), 0.0f));
			shader->SetDirectionalLight("directionalLights", lightCopy, directionalLights++);
		}
		break;

		case ELightType::Point:
		{
			PointLight lightCopy = *static_cast<PointLight*>(light);
			lightCopy.SetPosition(mSceneCamera->GetViewMatrix() * vec4(lightCopy.GetPosition(), 1.0f));
			shader->SetPointLight("pointLights", lightCopy, pointLights++);
		}
		break;

		case ELightType::Spot:
		{
			SpotLight lightCopy = *static_cast<SpotLight*>(light);
			lightCopy.SetPosition(mSceneCamera->GetViewMatrix() * vec4(lightCopy.GetPosition(), 1.0f));
			lightCopy.SetDirection(mSceneCamera->GetViewMatrix() * glm::vec4(lightCopy.GetDirection(), 0.0f));
			shader->SetSpotLight("spotLights", lightCopy, spotLights++);
		}
		break;
		}
	}
}

void Scene::PrepareModel(Shader* shader, Model* model)
{
	shader->SetFloat("time", (float)glfwGetTime());
}

void Scene::PrepareShader(Shader* shader)
{	
	if (shader)
	{
		shader->Use();
		shader->SetMatrix4("view", mSceneCamera->GetViewMatrix());
		shader->SetMatrix4("projection", mSceneCamera->GetProjectionMatrix());

		PrepareLights(shader);
	}	
}
