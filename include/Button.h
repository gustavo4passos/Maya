#ifndef BUTTON_H
#define BUTTON_H

#include "GameObject.h"

class Button : public GameObject {
public:
    Button(int x, int y, int w, int h, int collisionOffsetX, int collisionOffsetY,
        int collisionRectW, int collisionRectH, const std::string& activatesSwitch, bool initiallyOn = false);

    void Update() override;
    void Draw(Renderer* renderer, float deltaTime) override;

    virtual inline bool IsOn() const { return _on; }
    virtual inline void TurnOn() { _on = true; }
    virtual inline void TurnOf() { _on = false;  }
    virtual inline void Toggle() { _on = !_on; }
private:
    std::string _activatesSwitch;
    bool _on;
};

#endif