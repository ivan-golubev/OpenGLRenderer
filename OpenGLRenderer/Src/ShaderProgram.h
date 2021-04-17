#pragma once
#include <string>

using GLuint = unsigned int;

namespace awesome
{
	class ShaderProgram
	{
	public:
		ShaderProgram(std::string& vertexShaderPath, std::string fragmentShaderPath);
		GLuint shaderProgramId;
		bool inline LinkingComplete() const { return linkingComplete; }

	private:
		void CheckLinkErrors();

		bool linkingComplete{ false };
	};
}
