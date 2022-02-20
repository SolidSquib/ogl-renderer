#pragma once

#include "Model.h"
#include "Camera.h"

class Scene : public IRenderable
{
public:
	Scene(const std::shared_ptr<Camera>& sceneCamera);
	~Scene();

	std::shared_ptr<Camera> GetCamera() const { return mSceneCamera; }

	void Render(Shader* shader);
	void AddActor(Model* model);
	void AddLight(Light* light);

protected:
	void PrepareLights(Shader* shader);
	void PrepareModel(Shader* shader, Model* model);

private:
	std::vector<Model*> mSceneActors;
	std::vector<Light*> mSceneLights;
	std::shared_ptr<Camera> mSceneCamera;

	vec4 mClearColor = glm::vec4(0.1f, 0.1f, 0.2f, 1.0f);
};