#include "../include/Golem.h"
#include "../include/Renderer.h"
#include "../include/ResourceManager.h"

Golem::Golem(float x, float y)
:	GameObject(CollisionRect(Rect(x,y,26,23),CollisionBehavior::BLOCK,14,33),52,56)
{ }

void Golem::Draw(Renderer* renderer, float deltaTime){
	Rect rect1 = Rect(0,0,_spriteW,_spriteH);
	Rect rect2 = Rect(_collisionRect.originX(),_collisionRect.originY(),_spriteW,_spriteH);

	renderer->Draw(ResourceManager::GetTexture("../res/sprites/static_golem.png"),&rect1,&rect2);
	//first rect: dimension inside the original img
}

void Golem::HandleInput(){
	//ola
}

void Golem::Update(){
	//belessaman
}

bool Golem::OnNotify(Event* event){
	return true;
}