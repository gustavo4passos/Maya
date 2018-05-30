#include <iostream>

#include "../include/GameEntity.h"
#include "../include/ErrorHandler.h"
#include "../include/Rect.h"

GameEntity::GameEntity() : _loaded(false), _texture(NULL)
{}

GameEntity::~GameEntity()
{
    if(_texture != NULL) delete _texture;
}

void GameEntity::Load(int xPos, int yPos, int width, int height, std::string textureFile, float scale, bool flip)
{
    _position = Vector2D(xPos, yPos);
    _width = width;
    _height = height;
    _texture = new Texture(textureFile);
    _scale = scale;
    _flip = flip;
 
    _loaded = true;
}

bool GameEntity::Draw(Renderer* renderer)
{
    if(_loaded){
        Rect src = Rect(0,0, _width, _height);
        Rect dest = Rect(_position, _width, _height);        
        renderer->Draw(_texture, &src, &dest);
        return true;
    }
    LOG_ERROR("Entity is not loaded");
    return false;    
}

bool GameEntity::Update(unsigned int frameTime)
{
    if(_loaded){
        _velocity += _acceleration;
        _position += _velocity*((float)frameTime/1000);
        return true;
    }
    LOG_ERROR("Entity is not loaded");
    return false;    
}

void GameEntity::Clean()
{
    _position = Vector2D(0,0);
    _width = 0;
    _height = 0;
    _scale = 0;
    _flip = false;
    delete _texture;
    _texture = NULL;

    _loaded = false;
}









