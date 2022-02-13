#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inCol;
layout(location = 2) in vec2 inUV;

uniform vec3 offset;

out vec4 color;
out vec2 tex_coord;

void main()
{
	vec3 finalPos = inPos+offset;
	gl_Position = vec4(finalPos, 1.0);
	color = vec4(inCol, 1.0);
	tex_coord = inUV;
}