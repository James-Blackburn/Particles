#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class Shader
{
public:
	Shader() = default;
	Shader(const char* vertexShaderLocation, const char* fragmentShaderLocation);
	~Shader();
	
	int addShader(GLenum shaderType, const char* fileLocation);
	int linkProgram();
	GLuint getID() { return shaderProgramID; }

private:
	GLuint shaderProgramID;
	std::vector<GLuint> shaderList;
};

