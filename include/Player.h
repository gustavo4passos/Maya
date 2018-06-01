#ifndef PLAYER_H
#define PLAYER_H

#include "GameEntity.h"

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
    
};

#endif