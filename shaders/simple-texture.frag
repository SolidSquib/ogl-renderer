#version 330 core

in vec4 color;
in vec2 tex_coord;

uniform sampler2D diffuseTexture;

out vec4 fragColor;

void main()
{
	fragColor = texture(diffuseTexture, tex_coord);
}