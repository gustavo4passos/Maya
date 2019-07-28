#include "../include/SaveSystem.h"

#include "../include/Logger.h"
#include "../include/GameSwitches.h"
#include "../include/GameObject.h"
#include "../include/Level.h"
#include "../include/Region.h"
#include "../include/ServiceLocator.h"
#include "../include/TMXHelper.h"


SaveSystem::SaveSystem() { }

void SaveSystem::SaveGame() {
    using namespace tinyxml2;

    Region* currentRegion = ServiceLocator::GetCurrentRegion();
    Vector2D playerPosition = ServiceLocator::GetPlayer()->position();
    std::map<std::string, bool> gameSwitches = ServiceLocator::GetGameSwitches()->GetSwitches();

    // Create document and nodes
    XMLDocument savefile;
    XMLDeclaration* declaration = savefile.NewDeclaration();
    XMLElement* mainElement = savefile.NewElement("save");
    XMLElement* regionDataElement = savefile.NewElement("regiondata");
    XMLElement* playerPositionElement = savefile.NewElement("playerposition");
    XMLElement* gameSwitchesElement = savefile.NewElement("switches");

    // Stores region data
    regionDataElement->SetAttribute("filename", currentRegion->filename().c_str());
    regionDataElement->SetAttribute("subregion", currentRegion->currentSubRegion().c_str());

    // Stores player position data
    playerPositionElement->SetAttribute("x", playerPosition.x());
    playerPositionElement->SetAttribute("y", playerPosition.y());

    // Stores current game switches
    for(auto switchState : gameSwitches) {
        XMLElement* switchElement = savefile.NewElement("switch");
        switchElement->SetAttribute("id", switchState.first.c_str());
        switchElement->SetAttribute("state", switchState.second ? "true" : "false");
        gameSwitchesElement->InsertEndChild(switchElement);
    }

    // Insert elements in document
    savefile.InsertEndChild(declaration);
    mainElement->InsertEndChild(regionDataElement);
    mainElement->InsertEndChild(playerPositionElement);
    mainElement->InsertEndChild(gameSwitchesElement);
    savefile.InsertEndChild(mainElement);
    
    if(savefile.SaveFile("../res/saves/save2.sav") != XML_SUCCESS) {
        LOG_ERROR("Unable to write save file. Error: " + std::string(savefile.ErrorStr()));
    }
}

Save* SaveSystem::LoadGame() {
    using namespace tinyxml2;

    Save* save = nullptr;
    Vector2D playerPosition;
    std::string regionFilename, subregionName;
    std::map<std::string, bool> gameSwitches;

    XMLDocument savefile;
    XMLError openStatus = savefile.LoadFile("../res/saves/save2.sav");

    if(openStatus != XML_SUCCESS) {
        LOG_ERROR("Unable to open save file. Error: " + std::string(savefile.ErrorStr()));
        return nullptr;
    }

    XMLElement* root = savefile.RootElement();

    // Tries to read region and subregion data 
    XMLElement* regionDataNode = nullptr;
    regionDataNode = TMXHelper::GetChildElementByName(root, "regiondata");
    if(regionDataNode == nullptr) {
        LOG_ERROR("Unable to load save: region data is missing.");
        return nullptr;
    }

    if(regionDataNode->Attribute("filename") == nullptr) {
        LOG_ERROR("Unable to load save: Region filename is missing.");
        return nullptr;
    }    
    else regionFilename = std::string(regionDataNode->Attribute("filename"));

    if(regionDataNode->Attribute("subregion") == nullptr) {
        LOG_ERROR("Unable to load save: Subregion name is misssing.");
        return nullptr;
    }
    else subregionName = regionDataNode->Attribute("subregion");

    // Tries to read player position data
    XMLElement* playerPositionNode = regionDataNode->NextSiblingElement();
    if(playerPositionNode == nullptr || playerPositionNode->Attribute("x") == nullptr || playerPositionNode->Attribute("y") == nullptr) {
        LOG_ERROR("Unable to load save: Player position is incorrect.");
        return nullptr;
    }
    else {
        float x, y;
        XMLError xAttributeStatus = playerPositionNode->QueryFloatAttribute("x", &x);
        XMLError yAttributeStatus = playerPositionNode->QueryFloatAttribute("y", &y);
        if(xAttributeStatus != XML_SUCCESS) {
            LOG_ERROR("Unable to load save: Player x position is not a floating-point number.");
            return nullptr;
        } 
        else if(yAttributeStatus != XML_SUCCESS) {
            LOG_ERROR("Unable to load save: Player y position is not a floating-point number.");
            return nullptr;
        } 
        else playerPosition = Vector2D(x, y);
    }

    // Tries to read game switches data
    XMLElement* switchesNode = playerPositionNode->NextSiblingElement();
    if(switchesNode == nullptr || std::string(switchesNode->Name()) != "switches") {
        LOG_ERROR("Unable to load save: Switches field is missing.");
        return nullptr;
    }

    for(XMLElement* s = switchesNode->FirstChildElement(); s != nullptr; s = s->NextSiblingElement()) {
        if(s->Attribute("id") == nullptr) {
            LOG_WARNING("Unable to read switch in save file: Switch missing ID field. Ignoring...");
            continue;
        }
        else if( s->Attribute("state") == nullptr) {
            LOG_WARNING("Unable to read switch in save file: Switch missing STATE field. Ignoring...");
            continue;
        }
        else {
            std::string switchID = s->Attribute("id");
            bool switchState;
            XMLError switchStateStatus = s->QueryBoolAttribute("state", &switchState);
            if(switchStateStatus != XML_SUCCESS) {
                LOG_WARNING("Unable to read switch state: It is not a boolean value. ID: " + switchID + ". Ignoring...");
                continue;
            } 
            else {
                gameSwitches[switchID] = switchState;
            }
        }
    }

    save = new Save;
    save->regionFilename = regionFilename;
    save->subregionName = subregionName;
    save->playerPosition = playerPosition;
    save->gameSwitchesState = gameSwitches;

    for(auto Switch : gameSwitches) {
        ServiceLocator::GetGameSwitches()->PushSwitch(Switch.first, Switch.second);
    }

    return save;
}