#pragma once

//#define DEV_MODE

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "EnumFactory.h"
#include "Logger.h"

// Material type as it is defined in the Exodus JSON files
#define MATERIALTYPES(Code) \
Code(albedoTex, = 0) \
Code(specularTex,) \
Code(metallicTex,) \
Code(normalMapTex,) \
Code(occlusionTex,) \
Code(parallaxTex,) \
Code(emissionTex,) \
Code(numOfMaterialTypes, )
DECLARE_ENUM(MaterialType, MATERIALTYPES)

// Material type as a string, as it is defined in the Exodus JSON files
struct MaterialTypeNames
{
	MaterialTypeNames()
	{
		m_names[MaterialType::albedoTex] = GetString(MaterialType::albedoTex);
		m_names[MaterialType::specularTex] = GetString(MaterialType::specularTex);
		m_names[MaterialType::metallicTex] = GetString(MaterialType::metallicTex);
		m_names[MaterialType::normalMapTex] = GetString(MaterialType::normalMapTex);
		m_names[MaterialType::occlusionTex] = GetString(MaterialType::occlusionTex);
		m_names[MaterialType::parallaxTex] = GetString(MaterialType::parallaxTex);
		m_names[MaterialType::emissionTex] = GetString(MaterialType::emissionTex);
	}

	std::string m_names[MaterialType::numOfMaterialTypes];
};

// Holds material filename, path to the file and a flag whether the material is present
struct MaterialFilename
{
	MaterialFilename() : m_isPresent(false) { }

	// Setters that also mark the material as being present
	void setPath(const std::string &p_path) 
	{ 
		m_path = p_path;
		setIsPresent(true);
	}	
	void setFilename(const std::string &p_filename) 
	{ 
		m_filename = p_filename;
		setIsPresent(true);
	}
	void setIsPresent(const bool p_present) { m_isPresent = p_present; }

	bool m_isPresent;
	std::string m_path;
	std::string m_filename;
};

// Holds an array of material filenames for every possible texture type; matched with a material ID
struct Material
{
	Material() : m_id(-1) { }
	Material(int p_id)
		: m_id(p_id) { }

	int m_id;
	MaterialFilename m_materials[MaterialType::numOfMaterialTypes];
};

// A resource corresponds to a single JSON file containing data of a single asset
struct Resource
{
	Resource() : m_id(-1) { }
	Resource(std::string p_filename, int p_id) : m_filename(p_filename), m_id(p_id) { }

	nlohmann::json m_jsonFile;
	std::string m_filename;
	int m_id;
};

// The whole scene comprised of multiple resources
struct Scene
{
	Scene() 
	{ 
		m_numMaterials = 0;
		m_numMeshes = 0;
		m_numTextures = 0;
	}
	Scene(	std::vector<Resource>::size_type p_numOfMaterials,
			std::vector<Resource>::size_type p_numOfMeshes,
			std::vector<Resource>::size_type p_numOfTextures)
	{
		m_materials.reserve(p_numOfMaterials + 1);
		m_meshes.reserve(p_numOfMeshes + 1);
		m_textures.reserve(p_numOfTextures + 1);

		m_numMaterials = p_numOfMaterials;
		m_numMeshes = p_numOfMeshes;
		m_numTextures = p_numOfTextures;
	}

	// Searches for a resource matching a given name; returns true if found; 
	const Resource *findResource(const std::vector<Resource> &p_resources, const std::string& p_name) const
	{
		for(decltype(p_resources.size()) size = p_resources.size(), i = 0; i < size; i++)
		{
			if(p_resources[i].m_filename == p_name)
			{
				return &(p_resources[i]);
			}
		}

		return nullptr;
	}

	// Searches for a resource matching a given ID; returns true if found;
	const Resource *findResource(const std::vector<Resource> &p_resources, const int p_id) const
	{
		// If ID is not out of bounds for the array, check to see if the ID matches with the index first (as it should always match)
		if(p_id < p_resources.size() && p_resources[p_id].m_id == p_id)
		{
			return &(p_resources[p_id]);
		}
		else
		{
			// If and array index didn't match, look for it by cycling through the whole array
			for(decltype(p_resources.size()) size = p_resources.size(), i = 0; i < size; i++)
			{
				if(p_resources[i].m_id == p_id)
				{
					return &(p_resources[i]);
				}
			}
		}

		// If this point is reached, the resource was not found
		return nullptr;
	}

