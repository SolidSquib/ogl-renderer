#version 330 core

in vec4 color;
in vec2 tex_coord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float blend_alpha;

out vec4 fragColor;

void main()
{
	fragColor = mix(texture(texture0, tex_coord), texture(texture1, tex_coord), blend_alpha);
}