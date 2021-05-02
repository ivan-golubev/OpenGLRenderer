#include "ModelLoader.h"
#include "ShaderProgram.h"

#include <iostream>
#include <filesystem>

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
		size = assimpMesh->mNumVertices;
		Vertices = new Vertex[size];
		for (unsigned int i = 0; i < assimpMesh->mNumVertices; ++i)
		{
			auto assimpVertex = assimpMesh->mVertices[i];
			Vertices[i].x = static_cast<float>(assimpVertex.x);
			Vertices[i].y = static_cast<float>(assimpVertex.y);
			Vertices[i].z = static_cast<float>(assimpVertex.z);
		}
	}

	Mesh::Mesh(Mesh&& other) noexcept
	{
		size = other.size;
		Vertices = other.Vertices;
		other.Vertices = nullptr;
	}

	Mesh::~Mesh()
	{
		if (Vertices)
			delete[](Vertices);
	}
}
