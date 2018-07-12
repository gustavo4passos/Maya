#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>

class Renderer;

class GameState{
public:
    GameState(){}
    virtual ~GameState(){}
    
    virtual void HandleInput() = 0;
    virtual void Update() = 0;
    virtual void Render(Renderer* renderer) = 0;

    virtual bool OnEnter() = 0;

    virtual bool OnExit(){ return true; } //----clear the textures----
    virtual std::string stateID() const = 0;

protected:
    std::vector<std::string> _textureIDs;
};

#endif