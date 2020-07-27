#pragma once

#include <GL/glew.h>
#include "Shader.h"

class ResourceManager
{
private:
	ResourceManager() = default;

public:
	static float WIDTH;
	static float HEIGHT;
	
	static Shader basicShader;
	static Shader particleShader;
	static Shader particleComputeShader;

	static void setupShaders();
};

