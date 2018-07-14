#ifndef GAMESWITCHES_H
#define GAMESWITCHES_H

#include <string>
#include <map>

#include "Event.h"
#include "EventListener.h"

class GameSwitches : public EventListener {
public:
    GameSwitches();
    ~GameSwitches() { }
    
    void ActivateSwitch(std::string id);
    void DeactivateSwitch(std::string id);
    bool CheckSwitch(std::string id);
    void PushSwitch(std::string id, bool state = false);

    bool OnNotify(std::unique_ptr<Event>& event);

private:
    std::map<std::string, bool> _switches;

};

#endif