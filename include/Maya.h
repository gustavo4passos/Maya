#ifndef MAYA_H
#define MAYA_H

#include "Player.h"

class Maya : public Player
{
public:

    Maya(const CollisionRect& collisionRect, int spriteW, int spriteH);
    ~Maya();
    
    //void Load(int xPos, int yPos, int width, int height, std::string sprite, float scale=1, bool flip=false);
    void Draw(Renderer*, float positionFactor);
    void HandleInput();
    void Update();
    //void Clean();

private:
    void ChangeState(PlayerState);
};

#endif