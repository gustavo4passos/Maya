#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <string>
#include "../include/GameState.h"

class PlayState : public GameState{
public:    
    PlayState(){}
    ~PlayState(){}

    virtual void Update();
    virtual void Render();
    virtual bool OnEnter();
    virtual bool OnExit();

    virtual const std::string& stateID(){
        return _playID;
    }

private:
    static const std::string _playID;

};

#endif