#include "../include/Level.h"

Level::Level() {
	_collisionRects.push_back(new Rect(-200.f, 200.f, 880, 670));
	_collisionRects.push_back(new Rect(200.f, 139.f, 200.f, 10.f));
	_collisionRects.push_back(new Rect( 50.f, 79.f, 100.f, 10.f));
	_collisionRects.push_back(new Rect(0.f, 180.f, 70.f, 20.f));
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
