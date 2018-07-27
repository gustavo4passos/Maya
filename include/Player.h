#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
 

class Player : public GameObject
{



public:

    Player(float x, float y, int w, int h);
    Player(const CollisionRect& collisionRect, int spriteW, int spriteH);

    virtual ~Player();


    virtual void Draw(Renderer*, float deltaTime) override;
    virtual void HandleInput();
    virtual void Update() override;
    virtual bool OnNotify(Event* event) override;
    
protected:

    enum PlayerState { 
        STAND,
        RUN,
        JUMP,
        RUN_ATTACK,
        JUMP_ATTACK,
        STAND_ATTACK    
    };


    PlayerState _currentState;

    virtual void ChangeState(PlayerState) = 0;
    
   
};

#endif