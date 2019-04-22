#include "../include/LevelFile.h"

#include "../include/Logger.h"

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

    for(XMLElement* e = _rootElement->FirstChildElement();
        e != nullptr;
        e = e->NextSiblingElement())
    {
        if(std::string(e->Name()) == "tileset") 
        {
            _tilesetNode = e;
            break;
        }
    }

    // Tileset node is not present
    if(_tilesetNode == nullptr) {
        LOG_ERROR("Level file \"" + _filename + "\" is missing tileset node.");
        return false;
    }

    for(XMLElement* e = _rootElement->FirstChildElement();
        e != nullptr;
        e = e->NextSiblingElement())
    {
        if(std::string(e->Name()) == "layer") 
        {
            _layerNodes.push_back(e);
        }
    }

    if(_layerNodes.size() == 0) 
    {
        LOG_WARNING("No layer nodes were found in \"" + _filename + "\"");
    }
    
    for(XMLElement* e = _rootElement->FirstChildElement();
        e != nullptr;
        e = e->NextSiblingElement())
    {
        if(std::string(e->Name()) == "objectgroup")
        {
            std::string objectGroupType = std::string(e->Attribute("name"));
            if(objectGroupType == "CollisionLayer")
            {
                _collisionObjectsGroupNode = e;
                continue;
            }
            else if(objectGroupType == "Enemies") 
            {
                _enemiesGroupNode = e;
                continue;
            }
        }
    }

    if(_collisionObjectsGroupNode == nullptr) 
    {
        LOG_WARNING("Level file \"" + _filename + "\" has collision objects node.");
    }
    else
    {
        GetCollisionRectsFromCollisionGroup(_collisionObjectsGroupNode);
    }

    if(_enemiesGroupNode != nullptr)
    {
        GetEnemiesFromEnemiesGroup(_enemiesGroupNode);
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

void LevelFile::GetCollisionRectsFromCollisionGroup(tinyxml2::XMLElement* collisionLayerElement)
{
    GetAllChildElements(collisionLayerElement, &_collisionObjectsNodes);
}

void LevelFile::GetEnemiesFromEnemiesGroup(tinyxml2::XMLElement* enemiesElement)
{
    // The enemies element pointer is not checked here
    // This resopnsability is delegated to GetAllChildElements
    GetAllChildElements(enemiesElement, &_enemiesNodes);
    for(auto enemyNode = _enemiesNodes.begin();
        enemyNode != _enemiesNodes.end();
        enemyNode++)
    {
        int enemyId = GetObjectID(*enemyNode);
        if(enemyId == -1)
        {
            LOG_WARNING("An enemy has an invalid ID. Ignoring...");
            continue;
        }

        _levelGameObjects[enemyId] = *enemyNode;
    }
}

void LevelFile::GetAllChildElements(tinyxml2::XMLElement* element, std::vector<tinyxml2::XMLElement*>* childElements)
{
    #ifdef M_DEBUG
    if(element == nullptr) 
    {
        LOG_ERROR("Element is null.");
        return;
    }
    if(childElements == nullptr)
    {
        LOG_ERROR("Element is null.");
        return;
    }    
    #endif

    for(tinyxml2::XMLElement* e = element->FirstChildElement();
        e != nullptr;
        e = e->NextSiblingElement())
    {
        childElements->push_back(e);
    }
}
