#pragma once

#include "Mesh.h"
#include "Timer.h"

namespace feng {

	class Model
	{
	private:
		std::vector<Mesh> m_Meshes;
		std::vector<Texture> m_LoadedTextures;
		std::string m_DirectoryPath;
		std::string m_LoadModelPath;
		bool m_FlipTex;
		std::vector<float> m_InstanceMatrix;
	public:
		Model(const std::string& path, bool flipTex = true, const std::vector<float>& instanceMatrix = {});
		~Model();

		void Load();

		void Draw(Shader& shader, uint32_t instanceCount = 1);
	private:
		void LoadModel(const std::string& path);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat,
			aiTextureType type, const std::string& typeName);

		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};

}