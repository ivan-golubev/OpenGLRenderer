#pragma once
#include <vector>
#include <string>
#include <memory>

#include "ShaderProgram.h"

struct aiMesh;
struct aiScene;

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

	struct TextureCoord
	{
		TextureCoord() {}
		TextureCoord(float _u, float _v) : u(_u), v(_v)
		{}

		float u{ 0.0f };
		float v{ 0.0f };
		static const unsigned int TEXTURE_COMPONENTS{ 2 };
	};

	struct Mesh
	{
		Mesh(aiMesh const * assimpMesh, aiScene const* scene);
		Mesh(Mesh&& other) noexcept;

		~Mesh();

		Vertex* Vertices{ nullptr };
		unsigned int* Indices{ nullptr };
		Color* Colors{ nullptr };
		TextureCoord* TextureCoords{ nullptr };
		unsigned char* Texture{ nullptr };
		int TextureWidth{ 0 };
		int TextureHeight{ 0 };

		inline unsigned int GetNumIndices() const { return NumIndices; }

		inline unsigned int VerticesSize() const { return NumVertices * sizeof(Vertex); }
		inline unsigned int IndicesSize() const { return NumIndices * sizeof(unsigned int); }
		inline unsigned int ColorsSize() const { return NumVertices * sizeof(Color); }
		inline unsigned int TextureCoordsSize() const { return NumVertices * sizeof(TextureCoord); }
	private:
		void ReadVertices(aiMesh const * assimpMesh);
		void ReadIndices(aiMesh const * assimpMesh);
		void ReadColors(aiMesh const * assimpMesh, unsigned int setNumber = 0);
		void ReadTextureCoords(aiMesh const * assimpMesh, aiScene const* scene, unsigned int setNumber = 0);

		unsigned int NumVertices{ 0 };
		unsigned int NumIndices{ 0 };
	};

	struct Model
	{
	public:
		Model(std::string&& modelRelativePath, std::string&& vertexShaderRelativePath, std::string&& fragmentShaderRelativePath);
		Model(std::string& modelRelativePath, std::string& vertexShaderRelativePath, std::string& fragmentShaderRelativePath);
		
		Model(Model&& other) noexcept;

		std::shared_ptr<ShaderProgram> shaderProgram{};
		std::vector<Mesh> meshes{};
	private:
		void LoadShaders(std::string& vertexShaderRelativePath, std::string& fragmentShaderRelativePath);
		void LoadMeshes(std::string& modelRelativePath);
	};

}