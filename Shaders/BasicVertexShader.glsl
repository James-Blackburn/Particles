#version 430 core

layout (location = 0) in vec3 aPos;

uniform vec4 colour;
uniform mat4 model;
uniform mat4 projection;

out vec4 Colour;

void main()
{
	gl_Position = projection * model * vec4(aPos, 1.0f);
	Colour = colour;
}