#ifndef GOLEM_H
#define GOLEM_H

#include "GameObject.h"


class Golem : public GameObject {

public:
	Golem(float x, float y);

	virtual void Update() override;
	virtual void Draw(Renderer* renderer, float deltaTime) override;
	//virtual bool OnNotify(Event* event);

};

#endif