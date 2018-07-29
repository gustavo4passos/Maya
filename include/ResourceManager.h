#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include <vector>

#include <tinyxml.h>

#include "Texture.h"
#include "Tileset.h"
#include "Level.h"
#include "SoundPlayer.h"

class CollisionRect;

class Mesh;

enum class ResourceType {
    TEXTURE,
    MESH,
    SOUND_EFFECT,
    SONG   
};

class ResourceManager {

public:

    // Textures  
    static bool LoadTexture(const std::string& filename, const std::string& name); // Returns false if unable to load texture
    static void DeleteTexture(const std::string& textureName);    
    static Texture* const GetTexture(const std::string& textureName); 
    static void CleanTextures();    

    //Sounds & Music
    static bool LoadSoundEffect(const std::string& filename, const std::string& name);
    static Sound* GetSoundEffect(const std::string& name);
    static void DeleteSoundEffect(const std::string& name);
    static bool LoadMusic(const std::string& filename, const std::string& name);
    static Music* GetMusic(const std::string& name);
    static void DeleteMusic(const std::string& name);
    static void CleanAudio();

    //Meshes
    static bool LoadMesh(const void* data, std::size_t size, unsigned int count, const std::string& name);
    static Mesh* const GetMesh(const std::string& name);
    static void DeleteMesh(const std::string& name);
    static void CleanMeshes();

    static void CleanResource(ResourceType resourceType, const std::string& name);

    static void Clean();
private:

    ResourceManager() {}

    // Data maps
    static std::map<std::string, Texture*> _textureMap;
    static std::map<std::string, Mesh*> _meshMap;
    static std::map<std::string, Sound*> _soundEffectsMap;
    static std::map<std::string, Music*> _musicMap;
};

#endif