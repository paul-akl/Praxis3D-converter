#pragma once

#include "exodusImporter.h"
#include "PraxisExporter.h"

class SceneConverter
{
public:
	SceneConverter()
	{

	}
	~SceneConverter() 
	{ 

	}
	
	const std::string &getAssetDirectory(const std::string &p_importLocation)
	{
		m_importer.setSceneFilename(p_importLocation);
		return m_importer.getAssetDirectory();
	}

	void convert(const std::string &p_importLocation, const std::string &p_exportLocation)
	{
		Logger::logProgress(0);

		m_importer.setSceneFilename(p_importLocation);
		m_importer.importScene();
		auto scene = m_importer.getScene();
		
		Logger::logProgress(50);

		m_exporter.exportScene(scene, p_exportLocation);

		Logger::logProgress(100);
	}
	void convert(const std::string &p_importLocation, const std::string &p_exportLocation, const std::string &p_assetLocation)
	{
		m_importer.setAssetDirectory(p_assetLocation);
		convert(p_importLocation, p_exportLocation);
	}

private:
	exodusImporter m_importer;
	PraxisExporter m_exporter;
};

