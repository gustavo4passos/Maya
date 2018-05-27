#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <string>

#include "Vector2D.h"
#include "Renderer.h"

class GameEntity
{
public:

    GameEntity();
    virtual ~GameEntity();

    virtual void Load(int xPos, int yPos, int width, int height, std::string textureFile, float scale=1, bool flip=false);
	virtual bool Draw(Renderer*);
	virtual bool Update(unsigned int frameTime);
	virtual void Clean();

    // Getters
    Vector2D position() { return _position; }	
	Vector2D velocity() { return _velocity; }
	Vector2D acceleration() { return _acceleration; }
    int height() { return _height; }
	int width() { return _width; }
	float scale() { return _scale; }
    bool IsFlipped() { return _flip; }
    bool IsLoaded() { return _loaded; }
	

    // Setters
    void setPosition(int x, int y) { _position = Vector2D(x,y); }
	void setVelocity(int x, int y) { _velocity = Vector2D(x,y); }
	void setAcceleration(int x, int y) { _acceleration = Vector2D(x,y); }
    void setPosition(Vector2D vec) { _position = vec; }
    void setVelocity(Vector2D vec) { _velocity = vec; }
	void setAcceleration(Vector2D vec) { _acceleration = vec; }
    void Flip() { _flip = !_flip; }

protected:

    bool _loaded;

    // Body
    Texture* _texture;
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


