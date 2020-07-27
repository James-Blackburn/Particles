#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "ResourceManager.h"
#include "Camera.h"

class ParticleSimulator
{
public:
	ParticleSimulator(GLFWwindow* newWindow, Camera* newCamera);
	~ParticleSimulator();

	void drawParticles(glm::mat4& newViewProjection);
	void simulateParticles();
	void update();

private:
	GLuint particleVAO = 0, particleVBO = 0;
	GLuint particleColourVBO = 0;
	GLuint particleSSBO = 0, velocitySSBO = 0;
	
	GLuint uniformParticleProjectionLocation = 0;
	GLuint uniformCentreOfGravityLocation = 0;

	GLFWwindow* window = nullptr;
	Camera* camera;

	const int WORKGROUP_SIZE = 1024;
	const int PLACED_PER_FRAME = WORKGROUP_SIZE * 2;
	const int MAX_PARTICLES = 2048000;
	int numParticles = 0;

	void genBuffer();
};

