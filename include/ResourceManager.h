#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include <vector>

#include <tinyxml.h>

#include "Texture.h"
#include "Tileset.h"
#include "Level.h"

class Mesh;

class ResourceManager {

public:

    // Textures  
    static bool LoadTexture(const std::string& filename, const std::string& name); // Returns false if unable to load texture
    static void DeleteTexture(const std::string& textureName);    
    static Texture* const GetTexture(const std::string& textureName); 
    static void CleanTextures();    

    // Levels
    static Level* ParseLevel(const std::string& filename); // Returns NULL if unable to load level

	//Meshes
	static bool LoadMesh(const void* data, std::size_t size, unsigned int count, const std::string& name);
	static Mesh* const GetMesh(const std::string& name);
	static void DeleteMesh(const std::string& name);
	static void CleanMeshes();

private:

    ResourceManager() {}

    static Tileset* ParseTileset(TiXmlElement* node);

    static void ParseObjectGroup(TiXmlElement* objectsNode, Level* level);
    static Rect* ParseRect(TiXmlElement* objectNode);
    static Layer* ParseLayer(TiXmlElement* layerNode, Level* level, Tileset* tileset);
    static std::vector<int> ParseLayerData(TiXmlElement* dataNode);

	// Loads a mesh to video memory, and stores a pointer to it in _meshMap
	// Use DeleteMesh(name) to free it
	static void LoadLayerMesh(std::vector<int>& layerData, Level* level, Tileset* tileset, const std::string& name);

	// Data maps
    static std::map<std::string, Texture*> _textureMap;
	static std::map<std::string, Mesh*> _meshMap;
};

#endif
