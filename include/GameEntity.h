#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <string>

#include "Vector2D.h"
#include "Renderer.h"
#include "ResourceManager.h"

class GameEntity
{
public:

    GameEntity();
    virtual ~GameEntity();

    virtual bool Load(int xPos, int yPos, int width, int height, std::string sprite, float scale=1, bool flip=false);
	virtual void Draw(Renderer*, float positionFactor);
	virtual void Update();
	virtual void Clean();

    // Getters
    Vector2D position() { return _position; }	
	Vector2D velocity() { return _velocity; }
	Vector2D acceleration() { return _acceleration; }
    int height() { return _height; }
	int width() { return _width; }
	float scale() { return _scale; }
    bool IsFlipped() { return _flip; }
	
    // Setters
    void setPosition(int x, int y) { _position = Vector2D(x,y); }
	void setVelocity(int x, int y) { _velocity = Vector2D(x,y); }
	void setAcceleration(int x, int y) { _acceleration = Vector2D(x,y); }
    void setPosition(Vector2D vec) { _position = vec; }
    void setVelocity(Vector2D vec) { _velocity = vec; }
	void setAcceleration(Vector2D vec) { _acceleration = vec; }
    void Flip() { _flip = !_flip; }

protected:

    // Body
    std::string _defaultTexture;
    int _width;
    int _height;
    float _scale;
    bool _flip;

    // Motion
    Vector2D _position;
    Vector2D _velocity;
    Vector2D _acceleration;    

    // Frame (We are not using them for now)
    int _currentRow;
	int _currentFrame;
    int _numFrames; 
};

#endif


