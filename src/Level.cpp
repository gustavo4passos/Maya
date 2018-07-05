#include "../include/Level.h"

#include "../include/Layer.h"
#include "../include/ResourceManager.h"
#include "../include/GameEnemy.h"

Level::Level(Tileset* tileset, int width, int height, int tileWidth, int tileHeight) :
	_tileset(tileset), 
	_width(width),
	_height(height),
	_tileWidth(tileWidth),
	_tileHeight(tileHeight)
{ }

Level::~Level() {
	// Delete collision rects
	for(auto rect = _collisionRects.begin(); rect !=  _collisionRects.end(); rect++){
		delete *rect;
		*rect = NULL;
	}
	_collisionRects.clear();

	// Delete background layers
	for(auto backgroundLayer = _backgroundLayers.begin(); backgroundLayer != _backgroundLayers.end(); backgroundLayer++){
		delete *backgroundLayer;
		*backgroundLayer = NULL;
	}
	_backgroundLayers.clear();

	// Delete game objects
	for(auto gameObject = _gameObjects.begin(); gameObject != _gameObjects.end(); gameObject++){
		delete *gameObject;
		*gameObject = NULL;
	}
	_gameObjects.clear();

	// Deletes tileset texture
	ResourceManager::DeleteTexture(_tileset->name());
	
	// Delete tileset
	delete _tileset;
	_tileset = NULL;
}

void Level::DrawBackground(Renderer* renderer, float velocityInterpolation){
	for(auto layer = _backgroundLayers.begin(); layer != _backgroundLayers.end(); layer++){
		(*layer)->Draw(renderer);
	}
}

void Level::AddBackgroundLayer(Layer* layer){
	if(layer == NULL){
		LOG_ERROR("Unable to add Background Layer: Layer is NULL");
		return;
	}
	_backgroundLayers.push_back(layer);
}

void Level::AddCollisionRect(Rect* rect){
	if(rect == NULL){
		LOG_ERROR("Unable to add rect: rect is NULL");
		return;
	}
	_collisionRects.push_back(rect); 
}
