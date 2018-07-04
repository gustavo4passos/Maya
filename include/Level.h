#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "GameObject.h"
#include "Renderer.h"
#include "Tileset.h"

class Layer;

class Level {
public:
    Level(Tileset* tileset, int width, int height, int tileWidth, int tileHeight);
    ~Level(){}
	
	void DrawBackground(Renderer* renderer, float positionInterpolation);
	
	void AddBackgroundLayer(Layer* layer);
	void AddCollisionRect(Rect* rect);
	
	std::vector<Rect*>& collisionRects() { return _collisionRects; }
	std::vector<GameObject*>& gameObjects() { return _gameObjects; }

	Tileset* tileset() { return _tileset; }
	
	inline int width() const { return _width; }
	inline int height() const { return _height; }
	inline int tileWidth() const { return _tileWidth; }
	inline int tileHeight() const { return _tileHeight; }
		
	friend class InfoMenuGL3;

private:
	Level* _currentLevel;
    Tileset* _tileset;
	std::vector<Layer*> _backgroundLayers;
	std::vector<GameObject*> _gameObjects;
	std::vector<Rect*> _collisionRects;

	int _width, _height;
	int _tileWidth, _tileHeight;
};

#endif
