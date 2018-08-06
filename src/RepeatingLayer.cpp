#include "../include/RepeatingLayer.h"

#include "../include/Renderer.h"
#include "../include/ResourceManager.h"

RepeatingLayer::RepeatingLayer(const std::string& name, int width, int height, Vector2D offset, float zDistance, bool foreground)
:    Layer(name, width, height, "", zDistance, foreground),
    _offset(offset)
{ }

void RepeatingLayer::Draw(Renderer* renderer) {
    renderer->DrawRepeatingLayer(ResourceManager::GetTexture(_name), _height, _offset.y(), _zDistance);
}

