#include "../include/PlatformSwitch.h"

#include "../include/GameSwitches.h"
#include "../include/ResourceManager.h"
#include "../include/Renderer.h"
#include "../include/ServiceLocator.h"
#include "../include/SoundPlayer.h"

PlatformSwitch::PlatformSwitch(float x, float y, const std::string& activatesSwitch) 
:   GameObject(CollisionRect(Rect(x, y, 96, 21), CollisionBehavior::BLOCK), 96, 21),
    _activatesSwitch(activatesSwitch),
    _originPosition(x, y),
    _on(false),
    _displacement(0.f, 0.f),
    _distanceFromActivationArea(20.f)
{
    _textureName = "platform-switch";
}

void PlatformSwitch::Update() {    
    Rect playerRect = ServiceLocator::GetPlayer()->collisionRect();
    if(PhysicsEngine::IsOnTop(&_collisionRect, &playerRect)) {
        if(y() < (_originPosition.y() + _distanceFromActivationArea)){
            _velocity.setY(1.f);
            _displacement.setY(_velocity.y());
            setPosition(x(), y() + _velocity.y());
            ServiceLocator::GetPlayer()->setPosition(ServiceLocator::GetPlayer()->x(),  ServiceLocator::GetPlayer()->y() + _displacement.y());
        }
        else {
            if(!_on) {
                SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("button-press"), false);
                _velocity.setY(0);
                ServiceLocator::GetGameSwitches()->ActivateSwitch(_activatesSwitch);
                _on = true;
            }
        }
    }
    else {
        if(_on) {
            _on = false;
            ServiceLocator::GetGameSwitches()->DeactivateSwitch(_activatesSwitch);
        }
        if(y() > _originPosition.y()) {
            _velocity.setY(-1.f);
            _displacement.setY(1.f);
            setPosition(x(), y() + _velocity.y());
        }
        else {
            _velocity.setY(0.f);
        }
    }
}

void PlatformSwitch::Draw(Renderer* renderer, float deltaTime) {
    Rect src = Rect(0, 0, _spriteW, _spriteH);
    Rect dst = Rect(_collisionRect.originX(), _collisionRect.originY(), _spriteW, _spriteH);

    renderer->Draw(ResourceManager::GetTexture(_textureName), &src, &dst);
}