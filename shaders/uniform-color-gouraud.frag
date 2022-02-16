#version 330 core

in vec4 finalLightColor;

uniform vec4 uniformColor;

out vec4 fragColor;

void main()
{
	fragColor = uniformColor * finalLightColor;
}