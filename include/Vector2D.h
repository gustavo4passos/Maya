#ifndef VECTOR2D_H
#define VECTOR2D_H

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

	Vector2D operator+(const Vector2D&) const;
	Vector2D& operator+=(const Vector2D& rhs);

	Vector2D operator-() const;                //prefix unary minus
	Vector2D operator-(const Vector2D&) const; //binary minus

	template<typename T>
	Vector2D operator*(const T&) const;

	template<typename T>
	Vector2D& operator*=(const T&);

	template<typename T>
	Vector2D operator/(const T&) const;

	template<typename T>
	Vector2D& operator/=(const T&);

private:
	float _x;
	float _y;

};

#endif
