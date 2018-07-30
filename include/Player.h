#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject
{
public:

    Player(float x, float y, int w, int h);
    Player(const CollisionRect& collisionRect, int spriteW, int spriteH);
    virtual ~Player();
    
    //virtual void Load(int xPos, int yPos, int width, int height, std::string sprite, float scale=1, bool flip=false);

    

    virtual void Draw(Renderer*, float deltaTime);
    virtual void HandleInput();
    virtual void Update();
    virtual bool OnNotify(Event* event);
    
    // virtual void Clean();

protected:

    enum PlayerState { 
        STAND,
        RUN,
        JUMP,
        RUN_ATTACK,
        JUMP_ATTACK,
        STAND_ATTACK,
        BOUNCE_STUCK,
        BOUNCE,
        DRAG_WALL,
        INVENCIBLE,
        DYING,
        DEAD,
    };

    PlayerState _currentState;
    virtual void ChangeState(PlayerState) = 0;
    int _health;
};

#endif