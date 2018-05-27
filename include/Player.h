#ifndef PLAYER_H
#define PLAYER_H

#include "GameEntity.h"

class Player : public GameEntity
{
public:

    Player();
    ~Player();
    
    virtual void Load(int xPos, int yPos, int width, int height, std::string textureFile, float scale=1, bool flip=false);
    virtual bool Draw(Renderer*);
    virtual bool Update(unsigned int frameTime);
    virtual void Clean();
        
private:

    virtual void HandleInput();
};

#endif