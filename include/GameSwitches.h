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
    
    void ActivateSwitch(const std::string& id);
    void DeactivateSwitch(const std::string& id);
    bool CheckSwitch(const std::string& id);
    void PushSwitch(const std::string& id, bool state = false);

    const std::map<std::string, bool>& GetSwitches() { return _switches; }

    bool OnNotify(Event* event);

private:
    std::map<std::string, bool> _switches;

	std::map<std::string, bool> _gameSwitchesSnapshot;

    void ClearSwitches();
	void TakeSnapshot();
	void RestoreGameSwitchesFromSnapshot();
};

#endif
