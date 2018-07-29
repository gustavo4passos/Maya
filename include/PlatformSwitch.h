#ifndef PLATFORMSWITCH_H
#define PLATFORMSWITCH_H

#include "GameObject.h"

class PlatformSwitch : public GameObject {
public:
    PlatformSwitch(float x, float y, const std::string& activatesSwitch);

    virtual void Update();
    virtual void Draw(Renderer* renderer, float deltaTime);

private:
    std::string _activatesSwitch;
    Vector2D _originPosition;
    bool _on;
    Vector2D _displacement;
    float _distanceFromActivationArea;
};

#endif