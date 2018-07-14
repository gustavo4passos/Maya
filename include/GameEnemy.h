#include "GameObject.h"

class GameEnemy : public GameObject {
public:
	GameEnemy(float x, float y, float w, float h, float collisionOffsetX = 0, float collisionOffsetY = 0, float collisionRectW = 10, float collisionRectH = 30) :  
		GameObject(x, y, w, h, collisionOffsetX, collisionOffsetY, collisionRectW, collisionRectH) {
	}

	inline const int damage() { return _damage; }

private:
	int _damage;
};
