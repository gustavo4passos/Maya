#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include <vector>

#include <tinyxml.h>

#include "Animation.h"
#include "Level.h"
#include "SoundPlayer.h"
#include "Texture.h"
#include "Tileset.h"

class CollisionRect;

class Mesh;

enum class ResourceType {
	ANIMATION,
    TEXTURE,
    MESH,
    SOUND_EFFECT,
    SONG,
	SPRITESHEET
};

class ResourceManager {

public:

    // Textures
    static bool LoadTexture(const std::string& filename, const std::string& name); // Returns false if unable to load texture
    static void DeleteTexture(const std::string& textureName);
    static Texture* const GetTexture(const std::string& textureName);
    static void CleanTextures();

	// Spritesheets
	static bool LoadSpritesheet(const std::string& textureName, const std::string& name, int nRows, int nColumns = 4);
	static void DeleteSpritesheet(const std::string& name);
	static Spritesheet* GetSpritesheet(const std::string& name);
	static void CleanSpritesheets();

	// Animations
	static bool LoadAnimation(const std::string& spritesheetName, const std::string& name, int nFrames, int speed = 20);
	static void DeleteAnimation(const std::string& name);
	static Animation* GetAnimation(const std::string& name);
	static void CleanAnimations();

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
	static std::map<std::string, Spritesheet*> _spritesheetMap;
    static std::map<std::string, Animation*> _animationMap;
    static std::map<std::string, Mesh*> _meshMap;
    static std::map<std::string, Sound*> _soundEffectsMap;
    static std::map<std::string, Music*> _musicMap;
};

#endif
