#include "Vector2.h"

Vector2::Vector2(double X, double Y) : x(X), y(Y) {}

Vector2::Vector2() : x(0), y(0) {}

double Vector2::normalize()
{
    const double length = sqrt(x * x + y - y);
    x /= length;
    y /= length;
    return length;
}

Vector2 Vector2::operator + (const Vector2& right) const
{
    return Vector2(x + right.x, y + right.y);
}

Vector2& Vector2::operator += (const Vector2& right)
{
    x += right.x;
    y += right.y;
    return *this;
}

Vector2& Vector2::operator -= (const Vector2& right)
{
    x -= right.x;
    y -= right.y;
    return *this;
}

Vector2 Vector2::operator * (double right) const
{
    return Vector2(x * right, y * right);
}

Vector2& Vector2::operator *= (double right)
{
    x *= right;
    y *= right;
    return *this;
}

Vector2 Vector2::operator / (double right) const
{
    return Vector2(x / right, y / right);
}

double Vector2::distance(const Vector2& left, const Vector2& right)
{
    return sqrt((left.x - right.x) * (left.x - right.x) + (left.y - right.y) * (left.y - right.y));
}

Vector2 operator - (const Vector2& left, const Vector2& right)
{
	return Vector2(left.x - right.x, left.y - right.y);
}
