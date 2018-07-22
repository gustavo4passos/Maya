#include "../include/LevelChangedEvent.h"

#include "../include/ErrorHandler.h"

LevelChangedEvent::LevelChangedEvent(Level* newLevel) 
:   Event(EventType::LEVEL_CHANGED),
    _newLevel(newLevel)
{
    if(newLevel == nullptr) {
        LOG_WARNING("New level in LEVEL_CHANGED_EVENT is null.");        
    }
}