#ifndef LEVELCHANGEDEVENT_H
#define LEVELCHANGEDEVENT_H

#include "Event.h"

class Level;

class LevelChangedEvent : public Event {
public:
    LevelChangedEvent(Level* newLevel);

    const Level* newLevel() const { return _newLevel; }

private:
    const Level* _newLevel;
};

#endif