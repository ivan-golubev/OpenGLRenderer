#include "ShaderLoader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <filesystem>

namespace awesome
{
	static std::string ReadShader(std::string& path)
	{
		std::ifstream f(path, std::ios::in | std::ios::binary);
		const auto size = std::filesystem::file_size(path);
		std::string result(size, '\0');
		f.read(result.data(), size);
		return result;
	}

	static void CheckCompilationErrors(GLuint shaderId)
	{
		int compileStatus{-1};
		const int maxLogSize = 512;
		char infoLog[maxLogSize];
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
		if (!compileStatus)
		{
			glGetShaderInfoLog(shaderId, maxLogSize, nullptr, infoLog);
			std::cout << "Shader compilation failed: " << infoLog << std::endl;
		}
	}

	GLuint LoadShader(GLuint type, std::string& path)
	{
		GLuint shaderId = glCreateShader(type);
		std::string shaderText = ReadShader(path);
		char const* shaderTextCStr = shaderText.c_str();
		glShaderSource(shaderId, 1, &shaderTextCStr, nullptr);
		glCompileShader(shaderId);
		CheckCompilationErrors(shaderId);
		return shaderId;
	}
}