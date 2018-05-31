#include "../include/ResourceManager.h"

// TODO stb_image fails to load some png subformats; consider using SOIL 
#define STB_IMAGE_IMPLEMENTATION
#include "../extern/stb/stb_image.h"

#include "../include/ErrorHandler.h"

std::map<std::string, Texture*> ResourceManager::_textureMap;

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


// TODO For the release version, consider returning map[entry] instead of using find() 
// However map[entry] will create a new item in map if 'entry' isn't a valid entry
// and the caller will never be notified that the index he's trying to access is invalid,
// making the error hard to trace.
// In the final version, however, this type of access error must be sorted out, and this method 
// can possibly be implemented by returning the requested entry
Texture* const ResourceManager::GetTexture(const std::string& name){
    std::map<std::string, Texture*>::const_iterator texEntry = _textureMap.find(name);
    if(texEntry == _textureMap.end()){
	    LOG_ERROR("Texture has not been loaded. Name: " << name);
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
