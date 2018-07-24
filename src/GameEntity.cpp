#include "../include/GameEntity.h"

#include <iostream>
#include <SDL2/SDL.h>

#include "../include/ErrorHandler.h"
#include "../include/Rect.h"

GameEntity::GameEntity()
{}

GameEntity::~GameEntity()
{

}

void GameEntity::Load(int xPos, int yPos, int width, int height, std::string sprite, float scale, bool flip)
{
    _position = Vector2D(xPos, yPos);
    _width = width;
    _height = height;
    _defaultTexture = sprite;
    _scale = scale;
    _flip = flip;

    _frameTime = 0;
    _currentFrame = 0;
    _currentRow = 0;
    _numFrames = 1;
    _numRows = 1;
}

void GameEntity::Draw(Renderer* renderer, float positionFactor)
{
    Vector2D refacPosition = _position + (_velocity * positionFactor);
    Rect src = Rect(_currentFrame*_width,_currentRow*_height, _width, _height);              
    Rect dest = Rect(refacPosition, _width*_scale, _height*_scale);
    renderer->Draw(ResourceManager::GetTexture(_defaultTexture), &src, &dest);    
}

void GameEntity::Update()
{
    _frameTime++;
    _currentFrame = (_frameTime/5) % _numFrames;
    _currentRow = (_frameTime/(5*_numFrames)) % _numRows;
    _velocity += _acceleration;
    _position += _velocity;
    
}

void GameEntity::Clean()
{
    _position = Vector2D(0,0);
    _width = 0;
    _height = 0;
    _scale = 0;
    _flip = false;
    _defaultTexture = "";
}









