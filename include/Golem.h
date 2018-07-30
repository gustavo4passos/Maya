#ifndef GOLEM_H
#define GOLEM_H

#include "Enemy.h"


class Golem : public Enemy {

protected:
	enum GolemState {
		CROUCH,
		WALK,
		ATTACKING,
		CHASING,
		DEAD,
	};

public:
	Golem(float x, float y, const std::string& switchRequired = "");

	void Update() override;
	void Draw(Renderer* renderer, float deltaTime) override;
	bool OnNotify(Event* event);

private:
	void ChangeState(GolemState state);
	void StandWalk();
	void GetUp();

	int _life = 24;

	bool _attacking = false;
	GolemState _currentState;
	std::string _switchRequired;

	Vector2D _startPosition;
};

#endif