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

    static Tileset* ParseTileset(const tinyxml2::XMLElement* tilesetElement);
    static CollisionRect* ParseRect(tinyxml2::XMLElement* collisionRectElement);
    static Layer* ParseLayer(tinyxml2::XMLElement* layerElement, Level* level, Tileset* tileset);
    static Layer* ParseRepeatingLayer(tinyxml2::XMLElement* repeatingLayerNode, Level* level);
    static bool ParseObjectElement(tinyxml2::XMLElement* objectElement, Level* level);
    static bool ParseEnemy(tinyxml2::XMLElement* enemyElement, Level* level);
    static bool ParseCollisionRect(tinyxml2::XMLElement* collisionRectElement, Level* level);
    static bool ParseZone(tinyxml2::XMLElement* zoneElement, Level* level);
    static std::vector<int> ParseLayerData(tinyxml2::XMLElement* dataElement);
    static void LoadLayerMesh(std::vector<int>& layerData, Level* level, Tileset* tileset, const std::string& name);
    
    // Region methods
    static void ParseRegionResources(TiXmlElement* ResourcesNode, Region* region);
    static void ParseRegionLevelData(TiXmlElement* LevelDataNode, Region* region);
    static void ParseSoundEffects(TiXmlElement* SoundEffectsNode, Region* region);
    static void ParseSongs(TiXmlElement* SongsNode, Region* region);
    static void ParseSprites(TiXmlElement* SpritesNode, Region* region);

    // Error and Warning Loggers
    static void LogPropertyRetrievalWarning(
        const std::string&  propertyName,
        const std::string& levelFileName,
        int id,
        const std::string& objectType = "object"
    );
};

#endif