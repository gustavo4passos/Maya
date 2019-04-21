#ifndef GUIMENU_H
#define GUIMENU_H

#include "EventListener.h"

class Renderer;

class GuiMenu : public EventListener {
public:
    GuiMenu() { };
    virtual void Update() = 0;
    virtual void HandleInput() = 0;
    virtual void Render(Renderer* renderer) = 0;
    virtual void Clean() = 0;
};

#endif