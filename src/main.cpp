#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"

void framebuffer_size_changed_callback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void ProcessInput(GLFWwindow* window);

const std::vector<Vertex> cube_vertices = {
	{ {0.5f, 0.5f, 0.5f}, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
	{ {0.5f, -0.5f, 0.5f}, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
	{ {-0.5f, -0.5f, 0.5f}, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {-0.5f, 0.5f, 0.5f}, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },

	{ {0.5f, 0.5f, -0.5f}, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
	{ {0.5f, -0.5f, -0.5f}, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {-0.5f, -0.5f, -0.5f}, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
	{ {-0.5f, 0.5f, -0.5f}, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

	{ {0.5f, 0.5f, 0.5f}, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
	{ {-0.5f, 0.5f, 0.5f}, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
	{ {-0.5f, 0.5f, -0.5f}, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {0.5f, 0.5f, -0.5f}, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },

	{ {0.5f, -0.5f, 0.5f}, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
	{ {-0.5f, -0.5f, 0.5f}, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
	{ {-0.5f, -0.5f, -0.5f}, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
	{ {0.5f, -0.5f, -0.5f}, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }
};

const std::vector<unsigned int> cube_indices = {
	0, 1, 3,	// front face
	1, 2, 3,

	3, 2, 7,	// left face
	2, 6, 7,

	7, 6, 4,	// back face
	6, 5, 4,

	4, 5, 0,	// right face
	5, 1, 0,

	8, 9, 11, // top face
	9, 10, 11,

	12, 13, 15,	// bottom face
	13, 14, 15
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

	// Set up the models
	Texture containerTexture("content/textures/container.jpg", false);
	containerTexture.SetTextureSamplingMode(GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);

	Texture smilingFace("content/textures/awesomeface.png", true, GL_RGBA);
	smilingFace.SetTextureSamplingMode(GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);
		
	Shader textureShader("shaders/simple-texture.vert", "shaders/simple-texture.frag");
	textureShader.Use();
	textureShader.SetInt("texture0", 0);
	textureShader.SetInt("texture1", 1);
	textureShader.SetFloat("blend_alpha", 0.2f);

	Material mat(textureShader, { {0, containerTexture}, {1, smilingFace} });
	Mesh cube(cube_vertices, cube_indices, mat);

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
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		textureShader.SetMatrix4("view", mainCamera.GetViewMatrix());
		textureShader.SetMatrix4("projection", projection);
		
		cube.PreRender();

		for (unsigned int i = 0; i < (sizeof(cubePositions) / sizeof(cubePositions[0])); ++i)
		{
			glm::vec3 rotationAxis = i % 3 == 0
				? -glm::vec3(0.5f, 1.0f, 0.0f)
				: glm::vec3(0.5f, 1.0f, 0.0f);

			glm::mat4 modelTransform(1.0f);
			modelTransform = glm::translate(modelTransform, cubePositions[i]);
			float angle = glm::radians(20.0f * (float)i);
			modelTransform = glm::rotate(modelTransform, angle + ((float)glfwGetTime() * glm::radians(50.0f)), rotationAxis);
			textureShader.SetMatrix4("model", modelTransform);

			cube.Render();
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