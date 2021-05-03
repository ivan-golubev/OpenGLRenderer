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

		float x{ 0.0f };
		float y{ 0.0f };
		float z{ 0.0f };
		static const unsigned int POSITION_COMPONENTS{3};
	};

	struct Color
	{
		Color() {}
		Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a)
		{}

		float r{ 0.0f };
		float g{ 0.0f };
		float b{ 0.0f };
		float a{ 0.0f };
		static const unsigned int COLOR_COMPONENTS{4};
	};

	struct Mesh
	{
		Mesh(aiMesh* assimpMesh);
		Mesh(Mesh&& other) noexcept;

		~Mesh();

		Vertex* Vertices{ nullptr };
		Color* Colors{ nullptr };
		unsigned int* Indices{ nullptr };

		inline unsigned int GetNumIndices() const { return NumIndices; }
		inline unsigned int GetNumColors() const { return NumColors; }

		inline unsigned int VerticesSize() const { return NumVertices * sizeof(Vertex); }
		inline unsigned int IndicesSize() const { return NumIndices * sizeof(unsigned int); }
		inline unsigned int ColorsSize() const { return NumColors * sizeof(Color); }
	private:
		void ReadVertices(aiMesh* assimpMesh);
		void ReadIndices(aiMesh* assimpMesh);
		void ReadColors(aiMesh* assimpMesh, unsigned int setNumber=0);

		unsigned int NumVertices{ 0 };
		unsigned int NumIndices{ 0 };
		unsigned int NumColors{ 0 };
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