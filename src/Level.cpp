#include "../include/Level.h"

#include "../include/Layer.h"

Level::Level(Tileset* tileset, int width, int height, int tileWidth, int tileHeight) :
	_tileset(tileset), 
	_width(width),
	_height(height),
	_tileWidth(tileWidth),
	_tileHeight(tileHeight)
{ }

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
