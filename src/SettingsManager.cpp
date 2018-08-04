#include "../include/SettingsManager.h"

#include <string>

#include <tinyxml2.h>
using namespace tinyxml2;

#include "../include/Logger.h"

SettingsManager::SettingsManager()
:   _settingsFile(nullptr),
    _windowElement(nullptr),
    _windowWidth(966),
    _windowHeight(544),
    _vsync(true),  
    _fullscreen(false)
 {
    _settingsFile = new XMLDocument();

    if(_settingsFile->LoadFile(FILEPATH) != XML_SUCCESS) {
       
        delete _settingsFile;
        _settingsFile = nullptr;
        
        if(!HandleInvalidSettingsFile()) return;
    }

    XMLElement* root = _settingsFile->RootElement();
    _windowElement = root->FirstChildElement();

    if(_windowElement == nullptr || std::string(_windowElement->Name()) != "window") {
        delete _settingsFile;
        _settingsFile = nullptr;

        if(!HandleInvalidSettingsFile()) return;
    }

    if(_windowElement->Attribute("width")      == nullptr ||
       _windowElement->Attribute("height")     == nullptr || 
       _windowElement->Attribute("vsync")      == nullptr ||
       _windowElement->Attribute("fullscreen") == nullptr)
    {
        delete _settingsFile;
        _settingsFile = nullptr;

        if(!HandleInvalidSettingsFile()) return;
    }

    if(_windowElement->QueryIntAttribute("width", &_windowWidth)      != XML_SUCCESS ||
       _windowElement->QueryIntAttribute("height", &_windowHeight)    != XML_SUCCESS ||
       _windowElement->QueryBoolAttribute("vsync", &_vsync)           != XML_SUCCESS || 
       _windowElement->QueryBoolAttribute("fullscreen", &_fullscreen) != XML_SUCCESS)
    {
        delete _settingsFile;
        _settingsFile = nullptr;

        if(!HandleInvalidSettingsFile()) return;
    }
}

void SettingsManager::SetResolution(int width, int height) {
    _windowWidth = width;
    _windowHeight = height;

    if(_settingsFile != nullptr && _windowElement != nullptr) {
        _windowElement->SetAttribute("width", std::to_string(width).c_str());
        _windowElement->SetAttribute("height", std::to_string(height).c_str());
        SaveFile();

    }
}

void SettingsManager::SetVsync(bool active) {
    _vsync = active;

    if(_settingsFile != nullptr && _windowElement != nullptr) {
        _windowElement->SetAttribute("vsync", active ? "true" : "false");
        SaveFile();
    }
}

void SettingsManager::SetFullscreen(bool active) {
    _fullscreen = active;

    if(_settingsFile != nullptr && _windowElement != nullptr) {
        _windowElement->SetAttribute("fullscreen", active ? "true" : "false");
        SaveFile();
    }
}


XMLDocument* SettingsManager::CreateDefaultSettingsFile() {
    XMLDocument* defaultSettingsDocument = new XMLDocument();
    XMLDeclaration* declaration = defaultSettingsDocument->NewDeclaration();
    XMLElement* mainElement = defaultSettingsDocument->NewElement("settings");
    XMLElement* dataElement = defaultSettingsDocument->NewElement("window");
    dataElement->SetAttribute("width", "966");
    dataElement->SetAttribute("height", "544");
    dataElement->SetAttribute("vsync", "true");
    dataElement->SetAttribute("fullscreen", "false");
    mainElement->InsertEndChild(dataElement);

    defaultSettingsDocument->InsertEndChild(declaration);
    defaultSettingsDocument->InsertEndChild(mainElement);

    if(defaultSettingsDocument->SaveFile(FILEPATH) != XML_SUCCESS) {
        LOG_ERROR("Unable to create default settings file. Error: " + std::string(defaultSettingsDocument->ErrorStr()));
        delete defaultSettingsDocument;
        return nullptr;
    }

    return defaultSettingsDocument;
}

void SettingsManager::PrintInabilityToSaveSettingsErrorMessage() {
    LOG_WARNING("Unable to create settings file. Your settings will not be saved.");
}

void SettingsManager::SaveFile() {
    if(_settingsFile != nullptr) {
        _settingsFile->SaveFile(FILEPATH);
    }
}

bool SettingsManager::HandleInvalidSettingsFile() {
        LOG_WARNING("Unable to open settings file. Using default settings.");
        _settingsFile = CreateDefaultSettingsFile();

        if(_settingsFile == nullptr) {
            PrintInabilityToSaveSettingsErrorMessage();
            _windowElement = nullptr;
            return false;
        } 
        else {
            _windowElement = _settingsFile->RootElement()->FirstChildElement();
        }

        return true;
}