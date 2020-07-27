#version 430 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aVelocity;

uniform mat4 projection;

out vec4 FragColour;

const vec3 startMix = vec3(0.25f, 0.25f, 0.25f);
const vec3 endMix = vec3(1.0f, 1.0f, 1.0f);

void main()
{
	gl_Position = projection * aPos;
	vec3 colour = abs(vec3(aVelocity.xyz)) / 0.05f;
	FragColour = vec4(mix(startMix, endMix, colour), 1.0f);
}