#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>

#include "Texture.h"

// Texutre index
typedef unsigned int TextureID;

class ResourceManager {

public:

    // Textures  
    static TextureID LoadTexture(const std::string &filename);    // Returns 0 if unable to load texture
    inline static Texture* const GetTexture(TextureID id);    
    static void DeleteTexture(TextureID id);    
    static void CleanTextures();   

private:

    ResourceManager() {}

    static std::map<TextureID, Texture*> _textureMap;

};

#endif