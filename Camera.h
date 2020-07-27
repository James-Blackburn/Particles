#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;
	
	Camera() = default;
	Camera(const glm::vec3& pos);

	inline void setPosition(const glm::vec3& newPos) { pos = newPos; }
	inline glm::vec3& getPosition() { return pos; }
	glm::mat4 getProjection();

	void update();
	void processInput(GLFWwindow* window);
	void mouseMovement(float deltaX, float deltaY);
};

