#ifndef GOLEM_H
#define GOLEM_H

#include "Enemy.h"


class Golem : public Enemy {

public:
	Golem(float x, float y);

	void Update() override;
	void Draw(Renderer* renderer, float deltaTime) override;
	bool OnNotify(Event* event);

};

#endif