#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <stdexcept>

class Vector2D {
public:

	Vector2D();
	Vector2D(const Vector2D&);
	Vector2D(float x, float y);

	float x() const { return _x; }
	float y() const { return _y; }

	void setX(float x) { this->_x = x; }
	void setY(float y) { this->_y = y; }

	float DotProduct(const Vector2D& vec) const;
	float Length() const;

	Vector2D operator+(const Vector2D& rhs) const {
		return Vector2D(this->_x + rhs._x, this->_y + rhs._y);
	}

	Vector2D& operator +=(const Vector2D& rhs) {
		this->_x += rhs._x;
		this->_y += rhs._y;
		return *this;
	}

	// Unary minus
	Vector2D operator-() const {
		return Vector2D(-this->_x, -this->_y);
	}

	// Binary minus
	Vector2D operator-(const Vector2D& rhs) const {
		return Vector2D(this->_x - rhs._x, this->_y - rhs._y);
	}

	Vector2D operator*(const Vector2D& rhs) const {
		return Vector2D(this->_x * rhs._x, this->_y * rhs._y);
	}

	// Support for Vector2D * T operations
	template<typename T>
	Vector2D operator*(const T& scalar) const {
		return Vector2D(this->_x * scalar, this->_y * scalar);
	}

	template<typename T>
	Vector2D& operator*=(const T& scalar) {
		this->_x *= scalar;
		this->_y *= scalar;
		return *this;
	}

	Vector2D& operator*=(const Vector2D& rhs) {
		this->_x *= rhs._x;
		this->_y *= rhs._y;
		return *this;
	}

	template<typename T>
	Vector2D operator/(const T& scalar) const {
		if(scalar == 0) throw std::overflow_error("Division by zero is forbidden");
		return Vector2D(this->_x / scalar, this->_y / scalar);
	}

	Vector2D operator/(const Vector2D& rhs) const {
			if(rhs._x == 0 || rhs._y == 0) {
				throw std::overflow_error("Division by zero is forbidden");
			}
			return Vector2D(this->_x / rhs._x, this->_y / rhs._y);
	}

	template<typename T>
	Vector2D& operator/=(const T& scalar) {
		if (scalar == 0) throw std::overflow_error("Division by zero is forbiddenn");
		this->_x /= scalar;
		this->_y /= scalar;
		return *this;
	}

	Vector2D& operator/=(const Vector2D& rhs) {
		if(rhs._x == 0 || rhs._y == 0) {
			throw std::overflow_error("Divide by zero exception");
		}

		this->_x /= rhs._x;
		this->_y /= rhs._y;
		return *this;
	}

private:
	float _x;
	float _y;

};

// Support for T * Vector2D operations
template<typename T>
Vector2D operator*(const T& scalar, const Vector2D& rhs) {
	return rhs * scalar;
}

#endif
