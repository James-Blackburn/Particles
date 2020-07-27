#include "ParticleSimulator.h"

ParticleSimulator::ParticleSimulator(GLFWwindow* newWindow, Camera* newCamera)
{
	window = newWindow;
	camera = newCamera;
	genBuffer();
}

ParticleSimulator::~ParticleSimulator()
{
}

void ParticleSimulator::genBuffer()
{
	uniformParticleProjectionLocation = glGetUniformLocation(ResourceManager::particleShader.getID(), "projection");
	uniformCentreOfGravityLocation = glGetUniformLocation(ResourceManager::particleComputeShader.getID(), "centreOfGravity");

	// Particle
	glGenVertexArrays(1, &particleVAO);
	glBindVertexArray(particleVAO);

	glGenBuffers(1, &particleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, GL_NONE);
	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 1);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glGenBuffers(1, &particleColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, particleColourVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, GL_NONE);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glBindVertexArray(GL_NONE);	

	// Compute
	glGenBuffers(1, &particleSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_NONE);

	glGenBuffers(1, &velocitySSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocitySSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_NONE);
}

void ParticleSimulator::drawParticles(glm::mat4& viewProjection)
{
	glUseProgram(ResourceManager::particleShader.getID());
	
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSSBO);
	glCopyBufferSubData(GL_SHADER_STORAGE_BUFFER, GL_ARRAY_BUFFER, 0, 0, numParticles * 4 * sizeof(GLfloat));
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_NONE);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	
	glBindBuffer(GL_ARRAY_BUFFER, particleColourVBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocitySSBO);
	glCopyBufferSubData(GL_SHADER_STORAGE_BUFFER, GL_ARRAY_BUFFER, 0, 0, numParticles * 4 * sizeof(GLfloat));
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_NONE);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	// draw particles
	glUniformMatrix4fv(uniformParticleProjectionLocation, 1, GL_FALSE, glm::value_ptr(viewProjection));

	glBindVertexArray(particleVAO);
	glDrawArraysInstanced(GL_POINTS, 0, 1, numParticles);
	glBindVertexArray(GL_NONE);
}

void ParticleSimulator::update()
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && numParticles < MAX_PARTICLES)
	{
		std::vector<glm::vec4> particles(PLACED_PER_FRAME);
		std::vector<glm::vec4> velocities(PLACED_PER_FRAME);

		glm::vec3 cameraFront(camera->pos + camera->front);
		for (int i = 0; i < PLACED_PER_FRAME; i++)
		{
			const float x = ((float)(rand()) / (float)(RAND_MAX)-0.5f) / 2.0f;
			const float y = ((float)(rand()) / (float)(RAND_MAX)-0.5f) / 2.0f;
			const float z = ((float)(rand()) / (float)(RAND_MAX)-0.5f) / 2.0f;
			particles[i] = glm::vec4(cameraFront.x + x, cameraFront.y + y, cameraFront.z + z, 1.0f);
			velocities[i] = glm::vec4(((float)(rand()) / (float)(RAND_MAX)-0.5f) / 20.0f,
				((float)(rand()) / (float)(RAND_MAX)-0.5f) / 20.0f,
				((float)(rand()) / (float)(RAND_MAX)-0.5f) / 20.0f, 1.0f);
		}
		numParticles += PLACED_PER_FRAME;

		// send updated particle data to compute shader buffer
		glUseProgram(ResourceManager::particleComputeShader.getID());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSSBO);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, numParticles * 4 * sizeof(GLfloat), PLACED_PER_FRAME * 4 * sizeof(GLfloat), particles.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocitySSBO);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, numParticles * 4 * sizeof(GLfloat), PLACED_PER_FRAME * 4 * sizeof(GLfloat), velocities.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_NONE);
	}
}


void ParticleSimulator::simulateParticles()
{
	glUseProgram(ResourceManager::particleComputeShader.getID());

	// compute data
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velocitySSBO);

	glUniform3f(uniformCentreOfGravityLocation, 0.0f, 0.0f, 0.0f);
	const int NUM_WORKGROUPS = numParticles / WORKGROUP_SIZE;
	glDispatchCompute(NUM_WORKGROUPS, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, GL_NONE);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, GL_NONE);
}