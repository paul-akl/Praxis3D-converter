
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>
#include <utility>

#include "Logger.h"
#include "Filesystem.h"
#include "PraxisExporter.h"
#include "Utilities.h"

// Static struct declaration
PraxisExporter::FolderNames PraxisExporter::m_folderName;

PraxisExporter::PraxisExporter()
{
	m_modelExportType = ModelExportType::obj;
	m_textureExportFormat = TextureExportFormatType::tiff;
}

PraxisExporter::~PraxisExporter()
{
}

void PraxisExporter::exportScene(Scene &p_scene, std::string p_exportLocation)
{
	// Define values used to set the progress bar value for exporting process
	int progressValueLast = 50;
	int progressValueStart = 0;
	int progressValueObjects = 20;
	int progressValueExportTextures = 8;
	int progressValueCopyModels = 8;
	int progressValueExportModels = 8;
	int progressValueEnd = 4;
	
	// Log progress (needed for a progress bar)
	Logger::logProgress(progressValueLast + progressValueStart);
	progressValueLast += progressValueStart;

	// Make sure the export scene is empty
	if(!m_exportScene.empty())
		m_exportScene.clear();

	// Declarations of variables
	std::string exportLocation = p_exportLocation + p_scene.m_sceneName + "\\";
	std::string exportMapsLocation = exportLocation + m_folderName.m_maps + "\\";
	std::string exportMaterialsLocation = exportLocation + m_folderName.m_materials + "\\";
	std::string exportModelsLocation = exportLocation + m_folderName.m_models + "\\";
	std::string exportShadersLocation = exportLocation + m_folderName.m_shaders + "\\";
	std::string sceneFilename = p_scene.m_sceneName + ".pmap";
	std::string modelExportDirectory;// = p_scene.m_sceneName + "\\";

	std::string emptyVector = "0.0f, 0.0f, 0.0f";
	std::string fullVector = "1.0f, 1.0f, 1.0f";
	std::string upDirectionVector = "0.0f, 1.0f, 0.0f";
	std::string emptyFloat = "0.0f";
	std::string oneFloat = "1.0f";

	std::vector<std::string> modelsToCopy;
	std::vector<std::string> texturesToCopy;
	std::vector<std::pair<Texture*, std::string>> texturesToExport;
	std::vector<std::pair<Model*, std::string>> modelsToExport;

	// The start of the export JSON file
	// TODO: assign values based on the size of the given scene
	m_exportScene["LoadInBackground"] = "0";
	m_exportScene["Graphics"]["Scene"]["ModelPoolSize"] = "30";
	m_exportScene["Graphics"]["Scene"]["ShaderPoolSize"] = "10";
	m_exportScene["Graphics"]["Scene"]["PointLightPoolSize"] = "20";
	m_exportScene["Graphics"]["Scene"]["SpotLightPoolSize"] = "20";

	m_exportScene["Graphics"]["Objects"] = nlohmann::json::array();
	
	auto objectsArray = p_scene.m_scene.m_jsonFile.find("objects");
	if(objectsArray != p_scene.m_scene.m_jsonFile.end() && objectsArray->is_array())
	{
		for(decltype(objectsArray->size()) size = objectsArray->size(), i = 0; i < size; i++)
		{
			switch(getObjectType(objectsArray->at(i)))
			{
			case PraxisExporter::objectType::objectType_renderer:
			{
				auto rendererNode = objectsArray->at(i).find("renderer");

				Logger::log("INFO: object index \"" + Utilities::toString(i) + "\": renderer found - " + objectsArray->at(i).find("name")->dump());

				int meshID = static_cast<int>(objectsArray->at(i).find("mesh")->get<nlohmann::json::number_integer_t>());

				auto mesh = p_scene.findMesh(meshID);
				if(mesh != nullptr)
				{
					Logger::log("INFO: mesh ID \"" + Utilities::toString(meshID) + "\": mesh found - " + mesh->m_filename);

					std::string modelFilename;
					std::string name = "object" + Utilities::toString(meshID);
					std::string position = emptyVector;
					std::string rotation = emptyVector;
					std::string scale = emptyVector;

					auto nameNode = mesh->m_jsonFile.find("name");
					auto modelNameNode = mesh->m_jsonFile.find("path");
					auto positionNode = objectsArray->at(i).find("localPosition");
					auto rotationNode = objectsArray->at(i).find("localRotation");
					auto scaleNode = objectsArray->at(i).find("localScale");

					if(modelNameNode != mesh->m_jsonFile.end())
						modelFilename = modelNameNode->get<nlohmann::json::string_t>();
					if(nameNode != mesh->m_jsonFile.end())
						name = nameNode->get<nlohmann::json::string_t>();
					if(positionNode != objectsArray->at(i).end())
						position = getStringVector(*positionNode);
					if(rotationNode != objectsArray->at(i).end())
						rotation = getStringVector(*rotationNode);
					if(scaleNode != objectsArray->at(i).end())
						scale = getStringVector(*scaleNode);

					nlohmann::json modelObject = 
					{
						{"Type", "ModelObject"}, 
						{"Name", name},
						{"Position", position},
						{"Rotation", rotation},
						{"OffsetPosition", "0.0f, 0.0f, 0.0f"},
						{"OffsetRotation", "0.0f, 0.0f, 0.0f"},
						{"Scale", scale},
						{"AlphaThreshold", "0.0f"},
						{"HeightScale", "0.0f"},
						{"Lighting", "1"},
						{"TextureTilingFactor", "1.0f"}
					};

					// Check if the object has a model
					if(!modelFilename.empty())
					{
						// Convert the directory separators to backslash
						modelFilename = Utilities::slashToBackslash(modelFilename);

						// Check if the model exists in the asset directory
						if(Filesystem::exists(p_scene.m_assetsRootDirectory + modelFilename))
						{
							// Save the model to be copied later
							modelsToCopy.push_back(p_scene.m_assetsRootDirectory + modelFilename);
						}
						else
						{
							// Load the model from the mesh data acquired from the mesh JSON file
							auto model = m_modelLoader.loadModel(*getMeshData(p_scene, mesh->m_jsonFile));
							if(model != nullptr)
							{
								// Get the filename of the model without the path and file extension
								std::string exportModelFilename = Utilities::removeExtension(Utilities::stripFilename(modelFilename));

								// Add the appropriate file extension of the model export format
								exportModelFilename = exportModelFilename + "." + GetString(m_modelExportType);

								// Save the model to be exported
								modelsToExport.push_back(std::make_pair(model, exportModelFilename));

								// Set the filename of the to-be-exported model
								modelFilename = modelExportDirectory + exportModelFilename;
							}
							else
							{
								// No valid model data present
								modelFilename.clear();
							}
						}
						
						// Check if the object has a model again, as in some cases the filename might be cleared
						if(!modelFilename.empty())
							modelObject["Models"]["Filename"] = modelFilename;
					}

					// If the renderer node is a valid size array
					if(rendererNode->is_array() && rendererNode->size() > 0)
					{
						// Get the materials array node and check if it is valid
						auto materialsNode = rendererNode->at(0).find("materials");
						if(materialsNode != rendererNode->at(0).end() && !materialsNode->empty() && materialsNode->is_array())
						{
							// Create a materials data structure in the JSON file
							nlohmann::json materialsJSONObject;
							materialsJSONObject["Diffuse"] = nlohmann::json::array();
							materialsJSONObject["Normal"] = nlohmann::json::array();
							materialsJSONObject["Emissive"] = nlohmann::json::array();
							materialsJSONObject["RMHAO"] = nlohmann::json::array();

							// Go over each material
							for(decltype(materialsNode->size()) size = materialsNode->size(), i = 0; i < size; i++)
							{
								// Get material ID
								int materialID = static_cast<int>(materialsNode->at(i).get<nlohmann::json::number_integer_t>());

								// Get the material itself that corresponds to its ID
								auto material = p_scene.findMaterial(materialID);
								if(material != nullptr)
								{
									std::string textureFilename;
									const int materialIndex = static_cast<int>(i);
									
									// Get the JSON file of the material node
									auto& materialNode = material->m_jsonFile;

									// Create a RMHAO texture in advance
									RMHAOTexture RMHAOtexture(name + "_RMHAO");

									// Track if the RMHAO texture is present
									bool RMHAOtexturePresent = false;

									// Check if the diffuse texture is present; if it is add it to the list of textures to copy
									auto albedoTextureNode = processMaterial(p_scene, materialNode, "albedoTex", materialIndex, textureFilename);
									if(!albedoTextureNode.empty())
									{
										materialsJSONObject["Diffuse"].push_back(albedoTextureNode);

										texturesToCopy.push_back(Utilities::slashToBackslash(textureFilename));
									}
									
									// Check if the normal texture is present; if it is add it to the list of textures to copy
									auto normalTextureNode = processMaterial(p_scene, materialNode, "normalMapTex", materialIndex, textureFilename);
									if(!normalTextureNode.empty())
									{
										materialsJSONObject["Normal"].push_back(normalTextureNode);
										texturesToCopy.push_back(Utilities::slashToBackslash(textureFilename));
									}
									
									// Check if the emissive texture is present; if it is add it to the list of textures to copy
									auto emissionTextureNode = processMaterial(p_scene, materialNode, "emissionTex", materialIndex, textureFilename);
									if(!emissionTextureNode.empty())
									{
										materialsJSONObject["Emissive"].push_back(emissionTextureNode);
										texturesToCopy.push_back(Utilities::slashToBackslash(textureFilename));
									}
									
									// Check if the roughness texture is present; if it is add it to the RMHAO texture because this texture is part of combined RMHAO texture
									auto roughnessTextureNode = processMaterial(p_scene, materialNode, "specularTex", materialIndex, textureFilename);
									if(!roughnessTextureNode.empty())
									{
										// Check if the texture filename is valid
										if(!textureFilename.empty())
										{
											// Part of the RMHAO texture is present
											RMHAOtexturePresent = true;

											// Get the converted filename
											RMHAOtexture.m_roughnessTexFilename = Utilities::slashToBackslash(textureFilename);

											// Check if the texture is present in the asset directory
											if(Filesystem::exists(p_scene.m_assetsRootDirectory + RMHAOtexture.m_roughnessTexFilename))
											{
												RMHAOtexture.m_roughnessTexFilename = p_scene.m_assetsRootDirectory + RMHAOtexture.m_roughnessTexFilename;
											}
											else
											{
												// Check if the texture is present in the alternate asset directory
												if(Filesystem::exists(p_scene.m_assetsRootDirectoryAlternative + RMHAOtexture.m_roughnessTexFilename))
												{
													RMHAOtexture.m_roughnessTexFilename = p_scene.m_assetsRootDirectoryAlternative + RMHAOtexture.m_roughnessTexFilename;
												}
												else
												{
													// This part of the RMHAO texture could not be found
													RMHAOtexture.m_roughnessTexFilename.clear();
													RMHAOtexturePresent = false;
												}
											}
										}
									}
									
									// Check if the metalness texture is present; if it is add it to the RMHAO texture because this texture is part of combined RMHAO texture
									auto metalicTextureNode = processMaterial(p_scene, materialNode, "metallicTex", materialIndex, textureFilename);
									if(!metalicTextureNode.empty())
									{
										// Check if the texture filename is valid
										if(!textureFilename.empty())
										{
											// Part of the RMHAO texture is present
											RMHAOtexturePresent = true;

											// Get the converted filename
											RMHAOtexture.m_metallicTexFilename = Utilities::slashToBackslash(textureFilename);

											// Check if the texture is present in the asset directory
											if(Filesystem::exists(p_scene.m_assetsRootDirectory + RMHAOtexture.m_metallicTexFilename))
											{
												RMHAOtexture.m_metallicTexFilename = p_scene.m_assetsRootDirectory + RMHAOtexture.m_metallicTexFilename;
											}
											else
											{
												// Check if the texture is present in the alternate asset directory
												if(Filesystem::exists(p_scene.m_assetsRootDirectoryAlternative + RMHAOtexture.m_metallicTexFilename))
												{
													RMHAOtexture.m_metallicTexFilename = p_scene.m_assetsRootDirectoryAlternative + RMHAOtexture.m_metallicTexFilename;
												}
												else
												{
													// This part of the RMHAO texture could not be found
													RMHAOtexture.m_metallicTexFilename.clear();
													RMHAOtexturePresent = false;
												}
											}
										}
									}
									
									// Check if the height map texture is present; if it is add it to the RMHAO texture because this texture is part of combined RMHAO texture
									auto heightMapTextureNode = processMaterial(p_scene, materialNode, "parallaxTex", materialIndex, textureFilename);
									if(!heightMapTextureNode.empty())
									{										
										// Check if the texture filename is valid
										if(!textureFilename.empty())
										{
											// Part of the RMHAO texture is present
											RMHAOtexturePresent = true;

											// Get the converted filename
											RMHAOtexture.m_heightMapTexFilename = Utilities::slashToBackslash(textureFilename);

											// Check if the texture is present in the asset directory
											if(Filesystem::exists(p_scene.m_assetsRootDirectory + RMHAOtexture.m_heightMapTexFilename))
											{
												RMHAOtexture.m_heightMapTexFilename = p_scene.m_assetsRootDirectory + RMHAOtexture.m_heightMapTexFilename;
											}
											else
											{
												// Check if the texture is present in the alternate asset directory
												if(Filesystem::exists(p_scene.m_assetsRootDirectoryAlternative + RMHAOtexture.m_heightMapTexFilename))
												{
													RMHAOtexture.m_heightMapTexFilename = p_scene.m_assetsRootDirectoryAlternative + RMHAOtexture.m_heightMapTexFilename;
												}
												else
												{
													// This part of the RMHAO texture could not be found
													RMHAOtexture.m_heightMapTexFilename.clear();
													RMHAOtexturePresent = false;
												}
											}
										}
									}
									
									// Check if the ambient occlusion texture is present; if it is add it to the RMHAO texture because this texture is part of combined RMHAO texture
									auto ambientOcclTextureNode = processMaterial(p_scene, materialNode, "occlusionTex", materialIndex, textureFilename);
									if(!ambientOcclTextureNode.empty())
									{										
										// Check if the texture filename is valid
										if(!textureFilename.empty())
										{
											// Part of the RMHAO texture is present
											RMHAOtexturePresent = true;

											// Get the converted filename
											RMHAOtexture.m_ambientOclTexFilename = Utilities::slashToBackslash(textureFilename);

											// Check if the texture is present in the asset directory
											if(Filesystem::exists(p_scene.m_assetsRootDirectory + RMHAOtexture.m_ambientOclTexFilename))
											{
												RMHAOtexture.m_ambientOclTexFilename = p_scene.m_assetsRootDirectory + RMHAOtexture.m_ambientOclTexFilename;
											}
											else
											{
												// Check if the texture is present in the alternate asset directory
												if(Filesystem::exists(p_scene.m_assetsRootDirectoryAlternative + RMHAOtexture.m_ambientOclTexFilename))
												{
													RMHAOtexture.m_ambientOclTexFilename = p_scene.m_assetsRootDirectoryAlternative + RMHAOtexture.m_ambientOclTexFilename;
												}
												else
												{
													// This part of the RMHAO texture could not be found
													RMHAOtexture.m_ambientOclTexFilename.clear();
													RMHAOtexturePresent = false;
												}
											}
										}
									}

									if(RMHAOtexturePresent)
									{
										//RMHAOtexturesToCopy.push_back(RMHAOtexture);
										auto RMHAOtextureNode = createMaterialNode(RMHAOtexture.m_RMHAOfilename, materialIndex);
										materialsJSONObject["RMHAO"].push_back(RMHAOtextureNode);



										auto RMHAOLoadedTexture = m_textureLoader.loadCombinedTexture(RMHAOtexture.m_roughnessTexFilename, RMHAOtexture.m_metallicTexFilename, RMHAOtexture.m_heightMapTexFilename, RMHAOtexture.m_ambientOclTexFilename);
										if(RMHAOLoadedTexture != nullptr)
										{
											texturesToExport.push_back(std::make_pair(RMHAOLoadedTexture, RMHAOtexture.m_RMHAOfilename));
										}
									}
								}
							}
							modelObject["Materials"].push_back(materialsJSONObject);
						}
					}
				
					m_exportScene["Graphics"]["Objects"].push_back(modelObject);
				}
				else
				{
					
				}
			}
				break;
			case PraxisExporter::objectType::objectType_light:
			{
				auto lightNode = objectsArray->at(i).find("light");
				
				std::vector<nlohmann::json> jsonLightObjects;
				bool directionLightUsed = false;

				for(decltype(lightNode->size()) size = lightNode->size(), lightIndex = 0; lightIndex < size; lightIndex++)
				{
					auto lightTypeNode = lightNode->at(lightIndex).find("type");
					if(lightTypeNode != lightNode->at(lightIndex).end())
					{
						switch(getLightType(lightNode->at(lightIndex)))
						{
						case PraxisExporter::lightType::lightType_directional:
						{
							if(!directionLightUsed)
							{
								directionLightUsed = true;
								auto name = getString(objectsArray->at(i), "name") + (lightIndex != 0 ? Utilities::toString(lightIndex) : "");
								if(name.empty())
									name = "DirectionalLight " + getString(objectsArray->at(i), "id") +
									(lightIndex != 0 ? Utilities::toString(lightIndex) : "");

								std::string color = fullVector;
								std::string rotation = upDirectionVector;
								std::string intensity = oneFloat;

								auto rotationNode = objectsArray->at(i).find("localRotation");
								auto colorNode = lightNode->find("color");
								auto intensityNode = lightNode->find("intensity");

								if(rotationNode != objectsArray->at(i).end())
									rotation = getStringVector(*rotationNode);
								if(colorNode != lightNode->end())
									color = getStringVector(*colorNode);
								if(intensityNode != lightNode->end())
									intensity = getStringFloat(*intensityNode);

								jsonLightObjects.push_back(
									nlohmann::json{
										{"Type", "DirectionalLight"},
										{"Name", name},
										{"Color", color},
										{"Direction", rotation},
										{"Intensity", intensity},
									});
							}
						}
						break;

						case PraxisExporter::lightType::lightType_point:
						{
							auto name = getString(objectsArray->at(i), "name") + (lightIndex != 0 ? Utilities::toString(lightIndex) : "");
							if(name.empty())
								name = "PointLight " + getString(objectsArray->at(i), "id") +
								(lightIndex != 0 ? Utilities::toString(lightIndex) : "");

							std::string attenuation = emptyVector;
							std::string color = fullVector;
							std::string intensity = oneFloat;
							std::string offsetPosition = emptyVector;
							std::string position = emptyVector;
							
							auto colorNode = lightNode->find("color");
							auto intensityNode = lightNode->find("intensity");
							auto positionNode = objectsArray->at(i).find("localPosition");

							if(colorNode != lightNode->end())
								color = getStringVector(*colorNode);
							if(intensityNode != lightNode->end())
								intensity = getStringFloat(*intensityNode);
							if(positionNode != objectsArray->at(i).end())
								position = getStringVector(*positionNode);

							jsonLightObjects.push_back(
							nlohmann::json{
								{"Type", "PointLight"},
								{"Name", name},
								{"Attenuation", attenuation},
								{"Color", color},
								{"Intensity", intensity},
								{"OffsetPosition", offsetPosition},
								{"Position", position}
							});
						}
						break;

						case PraxisExporter::lightType::lightType_spot:
						{
							auto name = getString(objectsArray->at(i), "name") + (lightIndex != 0 ? Utilities::toString(lightIndex) : "");
							if(name.empty())
								name = "SpotLight " + getString(objectsArray->at(i), "id") +
								(lightIndex != 0 ? Utilities::toString(lightIndex) : "");

							std::string attenuation = emptyVector;
							std::string cutoffAngle = oneFloat;
							std::string color = fullVector;
							std::string rotation = upDirectionVector;
							std::string intensity = oneFloat;
							std::string offsetPosition = emptyVector;
							std::string offsetRotation = emptyVector;
							std::string position = emptyVector;
							
							auto cutoffAngleNode = lightNode->find("spotAngle");
							auto colorNode = lightNode->find("color");
							auto rotationNode = objectsArray->at(i).find("localRotation");
							auto intensityNode = lightNode->find("intensity");
							auto positionNode = objectsArray->at(i).find("localPosition");

							if(cutoffAngleNode != lightNode->end())
								cutoffAngle = getStringVector(*cutoffAngleNode);
							if(colorNode != lightNode->end())
								color = getStringVector(*colorNode);
							if(rotationNode != objectsArray->at(i).end())
								rotation = getStringVector(*rotationNode);
							if(intensityNode != lightNode->end())
								intensity = getStringFloat(*intensityNode);
							if(positionNode != objectsArray->at(i).end())
								position = getStringVector(*positionNode);

							jsonLightObjects.push_back(
							nlohmann::json{
								{"Type", "SpotLight"},
								{"Name", name},
								{"Attenuation", attenuation},
								{"CutoffAngle", cutoffAngle},
								{"Color", color},
								{"Direction", rotation},
								{"Intensity", intensity},
								{"OffsetPosition", offsetPosition},
								{"OffsetRotation", offsetRotation},
								{"Position", position}
							});
						}
						break;

						default:
							break;
						}
					}
				}

				for(decltype(jsonLightObjects.size()) size = jsonLightObjects.size(), i = 0; i < size; i++)
				{
					m_exportScene["Graphics"]["Objects"].push_back(jsonLightObjects[i]);
				}
			}
				break;
			default:
				break;
			}

			// Log progress (needed for a progress bar)
			Logger::logProgress(progressValueLast + (i / (float)size * progressValueObjects));
		}
	}
	
	// Log progress (needed for a progress bar)
	Logger::logProgress(progressValueLast + progressValueObjects);
	progressValueLast += progressValueObjects;

	// Create export directories
	Filesystem::createDirectories(exportLocation + m_folderName.m_maps);
	Filesystem::createDirectories(exportLocation + m_folderName.m_materials);
	Filesystem::createDirectories(exportLocation + m_folderName.m_models);

	// Declare export folder location
	std::string exportFolderMaterials = exportLocation + m_folderName.m_materials + "\\";
	for(decltype(texturesToCopy.size()) i = 0, size = texturesToCopy.size(); i < size; i++)
	{
		// Check if the material exists
		std::string textureFilename = p_scene.m_assetsRootDirectory + texturesToCopy[i];
		if(Filesystem::exists(textureFilename))
		{
			// Copy the material to the export location
			Filesystem::copyFile(textureFilename, exportFolderMaterials + Utilities::stripFilename(texturesToCopy[i]));
		}
		else
		{
			// If the material does exist, try to look for it in the alternative asset location
			textureFilename = p_scene.m_assetsRootDirectoryAlternative + texturesToCopy[i];
			if(Filesystem::exists(textureFilename))
			{
				// Copy the material to the export location
				Filesystem::copyFile(textureFilename, exportFolderMaterials + Utilities::stripFilename(texturesToCopy[i]));
			}
		}
	}
	
	// Export textures that can't be copied (like RMHAO combined textures, that have to be generated)
	for(decltype(texturesToExport.size()) i = 0, size = texturesToExport.size(); i < size; i++)
	{
		std::string exportTextureFilename = exportLocation + m_folderName.m_materials + "\\" + texturesToExport[i].second;
		m_textureLoader.saveTexture(texturesToExport[i].first, exportTextureFilename, m_textureExportFormat);
		
		// Log progress (needed for a progress bar)
		Logger::logProgress(progressValueLast + (i / (float)size * progressValueExportTextures));
	}
	
	// Log progress (needed for a progress bar)
	Logger::logProgress(progressValueLast + progressValueExportTextures);
	progressValueLast += progressValueExportTextures;

	// Copy models that are present in the assets directory to the export location
	for(decltype(modelsToCopy.size()) i = 0, size = modelsToCopy.size(); i < size; i++)
	{
		Filesystem::copyFile(modelsToCopy[i], exportLocation + m_folderName.m_models + "\\" + Utilities::stripFilename(modelsToCopy[i]));

		// Log progress (needed for a progress bar)
		Logger::logProgress(progressValueLast + (i / (float)size * progressValueCopyModels));
	}
	
	// Log progress (needed for a progress bar)
	Logger::logProgress(progressValueLast + progressValueCopyModels);
	progressValueLast += progressValueCopyModels;

	// Export models that were not present in the assets directory, from the JSON files to the export location
	for(decltype(modelsToExport.size()) i = 0, size = modelsToExport.size(); i < size; i++)
	{
		std::string exportModelFilename = exportLocation + m_folderName.m_models + "\\" + modelsToExport[i].second;
		m_modelLoader.saveModel(modelsToExport[i].first, exportModelFilename, m_modelExportType);
		
		// Log progress (needed for a progress bar)
		Logger::logProgress(progressValueLast + (i / (float)size * progressValueExportModels));
	}
	
	// Log progress (needed for a progress bar)
	Logger::logProgress(progressValueLast + progressValueExportModels);
	progressValueLast += progressValueExportModels;

	if(Filesystem::writeToFile(m_exportScene.dump(4), exportLocation + m_folderName.m_maps + "\\" + sceneFilename))
		Logger::log("#######################################\r\nINFO: " + sceneFilename + " - write successful.\r\n#######################################");
	else
		Logger::log("#######################################\r\nINFO: " + sceneFilename + " - write failed.\r\n#######################################");
	
	// Log progress (needed for a progress bar)
	Logger::logProgress(progressValueLast + progressValueEnd);
}

