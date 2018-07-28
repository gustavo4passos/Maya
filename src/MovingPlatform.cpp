#include "../include/MovingPlatform.h"

#include "../include/Renderer.h"
#include "../include/ResourceManager.h"

MovingPlatform::MovingPlatform(Vector2D position) 
:   GameObject(CollisionRect(Rect(position.x(), position.y(), 64, 8), CollisionBehavior::BLOCK, 0, 0), 64, 8)
{ }

void MovingPlatform::Move(Vector2D destination, float transitionSpeed) {
    setPosition(destination.x(), destination.y());
}

void MovingPlatform::Update() {

}

void MovingPlatform::Draw(Renderer* renderer, float deltaTime) {
    Rect src = Rect(0.f, 0.f, _spriteW, _spriteH);
    Rect dst = Rect(_collisionRect.originX(), _collisionRect.originY(), _spriteW, _spriteH);

    renderer->Draw(ResourceManager::GetTexture("wooden-moving-platform"), &src, &dst);
}