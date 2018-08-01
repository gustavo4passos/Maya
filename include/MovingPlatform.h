#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

#include "GameObject.h"
#include "Spritesheet.h"

class MovingPlatform : public GameObject {
public:
    MovingPlatform(Vector2D origin, Vector2D destination, bool loops = true, const std::string& switchRequired = "");

    virtual void Update();
    virtual void Draw(Renderer* renderer, float deltaTime);
    virtual inline const Vector2D& velocity() { return _displacement; }
    const Vector2D& displacement() const { return _displacement; }

private:
    void Move();
    Vector2D _origin;
    Vector2D _destination;
    bool _loops;
    std::string _switchRequired;
    bool _reachedDestination;
    Spritesheet _spritesheet;

    Vector2D _displacement;
    bool _on;
};

#endif