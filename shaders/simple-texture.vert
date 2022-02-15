#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inCol;
layout(location = 2) in vec2 inUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;
out vec2 tex_coord;

void main()
{
	// Matrix multiplication is done from right to left.
	gl_Position = projection * view * model * vec4(inPos, 1.0);
	color = vec4(inCol, 1.0);
	tex_coord = inUV;
}