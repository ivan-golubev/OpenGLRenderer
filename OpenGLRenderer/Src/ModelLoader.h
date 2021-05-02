#pragma once
#include <vector>
#include <string>

#include "ShaderProgram.h"

struct aiMesh;

namespace awesome
{
	struct Vertex
	{
		Vertex() {}
		Vertex(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
		{}
		// TODO: add other attributes

		float x{ 0.0f };
		float y{ 0.0f };
		float z{ 0.0f };
		static const unsigned int TOTAL_VERTEX_ATTRIBS{3};
	};

	struct Mesh
	{
		Mesh(aiMesh* assimpMesh);
		Mesh(Mesh&& other) noexcept;

		~Mesh();

		Vertex* Vertices{nullptr};
		inline unsigned int SizeInBytes() const { return size * sizeof(Vertex); }
	private:
		unsigned int size{0};
	};

	struct Model
	{
	public:
		Model(std::string&& modelRelativePath, std::string&& vertexShaderRelativePath, std::string&& fragmentShaderRelativePath);
		Model(std::string& modelRelativePath, std::string& vertexShaderRelativePath, std::string& fragmentShaderRelativePath);
		
		Model(Model&& other) noexcept;
		
		void ClearMeshes() { meshes.clear(); }

		ShaderProgram shaderProgram{};
		std::vector<Mesh> meshes{};
	private:
		void LoadShaders(std::string& vertexShaderRelativePath, std::string& fragmentShaderRelativePath);
		void LoadMeshes(std::string& modelRelativePath);
	};

}