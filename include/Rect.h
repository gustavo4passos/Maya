#ifndef RECT_H
#define RECT_H

#include "Vector2D.h"

class Rect {
public:
    Rect(float x, float y, int w, int h) 
    :   _w(w), 
        _h(h) 
    { 
        this->_position = Vector2D(x, y);
    }

    Rect(Vector2D position, int w, int h) : _position(position), _w(w), _h(h) { }

    inline  Vector2D const& position() const { return this->_position; }
    inline float x() const { return this->_position.x(); }
    inline float y() const { return this->_position.y(); }
    inline int w() const { return _w; }
    inline int h() const { return _h; }
	
	inline float left() { return x(); }
	inline float right() { return x() + _w; } 
	inline float top() { return y(); }
	inline float bottom() { return y() + _h; } 

    inline void setX(float x) { this->_position.setX(x); }
    inline void setY(float y) { this->_position.setY(y); }
    inline void setW(int w) { this->_w = w; }
    inline void setH(int h) { this->_h = h; }

    void setPosition(Vector2D& position) {
        this->_position.setX(position.x());
        this->_position.setY(position.y());
    }
    void setPosition(float x, float y) {
        this->_position.setX(x);
        this->_position.setY(y);
    }   

protected:
    Vector2D _position;
    int _w, _h;

};

#endif
