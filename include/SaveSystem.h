#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <string>
#include <map>

#include "Vector2D.h"

struct Save {
    std::string regionFilename;
    std::string subregionName;
    Vector2D playerPosition;
    std::map<std::string, bool> gameSwitchesState;
};

class SaveSystem {
public:
    SaveSystem();

    void SaveGame();
    Save* LoadGame();
};

#endif