
#include <fstream>
#include <iostream>
#include <sstream>

#include "exodusImporter.h"
#include "Filesystem.h"
#include "Logger.h"
#include "Utilities.h"

#define ID_LENGTH 8

#define RESOURCE_MATERIALS "materials"
#define RESOURCE_MESHES "meshes"
#define RESOURCE_TEXTURES "textures"
#define RESOURCE_SCENE "scenes"

exodusImporter::exodusImporter()
{
	m_importedScene = nullptr;
}

exodusImporter::~exodusImporter()
{
	if(m_importedScene != nullptr)
		delete m_importedScene;
}

bool exodusImporter::importScene()
{
	// Define values used to set the progress bar value for importing process
	int progressValueLast = 0;
	int progressValueStart = 2;
	int progressValueScene = 3;
	int progressValueMaterials = 5;
	int progressValueMeshes = 30;
	int progressValueTextures = 5;
	int progressValueEnd = 3;
	
	Logger::logProgress(progressValueLast + progressValueStart);
	progressValueLast += progressValueStart;

	// Bool to be returned
	bool importSuccessful = true;

	// Get the text stream
	std::ifstream sceneFile(m_sceneRootDirectory + m_sceneFilename);

	Logger::log("INFO: Parsing: " + m_sceneFilename);

	// Parse the root (scene) JSON file, if the stream was opened successfully
	if(sceneFile)
		m_sceneJson = nlohmann::json::parse(sceneFile);

	Logger::log("INFO: Getting resource array \"externResources\":");

	// Get the node containing all the resources
	auto externResources = m_sceneJson.find("externResources");
	if(externResources != m_sceneJson.end())
	{
		// Delete the scene container if it has been created already
		if(m_importedScene != nullptr)
			delete m_importedScene;

		// Create a new scene container
		m_importedScene = new Scene();

		// Set scene location and name strings
		m_importedScene->m_assetsRootDirectory = m_assetDirectory;
		m_importedScene->m_assetsRootDirectoryAlternative = m_resourceDirectory + "!Unsupported\\";
		m_importedScene->m_resourceDirectory = m_resourceDirectory;
		m_importedScene->m_sceneRootDirectory;
		m_importedScene->m_sceneName = m_sceneName;
		
		Logger::log("INFO: Getting resource array \"" + std::string(RESOURCE_SCENE) + "\":");

		// Get the scene resource node
		auto sceneResources = externResources->find(RESOURCE_SCENE);
		if(sceneResources != externResources->end())
		{
			// Check if resource is not empty and if it is an array
			if(!sceneResources->empty() && sceneResources->type() == nlohmann::json::value_t::array)
			{
				// Get the resource filename
				std::string resourceFilename = sceneResources->at(0).get<nlohmann::json::string_t>();

				// Go over each character in the filename to get the ID number 
				for(decltype(resourceFilename.size()) size = resourceFilename.size(), i = std::string(RESOURCE_SCENE).size() - 2; i < size; i++)
				{
					if(resourceFilename[i] == '.')
					{
						m_importedScene->m_scene.m_filename = resourceFilename;
						m_importedScene->m_scene.m_id = std::stoi(resourceFilename.substr(i - ID_LENGTH, ID_LENGTH));

						break;
					}
				}

				importResource(m_importedScene->m_scene);
			}
		}
		
		Logger::logProgress(progressValueLast + progressValueScene);
		progressValueLast += progressValueScene;
		Logger::log("INFO: Getting resource array \"" + std::string(RESOURCE_MATERIALS) + "\":");

		// Get each of the corresponding resource nodes
		auto materialResources = externResources->find(RESOURCE_MATERIALS);
		if(materialResources != externResources->end())
		{
			// Check if resource is not empty and if it is an array
			if(!materialResources->empty() && materialResources->type() == nlohmann::json::value_t::array)
			{
				// Reserve enough space in the resource vector, so it's a bit faster
				m_importedScene->m_materials.reserve(materialResources->size() + 1);

				// Iterate over all array elements in the resource node
				for(decltype(materialResources->size()) size = materialResources->size(), i = 0; i < size; i++)
				{
					// Declare variables and get resource filename
					std::string resourceFilename = materialResources->at(i).get<nlohmann::json::string_t>();
					int id = -1;

					// Go over each character in the filename to get the ID number 
					for(decltype(resourceFilename.size()) size = resourceFilename.size(), i = std::string(RESOURCE_MATERIALS).size() - 2; i < size; i++)
					{
						if(resourceFilename[i] == '-')
						{
							m_importedScene->m_materials.emplace_back(resourceFilename, std::stoi(resourceFilename.substr(i - ID_LENGTH, ID_LENGTH)));
							break;
						}
					}
				}
				
				for(decltype(m_importedScene->m_materials.size()) size = m_importedScene->m_materials.size(), i = 0; i < size; i++)
				{
					importResource(m_importedScene->m_materials[i]);
					Logger::logProgress(progressValueLast + (i / (float)size * progressValueMaterials));
				}
			}
		}
		
		Logger::logProgress(progressValueLast + progressValueMaterials);
		progressValueLast += progressValueMaterials;
		Logger::log("INFO: Getting resource array \"" + std::string(RESOURCE_MESHES) + "\":");
	
		auto meshResources = externResources->find(RESOURCE_MESHES);
		if(meshResources != externResources->end())
		{			
			// Check if resource is not empty and if it is an array
			if(!meshResources->empty() && meshResources->type() == nlohmann::json::value_t::array)
			{
				// Reserve enough space in the resource vector, so it's a bit faster
				m_importedScene->m_materials.reserve(meshResources->size() + 1);

				// Iterate over all array elements in the resource node
				for(decltype(meshResources->size()) size = meshResources->size(), i = 0; i < size; i++)
				{
					// Declare variables and get resource filename
					std::string resourceFilename = meshResources->at(i).get<nlohmann::json::string_t>();
					int id = -1;

					// Go over each character in the filename to get the ID number 
					for(decltype(resourceFilename.size()) size = resourceFilename.size(), i = std::string(RESOURCE_MESHES).size() - 2; i < size; i++)
					{
						if(resourceFilename[i] == '-')
						{
							m_importedScene->m_meshes.emplace_back(resourceFilename, std::stoi(resourceFilename.substr(i - ID_LENGTH, ID_LENGTH)));
							break;
						}
					}
				}
				
// During testing, only load one mesh, as it takes a very long time to load all the meshes
#ifdef DEV_MODE
				importResource(m_importedScene->m_meshes[0]);
#else
				for(decltype(m_importedScene->m_meshes.size()) size = m_importedScene->m_meshes.size(), i = 0; i < size; i++)
				{
					importResource(m_importedScene->m_meshes[i]);
					Logger::logProgress(progressValueLast + (i / (float)size * progressValueMeshes));
				}
#endif // DEV_MODE
			}
		}
		
		Logger::logProgress(progressValueLast + progressValueMeshes);
		progressValueLast += progressValueMeshes;
		Logger::log("INFO: Getting resource array \"" + std::string(RESOURCE_TEXTURES) + "\":");

		auto textureResources = externResources->find(RESOURCE_TEXTURES);
		if(textureResources != externResources->end())
		{			
			// Check if resource is not empty and if it is an array
			if(!textureResources->empty() && textureResources->type() == nlohmann::json::value_t::array)
			{
				// Reserve enough space in the resource vector, so it's a bit faster
				m_importedScene->m_materials.reserve(textureResources->size() + 1);

				// Iterate over all array elements in the resource node
				for(decltype(textureResources->size()) size = textureResources->size(), i = 0; i < size; i++)
				{
					// Declare variables and get resource filename
					std::string resourceFilename = textureResources->at(i).get<nlohmann::json::string_t>();
					int id = -1;

					// Go over each character in the filename to get the ID number 
					for(decltype(resourceFilename.size()) size = resourceFilename.size(), i = std::string(RESOURCE_TEXTURES).size() - 2; i < size; i++)
					{
						if(resourceFilename[i] == '-')
						{
							m_importedScene->m_textures.emplace_back(resourceFilename, std::stoi(resourceFilename.substr(i - ID_LENGTH, ID_LENGTH)));
							break;
						}
					}
				}
				
				for(decltype(m_importedScene->m_textures.size()) size = m_importedScene->m_textures.size(), i = 0; i < size; i++)
				{
					importResource(m_importedScene->m_textures[i]);
					Logger::logProgress(progressValueLast + (i / (float)size * progressValueTextures));
				}
			}
		}
	
		Logger::logProgress(progressValueLast + progressValueTextures);
		progressValueLast += progressValueTextures;
		m_importedScene->m_numMaterials = m_importedScene->m_materials.size();
		m_importedScene->m_numMeshes = m_importedScene->m_meshes.size();
		m_importedScene->m_numTextures = m_importedScene->m_textures.size();

		m_importedScene->reserveArrayData();
		
		importMaterialData();

		m_importedScene->allignArrayIDs();
		Logger::logProgress(progressValueLast + progressValueEnd);
	}
	else
	{
		importSuccessful = false;
	}

	return importSuccessful;
}

