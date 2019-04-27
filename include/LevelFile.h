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

    bool IsOpen() const { return _isOpen; }
    const std::string& filename() const { return _filename; }

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
    
    std::map<int, tinyxml2::XMLElement*> _levelObjects;

    // Gets the id of the element
    // Returns -1 if the id attribute is not found
    int GetObjectID(tinyxml2::XMLElement* objectElement);

    void GetLayerElements();

    // Stores all from an object group in _levelObjects.
    // If elements is not nullptr, stores it in it too.
    void GetObjects(tinyxml2::XMLElement* objectGroup, std::vector<tinyxml2::XMLElement*>* elements = nullptr);

    // Consults the map of level objects, and return the smallest
    // not yet used ID.
    // This query relies on the fact that the id's inside
    // the map are ordered.
    int GetFirstAvailableID();
};


#endif