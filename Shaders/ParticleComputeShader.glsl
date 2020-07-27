#version 430 core

layout (local_size_x = 1024) in;

uniform vec3 centreOfGravity;

layout (std430, binding=0) buffer particle_positions
{
	vec4 positions[];
};

layout (std430, binding=1) buffer particle_velocities
{
	vec4 velocities[];
};

const float bigG = pow(6.67, -11) * 100000.0f * 100000.0f;

void main()
{
	vec3 particle = vec3(positions[gl_GlobalInvocationID.x].xyz);
	vec3 velocity = vec3(velocities[gl_GlobalInvocationID.x].xyz);

	float forceMagnitude = bigG / pow(length(centreOfGravity - particle), 2);
	if (forceMagnitude > 0.005f) forceMagnitude = 0.005f;
	velocity += normalize(centreOfGravity - particle) * forceMagnitude;
	particle += velocity;

	positions[gl_GlobalInvocationID.x] = vec4(particle, 1.0f);
	velocities[gl_GlobalInvocationID.x] = vec4(velocity, 1.0f);
}