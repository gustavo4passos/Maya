#include "../include/ResourceManager.h"

#include <algorithm> // for std::transform

// NOTE(Gustavo): stb_image fails to load some png subformats; consider using SOIL
#define STB_IMAGE_IMPLEMENTATION
#include "../extern/stb/stb_image.h"

#include "../include/ErrorHandler.h"
#include "../include/Layer.h"

std::map<std::string, Texture*> ResourceManager::_textureMap;
std::map<std::string, Mesh*> ResourceManager::_meshMap;
std::map<std::string, Sound*> ResourceManager::_soundEffectsMap;
std::map<std::string, Music*> ResourceManager::_musicMap;

bool ResourceManager::LoadTexture(const std::string& filename, const std::string& name) {

	std::map<std::string, Texture*>::const_iterator it = _textureMap.find(name);
	if(it != _textureMap.end()) {
		LOG_WARNING("Unable to load texture [\"" + name + "\"] into texture map: A texture with the same name already exists");
		return true;
	}

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

// NOTE(Gustavo): For the release version, consider returning map[entry] instead of using find()
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

bool ResourceManager::LoadSoundEffect(const std::string& filename, const std::string& name) {
	Mix_Chunk* sample;
	sample = Mix_LoadWAV(filename.c_str());
	if (!sample) {
		LOG_ERROR("Unable to load the sample: " + std::string(Mix_GetError()));
		return false;
	}

	_soundEffectsMap.insert(std::pair<std::string, Sound*>(name, sample));
	return true;
}

bool ResourceManager::LoadMusic(const std::string& filename, const std::string& name) {
	Mix_Music* sample;
	sample = Mix_LoadMUS(filename.c_str());
	if (!sample) {
		LOG_ERROR("Unable to load the sample: " + std::string(Mix_GetError()));
		return false;
	}

	_musicMap.insert(std::pair<std::string, Music*>(name, sample));
	return true;
}

Sound* ResourceManager::GetSoundEffect(const std::string& name) {
    return _soundEffectsMap[name];
}

Music* ResourceManager::GetMusic(const std::string& name) {
    return _musicMap[name];
}

void ResourceManager::DeleteSoundEffect(const std::string& name) {
	 std::map<std::string, Sound*>::iterator entry = _soundEffectsMap.find(name);
	 if (entry == _soundEffectsMap.end()) {
		 LOG_ERROR("Unable to delete sound effect, invalid sound effect name: " + name);
	 }
	 else {
		 Mix_FreeChunk(entry->second);
		 entry->second = nullptr;
		 _soundEffectsMap.erase(entry);
	 }
}

void ResourceManager::DeleteMusic(const std::string& name) {
	 std::map<std::string, Music*>::iterator entry = _musicMap.find(name);
	 if (entry == _musicMap.end()) {
		 LOG_ERROR("Unable to delete sound effect, invalid music name: " + name);
	 }
	 else {
		 Mix_FreeMusic(entry->second);
		 entry->second = nullptr;
		 _musicMap.erase(entry);
	 }
}

void ResourceManager::CleanAudio() {
	for(std::map<std::string, Sound*>::iterator it = _soundEffectsMap.begin(); it != _soundEffectsMap.end(); it++)
    {
        Mix_FreeChunk(it->second);
		it->second = nullptr;
    }
    _soundEffectsMap.clear();

	for(std::map<std::string, Music*>::iterator it = _musicMap.begin(); it != _musicMap.end(); it++)
    {
        Mix_FreeMusic(it->second);
		it->second = nullptr;
    }
    _musicMap.clear();
}

bool ResourceManager::LoadMesh(const void* data, std::size_t size, unsigned int count, const std::string& name) {
	std::map<std::string, Mesh*>::const_iterator it = _meshMap.find(name.c_str());
	if(it != _meshMap.end()){
		LOG_WARNING("Unable to load mesh into mesh map: [\"" + name + "\"]. A mesh with the same name already exists.");
		return true;
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

void ResourceManager::CleanResource(ResourceType resourceType, const std::string& name) {
	switch(resourceType) {
		case ResourceType::TEXTURE:
			DeleteTexture(name);
			break;
		case ResourceType::MESH:
			DeleteMesh(name);
			break;
		case ResourceType::SOUND_EFFECT:
			DeleteSoundEffect(name);
			break;
		case ResourceType::SONG:
			DeleteMusic(name);
			break;
		default:
			LOG_WARNING("Unable to delete resource. Resource type is unknown.");
			break;
	}
}

void ResourceManager::Clean() {
	CleanMeshes();
	CleanTextures();
}
