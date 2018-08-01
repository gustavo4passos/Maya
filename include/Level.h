#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "CollisionRect.h"
#include "Tileset.h"

class Layer;
class Enemy;
class GameObject;
class Renderer;

class Level {
public:
    Level(Tileset* tileset, int width, int height, int tileWidth, int tileHeight, const std::string& filename);
    ~Level();
	
	void Update();
	void DrawBackground(Renderer* renderer, float deltaTime);
	void DrawForeground(Renderer* renderer, float deltaTime);
	
	void AddBackgroundLayer(Layer* layer);
	void AddForegroundLayer(Layer* layer);
	void AddCollisionRect(CollisionRect* rect);
	void AddEnemy(Enemy* enemy);
	void AddGameObject(GameObject* gameObject);
	
	const std::vector<CollisionRect*>& collisionRects() { return _collisionRects; }
	const std::vector<GameObject*>& gameObjects() { return _gameObjects; }
	const std::vector<Enemy*>& enemies() { return _enemies; }

	Tileset* tileset() { return _tileset; }
	
	inline int width() const { return _width; }
	inline int height() const { return _height; }
	inline int tileWidth() const { return _tileWidth; }
	inline int tileHeight() const { return _tileHeight; }

	const std::string& filename() const { return _filename; }
		
	friend class InfoMenuGL3;

private:
    Tileset* _tileset;
	std::vector<Layer*> _backgroundLayers;
	std::vector<Layer*> _foregroundLayers;
	std::vector<GameObject*> _gameObjects;
	std::vector<CollisionRect*> _collisionRects;
	std::vector<Enemy*> _enemies;

	int _width, _height;
	int _tileWidth, _tileHeight;

	// Filename for debugging purposes
	std::string _filename;
};

#endif
