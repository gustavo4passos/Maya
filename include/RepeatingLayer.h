#ifndef REPEATINGLAYER_H
#define REPEATINGLAYER_H

#include "Layer.h"
#include "Rect.h"

class Renderer;

class RepeatingLayer : public Layer {
public:
    // The name of the layer will be used to retrieve the layer texture from the Resource Manager
    RepeatingLayer(const std::string& name, int width, int height, Vector2D offset, float zDistance = 1.f, bool foreground = false);

    void Draw(Renderer* renderer);

private:
    Vector2D _offset;

};



#endif