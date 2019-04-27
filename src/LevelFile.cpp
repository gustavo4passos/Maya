#include "../include/LevelFile.h"

#include "../include/Logger.h"
#include "../include/TMXHelper.h"

LevelFile::LevelFile(const std::string& filename)
:   _filename(filename),
    _isOpen(false),
    _rootElement(nullptr),
    _tilesetNode(nullptr),
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

    _tilesetNode = TMXHelper::GetChildElementByName(_rootElement, "tileset");

    // Tileset node is not present
    if(_tilesetNode == nullptr) {
        LOG_ERROR("Level file \"" + _filename + "\" is missing tileset node.");
        return false;
    }

    GetLayerElements();
    if(_layerNodes.size() == 0) 
    {
        LOG_WARNING("No layer nodes were found in \"" + _filename + "\"");
    }

    std::vector<tinyxml2::XMLElement*> objectGroups;
    TMXHelper::GetChildElementsByName(_rootElement, "objectgroup", &objectGroups);

    for(auto objectGroup = objectGroups.begin(); 
        objectGroup != objectGroups.end(); 
        ++objectGroup)
    {
        const std::string groupName = (*objectGroup)->Attribute("name");
        if(groupName  == "CollisionLayer")
        {
            _collisionObjectsGroupNode = *objectGroup;
            GetObjects(*objectGroup, &_collisionObjectsNodes);
        }
        else if(groupName == "Enemies") 
        {
            _enemiesGroupNode = *objectGroup;
            GetObjects(*objectGroup, &_enemiesNodes);
        }
        else 
        {
            // Objectroup is none of the above,
            // but will still be added to _levelObjects because
            // their id should NOT be ignored when
            // creating new objects.
            GetObjects(*objectGroup);
        }
    }

    if(_collisionObjectsNodes.size() == 0) 
    {
        LOG_WARNING("Level file \"" + _filename + "\" has no collision objects node.");
    }

    std::cout << "Number of objects " << _levelObjects.size() << std::endl;
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
    TMXHelper::GetChildElementsByName(_rootElement, "layer", &_layerNodes);
}

void LevelFile::GetObjects(tinyxml2::XMLElement* objectGroup, std::vector<tinyxml2::XMLElement*>* elements)
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

int LevelFile::GetFirstAvailableID() 
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