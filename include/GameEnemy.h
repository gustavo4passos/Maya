#include "GameObject.h"

class GameEnemy : public GameObject {
	public:
	GameEnemy(float x, float y, float w, float h) :  
		GameObject(x, y, w, h) {
	}
};
