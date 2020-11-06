#include "ModelLoader.h"
#include "Utilities.h"

DEFINE_ENUM(ModelExportType, MODELEXPORTTYPES)

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
	// Cleanup all models
	for(decltype(m_models.size()) size = m_models.size(), i = 0; i < size; i++)
		if(m_models[i] != nullptr)
			delete m_models[i];
}

Model *ModelLoader::loadModel(MeshData &p_mesh)
{
	Model *model = nullptr;

	if(p_mesh.m_numVertex > 0 && p_mesh.m_subMeshes.size() > 0)
	{
		// Create a model and save it in the models array
		model = new Model();
		m_models.push_back(model);

		// Assign variables
		model->m_assimpScene = new aiScene();
		model->m_assimpScene->mMeshes = new aiMesh*[p_mesh.m_subMeshes.size()];
		model->m_assimpScene->mNumMeshes = p_mesh.m_subMeshes.size();
		model->m_assimpScene->mNumMaterials = p_mesh.m_numMaterials;
		
		// Create vector arrays
		aiVector3D *vertices = nullptr;
		aiVector3D *normals = nullptr;
		aiVector3D *tangents = nullptr;

		// Check if there are materials present
		if(p_mesh.m_materials.size() > 0)
		{
			// Create the material array
			model->m_assimpScene->mMaterials = new aiMaterial*[p_mesh.m_materials.size()];

			// Loop over all materials
			for(decltype(p_mesh.m_materials.size()) size = p_mesh.m_materials.size(), i = 0; i < size; i++)
			{
				// Create a new Assimp material
				model->m_assimpScene->mMaterials[i] = new aiMaterial();

				// Loop over all texture types within the material
				for(std::size_t matTypeIndex = 0; matTypeIndex < MaterialType::numOfMaterialTypes; matTypeIndex++)
				{
					// Go over each type of texture, check if it exists, add it with an appropriate MATKEY
					switch(matTypeIndex)
					{
					case albedoTex:
						if(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_isPresent)
							model->m_assimpScene->mMaterials[i]->AddProperty(&aiString(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_filename), AI_MATKEY_TEXTURE_DIFFUSE(0));
						break;
					case specularTex:
						if(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_isPresent)
							model->m_assimpScene->mMaterials[i]->AddProperty(&aiString(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_filename), AI_MATKEY_TEXTURE_SPECULAR(0));
						break;
					case metallicTex:
						if(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_isPresent)
							model->m_assimpScene->mMaterials[i]->AddProperty(&aiString(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_filename), AI_MATKEY_TEXTURE_SHININESS(0));
						break;
					case normalMapTex:
						if(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_isPresent)
							model->m_assimpScene->mMaterials[i]->AddProperty(&aiString(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_filename), AI_MATKEY_TEXTURE_NORMALS(0));
						break;
					case occlusionTex:
						if(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_isPresent)
							model->m_assimpScene->mMaterials[i]->AddProperty(&aiString(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_filename), AI_MATKEY_TEXTURE_AMBIENT(0));
						break;
					case parallaxTex:
						if(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_isPresent)
							model->m_assimpScene->mMaterials[i]->AddProperty(&aiString(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_filename), AI_MATKEY_TEXTURE_HEIGHT(0));
						break;
					case emissionTex:
						if(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_isPresent)
							model->m_assimpScene->mMaterials[i]->AddProperty(&aiString(p_mesh.m_materials[i]->m_materials[matTypeIndex].m_filename), AI_MATKEY_TEXTURE_EMISSIVE(0));
						break;
					}
				}
			}
		}

		// If there are position vertices present
		if(p_mesh.m_verts.size() > 0)
		{
			// Create a vertex array of the appropriate size
			vertices = new aiVector3D[p_mesh.m_numVertex];

			// Go over each vertex and assign it to the Assimp vertex data type
			for(decltype(p_mesh.m_numVertex) i = 0; i < p_mesh.m_numVertex; i++)
			{
				vertices[i].x = p_mesh.m_verts[i].x;
				vertices[i].y = p_mesh.m_verts[i].y;
				vertices[i].z = p_mesh.m_verts[i].z;
			}
		}
		
		// If there are normals present
		if(p_mesh.m_normals.size() > 0)
		{
			// Create a vertex array of the appropriate size
			normals = new aiVector3D[p_mesh.m_numVertex];
			
			// Go over each vertex and assign it to the Assimp vertex data type
			for(decltype(p_mesh.m_numVertex) i = 0; i < p_mesh.m_numVertex; i++)
			{
				normals[i].x = p_mesh.m_normals[i].x;
				normals[i].y = p_mesh.m_normals[i].y;
				normals[i].z = p_mesh.m_normals[i].z;
			}
		}
		
		// If there are tangents present
		if(p_mesh.m_tangents.size() > 0)
		{
			// Create a vertex array of the appropriate size
			tangents = new aiVector3D[p_mesh.m_numVertex];
			
			// Go over each vertex and assign it to the Assimp vertex data type
			for(decltype(p_mesh.m_numVertex) i = 0; i < p_mesh.m_numVertex; i++)
			{
				tangents[i].x = p_mesh.m_tangents[i].x;
				tangents[i].y = p_mesh.m_tangents[i].y;
				tangents[i].z = p_mesh.m_tangents[i].z;
			}
		}
		
		// Create, initialize the root node, and mesh index array inside of it
		model->m_assimpScene->mRootNode = new aiNode();
		model->m_assimpScene->mRootNode->mParent = NULL;
		model->m_assimpScene->mRootNode->mNumMeshes = model->m_assimpScene->mNumMeshes;
		model->m_assimpScene->mRootNode->mMeshes = new unsigned int[model->m_assimpScene->mRootNode->mNumMeshes];

		// Go over each mesh
		for(decltype(model->m_assimpScene->mNumMeshes) i = 0, size = model->m_assimpScene->mNumMeshes; i < size; i++)
		{
			// Create a mesh and define its properties
			model->m_assimpScene->mMeshes[i] = new aiMesh();
			model->m_assimpScene->mMeshes[i]->mNumVertices = p_mesh.m_numVertex;
			model->m_assimpScene->mMeshes[i]->mName.Set(p_mesh.m_uniqueName);
			model->m_assimpScene->mMeshes[i]->mPrimitiveTypes = aiPrimitiveType::aiPrimitiveType_TRIANGLE;
			model->m_assimpScene->mMeshes[i]->mMaterialIndex = i;

			// Set the mesh index in the root node
			model->m_assimpScene->mRootNode->mMeshes[i] = i;

			// Check the presence of each vertex array, before assigning it
			if(vertices != nullptr)
				model->m_assimpScene->mMeshes[i]->mVertices = vertices;
			if(normals != nullptr)
				model->m_assimpScene->mMeshes[i]->mNormals = normals;
			if(tangents != nullptr)
				model->m_assimpScene->mMeshes[i]->mTangents = tangents;

			// If there are sub-meshes present
			if(p_mesh.m_subMeshes[i].size() > 0)
			{
				// Create a sub-mesh (polygon) faces array and set the number of (polygon) faces
				model->m_assimpScene->mMeshes[i]->mFaces = new aiFace[p_mesh.m_subMeshes[i].size()];
				model->m_assimpScene->mMeshes[i]->mNumFaces = p_mesh.m_subMeshes[i].size();

				// Go over each sub-mesh
				for(decltype(p_mesh.m_subMeshes[i].size()) j = 0, size = p_mesh.m_subMeshes[i].size(); j < size; j++)
				{
					// Create the face index array (3 in size - because it's a polygon) and set the number of index
					model->m_assimpScene->mMeshes[i]->mFaces[j].mIndices = new unsigned int[3];
					model->m_assimpScene->mMeshes[i]->mFaces[j].mNumIndices = 3;

					// Set the vertex coordinates of the face
					model->m_assimpScene->mMeshes[i]->mFaces[j].mIndices[0] = p_mesh.m_subMeshes[i][j].x;
					model->m_assimpScene->mMeshes[i]->mFaces[j].mIndices[1] = p_mesh.m_subMeshes[i][j].y;
					model->m_assimpScene->mMeshes[i]->mFaces[j].mIndices[2] = p_mesh.m_subMeshes[i][j].z;
				}
			}
		}
	}

	// Return the model, nullptr if loading failed
	return model;
}

bool ModelLoader::saveModel(Model *p_model, std::string &p_pathAndFilename, ModelExportType p_fileType)
{
	// Check if model is valid and filename is present
	if(p_model != nullptr && !p_pathAndFilename.empty())
	{
		// Export the model to file
		m_exporter.Export(p_model->m_assimpScene, GetString(p_fileType), p_pathAndFilename);

		return true;
	}
	return false;
}
