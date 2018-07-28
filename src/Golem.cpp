#include "../include/Golem.h"

Golem::Golem(float x, float y)
:	Enemy(CollisionRect(Rect(x,y,26,23),CollisionBehavior::BLOCK,14,33),52,56)
{ 
	_textureName = "../res/assets/static-golem.png";
	_kind = Kind::ENEMY;
}

void Golem::Draw(Renderer* renderer, float deltaTime){
	Enemy::Draw(renderer, deltaTime);
}

bool Golem::OnNotify(Event* event){
	return Enemy::OnNotify(event);
}

void Golem::Update(){
	Enemy::Update();
}