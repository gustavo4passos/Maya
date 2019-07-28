#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "CollisionRect.h"

#include <string>
#include <vector>
#include <tinyxml.h>
#include <tinyxml2.h>

class Level;
class Tileset;
class Layer;
class Rect;
class Region;

class LevelLoader {

public:

    static Level* ParseLevel(const std::string& filename); // Returns nullptr if unable to load level
    static Region* ParseRegion(const std::string& filename);

private:

    static Tileset* ParseTileset(TiXmlElement* node);
    static Tileset* ParseTileset(const tinyxml2::XMLElement* tilesetElement);
    static void ParseObjectGroup(TiXmlElement* objectsNode, Level* level);
    static CollisionRect* ParseRect(tinyxml2::XMLElement* collisionRectElement);
    static Layer* ParseLayer(TiXmlElement* layerNode, Level* level, Tileset* tileset);
    static Layer* ParseLayer(tinyxml2::XMLElement* layerElement, Level* level, Tileset* tileset);
    static Layer* ParseRepeatingLayer(TiXmlElement* repeatingLayerNode, Level* level);
    static Layer* ParseRepeatingLayer(tinyxml2::XMLElement* repeatingLayerNode, Level* level);
    static bool ParseEnemy(tinyxml2::XMLElement* enemyElement, Level* level);
    static bool ParseCollisionRect(tinyxml2::XMLElement* collisionRectElement, Level* level);
    static bool ParseZone(tinyxml2::XMLElement* zoneElement, Level* level);
    static std::vector<int> ParseLayerData(tinyxml2::XMLElement* dataElement);
    static std::vector<int> ParseLayerData(TiXmlElement* dataElement);
    static void LoadLayerMesh(std::vector<int>& layerData, Level* level, Tileset* tileset, const std::string& name);
    static TiXmlElement* GetProperty(TiXmlElement* propertiesNode, std::string propertyName);
    
    //Region methods
    static void ParseRegionResources(TiXmlElement* ResourcesNode, Region* region);
    static void ParseRegionLevelData(TiXmlElement* LevelDataNode, Region* region);
    static void ParseSoundEffects(TiXmlElement* SoundEffectsNode, Region* region);
    static void ParseSongs(TiXmlElement* SongsNode, Region* region);
    static void ParseSprites(TiXmlElement* SpritesNode, Region* region);

};

#endif