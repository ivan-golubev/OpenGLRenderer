#include "ModelLoader.h"
#include "ShaderProgram.h"

#include <iostream>
#include <filesystem>
#include <cassert>

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

	void Mesh::ReadIndices(aiMesh const * assimpMesh)
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

	void Mesh::ReadColors(aiMesh const * assimpMesh, unsigned int setNumber)
	{
		if (!assimpMesh->HasVertexColors(setNumber))
			return;
		unsigned int NumColors = assimpMesh->mNumVertices;
		Colors = new Color[NumColors];
		for (unsigned int i = 0; i < NumColors; ++i)
		{
			auto assimpColor = assimpMesh->mColors[setNumber][i];
			Colors[i] = Color(assimpColor.r, assimpColor.g, assimpColor.b, assimpColor.a);
		}
	}	

	void Mesh::ReadTextureCoords(aiMesh const * assimpMesh, aiScene const* scene, unsigned int setNumber)
	{
		if (!assimpMesh->HasTextureCoords(setNumber))
			return;
		unsigned NumTextureCoords = assimpMesh->mNumVertices;
		TextureCoords = new TextureCoord[NumTextureCoords];
		for (unsigned int i = 0; i < NumTextureCoords; ++i)
		{
			auto assimpTextureCoord = assimpMesh->mTextureCoords[setNumber][i];
			TextureCoords[i] = TextureCoord(assimpTextureCoord.x, assimpTextureCoord.y);
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
				/* the texture path is relative to the model location */
				std::string const textureAbsPath{ std::filesystem::absolute(moduleRelativePath + textureRelativePath).generic_string() };
				int nrChannels;
				Texture = stbi_load(textureAbsPath.c_str(), &TextureWidth, &TextureHeight, &nrChannels, 0);
			}
		}
	}

	Mesh::Mesh(Mesh&& other) noexcept
	{
		NumVertices = other.NumVertices;
		NumIndices = other.NumIndices;

		Vertices = other.Vertices;
		Indices = other.Indices;
		Colors = other.Colors;
		TextureCoords = other.TextureCoords;
		Texture = other.Texture;

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
		if (TextureCoords)
			delete[](TextureCoords);
		if (Texture)
			stbi_image_free(Texture);
	}
}