void exodusImporter::setSceneFilename(const std::string &p_filename) 
{ 
	m_sceneFilename = Utilities::stripFilename(p_filename);
	m_sceneName = p_filename;

	// Remove the file extension
	m_sceneName = Utilities::removeExtension(m_sceneName);
	
	// Separate the directory from the scene name
	for(decltype(m_sceneName.size()) i = m_sceneName.size() - 1; i > 0; i--)
	{
		if(m_sceneName[i] == '\\')
		{
			m_sceneRootDirectory = m_sceneName.substr(0, i + 1);
			m_sceneName = m_sceneName.substr(i + 1, m_sceneName.size());
			m_resourceDirectory = m_sceneRootDirectory + m_sceneName + '\\';

			// Default assets location
			m_assetDirectory = m_resourceDirectory;

			// Check whether an alternative asset directory was given and is valid
			// This option overrides other asset paths, as this was specifically set
			if(!m_alternativeAssetDirectory.empty() && Filesystem::exists(m_alternativeAssetDirectory))
			{
				m_assetDirectory = m_alternativeAssetDirectory;
			}
			else
			{
				// If the scene is inside the Unity project file, the location of Assets folder is different
				// Check whether the Assets folder is inside the root scene directory or inside the resource directory
				// Assign the assets location accordingly
				if(Filesystem::exists(m_sceneRootDirectory + "Assets"))
					m_assetDirectory = m_sceneRootDirectory;
			}

			return;
		}
	}

	// If this is point is reached, something went wrong
	Logger::log("ERROR: wrong path or file name of the scene: " + p_filename);

	// Clear all the path and file names, so that scene cannot be loaded with the faulty ones
	m_sceneName.clear();
	m_sceneFilename.clear();
	m_sceneRootDirectory.clear();
	m_resourceDirectory.clear();
	m_assetDirectory.clear();
}

