#pragma once
#include <string>

using GLuint = unsigned int;

namespace awesome
{
	class ShaderProgram
	{
	public:
		ShaderProgram() = default;
		ShaderProgram(std::string& vertexShaderPath, std::string fragmentShaderPath);

		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;

		~ShaderProgram();
		
		GLuint shaderProgramId{0};
		bool inline LinkedSuccessfully() const { return linkingComplete; }

	private:
		void CheckLinkErrors();

		bool linkingComplete{ false };
	};
}
