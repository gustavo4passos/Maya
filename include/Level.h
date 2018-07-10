#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "Rect.h"
#include "Tileset.h"

class Layer;
class GameEnemy;
class GameObject;
class Renderer;

class Level {
public:
    Level(Tileset* tileset, int width, int height, int tileWidth, int tileHeight);
    ~Level();
	
	void DrawBackground(Renderer* renderer, float positionInterpolation);
	
	void AddBackgroundLayer(Layer* layer);
	void AddCollisionRect(Rect* rect);
	
	const std::vector<Rect*>& collisionRects() { return _collisionRects; }
	const std::vector<GameObject*>& gameObjects() { return _gameObjects; }
	const std::vector<GameEnemy*>& enemies() { return _enemies; }

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
	std::vector<GameEnemy*> _enemies;

	int _width, _height;
	int _tileWidth, _tileHeight;
};

#endif