std::string PraxisExporter::getStringVector(const nlohmann::json &p_jsonVector)
{
	// Initialize an 'empty' string
	std::string returnString = "0.0f, 0.0f, 0.0f";

	// Try to get each vectors variable
	const auto x = p_jsonVector.find("x");
	const auto y = p_jsonVector.find("y");
	const auto z = p_jsonVector.find("z");

	// Check if all the values were found
	if(x != p_jsonVector.end() && y != p_jsonVector.end() && z != p_jsonVector.end())
	{
		// Construct the string with appropriate punctuation
		returnString =	Utilities::toStringCpp(static_cast<float>(x->get<nlohmann::json::number_float_t>())) + ", " +
						Utilities::toStringCpp(static_cast<float>(y->get<nlohmann::json::number_float_t>())) + ", " +
						Utilities::toStringCpp(static_cast<float>(z->get<nlohmann::json::number_float_t>()));
	}

	return returnString;
}

std::string PraxisExporter::getStringFloat(const nlohmann::json &p_jsonVector)
{
	// Initialize an 'empty' float string
	std::string returnString = "0.0f";

	// Check if the given node is a number
	if(p_jsonVector.is_number())
	{
		// Get the value (retrieved as a double)
		const auto value = p_jsonVector.get<nlohmann::json::number_float_t>();

		// Cast the value to float and convert to CPP-styled string (i.e. '1.0f')
		returnString = Utilities::toStringCpp(static_cast<float>(value));
	}

	return returnString;
}

