#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

#include "Shader.h"
#include "managers/TextureManager.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"

void framebuffer_size_changed_callback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void ProcessInput(GLFWwindow* window);

const std::vector<Vertex> cube_vertices = {
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 1.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, -1.0f },		{ 0.0f, 0.0f } },

	{ { -0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 1.0f, 0.0f } },
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 0.0f, 1.0f },		{ 0.0f, 0.0f } },
	 
	{ { -0.5f, 0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 0.0f, 0.0f } },
	{ { -0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 1.0f, 0.0f } },
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 0.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ -1.0f, 0.0f, 0.0f },		{ 0.0f, 0.0f } },
	 
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 0.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 1.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 0.0f, 1.0f } },
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 1.0f, 0.0f, 0.0f },		{ 0.0f, 0.0f } },
	
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 1.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 0.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, -1.0f, 0.0f },		{ 0.0f, 0.0f } },
	
	{ { -0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 0.0f, 0.0f } },
	{ { 0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 1.0f, 0.0f } },
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, 0.5f },		{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 0.0f, 1.0f } },
	{ { -0.5f, 0.5f, -0.5f },	{ 1.0f, 1.0f, 1.0f },	{ 0.0f, 1.0f, 0.0f },		{ 0.0f, 0.0f } }
};

const std::vector<unsigned int> cube_indices = {
	0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11,
	12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23, 
	24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35
};

const glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
bool useOrthographicProjection = false;
glm::mat4 projection(1.0);
float fovDegrees = 45.0f;
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
	std::shared_ptr<Texture> containerDiffuse = TextureManager::Get().RequestTexture("../content/textures/container2.png");
	TextureManager::Get().SetTextureType(containerDiffuse, Texture::TEX_DIFFUSE);

	std::shared_ptr<Texture> containerSpecular = TextureManager::Get().RequestTexture("../content/textures/container2_specular.png");
	TextureManager::Get().SetTextureType(containerSpecular, Texture::TEX_SPECULAR);

	std::shared_ptr<Texture> matrix = TextureManager::Get().RequestTexture("../content/textures/matrix.jpg");
	TextureManager::Get().SetTextureType(matrix, Texture::TEX_EMISSION);

	// shaders
	std::shared_ptr<Shader> lightShader(new Shader("../shaders/simple-position-color.vert", "../shaders/uniform-color-unlit.frag"));
	std::shared_ptr<Shader> colorShader(new Shader("../shaders/simple-position-phong.vert", "../shaders/uniform-color-phong.frag"));

	// materials 
	Material containerMaterial;
	containerMaterial.diffuseColor = glm::vec3(1.0f, 0.5f, 0.31f);
	containerMaterial.textures = { containerDiffuse, containerSpecular, matrix };
	containerMaterial.shininess = 128.0f * 0.6f;

	// meshes
	std::shared_ptr<Mesh> cube(new Mesh(cube_vertices, cube_indices, EVA_POSITION | EVA_NORMAL | EVA_UV));
	cube->SetMaterial(containerMaterial);

	// lights
	PointLight pointLight;
	pointLight.position = glm::vec3(-1.0f, 1.0f, -0.8f);
	pointLight.ambient = glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f;
	pointLight.diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	pointLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight.constant = 1.0f;
	pointLight.linear = 0.09f;
	pointLight.quadratic = 0.032f;

	DirectionalLight directionalLight;
	directionalLight.direction = glm::normalize(glm::vec3(-1.0f, -0.5f, -1.0f));
	directionalLight.ambient = glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f;
	directionalLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f;
	directionalLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

	PointLight greenLight;
	greenLight.position = glm::vec3(1.0f, 1.0f, -1.3f);
	greenLight.ambient = glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f;
	greenLight.diffuse = glm::vec3(0.0f, 1.0f, 0.21f);
	greenLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	greenLight.constant = 1.0f;
	greenLight.linear = 0.09f;
	greenLight.quadratic = 0.032f;

	SpotLight spotLight;
	spotLight.position = glm::vec3(1.0f, 1.0f, -1.3f);
	spotLight.ambient = glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f;
	spotLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLight.constant = 1.0f;
	spotLight.linear = 0.045f;
	spotLight.quadratic = 0.0075f;
	spotLight.innerCutoff = glm::cos(glm::radians(12.5f));
	spotLight.outerCutoff = glm::cos(glm::radians(17.5f));

	// objects
	Model container(cube);
	container.SetShader(colorShader);

	Model bag("../content/backpack/backpack.obj");
	bag.SetShader(colorShader);

	std::vector<Model> sceneMeshes = { bag };
		
	std::vector<PointLight*> scenePointLights = {
		&pointLight,
		&greenLight
	};

	glEnable(GL_DEPTH_TEST);

	// start the main loop
	while (!glfwWindowShouldClose(window))
	{
		float time = (float)glfwGetTime();
		deltaTime = time - lastFrameTime;
		lastFrameTime = time;

		// handle input
		ProcessInput(window);

		// render 
		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
		for (auto& mesh : sceneMeshes)
		{
			colorShader->SetMatrix4("view", mainCamera.GetViewMatrix());
			colorShader->SetMatrix4("projection", projection);

			// lights
			DirectionalLight lightCopy = directionalLight;
			lightCopy.direction = mainCamera.GetViewMatrix() * glm::vec4(directionalLight.direction, 0.0f);
			colorShader->SetDirectionalLight("directionalLight", lightCopy, 0);

			SpotLight spotLightCopy = spotLight;
			spotLightCopy.direction = glm::vec3(0.0f, 0.0f, -1.0f);
			spotLightCopy.position = glm::vec3(0.0f, 0.0f, 0.0f);
			colorShader->SetSpotLight("spotLights", spotLightCopy, 0);

			for (int i = 0; i < scenePointLights.size(); ++i)
			{
				PointLight pointCopy = *scenePointLights[i];
				pointCopy.position = mainCamera.GetViewMatrix() * glm::vec4(pointCopy.position, 1.0f);
				colorShader->SetPointLight("pointLights", pointCopy, i);
			}

			colorShader->SetFloat("time", (float)glfwGetTime());
			mesh.Render(*colorShader);
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
	projection = useOrthographicProjection
		? glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f)
		: glm::perspective(glm::radians(fovDegrees), (float)width / (float)height, 0.1f, 100.0f);
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

	mainCamera.ProcessLookInput(deltaX, deltaY);
}

/// <summary>
/// Handle input from the mouse scroll wheel
/// </summary>
/// <param name="window"></param>
/// <param name="xOffset"></param>
/// <param name="yOffset"></param>
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	float cameraPanSpeed = mainCamera.GetPanSpeed() + (float)yOffset;
	if (cameraPanSpeed < 1.0f)
	{
		cameraPanSpeed = 1.0f;
	}
	else if (cameraPanSpeed > 50.0f)
	{
		cameraPanSpeed = 50.0f;
	}
	mainCamera.SetPanSpeed(cameraPanSpeed);
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
		if (!useOrthographicProjection)
		{
			useOrthographicProjection = true;
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			framebuffer_size_changed_callback(window, width, height);
		}
	}
	// set perspective projection
	else if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
	{
		if (useOrthographicProjection)
		{
			useOrthographicProjection = false;
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			framebuffer_size_changed_callback(window, width, height);
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
	mainCamera.ProcessMovementInput(movement, deltaTime);

	// FoV
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
	{
		fovDegrees -= fovScrollSpeed * deltaTime;
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		framebuffer_size_changed_callback(window, width, height);
	}
	else if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
	{
		fovDegrees += fovScrollSpeed * deltaTime;
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		framebuffer_size_changed_callback(window, width, height);
	}
}