#include "../include/GameSwitches.h"

#include "../include/ErrorHandler.h"
#include "../include/EventDispatcher.h"
#include "../include/ActivateSwitchEvent.h"
#include "../include/PlayerDiedEvent.h"

GameSwitches::GameSwitches() {
    EventDispatcher::AddListener(this, EventType::SWITCH_ACTIVATED);
	EventDispatcher::AddListener(this, EventType::PLAYER_DIED);
	EventDispatcher::AddListener(this, EventType::LEVEL_CHANGED);
}

void GameSwitches::ActivateSwitch(const std::string& id){
    if(_switches.find(id) != _switches.end()){
        _switches[id] = true;
    } else {
        LOG_ERROR("Switch ID is not known: " + id + ". Adding it to the list as 'on'.");
        PushSwitch(id, true);
    }
}

void GameSwitches::DeactivateSwitch(const std::string& id){
    if(_switches.find(id) != _switches.end()){
        _switches[id] = false;
    } else {
        LOG_ERROR("Switch ID is not known: " + id + ". Adding it to the list as 'off'.");
        PushSwitch(id, false);
    }
}

bool GameSwitches::CheckSwitch(const std::string& id){
    if(_switches.find(id) != _switches.end()){
        return _switches[id];
    } else {
        LOG_ERROR("Switch ID is not known: " + id + ". Adding it to the list as 'off'.");
        PushSwitch(id);
    }
    return false;
}

void GameSwitches::PushSwitch(const std::string& id, bool state){
    if(_switches.find(id) == _switches.end()){
        _switches[id] = state;
    } else {
        LOG_ERROR("Unable to push switch, switch already exists. ID: " + id);
    }   
}

bool GameSwitches::OnNotify(Event* event) {
    if(event->type() == EventType::SWITCH_ACTIVATED) {
        ActivateSwitch(dynamic_cast<ActivateSwitchEvent*>(event)->switchID());
    }
	if(event->type() == EventType::PLAYER_DIED)	{
		RestoreGameSwitchesFromSnapshot();
	}
	if(event->type() == EventType::LEVEL_CHANGED) {
		TakeSnapshot();
	}

    return false;
}

void GameSwitches::TakeSnapshot() {
	_gameSwitchesSnapshot.clear();
	
	for(auto gameSwitch : _switches) {
		_gameSwitchesSnapshot[gameSwitch.first] = gameSwitch.second;
	}
}

void GameSwitches::RestoreGameSwitchesFromSnapshot() {
	_switches.clear();

	for(auto snapshotSwitch : _gameSwitchesSnapshot) {
		_switches[snapshotSwitch.first] = snapshotSwitch.second;
	}

}
