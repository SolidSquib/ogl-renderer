#version 330 core

uniform vec4 uniformColor;

out vec4 fragColor;

void main()
{
	fragColor = uniformColor;
}