std::string PraxisExporter::getString(const nlohmann::json& p_node, const std::string& p_searchKeyword)
{
	std::string returnString;

	// Check if arguments are all valid
	if(!p_node.is_null() && !p_node.empty() && !p_searchKeyword.empty())
	{
		// Try to find the requested JSON node
		const auto& foundNode = p_node.find(p_searchKeyword);

		// Check if the node was found and retrieve the string if it was
		if(foundNode != p_node.end())// && !foundNode->empty())
			returnString = foundNode->get<nlohmann::json::string_t>();
	}

	return returnString;
}

PraxisExporter::objectType PraxisExporter::getObjectType(const nlohmann::json &p_object)
{
	// Check if the object has a renderer node (meaning it is of the model object type, i.e. renderer)
	if(p_object.find("renderer") != p_object.end())
		return PraxisExporter::objectType::objectType_renderer;
	
	// Check if the object has a light node, in which case is it a light object
	if(p_object.find("light") != p_object.end())
		return PraxisExporter::objectType::objectType_light;

	return objectType::objectType_null;
}

PraxisExporter::lightType PraxisExporter::getLightType(const nlohmann::json &p_light)
{
	// Get the string of light type node
	auto lightTypeString = getString(p_light, "type");

	// Check the string against all types of lights, starting from the most common type.
	// If light type is matched, returned the equivalent enum type

	if(lightTypeString == "Spot")
		return PraxisExporter::lightType::lightType_spot;

	if(lightTypeString == "Point")
		return PraxisExporter::lightType::lightType_point;

	if(lightTypeString == "Directional")
		return PraxisExporter::lightType::lightType_directional;

	// If this point is reached, no valid light type has been determined; return a null light type
	return PraxisExporter::lightType::lightType_null;
}

