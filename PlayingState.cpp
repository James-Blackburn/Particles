#include "PlayingState.h"

const GLfloat attractorVertices[24] = {
	-0.5f, -0.5f, -0.5f, // bl 0
	-0.5f, +0.5f, -0.5f, // tl 1
	+0.5f, +0.5f, -0.5f, // tr 2 
	+0.5f, -0.5f, -0.5f, // br 3

	-0.5f, -0.5f, +0.5f, // bl 4
	-0.5f, +0.5f, +0.5f, // tl 5
	+0.5f, +0.5f, +0.5f, // tr 6
	+0.5f, -0.5f, +0.5f, // br 7
};

const GLuint attractorIndices[36] = {
	0, 3, 2,	0, 2, 1,
	0, 1, 4,	1, 5, 4,
	4, 7, 6,	4, 6, 5,
	3, 7, 6,	2, 7, 6,
	0, 7, 4,	0, 3, 7,
	1, 2, 6,	1, 6, 5,
};

PlayingState::PlayingState(GLFWwindow* mainWindow) :
	window(mainWindow), particleSimulator(window, &camera)
{
	projection = glm::perspective(glm::radians(45.0f), ResourceManager::WIDTH / ResourceManager::HEIGHT, 0.1f, 1000.0f);
	glPointSize(1.0f);

	// Attractor
	uniformAttractorModelLocation = glGetUniformLocation(ResourceManager::basicShader.getID(), "model");
	uniformAttractorColourLocation = glGetUniformLocation(ResourceManager::basicShader.getID(), "colour");
	uniformAttractorProjectionLocation = glGetUniformLocation(ResourceManager::basicShader.getID(), "projection");

	glGenVertexArrays(1, &attractorVAO);
	glBindVertexArray(attractorVAO);

	glGenBuffers(1, &attractorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, attractorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(attractorVertices), attractorVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, GL_NONE);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glGenBuffers(1, &attractorEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attractorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(attractorIndices), attractorIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

	glBindVertexArray(GL_NONE);
}

PlayingState::~PlayingState()
{
}

void PlayingState::update()
{
	glfwPollEvents();
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	particleSimulator.update();
	particleSimulator.simulateParticles();

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	camera.mouseMovement(x - oldMouseX, oldMouseY - y);
	camera.processInput(window);
	oldMouseX = x;	oldMouseY = y;
}


void PlayingState::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 view = camera.getProjection();
	glm::mat4 viewProjection = projection * view;

	particleSimulator.drawParticles(viewProjection);

	glUseProgram(ResourceManager::basicShader.getID());
	glm::mat4 model(1.0f);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformAttractorModelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniformAttractorProjectionLocation, 1, GL_FALSE, glm::value_ptr(viewProjection));
	glUniform4f(uniformAttractorColourLocation, 0.0f, 0.75f, 0.0f, 1.0f);

	glBindVertexArray(attractorVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attractorEBO);
	glDrawElements(GL_TRIANGLES, sizeof(attractorIndices) / sizeof(attractorIndices[0]), GL_UNSIGNED_INT, GL_NONE);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
	
	glfwSwapBuffers(window);
}