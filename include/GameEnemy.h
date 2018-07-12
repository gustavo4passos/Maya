#include "GameObject.h"

class GameEnemy : public GameObject {
public:
	GameEnemy(float x, float y, float w, float h) :  
		GameObject(x, y, w, h) {
	}

	inline const int damage() { return _damage; }

private:
	int _damage;
};
