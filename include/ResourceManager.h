#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>

#include "Texture.h"

class ResourceManager {

public:

    // Textures  
    static bool LoadTexture(std::string filename); // Returns false if unable to load texture
    static void DeleteTexture(const std::string& textureName);    
    static Texture* const GetTexture(const std::string& name); 
    static void CleanTextures();   

private:

    ResourceManager() {}

    static std::map<std::string, Texture*> _textureMap;

};

#endif
