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

    bool OnNotify(Event* event);

    // Grants access to the game switches map when saving the game
    friend class SaveSystem;

private:
    std::map<std::string, bool> _switches;

	std::map<std::string, bool> _gameSwitchesSnapshot;

	void TakeSnapshot();
	void RestoreGameSwitchesFromSnapshot();
};

#endif
