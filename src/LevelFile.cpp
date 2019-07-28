#include "../include/LevelFile.h"

LevelFile::LevelFile(const std::string& filename)
:   _filename(filename),
    _isOpen(false),
    _rootElement(nullptr),
    _tilesetElement(nullptr),
    _collisionObjectsGroupNode(nullptr),
    _enemiesGroupNode(nullptr)
{ }

LevelFile::LevelFile(const LevelFile& l)
:   _filename(l._filename),
    _isOpen(false)
{ }

bool LevelFile::OpenFile()
{
    if(_isOpen)
    {
        LOG_WARNING("Attempting to open " + _filename + " but file is already open.");
        return true;
    }

    using namespace tinyxml2;
    
    XMLError openStatus = _tmxFile.LoadFile(_filename.c_str());
    if(openStatus != XML_SUCCESS) {
        LOG_ERROR("Unable to open level file. Error: " + std::string(_tmxFile.ErrorStr()));
        return false;
    }
    
    _isOpen = true;
    _rootElement = _tmxFile.RootElement();

    _tilesetElement = TMXHelper::GetChildElementByName(_rootElement, "tileset");

    // Tileset node is not present
    if(_tilesetElement == nullptr) {
        LOG_ERROR("Level file \"" + _filename + "\" is missing tileset node.");
        return false;
    }

    GetLayerElements();
    if(_layerElements.size() == 0) 
    {
        LOG_WARNING("No layer nodes were found in \"" + _filename + "\"");
    }
    
    std::vector<tinyxml2::XMLElement*> objectGroups;
    TMXHelper::GetChildElementsByName(_rootElement, "objectgroup", &objectGroups);

    for(const auto& objectGroup : objectGroups)
    {
        const std::string groupName = objectGroup->Attribute("name");
        if(groupName  == "CollisionLayer")
        {
            _collisionObjectsGroupNode = objectGroup;
            RetrieveObjects(objectGroup, &_collisionRectsElements);
        }
        else if(groupName == "Enemies") 
        {
            _enemiesGroupNode = objectGroup;
            RetrieveObjects(objectGroup, &_enemiesNodes);
        }
        else if(groupName == "Zones")
        {
            _zonesNode = objectGroup;
            RetrieveObjects(objectGroup, &_zonesElements);
        }
        else 
        {
            // When Objectroup is none of the above,
            // but will still needs to be added to _levelObjects
            // because their id should NOT be ignored when
            // creating new objects.
            RetrieveObjects(objectGroup);
        }
    }

    if(_collisionRectsElements.size() == 0) 
    {
        LOG_WARNING("Level file \"" + _filename + "\" has no collision objects node.");
    }

    return true; 
}

bool LevelFile::SaveFile()
{
    if(_tmxFile.SaveFile(_filename.c_str()) != tinyxml2::XML_SUCCESS)
    {
        LOG_ERROR("Unable to write to level file \"" + _filename + "\". Error: " + _tmxFile.ErrorStr());
        return false;
    }

    return true;
}

int LevelFile::GetObjectID(tinyxml2::XMLElement* objectElement)
{
    int id;
    if(objectElement == nullptr) return -1;
    if(objectElement->Attribute("id") == nullptr) return -1;
    objectElement->QueryIntAttribute("id", &id);
    return id;
}

void LevelFile::GetLayerElements()
{
    for(auto element = _rootElement->FirstChildElement();
        element != nullptr;
        element = element->NextSiblingElement())
    {
        if(std::string(element->Name()) == "layer" || std::string(element->Name()) == "imagelayer")
        {
            _layerElements.push_back(element);
        }
    }
}

void LevelFile::RetrieveObjects(tinyxml2::XMLElement* objectGroup, std::vector<tinyxml2::XMLElement*>* elements)
{
    #ifdef M_DEBUG
    if(objectGroup == nullptr)
    {
        LOG_ERROR("Unable to get objects from object group in file \"" + _filename + "\". Object group is nullptr.");
        return;
    }
    #endif

    std::vector<tinyxml2::XMLElement*> objectNodes;
    TMXHelper::GetAllChildElements(objectGroup, &objectNodes);

    for(auto objectNode = objectNodes.begin();
        objectNode != objectNodes.end();
        ++objectNode)
    {
        int id = GetObjectID(*objectNode);
        if(id == -1)
        {
            LOG_WARNING("Ignoring object with an invalid ID. File: " + _filename + ". Object group type: " + std::string(objectGroup->Attribute("name")));
            continue;
        }

        _levelObjects[id] = *objectNode;
        if(elements != nullptr) elements->push_back(*objectNode);
    }
}

int LevelFile::GetFirstUnusedID() 
{
    int id = 1;
    auto entry = _levelObjects.begin();

    while(entry != _levelObjects.end())
    {
        if(entry->first == id)
        {
            ++entry;
            id++;
        }      
        else break;  
    }

    return id;
}