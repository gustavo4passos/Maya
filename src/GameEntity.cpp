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

bool GameEntity::Load(int xPos, int yPos, int width, int height, TextureID sprite, float scale, bool flip)
{
    _position = Vector2D(xPos, yPos);
    _width = width;
    _height = height;
    _defaultTexture = sprite;
    _scale = scale;
    _flip = flip;

    _currentFrame = 0;
    _currentRow = 0;
    _numFrames = 1;

    return true;
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
    _currentFrame = int(SDL_GetTicks()/80) % _numFrames;
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
    _defaultTexture = 0;
}









