#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <string>
#include "GameState.h"
#include "Renderer.h"
#include "Maya.h"

class GameEnemy;
class GuiMenu;
class Region;

class PlayState : public GameState {
public:    
    PlayState(){}
    ~PlayState(){}

    virtual void HandleInput();
    virtual void Update();
    virtual void Render(Renderer* renderer, float deltatime);
    virtual bool OnEnter();
    virtual bool OnExit();
    virtual void Pause();
    virtual void Resume();

    virtual const std::string& stateID(){
        return _playID;
    }

private:
    Camera* _camera;
	Player* _maya;
	GuiMenu* _infoMenu;
    Region* _region;

    bool _isPaused;
    static const std::string _playID;

};

#endif