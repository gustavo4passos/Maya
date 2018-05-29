#include "../include/ResourceManager.h"

TextureID ResourceManager::LoadTexture(const std::string &filename) {
    Texture* tex = new Texture(filename);
    _textureMap[tex->id()] = tex;
}

Texture* const ResourceManager::GetTexture(TextureID id) {
    return _textureMap[id];
}

void ResourceManager::DeleteTexture(TextureID id) {
    delete _textureMap[id];
    _textureMap.erase(id);
} 

void ResourceManager::CleanTextures() {
    for(std::map<TextureID, Texture*>::iterator it = _textureMap.begin(); it != _textureMap.end(); it++)
    {
        delete it->second;
    }
    _textureMap.clear();
}