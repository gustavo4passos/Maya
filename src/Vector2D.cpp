#include "../include/Vector2D.h"

#include <cmath>
#include <stdexcept>

Vector2D::Vector2D() : _x(0), _y(0) {}

Vector2D::Vector2D(const Vector2D& vec) : _x(vec._x), _y(vec._y) {}

Vector2D::Vector2D(float x, float y) : _x(x), _y(y) {}

float Vector2D::DotProduct(const Vector2D& vec) const {
	return this->_x * vec._x + this->_y * vec._y;
}

float Vector2D::Length() const {
	return std::sqrt(pow(this->_x, 2.f) + pow(this->_y, 2.f));
}

Vector2D Vector2D::operator+(const Vector2D& rhs) const {
	return Vector2D(this->_x + rhs._x, this->_y + rhs._y);
}

Vector2D& Vector2D::operator +=(const Vector2D& rhs) {
	this->_x += rhs._x;
	this->_y += rhs._y;
	return *this;
}

Vector2D Vector2D::operator-() const {
	return Vector2D(-this->_x, -this->_y);
}

Vector2D Vector2D::operator-(const Vector2D& rhs) const {
	return Vector2D(this->_x - rhs._x, this->_y - rhs._y);
}

template<typename T>
Vector2D Vector2D::operator*(const T& factor) const {
	return Vector2D(this->_x * factor, this->_y * factor);
}

template<>
Vector2D Vector2D::operator*<Vector2D>(const Vector2D& rhs) const {
	return Vector2D(this->_x * rhs._x, this->_y * rhs._y);
}

template<typename T>
Vector2D& Vector2D::operator*=(const T& factor) {
	this->_x *= factor;
	this->_y *= factor;
	return *this;
}

template<>
Vector2D& Vector2D::operator*=<Vector2D>(const Vector2D& rhs) {
	this->_x *= rhs._x;
	this->_y *= rhs._y;
	return *this;
}


template<typename T>
Vector2D Vector2D::operator/(const T& factor) const {
	if(factor == 0) throw std::overflow_error("Divide by zero exception");
	return Vector2D(this->_x / factor, this->_y / factor);
}

template<> Vector2D Vector2D::operator/<Vector2D>(const Vector2D& rhs) const {
		if(rhs._x == 0 || rhs._y == 0) {
			throw std::overflow_error("Divide by zero execption");
		}
		return Vector2D(this->_x / rhs._x, this->_y / rhs._y);
}

template<typename T>
Vector2D& Vector2D::operator/=(const T& factor) {
	if (factor == 0) throw std::overflow_error("Divide by zero exception");
	this->_x /= factor;
	this->_y /= factor;
	return *this;
}

template<>
Vector2D& Vector2D::operator/=<Vector2D>(const Vector2D& rhs) {
	if(rhs._x == 0 || rhs._y == 0) {
		throw std::overflow_error("Divide by zero exception");
	}

	this->_x /= rhs._x;
	this->_y /= rhs._y;
	return *this;
}
