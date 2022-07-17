#include "Bullet.h"

Bullet::Bullet(const Player& player)
	 : m_vecCoord(player.getCoord()),
	m_vecDXDY(cos(DEG_TO_RAD(player.getAngle())) * 2, sin(DEG_TO_RAD(player.getAngle())) * 2),
	m_iAngle(player.getAngle()), m_bIsAlive(true)
	{}

void Bullet::update()
{
	if (m_bIsAlive)
	{
		m_vecCoord = m_vecCoord + m_vecDXDY;
		if (m_vecCoord.x < 0 || m_vecCoord.x > 799 || m_vecCoord.y < 0 || m_vecCoord.y > 799
			|| arField[static_cast<int>(m_vecCoord.y / 40)][static_cast<int>(m_vecCoord.x / 40)])
		{
			m_bIsAlive = false;
		}
	}
}

bool Bullet::getExist() const
{
	return m_bIsAlive;
}

int Bullet::getAngle() const
{
	return m_iAngle;
}

Vector2 Bullet::getCoord() const
{
	return m_vecCoord;
}
