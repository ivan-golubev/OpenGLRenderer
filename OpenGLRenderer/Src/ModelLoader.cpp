#include "ModelLoader.h"
#include "ShaderProgram.h"

#include <iostream>
#include <filesystem>
#include <cassert>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

/* Forward declare a texture loading function from stbi, which is part of Assimp */
extern "C" {
	extern unsigned char* stbi_load(char const* filename, int* x, int* y, int* comp, int req_comp);
	extern void stbi_image_free(void* retval_from_stbi_load);
}

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

		shaderProgram = std::make_shared<ShaderProgram>(vertexShaderAbsPath, fragmentShaderAbsPath);

		if (!shaderProgram->LinkedSuccessfully())
		{
			std::cout << "Failed to initialize the shaders" << std::endl;
			exit(-1);
		}
	}

	void Model::LoadMeshes(std::string& modelRelativePath)
	{
		std::string modelFileAbsPath{ std::filesystem::absolute(modelRelativePath).generic_string() };
		Assimp::Importer importer;
		aiScene const * scene = importer.ReadFile(modelFileAbsPath, 0);
		if (!scene) {
			std::cout << "Failed to read the input model " << modelFileAbsPath << std::endl;
			std::cout << importer.GetErrorString() << modelFileAbsPath << std::endl;
			exit(-1);
		}
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) 
			meshes.emplace_back(scene->mMeshes[i], scene);
	}

	Mesh::Mesh(aiMesh const * assimpMesh, aiScene const * scene)
	{
		ReadVertices(assimpMesh);
		ReadIndices(assimpMesh);
		/* read the first set of colors and textures for each vertex, the rest are ignored */
		ReadColors(assimpMesh);
		ReadTextureCoords(assimpMesh, scene);
	}

	void Mesh::ReadVertices(aiMesh const * assimpMesh)
	{
		for (unsigned int i = 0; i < assimpMesh->mNumVertices; ++i)
		{
			auto assimpVertex = assimpMesh->mVertices[i];
			Vertices.emplace_back(
				static_cast<float>(assimpVertex.x),
				static_cast<float>(assimpVertex.y),
				static_cast<float>(assimpVertex.z)
			);
		}
	}

	void Mesh::ReadIndices(aiMesh const * assimpMesh)
	{
		assert(assimpMesh->HasFaces());
		for (unsigned int i = 0; i < assimpMesh->mNumFaces; ++i)
		{
			auto face = assimpMesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
				Indices.emplace_back(face.mIndices[j]);
		}
	}

	void Mesh::ReadColors(aiMesh const * assimpMesh, unsigned int setNumber)
	{
		unsigned int NumColors = assimpMesh->mNumVertices;
		if (!assimpMesh->HasVertexColors(setNumber))
		{
			/* If imported model has no colors - use the default one */
			for (unsigned int i = 0; i < NumColors; ++i)
				Colors.push_back(DefaultColor);
		}
		else {
			for (unsigned int i = 0; i < NumColors; ++i)
			{
				auto assimpColor = assimpMesh->mColors[setNumber][i];
				Colors.emplace_back(assimpColor.r, assimpColor.g, assimpColor.b, assimpColor.a);
			}
		}
	}	

	void Mesh::ReadTextureCoords(aiMesh const * assimpMesh, aiScene const* scene, unsigned int setNumber)
	{
		if (!assimpMesh->HasTextureCoords(setNumber))
			return;
		for (unsigned int i = 0; i < assimpMesh->mNumVertices; ++i)
		{
			auto assimpTextureCoord = assimpMesh->mTextureCoords[setNumber][i];
			TextureCoords.emplace_back(assimpTextureCoord.x, assimpTextureCoord.y);
		}

		/* grab the texture name, then load the texture */
		if (scene->HasMaterials() && assimpMesh->mMaterialIndex >= 0)
		{
			auto const material = scene->mMaterials[assimpMesh->mMaterialIndex];
			aiString aiTexturePath;
			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0
				&& material->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexturePath) == AI_SUCCESS)
			{
				std::string const moduleRelativePath{ "Models/" };
				std::string const textureRelativePath{aiTexturePath.C_Str()};

				//TODO: assert if texture cannot be found

				/* the texture path is relative to the model location */
				std::string const textureAbsPath{ std::filesystem::absolute(moduleRelativePath + textureRelativePath).generic_string() };
				int nrChannels;
				Texture = stbi_load(textureAbsPath.c_str(), &TextureWidth, &TextureHeight, &nrChannels, 0);
			}
		}
	}

	Mesh::Mesh(Mesh&& other) noexcept
	{
		Vertices = std::move(other.Vertices);
		Indices = std::move(other.Indices);
		Colors = std::move(other.Colors);
		TextureCoords = std::move(other.TextureCoords);
		Texture = other.Texture;

		memset(&other, 0, sizeof(Mesh));
	}

	Mesh::~Mesh()
	{
		if (Texture)
			stbi_image_free(Texture);
	}
}
