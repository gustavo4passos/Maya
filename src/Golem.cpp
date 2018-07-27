#include "../include/Golem.h"

Golem::Golem(float x, float y)
:	GameObject(CollisionRect(Rect(x,y,26,23),CollisionBehavior::BLOCK,14,33),52,56)
{ }

void Golem::Draw(Renderer* renderer, float deltaTime){
	renderer->Draw(ResourceManager::GetTexture("../res/sprites/static_golem.png"),Rect(0,0,_spriteW,_spriteH),
		Rect(x,y,_spriteW,_spriteH))
	//first rect: dimension inside the original img
}