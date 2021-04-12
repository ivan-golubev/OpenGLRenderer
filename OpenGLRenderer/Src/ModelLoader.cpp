#include "ModelLoader.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace awesome
{
	std::vector<Mesh> LoadModel(std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, 0);
		if (!scene) {
			std::cout << "Failed to read the input model " << filename << std::endl;
			std::cout << importer.GetErrorString() << filename << std::endl;
			exit(-1);
		}
		std::vector<Mesh> meshes{};
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			auto assimpMesh = scene->mMeshes[i];
			Mesh mesh{};
			for (unsigned int j = 0; j < assimpMesh->mNumVertices; ++j)
			{
				auto vertex = assimpMesh->mVertices[j];
				mesh.vertices.emplace_back(
					static_cast<float>(vertex.x),
					static_cast<float>(vertex.y),
					static_cast<float>(vertex.z)
				);
			}
			meshes.push_back(mesh);
		}
		return meshes;
	}
}
