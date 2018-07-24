#include "../include/Level.h"

#include "../include/Layer.h"
#include "../include/ResourceManager.h"
#include "../include/GameEnemy.h"
#include "../include/Renderer.h"

Level::Level(Tileset* tileset, int width, int height, int tileWidth, int tileHeight, const std::string& filename) 
:	_tileset(tileset), 
	_width(width),
	_height(height),
	_tileWidth(tileWidth),
	_tileHeight(tileHeight),
	_filename(filename)
{ }

Level::~Level() {
	// Delete collision rects
	for(auto rect = _collisionRects.begin(); rect !=  _collisionRects.end(); rect++){
		delete *rect;
		*rect = nullptr;
	}
	_collisionRects.clear();

	// Delete background layers
	for(auto backgroundLayer = _backgroundLayers.begin(); backgroundLayer != _backgroundLayers.end(); backgroundLayer++){
		delete *backgroundLayer;
		*backgroundLayer = nullptr;
	}
	_backgroundLayers.clear();

	// Delete game objects
	for(auto gameObject = _gameObjects.begin(); gameObject != _gameObjects.end(); gameObject++){
		delete *gameObject;
		*gameObject = nullptr;
	}
	_gameObjects.clear();

	// Delete enemies
	for(auto enemy = _enemies.begin(); enemy != _enemies.end(); enemy++) {
		delete *enemy;
		*enemy = nullptr;
	}

	// Deletes tileset texture
	ResourceManager::DeleteTexture(_tileset->name());
	
	// Delete tileset
	delete _tileset;
	_tileset = nullptr;
}

void Level::Update() {
	for(auto gameObject = _gameObjects.begin(); gameObject != _gameObjects.end(); gameObject++) {
		(*gameObject)->Update();
	}

	for(auto enemy = _enemies.begin(); enemy != _enemies.end(); enemy++){
		(*enemy)->Update();
	}
}

void Level::DrawBackground(Renderer* renderer, float deltaTime){
	for(auto layer = _backgroundLayers.begin(); layer != _backgroundLayers.end(); layer++){
		(*layer)->Draw(renderer);
	}

	for(auto gameObject = _gameObjects.begin(); gameObject != _gameObjects.end(); gameObject++) {
		(*gameObject)->Draw(renderer, deltaTime);
	}

	for(auto enemy = _enemies.begin(); enemy != _enemies.end(); enemy++){
		(*enemy)->Draw(renderer, deltaTime);
	}
}

void Level::AddBackgroundLayer(Layer* layer){
	if(layer == nullptr){
		LOG_ERROR("Unable to add Background Layer: Layer is NULL");
		return;
	}
	_backgroundLayers.push_back(layer);
}

void Level::AddCollisionRect(CollisionRect* rect){
	if(rect == nullptr){
		LOG_ERROR("Unable to add rect: rect is NULL");
		return;
	}
	_collisionRects.push_back(rect); 

}

void Level::AddEnemy(GameEnemy* enemy){
	if(enemy == nullptr) {
		LOG_WARNING("Unable to add enemy. Enemy is NULL.");
		return;
	}
	_enemies.push_back(enemy);
}

void Level::AddGameObject(GameObject* gameObject) {
	if(gameObject == nullptr) {
		LOG_WARNING("Unable to add Game Object. Game Object is NULL.");
	}
	_gameObjects.push_back(gameObject);
}