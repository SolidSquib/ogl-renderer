#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

#include "Shader.h"
#include "managers/TextureManager.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "Scene.h"

void framebuffer_size_changed_callback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void ProcessInput(GLFWwindow* window);

const std::vector<Vertex> cube_vertices = {
	// back face
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 1.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 0.0f, 0.0f } },

	// front face
	{ { -0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 1.0f, 0.0f } },
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 0.0f, 0.0f } },
	 
	// left face
	{ { -0.5f, 0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 0.0f, 0.0f } },
	{ { -0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 1.0f, 0.0f } },
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 0.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 0.0f, 0.0f } },
	 
	// right face
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 0.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 1.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 0.0f, 1.0f } },
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 0.0f, 0.0f } },
	
	// bottom face
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 1.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 0.0f, 0.0f } },
	
	// top face
	{ { -0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 0.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 1.0f, 0.0f } },
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 0.0f, 1.0f } },
	{ { -0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 0.0f, 0.0f } }
};

const std::vector<unsigned int> cube_indices = {
	// back face
	0, 2, 1, 3, 5, 4,
	// front face
	6, 7, 8, 9, 10, 11,
	// left face
	12, 13, 14, 15, 16, 17,
	// right face
	18, 20, 19, 21, 23, 22, 
	// bottom face
	24, 25, 26, 27, 28, 29,
	// top face
	30, 32, 31, 33, 35, 34
};

float planeVertices[] = {
	// positions          // texture Coords 
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,	
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,	
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f,
	 -5.0f, -0.5f, -5.0f,  0.0f, 2.0f
};

