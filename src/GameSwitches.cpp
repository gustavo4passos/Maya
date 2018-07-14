#include "../include/GameSwitches.h"

#include "../include/ErrorHandler.h"
#include "../include/EventManager.h"
#include "../include/ActivateSwitchEvent.h"

GameSwitches::GameSwitches() {
    EventManager::AddListener(this, EventType::ACTIVATE_SWITCH);
}
void GameSwitches::ActivateSwitch(std::string id){
    if(_switches.find(id) != _switches.end()){
        _switches[id] = true;
    } else {
        LOG_ERROR("Unable to activate switch, invalid switch ID: " + id);
    }
}

void GameSwitches::DeactivateSwitch(std::string id){
    if(_switches.find(id) != _switches.end()){
        _switches[id] = false;
    } else {
        LOG_ERROR("Unable to deactivate switch, invalid switch ID: " + id);
    }
}

bool GameSwitches::CheckSwitch(std::string id){
    if(_switches.find(id) != _switches.end()){
        return _switches[id];
    } else {
        LOG_ERROR("Unable to check switch, invalid switch ID: " + id);
    }
    return false;
}

void GameSwitches::PushSwitch(std::string id, bool state){
    if(_switches.find(id) == _switches.end()){
        _switches[id] = state;
    } else {
        LOG_ERROR("Unable to push switch, switch already exists. ID: " + id);
    }   
}

bool GameSwitches::OnNotify(std::unique_ptr<Event>& event) {
    if(event->type() == EventType::ACTIVATE_SWITCH) {
        ActivateSwitch(dynamic_cast<ActivateSwitchEvent*>(event.get())->switchID());
        return false;
    }

    return false;
}

