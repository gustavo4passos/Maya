#include "../include/Level.h"

Level::Level() {
	_collisionRects.push_back(new Rect(0.f, 200.f, 480, 270));
}

void Level::DrawBackground(Renderer* renderer, float positionInterpolation){
	for(std::vector<Rect*>::iterator it = _collisionRects.begin(); 	
	    it != _collisionRects.end();
		it++) {
		Color red;
		red.r = 1.f;
		red.g = 0.f;
		red.b = 0.f;
		red.a = 0.5f;
		renderer->DrawFillRect(*it, &red);
	}
}
