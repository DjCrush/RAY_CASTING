#pragma once

#include <cmath>

class Vector2
{
public:
    Vector2(double X, double Y);
    Vector2();
    double normalize();

    Vector2 operator + (const Vector2& right) const;
    Vector2& operator += (const Vector2& right);
    Vector2& operator -= (const Vector2& right);
	Vector2 operator * (double right) const;
    Vector2& operator *= (double right);
    Vector2 operator / (double right) const;
    static double distance(const Vector2& left, const Vector2& right);
	double x = 0.0; ///< X coordinate of the vector
    double y = 0.0; ///< Y coordinate of the vector
};

Vector2 operator - (const Vector2& left, const Vector2& right);
