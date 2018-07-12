#include "../include/GameSwitches.h"

#include "../include/ErrorHandler.h"

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


