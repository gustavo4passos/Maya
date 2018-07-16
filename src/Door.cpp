#include "../include/Door.h"

#include "../include/GameSwitches.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"

 Door::Door(int x, int y, int w, int h, int collisionOffsetX, int collisionOffsetY,
        int collisionRectW, int collisionRectH, const std::string& switchRequired, bool initiallyOpen)
:   GameObject(x, y, w, h, collisionOffsetX, collisionOffsetY, collisionRectW, collisionRectH),
    _switchRequired(switchRequired),
    _open(initiallyOpen)
{

}

void Door::Update() {
    if(ServiceLocator::GetGameSwitches()->CheckSwitch(_switchRequired)){
        Open();
    }
}

void Door::Draw(Renderer* renderer, float deltaTime) {
    Rect src = Rect(0, 0, 32, 32);
    if(_open) src.setX(32);

    Rect dst = Rect(_position, _w, _h);

    renderer->Draw(ResourceManager::GetTexture("door"), &src, &dst);
}