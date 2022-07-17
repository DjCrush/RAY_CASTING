#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#include "Vector2.h"

#define DEG_TO_RAD(degrees) M_PI * (degrees) / 180

extern int arField[20][20];

class Player
{
public:
	void update();
	int getAngle() const;
	Vector2 getCoord() const;
	void setMovingDirection(int iDirection);
	void setRotationDirection(int iRotationDirection);
	Vector2 m_vecCoord{180.0, 250.0};
	int m_iAngle = 250;
	int m_iRotationDirection = 0;
	int m_iMovingDirection = 0;
};
