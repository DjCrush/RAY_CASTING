#pragma once
#include "Player.h"
#include "Vector2.h"

extern int arField[20][20];

class Bullet
{
public:
	Bullet(const Player& player);
	void update();
	bool getExist() const;
	int getAngle() const;
	Vector2 getCoord() const;
private:
	Vector2 m_vecCoord;
	Vector2 m_vecDXDY;
	int m_iAngle;
	bool m_bIsAlive;
};