	const Resource *findMaterial(const std::string& p_name)	const	{ return findResource(m_materials, p_name);	}
	const Resource *findMesh(const std::string &p_name) 	const	{ return findResource(m_meshes, p_name);	}
	const Resource *findTexture(const std::string &p_name) 	const	{ return findResource(m_textures, p_name);	}

	const Resource *findMesh(const int p_id) 		const { return findResource(m_meshes, p_id);	}
	const Resource *findTexture(const int p_id) 	const { return findResource(m_textures, p_id);	}
	const Resource *findMaterial(const int p_id) 	const { return findResource(m_materials, p_id);	}

	// Prefill material array, so any element in it can be accessed by index (in case the material ID and index mismatch)
	const void reserveResourceData(std::vector<Material> &p_materials, std::size_t p_size)
	{ 
		if(p_materials.size() < p_size)
			std::fill_n(std::back_inserter(p_materials), p_size - p_materials.size(), Material());
	}
	const void reserveResourceData(std::vector<Resource> &p_resources, std::size_t p_size)
	{
		if(p_resources.size() < p_size)
			std::fill_n(std::back_inserter(p_resources), p_size - p_resources.size(), Resource());
	}

	const void matchResourceIDandIndex(std::vector<Resource> &p_resources)
	{
		bool IDandIndexMissmatch = true;

		// Repeat the process until there are no more mismatches
		while(IDandIndexMissmatch)
		{
			IDandIndexMissmatch = false;
			for(decltype(p_resources.size()) size = p_resources.size(), i = 0; i < size; i++)
			{
				// If the resource ID and array index does not match
				if(p_resources[i].m_id != i)
				{
					// Make sure to not go out of bounds on the resource array
					if(p_resources[i].m_id > p_resources.size())
					{
						Logger::log("ERROR: " + p_resources[i].m_filename + " - resource ID is out of bounds (of resource array size)");
						assert(p_resources[i].m_id > p_resources.size());
					}
					else
					{
						// Switch the outsources so that the ID and array index matches
						Resource tempResource = p_resources[p_resources[i].m_id];
						p_resources[p_resources[i].m_id] = p_resources[i];
						p_resources[i] = tempResource;

						// Set a flag that a mismatch was found
						IDandIndexMissmatch = true;
					}
				}
			}
		}
	}
	const void matchResourceIDandIndex(std::vector<Material> &p_materials)
	{
		bool IDandIndexMissmatch = true;

		// Repeat the process until there are no more mismatches
		while(IDandIndexMissmatch)
		{
			IDandIndexMissmatch = false;
			for(decltype(p_materials.size()) size = p_materials.size(), i = 0; i < size; i++)
			{
				// If the resource ID and array index does not match
				if(p_materials[i].m_id != i)
				{
					// Make sure to not go out of bounds on the resource array
					if(p_materials[i].m_id > p_materials.size())
					{
						Logger::log("ERROR: " + p_materials[i].m_materials[0].m_filename + " - material ID is out of bounds (of resource array size)");
						assert(p_materials[i].m_id > p_materials.size());
					}
					else
					{
						// Switch the resources so that the ID and array index matches
						Material tempResource = p_materials[p_materials[i].m_id];
						p_materials[p_materials[i].m_id] = p_materials[i];
						p_materials[i] = tempResource;

						// Set a flag that a mismatch was found
						IDandIndexMissmatch = true;
					}
				}
			}
		}
	}

	const void allignArrayIDs()
	{
		matchResourceIDandIndex(m_materials);
		matchResourceIDandIndex(m_meshes);
		matchResourceIDandIndex(m_textures);
		matchResourceIDandIndex(m_materialData);
	}
	const void reserveArrayData()
	{
		reserveResourceData(m_materials, m_numMaterials);
		reserveResourceData(m_meshes, m_numMeshes);
		reserveResourceData(m_textures, m_numTextures);
		reserveResourceData(m_materialData, m_numMaterials);
	}

	Resource m_scene;
	std::string				m_resourceDirectory,				// Location of the scene folder containing all the resource JSON files
							m_sceneRootDirectory,				// Location of the root scene folder
							m_assetsRootDirectory,				// Location of the root folder containing assets folder
							m_assetsRootDirectoryAlternative,	// Location of the root folder containing assets folder under "!Unsupported" directory
							m_sceneName;					
	std::vector<Resource>	m_materials,
							m_meshes,
							m_textures;
	MaterialTypeNames		m_materialTypeNames;
	std::vector<Material>	m_materialData;
	std::size_t				m_numMaterials,
							m_numMeshes,
							m_numTextures;
};