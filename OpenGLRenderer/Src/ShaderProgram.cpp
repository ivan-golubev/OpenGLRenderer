#include "ShaderProgram.h"
#include "ShaderLoader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace awesome
{
	ShaderProgram::ShaderProgram(std::string& vertexShaderPath, std::string fragmentShaderPath)
	{
		GLuint vertexShaderId = LoadShader(GL_VERTEX_SHADER, vertexShaderPath);
		GLuint fragmentShaderId = LoadShader(GL_FRAGMENT_SHADER, fragmentShaderPath);
		shaderProgramId = glCreateProgram();
		glAttachShader(shaderProgramId, vertexShaderId);
		glAttachShader(shaderProgramId, fragmentShaderId);
		glLinkProgram(shaderProgramId);
		CheckLinkErrors();
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	{
		linkingComplete = other.linkingComplete;
		shaderProgramId = other.shaderProgramId;
		other.shaderProgramId = 0;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
	{
		linkingComplete = other.linkingComplete;
		shaderProgramId = other.shaderProgramId;
		other.shaderProgramId = 0;
		return *this;
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(shaderProgramId);
	}

	void ShaderProgram::CheckLinkErrors()
	{
		int linkStatus{ -1 };
		const int maxLogSize = 512;
		char infoLog[maxLogSize];
		glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &linkStatus);
		linkingComplete = linkStatus > 0;
		if (!linkingComplete) {
			glGetProgramInfoLog(shaderProgramId, maxLogSize, nullptr, infoLog);
			std::cout << "Shader linking failed: " << infoLog << std::endl;
		}
	}

}
