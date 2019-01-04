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
    _fullscreen(false),
    _masterVolume(128)
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
        if(!HandleInvalidSettingsFile()) return;
    }

    if(_windowElement->Attribute("width")      == nullptr ||
       _windowElement->Attribute("height")     == nullptr || 
       _windowElement->Attribute("vsync")      == nullptr ||
       _windowElement->Attribute("fullscreen") == nullptr)
    {
        if(!HandleInvalidSettingsFile()) return;
    }

    if(_windowElement->QueryIntAttribute("width", &_windowWidth)      != XML_SUCCESS ||
       _windowElement->QueryIntAttribute("height", &_windowHeight)    != XML_SUCCESS ||
       _windowElement->QueryBoolAttribute("vsync", &_vsync)           != XML_SUCCESS || 
       _windowElement->QueryBoolAttribute("fullscreen", &_fullscreen) != XML_SUCCESS)
    {
        if(!HandleInvalidSettingsFile()) return;
    }

    _soundElement = _windowElement->NextSiblingElement();

    if(_soundElement == nullptr || std::string(_soundElement->Name()) != "sound") {
        if(!HandleInvalidSettingsFile()) return;
    }

    if(_soundElement->Attribute("mastervolume") == nullptr) {
        if(!HandleInvalidSettingsFile()) return;
    }

    if(_soundElement->QueryIntAttribute("mastervolume", &_masterVolume) != XML_SUCCESS)
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

void SettingsManager::SetMasterVolume(int volume) {
    // Avoids bad values (volume must be between 0 ~ 128)
    if(volume < 0) volume = 0;
    if(volume > 128) volume = 128;

    _masterVolume = volume; 

    if(_settingsFile != nullptr && _soundElement != nullptr) {
        _soundElement->SetAttribute("mastervolume", volume);
        SaveFile();
    }
}


XMLDocument* SettingsManager::CreateDefaultSettingsFile() {
    XMLDocument* defaultSettingsDocument = new XMLDocument();
    XMLDeclaration* declaration = defaultSettingsDocument->NewDeclaration();
    XMLElement* mainElement = defaultSettingsDocument->NewElement("settings");
    XMLElement* windowDataElement = defaultSettingsDocument->NewElement("window");
    XMLElement* soundDataElement = defaultSettingsDocument->NewElement("sound");

    // Default window settings data
    windowDataElement->SetAttribute("width", "966");
    windowDataElement->SetAttribute("height", "544");
    windowDataElement->SetAttribute("vsync", "true");
    windowDataElement->SetAttribute("fullscreen", "false");
    mainElement->InsertEndChild(windowDataElement);

    // Default sound settings data
    soundDataElement->SetAttribute("mastervolume", (int)128);
    mainElement->InsertEndChild(soundDataElement);

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
        delete _settingsFile;
        _settingsFile = nullptr;
        _settingsFile = CreateDefaultSettingsFile();

        if(_settingsFile == nullptr) {
            PrintInabilityToSaveSettingsErrorMessage();
            _windowElement = nullptr;
            return false;
        } 
        else {
            _windowElement = _settingsFile->RootElement()->FirstChildElement();
            _soundElement = _windowElement->NextSiblingElement();
        }

        return true;
}