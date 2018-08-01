#ifndef BUTTON_H
#define BUTTON_H

#include "GameObject.h"
#include "Animation.h"

class Button : public GameObject {
public:    
    Button(float x, float y, const std::string& activatesSwitch, bool initiallyPressed);

    void Update() override;
    void Draw(Renderer* renderer, float deltaTime) override;

    virtual inline bool IsOn() const { return _isPressed; }
    virtual inline void TurnOn();
    virtual inline void TurnOf() { _isPressed = false;  }
    virtual inline void Toggle() { _isPressed = !_isPressed; }
    
private:
    std::string _activatesSwitch;
    bool _isPressed;

    Animation _animation;
};

#endif