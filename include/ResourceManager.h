#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include <vector>

#include <tinyxml.h>

#include "Texture.h"
#include "Tileset.h"
#include "Level.h"


class ResourceManager {

public:

    // Textures  
    static bool LoadTexture(const std::string& filename, const std::string& name); // Returns false if unable to load texture
    static void DeleteTexture(const std::string& textureName);    
    static Texture* const GetTexture(const std::string& textureName); 
    static void CleanTextures();    

    // Levels
    static Level* ParseLevel(const std::string& filename); // Returns NULL if unable to load level

private:

    ResourceManager() {}

    static Tileset ParseTileset(TiXmlElement* node);
    static std::map<std::string, Texture*> _textureMap;

};

#endif
