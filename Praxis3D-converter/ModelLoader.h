#pragma once

#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <iostream>
#include <string>
#include <vector>

#include "Containers.h"
#include "EnumFactory.h"

#define MODELEXPORTTYPES(Code) \
Code(obj, = 0) \
Code(numOfModelExportTypes, )
DECLARE_ENUM(ModelExportType, MODELEXPORTTYPES)

// A 3D vector of floats
struct vec3
{
	vec3() : x(0.0f), y(0.0f), z(0.0f) { }
	vec3(float p_x, float p_y, float p_z) : x(p_x), y(p_y), z(p_z) { }
	float x, y, z;
};

// Holds all data related to a single mesh (including materials)
struct MeshData
{
	MeshData()
	{
		m_numVertex = 0;
		m_numMeshes = 0;
		m_numMaterials = 0;
	}

	std::string m_uniqueName;

	std::size_t m_numVertex,
				m_numMeshes,
				m_numMaterials;

	std::vector<vec3>	m_verts,
						m_normals,
						m_tangents,
						m_uv0,
						m_uv1;
	
	std::vector<const Material*> m_materials;
	std::vector<std::vector<vec3>>	m_subMeshes;
};

// Holds all model data in an Assimp scene format
// Constructor is private, so only ModelLoader can create a model
class Model
{
	friend class ModelLoader;
public:
	~Model()
	{
		//delete m_assimpScene;
	}

private:
	Model()
	{
		m_assimpScene = nullptr;
	}

	aiScene *m_assimpScene;
};

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	// Load a model from mesh data; returns a nullptr if loading failed
	Model *loadModel(MeshData &p_mesh);

	// Export a model to file; returns true if successful
	bool saveModel(Model* p_model, std::string &p_pathAndFilename, ModelExportType p_fileType);

private:
	Assimp::Exporter m_exporter;

	// Store all created models
	std::vector<Model*> m_models;
};

