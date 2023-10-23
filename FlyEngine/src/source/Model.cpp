#include "Model.h"

using namespace feng;

Model::Model(const std::string& path, bool flipTex, const std::vector<glm::mat4>& instanceMatrix)
	: m_LoadModelPath(path), m_DirectoryPath(path.substr(0, path.find_last_of('\\'))), m_FlipTex(flipTex), m_InstanceMatrix(instanceMatrix) { }

Model::~Model()
{
	for (auto& texture : m_LoadedTextures)
		texture.DeleteTexture();
}

void Model::Load()
{
	Timer timer;
	Log::Print("STARTED MODEL LOADING FROM PATH: " + m_LoadModelPath + '\n');
	LoadModel(m_LoadModelPath);
	Log::Print("FINISHED MODEL LOADING!" + '\n');
}

void Model::Draw(Shader& shader, uint32_t instanceCount)
{
	for (auto& mesh : m_Meshes)
		mesh.Draw(shader, instanceCount);
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer; 
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Log::Print("ASSIMP ERROR: " + std::string(importer.GetErrorString()) + '\n');
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture> textures;

	for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
	{ 
		aiString str;
		mat->GetTexture(type, i, &str);
		bool isLoaded = false;

		for (auto& textureLoadElem : m_LoadedTextures)
		{
			if (!std::strcmp(textureLoadElem.GetPath().c_str(), (m_DirectoryPath + "\\" + str.C_Str()).c_str()))
			{
				textures.push_back(textureLoadElem);
				isLoaded = true;
				break;
			}
		}

		if (!isLoaded)
		{
			Texture texture(m_DirectoryPath + '\\' + str.C_Str(), typeName, GL_REPEAT, m_FlipTex);
			textures.push_back(texture);
			m_LoadedTextures.push_back(texture);
		}
	}
	return textures;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector3;

		vector3.x = mesh->mVertices[i].x;
		vector3.y = mesh->mVertices[i].y;
		vector3.z = mesh->mVertices[i].z;
		vertex.position = vector3;

		if (mesh->HasNormals())
		{
			vector3.x = mesh->mNormals[i].x;
			vector3.y = mesh->mNormals[i].y;
			vector3.z = mesh->mNormals[i].z;
			vertex.normal = vector3;
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vector2;
			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vector2;

			vector3.x = mesh->mTangents[i].x;
			vector3.y = mesh->mTangents[i].y;
			vector3.z = mesh->mTangents[i].z;
			vertex.tangent = vector3;

			vector3.x = mesh->mBitangents[i].x;
			vector3.y = mesh->mBitangents[i].y;
			vector3.z = mesh->mBitangents[i].z;
			vertex.bitangent = vector3;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex); 
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return Mesh(vertices, indices, textures, m_InstanceMatrix);
}
