#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include "CollisionRect.h"

#include <string>
#include <vector>
#include <tinyxml.h>

class Level;
class Tileset;
class Layer;
class Rect;

class LevelLoader {

public:

    static Level* ParseLevel(const std::string& filename); // Returns NULL if unable to load level
    static void ParseRegion(const std::string& filename);

private:

    static Tileset* ParseTileset(TiXmlElement* node);
    static void ParseObjectGroup(TiXmlElement* objectsNode, Level* level);
    static CollisionRect* ParseRect(TiXmlElement* objectNode);
    static Layer* ParseLayer(TiXmlElement* layerNode, Level* level, Tileset* tileset);
    static std::vector<int> ParseLayerData(TiXmlElement* dataNode);
    static void LoadLayerMesh(std::vector<int>& layerData, Level* level, Tileset* tileset, const std::string& name);
    static TiXmlElement* GetProperty(TiXmlElement* propertiesNode, std::string propertyName);
    
    //Region methods
    static void ParseRegionResources(TiXmlElement* ResourcesNode);
    static void ParseRegionLevelData(TiXmlElement* LevelDataNode);
    static void ParseSoundEfects(TiXmlElement* SoundEfectsNode);
    static void ParseSongs(TiXmlElement* SongsNode);
    static void ParseSprites(TiXmlElement* SpritesNode);

};

#endif