#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

static const float triangle_verts[] = {
	// triangle 1
	0.5f, 0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.0f, -0.5f, 0.0f	
};

static const float triangle_verts2[] = {
	// triangle 2
	0.5f, 0.5f, 0.0f,
	1.0f, -0.5f, 0.0f,
	0.0f, -0.5f, 0.0f
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

const char* vertexShaderSource = "#version 330 core\n"
	"layout(location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 fragColor;\n"
	"void main()\n"
	"{\n"
	"	fragColor = vec4(%f, %f, %f, %f);\n"
	"}";

void framebuffer_size_changed_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool CheckShaderCompiledCorrectly(unsigned int shader)
{
	int success;
	char infoLog[512];
	char shaderSource[1024];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		glGetShaderSource(shader, 1024, NULL, shaderSource);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED: " << infoLog << std::endl
			<< shaderSource << std::endl;
	}

	return success;
}

bool CheckShaderProgramLinkedCorrectly(unsigned int program)
{
	int success;
	char infoLog[512];

	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::LINK_FAILED" << infoLog << std::endl;
	}

	return success;
}

unsigned int CreateBasicVertexShader()
{
	unsigned int shader;

	shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader, 1, &vertexShaderSource, NULL);
	glCompileShader(shader);

	return CheckShaderCompiledCorrectly(shader) ? shader : -1;
}

unsigned int CreateBasicColorFragmentShader(float red, float green, float blue, float alpha)
{
	char buffer[512];
	sprintf_s(buffer, 512, fragmentShaderSource, red, green, blue, alpha);
	const char* shaderSource = buffer; // this is not the best way

	unsigned int shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	return CheckShaderCompiledCorrectly(shader) ? shader : -1;
}

unsigned int CreateBasicColorShaderProgram(float red, float green, float blue, float alpha)
{
	unsigned int program;
	unsigned int vertexShader = CreateBasicVertexShader();
	unsigned int fragmentShader = CreateBasicColorFragmentShader(red, green, blue, alpha);

	// Create a shader program from our shaders we created above
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	return CheckShaderProgramLinkedCorrectly(program) ? program : -1;
}

void processInput(GLFWwindow* window)
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

	unsigned int orange = CreateBasicColorShaderProgram(1.0f, 0.5f, 0.2f, 1.0f);
	unsigned int teal = CreateBasicColorShaderProgram(0.0f, 0.5f, 0.5f, 1.0f);
	
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verts2), triangle_verts2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

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
		processInput(window);

		// render 
		glClearColor(0.1f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(orange);
		glBindVertexArray(triangleArrayBuffers[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(teal);
		glBindVertexArray(triangleArrayBuffers[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(orange);
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