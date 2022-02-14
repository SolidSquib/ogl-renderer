#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"

const float meshData[] = {
	// position data	// color data		// uv coords
	0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f
};

const int meshIndices[] = {
	0, 1, 3,
	1, 2, 3
};

void framebuffer_size_changed_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

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
}

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

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_changed_callback);

	Texture containerTexture("content/textures/container.jpg", false);
	containerTexture.SetTextureSamplingMode(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
	Texture smilingFace("content/textures/awesomeface.png", true, GL_RGBA);
	smilingFace.SetTextureSamplingMode(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);

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

	// start the main loop
	while (!glfwWindowShouldClose(window))
	{
		// handle input
		ProcessInput(window);

		// render 
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		textureShader.Use();
		containerTexture.Use(0);
		smilingFace.Use(1);
		glBindVertexArray(meshVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		// glfw events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// release resources
	glfwTerminate();
	return 0;
}