#include "../include/PlatformSwitch.h"

#include "../include/GameSwitches.h"
#include "../include/ResourceManager.h"
#include "../include/Renderer.h"
#include "../include/ServiceLocator.h"
#include "../include/SoundPlayer.h"

PlatformSwitch::PlatformSwitch(float x, float y, const std::string& activatesSwitch)
:   GameObject(CollisionRect(Rect(x, y, 96, 14), CollisionBehavior::BLOCK), 96, 14),
    _activatesSwitch(activatesSwitch),
    _originPosition(x, y),
    _on(false),
    _displacement(0.f, 0.f),
    _distanceFromActivationArea(30.f),
    _isBeingSteppedOn(false)
{
    _textureName = "platform-switch";
    _kind = Kind::PLATFORM_SWITCH;
}

void PlatformSwitch::Update() {
    _isBeingSteppedOn = false;
    std::set<GameObject*> objectsOnTop;
    while(!_unresolvedCollisionEvents.empty()) {
        CollisionEvent collision = _unresolvedCollisionEvents.front();
        if(collision.collisionPosition == CollisionPosition::TOP_COLLISION) _isBeingSteppedOn = true;
        objectsOnTop.insert(collision.subject);
        _unresolvedCollisionEvents.pop();
    }
    
    if(_isBeingSteppedOn) {
        if(y() < (_originPosition.y() + _distanceFromActivationArea)){
            _velocity.setY(1.f);
            _displacement.setY(_velocity.y());
            setPosition(x(), y() + _velocity.y());
            for(auto object = objectsOnTop.begin(); object != objectsOnTop.end(); object++) {
               (*object)->setPosition((*object)->x() , (*object)->y() + _displacement.y());
            }
        }
        else {
            if(!_on) {
                SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("button-press"), false);
                ServiceLocator::GetGameSwitches()->ActivateSwitch(_activatesSwitch);
                _velocity.setY(0);
                _displacement.setY(0.f);                
                _on = true;
            }
        }
    }
    else {
        ServiceLocator::GetGameSwitches()->DeactivateSwitch(_activatesSwitch);
        if(_on) {
            _on = false;
        }
        if(y() > _originPosition.y()) {
            _velocity.setY(-1.f);
            _displacement.setY(1.f);
            setPosition(x(), y() + _velocity.y());
        }
        else {
            _velocity.setY(0.f);
            _displacement.setY(0.f);
        }
    }
}

void PlatformSwitch::Draw(Renderer* renderer, float deltaTime) {
    Rect src = Rect(0, 0, _spriteW, _spriteH);
    if(_on) src.setX(_spriteW);
    Rect dst = Rect(_collisionRect.originX(), _collisionRect.originY(), _spriteW, _spriteH);

    renderer->Draw(ResourceManager::GetTexture(_textureName), &src, &dst);
}
