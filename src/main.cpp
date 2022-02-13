#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"

static const float triangle_verts[] = {
	// triangle 1
	0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.0f, -0.5f, 0.0f	
};

static const float triangle2_data[] = {
	// triangle 2
	/*position*/ 0.5f, 0.5f, 0.0f,  /*color*/ 1.0f, 0.0f, 0.0f,
	/*position*/ 1.0f, -0.5f, 0.0f, /*color*/ 0.0f, 1.0f, 0.0f,
	/*position*/ 0.0f, -0.5f, 0.0f, /*color*/ 0.0f, 0.0f, 1.0f
};

static const float quad_verts[] = {
	-1.0f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f
};

static const unsigned int quad_indices[] = {
	0, 1, 2,
	2, 3, 0
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

	Shader uniformColorTransitionShader("shaders/simple-position.vert", "shaders/uniform-color.frag");
	Shader inputColorShader("shaders/simple-position-color.vert", "shaders/simple-color.frag");
	Shader orangeShader("shaders/simple-position.vert", "shaders/uniform-color.frag");
	orangeShader.Use();
	orangeShader.SetColor("uniformColor", 1.0f, 0.5f, 0.2f, 1.0f);
			
	unsigned int triangleBuffers[2];
	unsigned int triangleArrayBuffers[2];

	unsigned int quadBuffers[2];
	unsigned int quadVertexBufferArray;

	/* I am only generating a single buffer here, but I guess I should generate as many as I know I will need
	 * and perhaps seperate the calls based on whether the draw type should be static or dynamic? */
	glGenBuffers(2, triangleBuffers);
	glGenVertexArrays(2, triangleArrayBuffers);

	// I could merge these if I stored everything in an array.
	glGenBuffers(2, quadBuffers);
	glGenVertexArrays(1, &quadVertexBufferArray);

	// Setup triangle 1:
	glBindVertexArray(triangleArrayBuffers[0]);

	glBindBuffer(GL_ARRAY_BUFFER, triangleBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verts), triangle_verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Setup triangle 2:
	glBindVertexArray(triangleArrayBuffers[1]);

	glBindBuffer(GL_ARRAY_BUFFER, triangleBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2_data), triangle2_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// Setup the quad
	glBindVertexArray(quadVertexBufferArray);

	glBindBuffer(GL_ARRAY_BUFFER, quadBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_verts), quad_verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

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
		
		float time = (float)glfwGetTime();
		float red = sin(time) / 2.0f + 0.5f;
		float green = cos(time) / 2.0f + 0.5f;
		float blue = 0.0f;
		uniformColorTransitionShader.Use();
		uniformColorTransitionShader.SetColor("uniformColor", red, green, blue, 1.0f);
		glBindVertexArray(triangleArrayBuffers[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		inputColorShader.Use();
		glBindVertexArray(triangleArrayBuffers[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		orangeShader.Use();
		glBindVertexArray(quadVertexBufferArray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// release resources
	glfwTerminate();
	return 0;
}