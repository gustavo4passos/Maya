#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "GameObject.h"
#include "Renderer.h"
#include "Tileset.h"

class Layer;
class Level {
public:
    Level();
    ~Level(){}
	
	void DrawBackground(Renderer* renderer, float positionInterpolation);
	
	void AddBackgroundLayer(Layer* layer);
	void AddCollisionRect(Rect* rect);
	std::vector<Rect*>& collisionRects() { return _collisionRects; }
	std::vector<GameObject*>& gameObjects() { return _gameObjects; }
		
	friend class InfoMenuGL3;
private:
	Level* _currentLevel;
    Tileset* _tileset;
	std::vector<Layer*> _backgroundLayers;
	std::vector<GameObject*> _gameObjects;
	std::vector<Rect*> _collisionRects;
};

#endif
