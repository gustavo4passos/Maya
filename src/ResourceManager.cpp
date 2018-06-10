#include "../include/ResourceManager.h"

// NOTE(Gustavo) stb_image fails to load some png subformats; consider using SOIL 
#define STB_IMAGE_IMPLEMENTATION
#include "../extern/stb/stb_image.h"

#include "../include/ErrorHandler.h"

std::map<std::string, Texture*> ResourceManager::_textureMap;
std::map<std::string, Mesh*> ResourceManager::_meshMap;

bool ResourceManager::LoadTexture(const std::string& filename, const std::string& name) {
    
    int w, h, nChannels;
    unsigned char* textureData = stbi_load(filename.c_str(), &w, &h, &nChannels, 0);

    if(textureData == NULL) {
        LOG_ERROR("Unable to load image file: " + filename);
        return false;
    }

    Texture* tex = new Texture((const void*)textureData, w, h, nChannels);
    _textureMap[name.c_str()] = tex; 
   
    // Frees image data from RAM
    stbi_image_free(textureData);

    return true;
}


// NOTE(Gustavo) For the release version, consider returning map[entry] instead of using find() 
// However map[entry] will create a new item in map if 'entry' isn't a valid entry
// and the caller will never be notified that the index he's trying to access is invalid,
// making the error hard to trace.
// In the final version, however, this type of access error must be sorted out, and this method 
// can possibly be implemented by returning the requested entry
Texture* const ResourceManager::GetTexture(const std::string& name){
    std::map<std::string, Texture*>::const_iterator texEntry = _textureMap.find(name);
    if(texEntry == _textureMap.end()){
	    LOG_ERROR("Texture is not in texture map: " << name);
	    DEBUG_BREAK();
	    return NULL;
    }
    return texEntry->second;
}

void ResourceManager::DeleteTexture(const std::string& textureName) {
    std::map<std::string, Texture*>::iterator entry = _textureMap.find(textureName);
    if(entry == _textureMap.end()){
	    LOG_ERROR("Unable to delete texture, invalid texture name: " + textureName);
    }
    else{
	    delete entry->second;
	    _textureMap.erase(entry);
    }
} 

void ResourceManager::CleanTextures() {
    for(std::map<std::string, Texture*>::iterator it = _textureMap.begin(); it != _textureMap.end(); it++)
    {
        delete it->second;
    }
    _textureMap.clear();
}

bool ResourceManager::LoadMesh(const void* data, std::size_t size, unsigned int count, const std::string& name) {
	std::map<std::string, Mesh*>::const_iterator it = _meshMap.find(name.c_str());
	if(it != _meshMap.end()){
		LOG_ERROR("Unable to load mesh into mesh map: " + name + ". A mesh with the same name already exists.");
		DEBUG_BREAK();
		return false;
	}

	Mesh* mesh = new Mesh(data, size, count);
	_meshMap[name.c_str()] = mesh;

	return true;
}

Mesh* const ResourceManager::GetMesh(const std::string& name) {
	std::map<std::string, Mesh*>::const_iterator meshEntry = _meshMap.find(name.c_str());

	if(meshEntry == _meshMap.end()){
		LOG_ERROR("Mesh is not in mesh map: " + name);
		DEBUG_BREAK();
		return NULL;
	}
	return meshEntry->second;
}

void ResourceManager::DeleteMesh(const std::string& name){
	std::map<std::string, Mesh*>::iterator meshEntry = _meshMap.find(name.c_str());

	if(meshEntry == _meshMap.end()){
		LOG_ERROR("Unable to delete mesh: " + name + ". Mesh is not in mesh map.");
	}
	else {
		delete meshEntry->second;
		_meshMap.erase(meshEntry);
	}
}

void ResourceManager::CleanMeshes(){
	for(std::map<std::string, Mesh*>::iterator it = _meshMap.begin();
	    it != _meshMap.end(); ++it){
		delete it->second;
	}	

	_meshMap.clear();
}
		
Level* ResourceManager::ParseLevel(const std::string& filename){
    // create the XML document 
	TiXmlDocument xmlDoc;

	// load the XML document																														  
	if (!xmlDoc.LoadFile(filename)) {
        LOG_ERROR("Unable to open level file: " + std::string(xmlDoc.ErrorDesc()));
		return NULL;
	}

	// get the root element
	TiXmlElement* pRoot = xmlDoc.RootElement();

	std::vector<int> tileLayer; // ints vector that will contain the tile values

	TiXmlElement* e;

    //looping the xml file to find the tileset
	for(e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) { 
		if(e->Value() == std::string("tileset")){
			break;  
        }    
	}

    if(e == NULL){
        LOG_ERROR("Cannot find tileset node on xml file");
        return NULL;
    } 
    else {
        ParseTileset(e);
    }
    return new Level();

}

Tileset ResourceManager::ParseTileset(TiXmlElement* node){
    TiXmlElement* imagenode = node->FirstChildElement();

    std::string source;
    std::string name;

    int width, height;
	int tileWidth, tileHeight;	
    int margin, spacing;
	int nColumns, nRows;
    
    name = std::string(node->Attribute("name"));
    node->Attribute("tilewidth", &tileWidth);
    node->Attribute("tileheight", &tileHeight);
    node->Attribute("columns", &nColumns);
    node->Attribute("margin", &margin);
    node->Attribute("spacing", &spacing);
    source = std::string(imagenode->Attribute("source"));
    imagenode->Attribute("width", &width);
    imagenode->Attribute("height", &height);
    nRows = (height - 2*margin + spacing) / (tileHeight + spacing); 

    return Tileset(source, name, width, height, tileWidth, tileHeight, margin, spacing, nColumns, nRows);

}
