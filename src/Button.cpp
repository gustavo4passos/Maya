#include "../include/Button.h"

#include "../include/GameSwitches.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"

Button::Button(const CollisionRect& collisionRect, int spriteW, int spriteH, const std::string& activatesSwitch, bool isAlreadyPressed)
:   GameObject(collisionRect, spriteW, spriteH),
    _activatesSwitch(activatesSwitch),
    _isPressed(isAlreadyPressed)
{ }

void Button::Update() {
   Rect playerRect = ServiceLocator::GetPlayer()->collisionRect();

    if(!_isPressed){
        if(PhysicsEngine::IsOnTop(&_collisionRect, &playerRect)) {
             _isPressed = true;
            _collisionRect.setH(0);
            _collisionRect.setW(0);
            _collisionRect.setOffsetY(_collisionRect.offsetY() + 15);
            ServiceLocator::GetGameSwitches()->ActivateSwitch(_activatesSwitch);
        }
    }
}

void Button::Draw(Renderer* renderer, float deltatime) {
    Rect src = Rect(0.f, 0.f, 32, 32);
    if(_isPressed){ 
        src.setX(32);
    }

    Rect dst = Rect(_collisionRect.originX(), _collisionRect.originY(), _spriteW, _spriteH);
    if(_isPressed) { dst.setY(dst.y() +15); }
    renderer->Draw(ResourceManager::GetTexture("button"), &src, &dst);
} 