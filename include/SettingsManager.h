#ifndef SETTINGS_H
#define SETTINGS_H

namespace tinyxml2 {
    class XMLDocument;
    class XMLElement;
}

class SettingsManager {
public:
    SettingsManager();
    ~SettingsManager() { };


    // Window settings
    void SetResolution(int width, int height);
    void SetVsync(bool active);
    void SetFullscreen(bool active);

    inline int WindowWidth() const { return _windowWidth; }
    inline int WindowHeight() const { return _windowHeight; }
    inline bool Vsync() const { return _vsync; }
    inline bool Fullscreen() const { return _fullscreen; }

    // Sound settings
    void SetMasterVolume(int volume);
    inline int MasterVolume() const { return _masterVolume; }

private:
    tinyxml2::XMLDocument* _settingsFile;
    tinyxml2::XMLElement* _windowElement;
    tinyxml2::XMLElement* _soundElement;

    int _windowWidth, _windowHeight;
    bool _vsync, _fullscreen;

    int _masterVolume;

    tinyxml2::XMLDocument* CreateDefaultSettingsFile();
    void PrintInabilityToSaveSettingsErrorMessage();
    bool HandleInvalidSettingsFile();
    void SaveFile();

    const char * FILEPATH = "../res/settings.xml";
};

#endif