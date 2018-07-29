#include "../include/Door.h"

#include "../include/EventDispatcher.h"
#include "../include/GameSwitches.h"
#include "../include/PlayerHitTeleportEvent.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"
#include "../include/ServiceLocator.h"

Door::Door(const CollisionRect& collisionRect, int spriteW, int spriteH, const std::string& switchRequired, bool initiallyOpen)
 :  GameObject(collisionRect, spriteW, spriteH),
    _switchRequired(switchRequired),
    _open(initiallyOpen)
{
    _collisionRect.setCollisionBehavior(CollisionBehavior::IGNORE);
}

void Door::Update() {
    if(ServiceLocator::GetGameSwitches()->CheckSwitch(_switchRequired) && !IsOpen()){
        Open();
    }

    if(_open) {
        CollisionRect playerRct = ServiceLocator::GetPlayer()->collisionRect();
        CollisionRect collision = _collisionRect;
        collision.setW(_collisionRect.w() - 15);
        collision.setX(x() + 7);

        if(PhysicsEngine::CheckCollision(&collision, &playerRct)){
            std::unique_ptr<Event> playerHitTeleport(new PlayerHitTeleportEvent("mountain", Vector2D(-14, 289)));
            EventDispatcher::Notify(playerHitTeleport.get());
        }
    }
}

void Door::Draw(Renderer* renderer, float deltaTime) {
    Rect src = Rect(0, 0, 32, 32);
    if(_open) src.setX(32);

    Rect dst = Rect(_collisionRect.originX(), _collisionRect.originY(), _spriteW, _spriteH);
    renderer->Draw(ResourceManager::GetTexture("door"), &src, &dst);
}
