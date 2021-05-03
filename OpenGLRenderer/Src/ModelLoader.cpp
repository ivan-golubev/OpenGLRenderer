#include "ModelLoader.h"
#include "ShaderProgram.h"

#include <iostream>
#include <filesystem>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace awesome
{
	Model::Model(std::string&& modelRelativePath, std::string&& vertexShaderRelativePath, std::string&& fragmentShaderRelativePath)
		: Model(modelRelativePath, vertexShaderRelativePath, fragmentShaderRelativePath)
	{}

	Model::Model(std::string& modelRelativePath, std::string& vertexShaderRelativePath, std::string& fragmentShaderRelativePath)
	{
		LoadShaders(vertexShaderRelativePath, fragmentShaderRelativePath);
		LoadMeshes(modelRelativePath);
	}

	Model::Model(Model&& other) noexcept
	{
		shaderProgram = std::move(other.shaderProgram);
		meshes = std::move(other.meshes);
		memset(&other, 0, sizeof(Model));
	}

	void Model::LoadShaders(std::string& vertexShaderRelativePath, std::string& fragmentShaderRelativePath)
	{
		std::string vertexShaderAbsPath{ std::filesystem::absolute(vertexShaderRelativePath).generic_string() };
		std::string fragmentShaderAbsPath{ std::filesystem::absolute(fragmentShaderRelativePath).generic_string() };

		shaderProgram = ShaderProgram(vertexShaderAbsPath, fragmentShaderAbsPath);

		if (!shaderProgram.LinkedSuccessfully())
		{
			std::cout << "Failed to initialize the shaders" << std::endl;
			exit(-1);
		}
	}

	void Model::LoadMeshes(std::string& modelRelativePath)
	{
		std::string modelFileAbsPath{ std::filesystem::absolute(modelRelativePath).generic_string() };
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(modelFileAbsPath, 0);
		if (!scene) {
			std::cout << "Failed to read the input model " << modelFileAbsPath << std::endl;
			std::cout << importer.GetErrorString() << modelFileAbsPath << std::endl;
			exit(-1);
		}
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
			meshes.emplace_back(scene->mMeshes[i]);
	}

	Mesh::Mesh(aiMesh* assimpMesh)
	{
		ReadVertices(assimpMesh);
		ReadIndices(assimpMesh);
		/* read the first set of colors for each vertex */
		ReadColors(assimpMesh);
	}

	void Mesh::ReadVertices(aiMesh* assimpMesh)
	{
		NumVertices = assimpMesh->mNumVertices;
		Vertices = new Vertex[NumVertices];
		for (unsigned int i = 0; i < assimpMesh->mNumVertices; ++i)
		{
			auto assimpVertex = assimpMesh->mVertices[i];
			Vertices[i].x = static_cast<float>(assimpVertex.x);
			Vertices[i].y = static_cast<float>(assimpVertex.y);
			Vertices[i].z = static_cast<float>(assimpVertex.z);
		}
	}

	void Mesh::ReadIndices(aiMesh* assimpMesh)
	{
		assert(assimpMesh->HasFaces());
		unsigned int polygonSize = assimpMesh->mFaces[0].mNumIndices;
		/* IMPORTANT: assume that all faces are of the same size (generally should be triangles) */
		NumIndices = assimpMesh->mNumFaces * polygonSize;

		Indices = new unsigned int[NumIndices];
		for (unsigned int i = 0; i < assimpMesh->mNumFaces; ++i)
		{
			auto face = assimpMesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				unsigned int ix = polygonSize * i + j;
				Indices[ix] = face.mIndices[j];
			}
		}
	}

	void Mesh::ReadColors(aiMesh* assimpMesh, unsigned int setNumber)
	{
		if (!assimpMesh->HasVertexColors(setNumber))
			return;
		NumColors = assimpMesh->mNumVertices;
		Colors = new Color[NumColors];
		for (unsigned int i = 0; i < NumColors; ++i)
		{
			auto assimpColor = assimpMesh->mColors[setNumber][i];
			Colors[i] = Color(assimpColor.r, assimpColor.g, assimpColor.b, assimpColor.a);
		}
	}

	Mesh::Mesh(Mesh&& other) noexcept
	{
		NumVertices = other.NumVertices;
		NumIndices = other.NumIndices;
		Vertices = other.Vertices;
		Indices = other.Indices;
		Colors = other.Colors;
		
		memset(&other, 0, sizeof(Mesh));
	}

	Mesh::~Mesh()
	{
		if (Vertices)
			delete[](Vertices);
		if (Indices)
			delete[](Indices);
		if (Colors)
			delete[](Colors);
	}
}
