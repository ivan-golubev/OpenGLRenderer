#pragma once
#include <string>

typedef unsigned int GLuint;

namespace awesome
{
	GLuint LoadShader(GLuint type, std::string& path);
}
