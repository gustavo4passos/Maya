#ifndef GOLEM_H
#define GOLEM_H

#include "GameObject.h"


class Golem : public GameObject {

public:
	Golem(float x, float y);

	void Update() override;
	void Draw(Renderer* renderer, float deltaTime) override;
	bool OnNotify(Event* event) override;
	void HandleInput() override;

};

#endif