nlohmann::json PraxisExporter::processMaterial(const Scene &p_scene, const nlohmann::json& p_materialNode, const std::string& p_materialName, const int p_materialIndex, std::string &p_returnRelativeFilename)
{					
	// Declare an empty return object
	nlohmann::json materialNode;

	// Clear the relative filename, as it might not be empty, and if the texture is not found, it can be left with previous data
	p_returnRelativeFilename.clear();

	// Try to find the node of a specific type of material, and check if it exists / is not empty
	auto textureIt = p_materialNode.find(p_materialName);
	if(textureIt != p_materialNode.end() && !textureIt->empty())
	{
		// Get the texture index (for matching texture with a mesh) as an integer
		int textureIDint = static_cast<int>(textureIt->get<nlohmann::json::number_integer_t>());
		
		auto textureResource = p_scene.findTexture(textureIDint);

		// Look for a specific texture that the texture ID is specifying
		if(textureIDint != -1 && p_scene.findTexture(textureIDint))
		{
			// Get the actual texture filename
			p_returnRelativeFilename = getString(p_scene.m_textures[textureIDint].m_jsonFile, "path");
			
			// Construct the material node to be exported
			materialNode = createMaterialNode(Utilities::stripFilename(p_returnRelativeFilename), p_materialIndex);
		}
	}

	return materialNode;
}

