#include "Shader.h"

Shader::Shader(const char* vertexShaderLocation, const char* fragmentShaderLocation)
{
	addShader(GL_VERTEX_SHADER, vertexShaderLocation);
	addShader(GL_FRAGMENT_SHADER, fragmentShaderLocation);
}

int Shader::addShader(GLenum shaderType, const char* fileLocation)
{
	std::ifstream file(fileLocation);
	std::string text, line;
	const char* data;

	GLuint shaderID;
	char infoLog[1024];
	int success;

	if (!file.is_open())
	{
		std::cout << "Unable to open shader: " << fileLocation << std::endl;
		return -1;
	}

	while (std::getline(file, line))
	{
		text += line + "\n";
	}
	data = text.c_str();

	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &data, GL_NONE);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 1024, GL_NONE, infoLog);
		std::cout << "Error compiling shader\n" << std::endl;
		std::cout << infoLog << std::endl;
		return -1;
	}
	shaderList.push_back(shaderID);
	return 0;
}

int Shader::linkProgram()
{
	char infoLog[1024];
	int success;

	shaderProgramID = glCreateProgram();
	for (GLuint shaderID : shaderList)
		glAttachShader(shaderProgramID, shaderID);
	glLinkProgram(shaderProgramID);
	
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 1024, GL_NONE, infoLog);
		std::cout << "Error linking shader\n" << std::endl;
		std::cout << infoLog << std::endl;
		return -1;
	}

	for (GLuint shaderID : shaderList)
	{
		glDetachShader(shaderProgramID, shaderID);
		glDeleteShader(shaderID);
	}
	
	shaderList.clear();
	return 0;
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgramID);
}