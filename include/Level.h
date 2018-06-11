#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "GameObject.h"
#include "Renderer.h"
#include "Tileset.h"

class Level {
public:
    Level();
    ~Level(){}
	
	void DrawBackground(Renderer* renderer, float positionInterpolation);
	
	std::vector<Rect*>& collisionRects() { return _collisionRects; }
	std::vector<GameObject*>& gameObjects() { return _gameObjects; }
		
private:
	Level* _currentLevel;
    Tileset* _tileset;

	std::vector<GameObject*> _gameObjects;
	std::vector<Rect*> _collisionRects;
};

#endif