nlohmann::json PraxisExporter::createMaterialNode(const std::string &p_materialFilePath, const int p_materialIndex)
{
	return nlohmann::json(
			{
				{"Filename", p_materialFilePath},
				{"Index", Utilities::toString(p_materialIndex)}
			}
	);
}

MeshData *PraxisExporter::getMeshData(Scene &p_scene, const nlohmann::json &p_meshNode)
{
	// Declare variables used
	MeshData *mesh = new MeshData();

	// Get the unique mesh name; also used for showing errors
	mesh->m_uniqueName = getString(p_meshNode, "uniqueName");

	// Get the number of vertices in the mesh
	auto numberVertexNode = p_meshNode.find("vertexCount");
	if(numberVertexNode != p_meshNode.end() && numberVertexNode->is_number())
		mesh->m_numVertex = numberVertexNode->get<nlohmann::json::number_integer_t>();
	
	Logger::log("INFO: Loading mesh: \"" + mesh->m_uniqueName + "\".");

	// If there are no vertices, there is no data
	if(mesh->m_numVertex > 0)
	{
		// Get material index data
		auto materialsNode = p_meshNode.find("materials");
		if(materialsNode != p_meshNode.end() && materialsNode->is_array())
		{
			// Fill material array
			for(decltype(materialsNode->size()) size = materialsNode->size(), i = 0; i < size; i++)
			{
				// Get the material ID
				int ID = materialsNode->at(i).get<nlohmann::json::number_integer_t>();

				// Assign the material to the mesh by its ID
				mesh->m_materials.push_back(&p_scene.m_materialData[ID]);
			}

			// Set the number of materials
			mesh->m_numMaterials = mesh->m_materials.size();
		}

		// Get the vertex data
		auto vertexNode = p_meshNode.find("verts");
		if(vertexNode != p_meshNode.end() && vertexNode->is_array())
			fillMeshVectors(mesh->m_verts, *vertexNode, mesh->m_numVertex, mesh->m_uniqueName);

		// Get the normals data
		auto normalsNode = p_meshNode.find("normals");
		if(normalsNode != p_meshNode.end() && normalsNode->is_array())
			fillMeshVectors(mesh->m_normals, *normalsNode, mesh->m_numVertex, mesh->m_uniqueName);

		// Get the tangents data
		auto tangentsNode = p_meshNode.find("tangents");
		if(tangentsNode != p_meshNode.end() && tangentsNode->is_array())
			fillMeshVectors(mesh->m_tangents, *tangentsNode, mesh->m_numVertex, mesh->m_uniqueName);

		// Get the texture coordinate (uv0) data
		auto uv0Node = p_meshNode.find("uv0");
		if(uv0Node != p_meshNode.end() && uv0Node->is_array())
			fillMeshVectors(mesh->m_uv0, *uv0Node, mesh->m_numVertex, mesh->m_uniqueName);

		// Get the texture coordinate (uv1) data
		auto uv1Node = p_meshNode.find("uv1");
		if(uv1Node != p_meshNode.end() && uv1Node->is_array())
			fillMeshVectors(mesh->m_uv1, *uv1Node, mesh->m_numVertex, mesh->m_uniqueName);

		// Get the number of sub-meshes
		auto numberSubMeshesNode = p_meshNode.find("subMeshCount");
		if(numberSubMeshesNode != p_meshNode.end() && numberSubMeshesNode->is_number())
		mesh->m_numMeshes = numberSubMeshesNode->get<nlohmann::json::number_integer_t>();

		//mesh->m_numVertex = mesh->m_verts.size();

		// Check if there are any sub-meshes present
		if(mesh->m_numMeshes > 0)
		{
			auto subMesheArrayNode = p_meshNode.find("subMeshes");
			if(subMesheArrayNode != p_meshNode.end() && subMesheArrayNode->is_array())
			{
				Logger::log("INFO | " + mesh->m_uniqueName + ": Sub-meshes present.");

				// Check if the number of meshes and the mesh array size matches
				if(subMesheArrayNode->size() == mesh->m_numMeshes)
				{
					for(decltype(subMesheArrayNode->size()) size = subMesheArrayNode->size(), i = 0; i < size; i++)
					{
						auto subMeshTriangleNode = subMesheArrayNode->at(i).find("triangles");
						if(subMeshTriangleNode != subMesheArrayNode->at(i).end())
						{
							// Make sure the mesh has been initialized
							if(mesh->m_subMeshes.size() < i + 1)
								mesh->m_subMeshes.push_back(std::vector<vec3>());

							fillSubmeshVectors(mesh->m_subMeshes[i], *subMeshTriangleNode);
						}

					}
				}
				else
				{
					Logger::log("WARNING: Mesh \"" + mesh->m_uniqueName + "\" sub-mesh count mismatch.");
				}
			}

		}
	}

	return mesh;
}

