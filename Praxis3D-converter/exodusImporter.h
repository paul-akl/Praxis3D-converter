#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "Containers.h"

class exodusImporter
{
public:
	exodusImporter();
	~exodusImporter();

	// Import the scene from file
	bool importScene();

	// Scene filename setter, must be called before importing the scene
	void setSceneFilename(const std::string &p_filename);

	// Set an alternative asset directory
	void setAssetDirectory(const std::string &p_assetDirectory);

	// Scene getter; returns empty scene if none have been imported
	Scene &getScene() { return m_importedScene == nullptr ? m_nullScene : *m_importedScene; }

	// Get the scene asset directory
	const std::string &getAssetDirectory() const { return m_assetDirectory; }

	// Get scene name, extracted from the filename
	const inline std::string &getSceneName() const { return m_sceneName; }

	// Get scene filename (JSON filename that the scene was loaded from)
	const inline std::string &getSceneFileName() const { return m_sceneFilename; }

private:
	void importResource(Resource &p_resource);
	void importMaterialData();

	std::string m_sceneName,
				m_sceneFilename,
				m_sceneRootDirectory,
				m_resourceDirectory,
				m_assetDirectory,
				m_alternativeAssetDirectory;
	nlohmann::json m_sceneJson;
	Scene	*m_importedScene,
			m_nullScene;
};

