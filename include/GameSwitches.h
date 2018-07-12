#ifndef GAMESWITCHES_H
#define GAMESWITCHES_H

#include <string>
#include <map>

class GameSwitches{

public:
    void ActivateSwitch(std::string id);
    void DeactivateSwitch(std::string id);
    bool CheckSwitch(std::string id);
    void PushSwitch(std::string id, bool state = false);

private:
    std::map<std::string, bool> _switches;

};

#endif