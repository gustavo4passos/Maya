#ifndef PLAYER_H
#define PLAYER_H

#include "GameEntity.h"
 
enum PlayerState { 
    STAND,
    RUN,
    JUMP,
    RUN_ATTACK,
    JUMP_ATTACK,
    STAND_ATTACK
};

class Player : public GameEntity
{
public:

    Player();
    ~Player();
    
    virtual void Load(int xPos, int yPos, int width, int height, std::string sprite, float scale=1, bool flip=false);
    virtual void Draw(Renderer*, float positionFactor);
    virtual void HandleInput();
    virtual void Update();
    virtual void Clean();

protected:
    PlayerState _currentState;
    
   
};

#endif