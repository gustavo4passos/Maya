#include "../include/Button.h"

#include "../include/EventDispatcher.h"
#include "../include/GameSwitches.h"
#include "../include/PlayerHitTeleportEvent.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/SoundPlayer.h"
#include "../include/ServiceLocator.h"
#include "../include/LevelChangedEvent.h"

Button::Button(float x, float y, const std::string& activatesSwitch, bool isAlreadyPressed)
:   GameObject(CollisionRect(Rect(x, y, 31, 12), CollisionBehavior::BLOCK, 1, 19), 32, 32),
    _activatesSwitch(activatesSwitch),
    _isPressed(isAlreadyPressed)
{ }

void Button::Update() {
   Rect playerRect = ServiceLocator::GetPlayer()->collisionRect();

    if(!_isPressed){
        if(PhysicsEngine::IsOnTop(&_collisionRect, &playerRect)) {
            TurnOn();
            _collisionRect.setH(0);
            _collisionRect.setW(0);
            _collisionRect.setCollisionBehavior(CollisionBehavior::IGNORE);
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
    renderer->Draw(ResourceManager::GetTexture("button"), &src, &dst);
}

void Button::TurnOn() {
    _isPressed = true;
    SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("button-press"), false);
}
