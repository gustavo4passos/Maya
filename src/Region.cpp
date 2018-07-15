#include "../include/Region.h"

#include <memory> // For std::unique_ptr

#include "../include/ErrorHandler.h"
#include "../include/EventDispatcher.h"
#include "../include/Level.h"
#include "../include/LevelChangedEvent.h"
#include "../include/PhysicsEngine.h"
#include "../include/ServiceLocator.h"

Region::Region() 
:   _currentLevel("")
{ }

Region::~Region() {
    for(auto it = _levels.begin(); it != _levels.end(); it++) {
        delete it->second;
    }

    _levels.clear();
}

void Region::HandleInput() {
    if(!HasLevelBenSet()){
        return;
    }
    //_levels[_currentLevel]->HandleInput();
}

void Region::Update() {
     if(!HasLevelBenSet()){
        return;
    }
    _levels[_currentLevel]->Update();
}

void Region::Render(Renderer* renderer, float deltaTime) {
     if(!HasLevelBenSet()){
        return;
    }
    _levels[_currentLevel]->DrawBackground(renderer, deltaTime);
}

bool Region::AddLevel(Level* level, const std::string& levelID){
    std::map<std::string, Level*>::const_iterator it = _levels.find(levelID);
    if(it != _levels.end()) {
        LOG_ERROR("Unable to add level to region. ID [\"" + levelID + "\"] ID is already in _levels map");
        return false;
    }

    if(level == nullptr){
        LOG_ERROR("Unable to add level to region. ID [\"" + levelID + "\"]. Level is NULL");
        return false;
    }

    _levels[levelID] =  level;
    return true;
}

bool Region::ChangeCurrentLevel(const std::string& levelID){
    std::map<std::string, Level*>::const_iterator it = _levels.find(levelID);
    if(it == _levels.end()) {
        LOG_ERROR("Unable to change current level. levelID \"[" + levelID + "\"] is not in level map.");
        return false;
    }
    if(levelID == _currentLevel) {
        LOG_WARNING("Unable to change current level. Level is already current: " + levelID);
        return false;
    }

    _currentLevel = levelID;
    ServiceLocator::ProvideCurrentLevel(_levels[_currentLevel]);
    PhysicsEngine::setCurrentLevel(_levels[levelID]);

    std::unique_ptr<Event> levelChangedEvent(new LevelChangedEvent(_levels[levelID]));
    EventDispatcher::Notify(levelChangedEvent.get());
    
    return true;
}

std::vector<std::string> Region::SubRegionList() {
    std::vector<std::string> list;

    for(auto subregion = _levels.begin(); subregion != _levels.end(); subregion++) {
        list.push_back(subregion->first);
    }

    return list;
}
bool Region::HasLevelBenSet(){
    if(_currentLevel == "") {
        LOG_ERROR("Level has not been set");
        return false;
    }

    return true;
}