#ifndef LEVELFILE_H
#define LEVELFILE_H

#include <string>
#include <vector>
#include <map>
#include <tinyxml2.h>

#include "../include/TMXHelper.h"
#include "../include/Logger.h"

class LevelFile {
public:
    LevelFile(const std::string& filename);

    bool IsOpen() const { return _isOpen; }
    const std::string& filename()                const { return _filename;       }
    const tinyxml2::XMLElement* rootElement()    const { return _rootElement;    }
    const tinyxml2::XMLElement* tilesetElement() const { return _tilesetElement; }

    const std::vector<tinyxml2::XMLElement*>& enemiesElements()         const { return _enemiesElements;        }
    const std::vector<tinyxml2::XMLElement*>& layerElements()           const { return _layerElements;          }
    const std::vector<tinyxml2::XMLElement*>& collisionRectsElements()  const { return _collisionRectsElements; }
    const std::vector<tinyxml2::XMLElement*>& zonesElements()           const { return _zonesElements;          }
    const std::vector<tinyxml2::XMLElement*>& gameObjectsElements()     const { return _gameObjectsElements;    }
    
    // Opens the TMX level file
    // This must be called before any operation in the file is performed.
    // Returns true if file has been succesfully opened and the level
    // file is consistent.
    // Returns false otherwise.
    bool OpenFile();

    // Saves the level file
    // Any changes made to the level file will only be saved after this call
    bool SaveFile();

    // Sets a property of a game object 
    // Return true if the property has been succesfully set
    // Returns false if the property is not found
    // or if the object id is invalid
    template<typename T>
    bool SetObjectProperty(int objectID, const std::string& property, T value);

    // Retrieves an object property to *value.
    // Returns true if the property is successfully retrieved.
    // Returns false otherwise, and no change is made to value
    template<typename T>
    bool GetObjectProperty(int objectID, const std::string& property, T* value);

private:
    LevelFile(const LevelFile&);
    
    std::string _filename;
    tinyxml2::XMLDocument _tmxFile;
    bool _isOpen;

    tinyxml2::XMLElement* _rootElement;
    tinyxml2::XMLElement* _tilesetElement;
    tinyxml2::XMLElement* _collisionObjectsGroupNode;
    tinyxml2::XMLElement* _zonesNode;
    tinyxml2::XMLElement* _enemiesGroupNode;
    tinyxml2::XMLElement* _gameObjectsNode;

    std::vector<tinyxml2::XMLElement*> _layerElements;
    std::vector<tinyxml2::XMLElement*> _collisionRectsElements;
    std::vector<tinyxml2::XMLElement*> _enemiesElements;
    std::vector<tinyxml2::XMLElement*> _zonesElements; 
    std::vector<tinyxml2::XMLElement*> _gameObjectsElements;;
    
    std::map<int, tinyxml2::XMLElement*> _levelObjects;

    // Gets the id of the element
    // Returns -1 if the id attribute is not found
    int GetObjectID(tinyxml2::XMLElement* objectElement);

    void GetLayerElements();

    // Stores all objects from an object group in _levelObjects.
    // If elements is not nullptr, stores it in it too.
    void RetrieveObjects(tinyxml2::XMLElement* objectGroup, std::vector<tinyxml2::XMLElement*>* elements = nullptr);

    bool IsIdAlreadyInUse(int id);

    // Consults the map of level objects, and return the smallest
    // not yet used ID.
    // This query relies on the fact that the id's inside
    // the map are ordered.
    int GetFirstUnusedID();
};

template<typename T>
bool LevelFile::SetObjectProperty(
    int objectID, 
    const std::string& property,
    T value)
{
    auto entry = _levelObjects.find(objectID);
    if(entry == _levelObjects.end()) 
    {
        LOG_ERROR("Unable to set property \"" + property + "\" of object with id \"" +
            std::to_string(objectID) + "\" in level \"" + _filename + "\". ID is invalid.");
        return false;
    }

    bool success = TMXHelper::SetProperty(entry->second, property, value);
    if(success) SaveFile();
    return success;
}

template<typename T>
bool LevelFile::GetObjectProperty(
    int objectID,
    const std::string& property,
    T* value)
{
    auto entry = _levelObjects.find(objectID);
    if(entry == _levelObjects.end()) 
    {
        LOG_ERROR("Unable to get property \"" + property + "\" of object with id \"" +
            std::to_string(objectID) + "\" in level \"" + _filename + "\". ID is invalid.");
        return false;
    }

    return TMXHelper::GetProperty(entry->second, property, value);
}

#endif