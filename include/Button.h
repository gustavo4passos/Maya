#ifndef BUTTON_H
#define BUTTON_H

#include "GameObject.h"

class Button : public GameObject {
public:
    Button(int x, int y, int w, int h, int collisionOffsetX, int collisionOffsetY,
        int collisionRectW, int collisionRectH, const std::string& activatesSwitch, bool initiallyOn = false);
    
    Button(const CollisionRect& collisionRect, int spriteW, int spriteH, const std::string& activatesSwitch, bool initiallyPressed);

    void Update() override;
    void Draw(Renderer* renderer, float deltaTime) override;

    virtual inline bool IsOn() const { return _isPressed; }
    virtual inline void TurnOn() { _isPressed = true; }
    virtual inline void TurnOf() { _isPressed = false;  }
    virtual inline void Toggle() { _isPressed = !_isPressed; }
    
private:
    std::string _activatesSwitch;
    bool _isPressed;
};

#endif