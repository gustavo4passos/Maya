#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
 
<<<<<<< HEAD
enum PlayerState { 
    STAND,
    RUN,
    JUMP,
    RUN_ATTACK,
    JUMP_ATTACK,
    STAND_ATTACK
};

=======


>>>>>>> 31ed02c98b2118871c5e6528cb1d41bd8ee75668
class Player : public GameObject
{



public:

    Player(float x, float y, int w, int h);
    Player(const CollisionRect& collisionRect, int spriteW, int spriteH);
<<<<<<< HEAD
    ~Player();
=======
    virtual ~Player();
>>>>>>> 31ed02c98b2118871c5e6528cb1d41bd8ee75668
    
    //virtual void Load(int xPos, int yPos, int width, int height, std::string sprite, float scale=1, bool flip=false);

    

<<<<<<< HEAD
    virtual void Draw(Renderer*, float deltaTime) override;
    virtual void HandleInput() override;
    virtual void Update() override;
    virtual bool OnNotify(Event* event) override;
=======
    virtual void Draw(Renderer*, float deltaTime);
    virtual void HandleInput();
    virtual void Update();
    virtual bool OnNotify(Event* event);
>>>>>>> 31ed02c98b2118871c5e6528cb1d41bd8ee75668
    
    // virtual void Clean();

protected:

<<<<<<< HEAD
=======
    enum PlayerState { 
        STAND,
        RUN,
        JUMP,
        RUN_ATTACK,
        JUMP_ATTACK,
        STAND_ATTACK    
    };


>>>>>>> 31ed02c98b2118871c5e6528cb1d41bd8ee75668
    PlayerState _currentState;

    virtual void ChangeState(PlayerState) = 0;
    
   
};

#endif