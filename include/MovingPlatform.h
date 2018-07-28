#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

#include "GameObject.h"

class MovingPlatform : public GameObject {
public:
    MovingPlatform(Vector2D position);

    virtual void Update();
    virtual void Draw(Renderer* renderer, float deltaTime);

    void Move(Vector2D destination, float transitionSpeed);
};

#endif