#ifndef MAYA_H
#define MAYA_H

#include "Player.h"

class Maya : public Player
{
public:

    Maya(float x, float y);
    Maya(const CollisionRect& collisionRect, int spriteW, int spriteH);
    ~Maya();
    
    void Draw(Renderer*, float positionFactor);
    void HandleInput();
    void Update();
    bool OnNotify(Event* event);

    GameObject* weapon() { return _weapon; }

private:
    void ChangeState(PlayerState);
    GameObject* _weapon;
    PlayerState _lastState;
};

#endif