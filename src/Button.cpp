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
    _isPressed(isAlreadyPressed),
    _animation(Spritesheet("button", 1, 2), 2)
{
    _animation.PauseAtFrame(0);
    _kind = Kind::BUTTON;
}

void Button::Update() {

    if(!_isPressed){
        // Checks if the switch activated by the button is not already on
        if(ServiceLocator::GetGameSwitches()->CheckSwitch(_activatesSwitch)) TurnOn();

        while(!_unresolvedCollisionEvents.empty()) {
            CollisionEvent collision = _unresolvedCollisionEvents.front();
            if(collision.collisionPosition == CollisionPosition::TOP_COLLISION && !_isPressed) {
                TurnOn();            
            }
            _unresolvedCollisionEvents.pop();
        }
    }
}

void Button::Draw(Renderer* renderer, float deltatime) {
    _animation.Render(renderer, _collisionRect.origin()); 
}

void Button::TurnOn() {
    _isPressed = true;
    SoundPlayer::PlaySFX(ResourceManager::GetSoundEffect("button-press"), false);
     // Show pressed animation
    _animation.PauseAtFrame(1);
    _collisionRect.setCollisionBehavior(CollisionBehavior::IGNORE);
    ServiceLocator::GetGameSwitches()->ActivateSwitch(_activatesSwitch);
}
