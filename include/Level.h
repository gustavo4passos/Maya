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
	
	void AddCollisionRect(Rect* rect);
	void DrawBackground(Renderer* renderer, float positionInterpolation);
	
	std::vector<Rect*>& collisionRects() { return _collisionRects; }
	std::vector<GameObject*>& gameObjects() { return _gameObjects; }
		
	friend class InfoMenuGL3;
private:
	Level* _currentLevel;
    Tileset* _tileset;

	std::vector<GameObject*> _gameObjects;
	std::vector<Rect*> _collisionRects;
};

#endif
