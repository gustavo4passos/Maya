#include "../include/Region.h"

#include <memory> // For std::unique_ptr

#include "../include/Logger.h"
#include "../include/EventDispatcher.h"
#include "../include/Level.h"
#include "../include/LevelChangedEvent.h"
#include "../include/PhysicsEngine.h"
#include "../include/Player.h"
#include "../include/PlayerHitTeleportEvent.h"
#include "../include/ServiceLocator.h"

Region::Region(const std::string& filename)
:   _filename(filename),
    _currentLevel("")
{
    EventDispatcher::AddListener(this, EventType::PLAYER_HIT_TELEPORT);
}

Region::~Region() {
    for(auto it = _levels.begin(); it != _levels.end(); it++) {
        delete it->second;
    }

    _levels.clear();

    for(auto mapItem = _resources.begin(); mapItem != _resources.end(); mapItem++) {
        for(auto resource = mapItem->second.begin(); resource != mapItem->second.end(); resource++) {
            ResourceManager::CleanResource(mapItem->first, *resource);
        }
    }

    _resources.clear();

    EventDispatcher::RemoveListener(this, EventType::PLAYER_HIT_TELEPORT);
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

    while(!_unresolvedEvents.empty()) {
        Event* unresolvedEvent = _unresolvedEvents.back().get();

        if(unresolvedEvent->type() == EventType::PLAYER_HIT_TELEPORT) {
            PlayerHitTeleportEvent* event = static_cast<PlayerHitTeleportEvent*>(unresolvedEvent);
            ChangeCurrentLevel(event->destinationLevel());
            ServiceLocator::GetPlayer()->setPosition(event->destinationPosition().x(), event->destinationPosition().y());
        }

        _unresolvedEvents.pop();
    }
}

void Region::RenderBackground(Renderer* renderer, float deltaTime) {
     if(!HasLevelBenSet()){
        return;
    }
    _levels[_currentLevel]->DrawBackground(renderer, deltaTime);
}

void Region::RenderForeground(Renderer* renderer, float deltaTime) {
     if(!HasLevelBenSet()){
        return;
    }
    _levels[_currentLevel]->DrawForeground(renderer, deltaTime);
}

bool Region::AddResource(ResourceType resourceType, const std::string& resourceID) {
    // TODO(Gustavo): Missing error checking
    _resources[resourceType].push_back(resourceID);

    return true;
}

bool Region::AddLevel(Level* level, const std::string& levelID){
    std::map<std::string, Level*>::const_iterator it = _levels.find(levelID);
    if(it != _levels.end()) {
        LOG_ERROR("Unable to add level to region. ID [\"" + levelID + "\"] ID is already in _levels map");
        return false;
    }

    if(level == nullptr){
        LOG_ERROR("Unable to add level to region. ID [\"" + levelID + "\"]. Level is nullptr");
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

bool Region::OnNotify(Event* event) {
    if(event->type() == EventType::PLAYER_HIT_TELEPORT) EnqueueEventForLater(event);
    return false;
}

bool Region::HasLevelBenSet(){
    if(_currentLevel == "") {
        LOG_ERROR("Level has not been set");
        return false;
    }

    return true;
}
