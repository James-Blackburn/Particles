#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "GameState.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "ParticleSimulator.h"

class PlayingState : public GameState
{
public:
	PlayingState(GLFWwindow* mainWindow);
	~PlayingState();

	void update();
	void draw();

private:
	GLFWwindow* window = nullptr;
	ParticleSimulator particleSimulator;
	glm::mat4 projection;
	Camera camera;

	GLuint attractorVAO = 0;
	GLuint attractorVBO = 0;
	GLuint attractorEBO = 0;

	GLuint uniformAttractorColourLocation = 0;
	GLuint uniformAttractorModelLocation = 0;
	GLuint uniformAttractorProjectionLocation = 0;

	float oldMouseX = 400.0f;
	float oldMouseY = 300.0f;
};

