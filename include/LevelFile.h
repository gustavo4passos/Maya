#ifndef LEVELFILE_H
#define LEVELFILE_H

#include <string>
#include <vector>
#include <map>

#include <tinyxml2.h>

class LevelFile {
public:
    LevelFile(const std::string& filename);
    LevelFile(const LevelFile&);

    // Opens the TMX level file
    // This must be called before any operation in the file is performed.
    // Returns true if file has been succesfully opened and the level
    // file is consistent.
    // Returns false otherwise.
    bool OpenFile();

    // Saves the level file
    // Any changes made to the level file will only be saved after this call
    bool SaveFile();

    // Sets a game object property
    // Return true if the property has been succesfully set
    // Returns false if the property is not found
    // or if the object id is invalid
    template<typename T>
    bool SetGameObjectProperty(int objectID, const std::string& property, T value);

private:
    std::string _filename;
    tinyxml2::XMLDocument _tmxFile;
    bool _isOpen;

    tinyxml2::XMLElement* _rootElement;
    tinyxml2::XMLElement* _tilesetNode;
    tinyxml2::XMLElement* _collisionObjectsGroupNode;
    tinyxml2::XMLElement* _enemiesGroupNode;
    std::vector<tinyxml2::XMLElement*> _layerNodes;
    std::vector<tinyxml2::XMLElement*> _collisionObjectsNodes;
    std::vector<tinyxml2::XMLElement*> _enemiesNodes;
    
    std::map<int, tinyxml2::XMLElement*> _levelGameObjects;

    // Gets the id of the element
    // Returns -1 if the id is not found
    int GetObjectID(tinyxml2::XMLElement* objectElement);
    void GetAllChildElements(tinyxml2::XMLElement* element, std::vector<tinyxml2::XMLElement*>* childElements);

    void GetCollisionRectsFromCollisionGroup(tinyxml2::XMLElement* collisionLayerElement);
    void GetEnemiesFromEnemiesGroup(tinyxml2::XMLElement* enemiesElement);

};


#endif