float transparentVertices[] = {
	// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

Scene* gActiveScene = nullptr;

float fovScrollSpeed = 5.0f;
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;
float mouseX = 0.0f, mouseY = 0.0f;
bool firstMouse = true;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef CE_PLATFORM_OSX
	// Required for OSX
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "A Wonderful Window", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
		
	glfwSetFramebufferSizeCallback(window, framebuffer_size_changed_callback);
	framebuffer_size_changed_callback(window, 800, 600);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	// textures
	std::shared_ptr<Texture> containerDiffuse = TextureManager::Get().RequestTexture("content/textures/container2.png");
	TextureManager::Get().SetTextureType(containerDiffuse, Texture::TEX_DIFFUSE);

	std::shared_ptr<Texture> containerSpecular = TextureManager::Get().RequestTexture("content/textures/container2_specular.png");
	TextureManager::Get().SetTextureType(containerSpecular, Texture::TEX_SPECULAR);

	std::shared_ptr<Texture> matrix = TextureManager::Get().RequestTexture("content/textures/matrix.jpg");
	TextureManager::Get().SetTextureType(matrix, Texture::TEX_EMISSION);

	std::shared_ptr<Texture> floorDiffuse = TextureManager::Get().RequestTexture("content/textures/wall.jpg");
	TextureManager::Get().SetTextureType(floorDiffuse, Texture::TEX_DIFFUSE);

	std::shared_ptr<Texture> windowDiffuse = TextureManager::Get().RequestTexture("content/textures/blending_transparent_window.png");
	TextureManager::Get().SetTextureType(windowDiffuse, Texture::TEX_DIFFUSE);

	// shaders
	std::shared_ptr<Shader> lightShader(new Shader("shaders/simple-position-color.vert", "shaders/uniform-color-unlit.frag"));
	std::shared_ptr<Shader> colorShader(new Shader("shaders/simple-position-phong.vert", "shaders/uniform-color-phong.frag"));
	std::shared_ptr<Shader> outlineShader(new Shader("shaders/simple-position-phong.vert", "shaders/outline.frag"));

	// materials 
	Material containerMaterial;
	containerMaterial.diffuseColor = glm::vec3(1.0f, 0.5f, 0.31f);
	containerMaterial.textures = { containerDiffuse, containerSpecular, matrix };
	containerMaterial.shininess = 128.0f * 0.6f;

	Material floorMaterial;
	floorMaterial.textures = { floorDiffuse };

	Material windowMaterial;
	windowMaterial.textures = { windowDiffuse };
	windowMaterial.usesTransparency = true;

	// meshes
	std::shared_ptr<Mesh> cube(new Mesh(cube_vertices, cube_indices, EVA_POSITION | EVA_NORMAL | EVA_UV));
	cube->SetMaterial(containerMaterial);

	std::shared_ptr<Mesh> plane(new Mesh(planeVertices, 6, EVA_POSITION | EVA_UV));
	plane->SetMaterial(floorMaterial);

	std::shared_ptr<Mesh> windowMesh(new Mesh(transparentVertices, 6, EVA_POSITION | EVA_UV));
	windowMesh->SetMaterial(windowMaterial);

	// lights
	PointLight redLight(
		glm::vec3(-1.0f, 1.0f, -0.8f),
		glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f,
		glm::vec3(1.0f, 0.5f, 0.31f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	);

	DirectionalLight directionalLight(
		glm::normalize(glm::vec3(-1.0f, -0.5f, -1.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f,
		glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);	

	PointLight greenLight(
		glm::vec3(1.0f, 1.0f, -1.3f),
		glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f,
		glm::vec3(0.0f, 1.0f, 0.21f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	);

	SpotLight spotLight(
		glm::vec3(0.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f,
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
	spotLight.SetLinearComponent(0.045f);
	spotLight.SetQuadraticComponent(0.0075f);

	// objects
	Model container(cube);
	container.SetPosition(glm::vec3(-1.0f, 0.0f, -1.0f));
	Model container2(cube);
	container2.SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));

	Model bag("content/backpack/backpack.obj");

	Model floorPlane(plane);

	Model windowModel(windowMesh);
	windowModel.SetPosition(glm::vec3(-1.5f, 0.0f, -0.48f));
	Model windowModel2(windowMesh);
	windowModel2.SetPosition(glm::vec3(1.5f, 0.0f, 0.51f));
	Model windowModel3(windowMesh);
	windowModel3.SetPosition(glm::vec3(0.0f, 0.0f, 0.7f));
	Model windowModel4(windowMesh);
	windowModel4.SetPosition(glm::vec3(-0.3f, 0.0f, -2.3f));
	Model windowModel5(windowMesh);
	windowModel5.SetPosition(glm::vec3(0.5f, 0.0f, -0.6f));

	Scene bagScene(std::shared_ptr<Camera>(new Camera(glm::vec3(0.0f, 0.0f, 3.0f))));
	bagScene.AddActor(&bag);
	bagScene.AddLight(&directionalLight);

	Scene boxScene(std::shared_ptr<Camera>(new Camera(glm::vec3(0.0f, 0.0f, 3.0f))));
	boxScene.AddActor(&container);
	boxScene.AddActor(&container2);
	boxScene.AddActor(&floorPlane);
	boxScene.AddActor(&windowModel);
	boxScene.AddActor(&windowModel2);
	boxScene.AddActor(&windowModel3);
	boxScene.AddActor(&windowModel4);
	boxScene.AddActor(&windowModel5);
	boxScene.AddLight(&directionalLight);
	boxScene.AddLight(&redLight);
	boxScene.AddLight(&spotLight);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	gActiveScene = &boxScene;
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	framebuffer_size_changed_callback(window, width, height);

	// start the main loop
	while (!glfwWindowShouldClose(window))
	{
		float time = (float)glfwGetTime();
		deltaTime = time - lastFrameTime;
		lastFrameTime = time;

		// handle input
		ProcessInput(window);

		// render 
		if (gActiveScene)
		{
			gActiveScene->Render(colorShader.get());
		}

		// glfw events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// release resources
	glfwTerminate();
	return 0;
}

void framebuffer_size_changed_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	
	if (gActiveScene)
	{
		auto camera = gActiveScene->GetCamera();
		if (camera->IsOrthographic())
		{
			camera->SetOrtho((float)width, (float)height, camera->GetNearClip(), camera->GetFarClip());
		}
		else
		{
			camera->SetPerspective((float)width, (float)height, camera->GetFovDegrees(), camera->GetNearClip(), camera->GetFarClip());
		}
	}	
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		mouseX = (float)xPos;
		mouseY = (float)yPos;
		firstMouse = false;
	}

	float deltaX = (float)xPos - mouseX;
	float deltaY = mouseY - (float)yPos;
	mouseX = (float)xPos;
	mouseY = (float)yPos;

	if (gActiveScene)
	{
		auto camera = gActiveScene->GetCamera();
		camera->ProcessLookInput(deltaX, deltaY);
	}
}

/// <summary>
/// Handle input from the mouse scroll wheel
/// </summary>
/// <param name="window"></param>
/// <param name="xOffset"></param>
/// <param name="yOffset"></param>
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	if (gActiveScene)
	{
		auto camera = gActiveScene->GetCamera();
		float cameraPanSpeed = camera->GetPanSpeed() + (float)yOffset;
		if (cameraPanSpeed < 1.0f)
		{
			cameraPanSpeed = 1.0f;
		}
		else if (cameraPanSpeed > 50.0f)
		{
			cameraPanSpeed = 50.0f;
		}
		camera->SetPanSpeed(cameraPanSpeed);
	}
}

/// <summary>
/// Handle kayboard input captures by GLFW
/// </summary>
/// <param name="window"></param>
void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// set orthographic projection
	else if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		if (gActiveScene)
		{
			auto camera = gActiveScene->GetCamera();
			if (!camera->IsOrthographic())
			{
				camera->SetOrtho(camera->GetViewWidth(), camera->GetViewHeight(),
					camera->GetNearClip(), camera->GetFarClip());
			}
		}
	}
	// set perspective projection
	else if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
	{
		if (gActiveScene)
		{
			auto camera = gActiveScene->GetCamera();
			if (camera->IsOrthographic())
			{
				camera->SetPerspective(camera->GetViewWidth(), camera->GetViewHeight(), camera->GetFovDegrees(),
					camera->GetNearClip(), camera->GetFarClip());
			}
		}
	}

	// pan camera position
	glm::vec3 movement(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		movement.z = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		movement.z = -1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		movement.x = -1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		movement.x = 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		movement.y = -1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		movement.y = 1.0f;
	}

	if (gActiveScene)
	{
		auto camera = gActiveScene->GetCamera();
		camera->ProcessMovementInput(movement, deltaTime);
	}	

	// FoV
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
	{
		if (gActiveScene)
		{
			auto camera = gActiveScene->GetCamera();
			camera->SetFovDegrees(camera->GetFovDegrees() - fovScrollSpeed * deltaTime);
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
	{
		if (gActiveScene)
		{
			auto camera = gActiveScene->GetCamera();
			camera->SetFovDegrees(camera->GetFovDegrees() + fovScrollSpeed * deltaTime);
		}
	}
}
