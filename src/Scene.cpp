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

	// sort the scene 
	std::map<float, Model*> sortedActorArray;
	for (auto& actor : mSceneActors)
	{
		float distanceSqr = glm::length2(mSceneCamera->GetLocation() - actor->GetPosition());
		sortedActorArray[distanceSqr] = actor;
	}

	Shader* lastUsedShader = nullptr;
	// render 
	for (auto actor_iter = sortedActorArray.rbegin(); actor_iter != sortedActorArray.rend(); ++actor_iter)
	{
		Shader* useShader = actor_iter->second->GetShader() ? actor_iter->second->GetShader().get() : shader;
		if (useShader)
		{
			/* we don't need to set certain uniforms again if we're using the same shader as the last 
			 * draw call, so we can save a little processing. */
			if (useShader != lastUsedShader)
			{
				useShader->Use();
				useShader->SetMatrix4("view", mSceneCamera->GetViewMatrix());
				useShader->SetMatrix4("projection", mSceneCamera->GetProjectionMatrix());

				PrepareLights(useShader);
				lastUsedShader = useShader;
			}		

			PrepareModel(useShader, actor_iter->second);
			actor_iter->second->Render(actor_iter->second->GetShader() ? nullptr : shader);
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
