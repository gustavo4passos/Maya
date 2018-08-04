#ifndef REPEATINGBACKGROUND_H
#define REPEATINGBACKGROUND_H

#include "Rect.h"
#include "Renderer.h"

class RepeatingBackground {
public:
    RepeatingBackground(const Rect& dimensions, const std::string& textureName);

    void Draw(Renderer* renderer);

private:
    std::string _textureName;
    Rect _dimensions;
};



#endif