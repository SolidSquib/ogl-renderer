#version 330 core

layout(location = 0) in vec3 aPos;

uniform vec3 offset;

void main()
{
	vec3 finalPos = aPos+offset;
	gl_Position = vec4(finalPos, 1.0);
}