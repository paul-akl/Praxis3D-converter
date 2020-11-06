#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "Containers.h"
#include "ModelLoader.h"
#include "TextureLoader.h"

class PraxisExporter
{
public:
	PraxisExporter();
	~PraxisExporter();

	void exportScene(Scene &p_scene, std::string p_exportLocation);

private:
	// Used as a static struct to hold folder names
	struct FolderNames
	{
		FolderNames()
		{
			m_maps = "Maps";
			m_materials = "Materials";
			m_models = "Models";
			m_shaders = "Shaders";
		}

		std::string m_maps;
		std::string m_materials;
		std::string m_models;
		std::string m_shaders;
	};

	struct RMHAOTexture
	{
		RMHAOTexture(const std::string &p_name) : m_RMHAOfilename(p_name) 
		{
			clearTextures();
		}

		void clearFilenames()
		{
			m_roughnessTexFilename.clear();
			m_metallicTexFilename.clear();
			m_heightMapTexFilename.clear();
			m_ambientOclTexFilename.clear();
		}		
		void clearTextures()
		{
			m_roughnessTex = nullptr;
			m_metallicTex = nullptr;
			m_heightMapTex = nullptr;
			m_ambientOclTex = nullptr;
		}

		std::string m_RMHAOfilename,
					m_roughnessTexFilename,
					m_metallicTexFilename,
					m_heightMapTexFilename,
					m_ambientOclTexFilename;

		Texture *m_roughnessTex,
				*m_metallicTex,
				*m_heightMapTex,
				*m_ambientOclTex;
	};

	enum class objectType : unsigned int
	{
		objectType_null = 0,
		objectType_renderer,
		objectType_light
	};
	enum class lightType : unsigned int
	{
		lightType_null = 0,
		lightType_directional,
		lightType_point,
		lightType_spot
	};

	std::string getStringVector(const nlohmann::json &p_jsonVector);
	std::string getStringFloat(const nlohmann::json &p_jsonVector);
	std::string getString(const nlohmann::json &p_node, const std::string& p_searchKeyword);
	PraxisExporter::objectType getObjectType(const nlohmann::json &p_object);
	PraxisExporter::lightType getLightType(const nlohmann::json &p_light);

	nlohmann::json processMaterial(const Scene &p_scene, const nlohmann::json& p_materialNode, const std::string& p_materialName, const int p_materialIndex, std::string &p_returnRelativeFilename);
	nlohmann::json createMaterialNode(const std::string &p_materialFilePath, const int p_materialIndex);

	MeshData *getMeshData(Scene &p_scene, const nlohmann::json &p_meshNode);
	void fillMeshVectors(std::vector<vec3> &p_vectors, const nlohmann::json &p_arrayNode, std::size_t p_vertCount, std::string p_name = "");
	void fillSubmeshVectors(std::vector<vec3> &p_vectors, const nlohmann::json &p_arrayNode, std::string p_name = "");

	std::string m_filename;
	nlohmann::json m_exportScene;

	ModelLoader m_modelLoader;
	ModelExportType m_modelExportType;

	TextureLoader m_textureLoader;
	TextureExportFormatType m_textureExportFormat;

	static FolderNames m_folderName;

	MaterialTypeNames m_materialTypeNames;
};