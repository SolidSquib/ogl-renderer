#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inCol;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 viewPosition = view * model * vec4(inPos, 1.0);
	frag_position = viewPosition.xyz;

	gl_Position = projection * viewPosition;

	// should not run these operations on the GPU, it's very innefficient
	frag_normal = mat3(transpose(inverse(view * model))) * inNormal;
	frag_tex_coords = inUV;
}