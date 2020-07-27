#include "ResourceManager.h"

Shader ResourceManager::basicShader = Shader();
Shader ResourceManager::particleShader = Shader();
Shader ResourceManager::particleComputeShader = Shader();

float ResourceManager::WIDTH = 800.0f;
float ResourceManager::HEIGHT = 600.0f;

void ResourceManager::setupShaders()
{
	basicShader.addShader(GL_VERTEX_SHADER, "Shaders/BasicVertexShader.glsl");
	basicShader.addShader(GL_FRAGMENT_SHADER, "Shaders/BasicFragmentShader.glsl");
	basicShader.linkProgram();

	particleShader.addShader(GL_VERTEX_SHADER, "Shaders/ParticleVertexShader.glsl");
	particleShader.addShader(GL_FRAGMENT_SHADER, "Shaders/ParticleFragmentShader.glsl");
	particleShader.linkProgram();

	particleComputeShader.addShader(GL_COMPUTE_SHADER, "Shaders/ParticleComputeShader.glsl");
	particleComputeShader.linkProgram();
}