void PraxisExporter::fillMeshVectors(std::vector<vec3> &p_vectors, const nlohmann::json &p_arrayNode, std::size_t p_vertCount, std::string p_name)
{
	// If the vertex count is zero, there is no data to be filled in
	if(p_vertCount > 0)
	{
		// Clear the vectors array if it isn't empty
		if(!p_vectors.empty())
			p_vectors.clear();

		// Check if the given node is an array (that holds all the vertex data)
		if(p_arrayNode.is_array())
		{
			// The size of the array should match the vertex count
			if(p_arrayNode.size() == p_vertCount * 3)
			{
				// Fill the vertex data, 3 at a time (X,Y,Z coordinates)
				for(decltype(p_arrayNode.size()) size = p_arrayNode.size(), i = 0; i < size; i += 3)
				{
					p_vectors.emplace_back(p_arrayNode.at(i).get<nlohmann::json::number_float_t>(),
										   p_arrayNode.at(i + 1).get<nlohmann::json::number_float_t>(),
										   p_arrayNode.at(i + 2).get<nlohmann::json::number_float_t>());
				}
			}
			else
			{
				// If the vertex count doesn't match and is greater than zero (i.e. some data is contained),
				// show a vertex count mismatch warning
				if(p_arrayNode.size() > 0)
				{
					//std::string meshName = p_name.empty() ? "" : " \"" + p_name + "\" ";
					Logger::log("WARNING | Mesh" + p_name.empty() ? "" : " \"" + p_name + "\" " + "vertex count mismatch.");
				}
			}
		}
	}
}

void PraxisExporter::fillSubmeshVectors(std::vector<vec3> &p_vectors, const nlohmann::json &p_arrayNode, std::string p_name)
{	
	// Clear the vectors array if it isn't empty
	if(!p_vectors.empty())
		p_vectors.clear();

	// Check if the given node is an array (that holds all the vertex data)
	if(p_arrayNode.is_array())
	{
		// Fill the vertex data, 3 at a time (X,Y,Z coordinates)
		for(decltype(p_arrayNode.size()) size = p_arrayNode.size(), i = 0; i < size; i += 3)
		{
			p_vectors.emplace_back(p_arrayNode.at(i).get<nlohmann::json::number_float_t>(),
									p_arrayNode.at(i + 1).get<nlohmann::json::number_float_t>(),
									p_arrayNode.at(i + 2).get<nlohmann::json::number_float_t>());
		}
	}
}