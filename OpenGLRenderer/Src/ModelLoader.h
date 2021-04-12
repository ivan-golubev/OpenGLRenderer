#pragma once
#include <vector>
#include <string>

namespace awesome
{
	struct Vertex
	{
		Vertex(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
		{}
		// TODO: add other attributes
		float x, y, z;
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
	};

	std::vector<Mesh> LoadModel(std::string& filename);
}