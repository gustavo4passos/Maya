#ifndef PushableObject_H
#define PushableObject_H

#include "GameObject.h"

class PushableObject : public GameObject
{
public:

    PushableObject(float x, float y);
    PushableObject(const CollisionRect& collisionRect, int spriteW, int spriteH, std::string texture="box");
    ~PushableObject();
    
    void Update();
	void Draw(Renderer* renderer, float deltaTime);
};

#endif