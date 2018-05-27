#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>

class GameState{
public:
    GameState(){}
    ~GameState(){}
    
    virtual void Update() = 0;
    virtual void Render() = 0;

    virtual bool OnEnter() = 0;

    virtual bool OnExit(){ return true; } //----clear the textures----
    virtual std::string stateID() const = 0;

protected:
    std::vector<std::string> _textureIDs;
};

#endif