void exodusImporter::setAssetDirectory(const std::string &p_assetDirectory)
{ 
	m_alternativeAssetDirectory = p_assetDirectory;

	// Check whether an alternative asset directory was given and is valid
	// This option overrides other asset paths, as this was specifically set
	if(!m_alternativeAssetDirectory.empty() && Filesystem::exists(m_alternativeAssetDirectory))
	{
		m_assetDirectory = m_alternativeAssetDirectory;
	}
}

void exodusImporter::importResource(Resource &p_resource)
{
	std::ifstream resourceFile(m_resourceDirectory + p_resource.m_filename);
	
	Logger::log("INFO: Parsing: " + m_resourceDirectory + p_resource.m_filename);

	if(resourceFile)
      p_resource.m_jsonFile = nlohmann::json::parse(resourceFile); 
}

void exodusImporter::importMaterialData()
{
	// If material array size is smaller than the number of materials
	// Prefill material array, so any element in it can be accessed by index (in case the material ID and index mismatch)
	if(m_importedScene->m_materialData.size() < m_importedScene->m_numMaterials)
		std::fill_n(std::back_inserter(m_importedScene->m_materialData), m_importedScene->m_numMaterials - m_importedScene->m_materialData.size(), Material());

	for(decltype(m_importedScene->m_materials.size()) size = m_importedScene->m_materials.size(), i = 0; i < size; i++)
	{
		// Get the material ID (for matching texture with a mesh) as an integer; assign it to the material data array
		int materialID = static_cast<int>(m_importedScene->m_materials.at(i).m_jsonFile.find("id")->get<nlohmann::json::number_integer_t>());
		m_importedScene->m_materialData[materialID].m_id = materialID;

		for(std::size_t matTypeIndex = 0; matTypeIndex < MaterialType::numOfMaterialTypes; matTypeIndex++)
		{
			// Get the texture ID
			int textureID = static_cast<int>(m_importedScene->m_materials.at(i).m_jsonFile.find(m_importedScene->m_materialTypeNames.m_names[matTypeIndex])->get<nlohmann::json::number_integer_t>());

			if(textureID >= 0)
			{
				// Get the texture resource matched with the given ID
				auto textureNode = m_importedScene->findTexture(textureID);

				// Try to find the requested JSON node
				const auto &texturePathNode = textureNode->m_jsonFile.find("path");

				// Check if the node was found and retrieve the string if it was
				std::string texturePath;
				if(texturePathNode != textureNode->m_jsonFile.end())
					texturePath = texturePathNode->get<nlohmann::json::string_t>();

				// Assign the file name and path
				m_importedScene->m_materialData[materialID].m_materials[matTypeIndex].setPath(Utilities::stripFilePath(texturePath));
				m_importedScene->m_materialData[materialID].m_materials[matTypeIndex].setFilename(Utilities::stripFilename(texturePath));
			}
			else
				m_importedScene->m_materialData[materialID].m_materials[matTypeIndex].setIsPresent(false);
		}
	}
}
