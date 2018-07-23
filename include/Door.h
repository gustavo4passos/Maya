#ifndef DOOR_H
#define DOOR_H

#include "GameObject.h"

class Door : public GameObject {
public:
    Door(const CollisionRect& collisionRect, int spriteW, int spriteH, const std::string& switchRequired = "", bool initiallyOpen = true);

    virtual void Update() override;
    virtual void Draw(Renderer* renderer, float deltaTime);

    void Open() { _open = true; }
    void Close() { _open = false; }

    bool IsOpen() const { return _open; }

private:
    std::string _switchRequired;

    bool _open;
};

#endif
