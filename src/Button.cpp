#include "../include/Button.h"

#include "../include/GameSwitches.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"

Button::Button(int x, int y, int w, int h, int collisionOffsetX, int collisionOffsetY,
        int collisionRectW, int collisionRectH, const std::string& activatesSwitch, bool initiallyOn) 
:   GameObject(x, y, w, h, collisionOffsetX, collisionOffsetY, collisionRectW, collisionRectH),
    _activatesSwitch(activatesSwitch),
    _on(initiallyOn)
{ }

void Button::Update() {
   Rect playerRect = ServiceLocator::GetPlayer()->collisionRect();

    if(!_on){
        if(PhysicsEngine::IsOnTop(&_collisionRect, &playerRect)) {
             _on = true;
            _collisionRect.setH(_collisionRect.h() - 15);
            _collisionRect.setY(_collisionRect.y() + 15);
            _collisionOffsetY += 15;
            ServiceLocator::GetGameSwitches()->ActivateSwitch(_activatesSwitch);
        }
    }
}

void Button::Draw(Renderer* renderer, float deltatime) {
    Rect src = Rect(0.f, 0.f, 32, 32);
    if(_on){ 
        src.setX(32);
    }

    Rect dst = Rect(_position, _w, _h);
    renderer->Draw(ResourceManager::GetTexture("button"), &src, &dst);
} 