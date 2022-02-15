#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

#include "Shader.h"
#include "Texture.h"

void framebuffer_size_changed_callback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void ProcessInput(GLFWwindow* window);

const float meshData[] = {
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f,

	0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,

	0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f,

	0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f,
};

const int meshIndices[] = {
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

bool useOrthographicProjection = false;
glm::mat4 projection(1.0);
glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
glm::vec3 cameraDirection(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
float cameraPanSpeed = 2.0f;
float fovDegrees = 45.0f;
float fovScrollSpeed = 5.0f;
float deltaTime = 0.0f;
float lastFrameTime = 0.0f;
float mouseX = 0.0f, mouseY = 0.0f;
float cameraPitch = 0.0f, cameraYaw = -90.0f;
bool firstMouse = true;
float mouseSensitivity = 0.1f;

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

	Texture containerTexture("content/textures/container.jpg", false);
	containerTexture.SetTextureSamplingMode(GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);
	Texture smilingFace("content/textures/awesomeface.png", true, GL_RGBA);
	smilingFace.SetTextureSamplingMode(GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);

	Shader uniformColorTransitionShader("shaders/simple-position.vert", "shaders/uniform-color.frag");

	Shader inputColorShader("shaders/simple-position-color.vert", "shaders/simple-color.frag");
		
	Shader textureShader("shaders/simple-texture.vert", "shaders/simple-texture.frag");
	textureShader.Use();
	textureShader.SetInt("texture0", 0);
	textureShader.SetInt("texture1", 1);
	textureShader.SetFloat("blend_alpha", 0.2f);	

	Shader orangeShader("shaders/simple-position.vert", "shaders/uniform-color.frag");	
	orangeShader.Use();
	orangeShader.SetColor("uniformColor", 1.0f, 0.5f, 0.2f, 1.0f);

	Shader::Unbind();
	
	unsigned int meshVAO;
	unsigned int meshBufferObjects[2];
	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);

	glGenBuffers(2, meshBufferObjects);
	glBindBuffer(GL_ARRAY_BUFFER, meshBufferObjects[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshData), meshData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshBufferObjects[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(meshIndices), meshIndices, GL_STATIC_DRAW);

	// bind the vertex position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, NULL);
	glEnableVertexAttribArray(0);

	// bind the vertex color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// bind the vertex uv coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind the VAO, just to be safe;
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	// start the main loop
	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();
		deltaTime = time - lastFrameTime;
		lastFrameTime = time;

		// handle input
		ProcessInput(window);

		/* view transform.Think of this as the opposite of the camera's location.
		* in order to make it appear like the camera is moving around a scene, we can apply
		* the opposite of the camera's transform to all objects in a scene. Simply moving the camera would achieve
		* nothing, since we're not actually interested in rendering the camera itself. */
		glm::mat4 viewTransform = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);

		// render 
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		textureShader.Use();
		containerTexture.Use(0);
		smilingFace.Use(1);		
		textureShader.SetMatrix4("view", viewTransform);
		textureShader.SetMatrix4("projection", projection);
		glBindVertexArray(meshVAO);

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
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
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
		mouseX = xPos;
		mouseY = yPos;
		firstMouse = false;
	}

	float deltaX = xPos - mouseX;
	float deltaY = mouseY - yPos;
	mouseX = xPos;
	mouseY = yPos;

	cameraPitch += deltaY * mouseSensitivity;
	cameraYaw += deltaX * mouseSensitivity;

	glm::vec3 direction;
	direction.x = glm::cos(glm::radians(cameraYaw)) * glm::cos(glm::radians(cameraPitch));
	direction.y = glm::sin(glm::radians(cameraPitch));
	direction.z = glm::sin(glm::radians(cameraYaw)) * glm::cos(glm::radians(cameraPitch));
	cameraDirection = glm::normalize(direction);
}

/// <summary>
/// Handle input from the mouse scroll wheel
/// </summary>
/// <param name="window"></param>
/// <param name="xOffset"></param>
/// <param name="yOffset"></param>
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	cameraPanSpeed += yOffset;
	if (cameraPanSpeed < 1.0f)
	{
		cameraPanSpeed = 1.0f;
	}
	else if (cameraPanSpeed > 50.0f)
	{
		cameraPanSpeed = 50.0f;
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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPosition += cameraDirection * cameraPanSpeed * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPosition -= cameraDirection * cameraPanSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPosition += glm::normalize(glm::cross(cameraUp, cameraDirection)) * cameraPanSpeed * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPosition -= glm::normalize(glm::cross(cameraUp, cameraDirection)) * cameraPanSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		cameraPosition.g -= cameraPanSpeed * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		cameraPosition.g += cameraPanSpeed * deltaTime;
	}

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