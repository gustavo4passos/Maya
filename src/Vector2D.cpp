#include "../include/Vector2D.h"

#include <cmath>

Vector2D::Vector2D() : _x(0), _y(0) {}

Vector2D::Vector2D(const Vector2D& vec) : _x(vec._x), _y(vec._y) {}

Vector2D::Vector2D(float x, float y) : _x(x), _y(y) {}

float Vector2D::DotProduct(const Vector2D& vec) const {
	return this->_x * vec._x + this->_y * vec._y;
}

float Vector2D::Length() const {
	return std::sqrt(pow(this->_x, 2.f) + pow(this->_y, 2.f));
}
