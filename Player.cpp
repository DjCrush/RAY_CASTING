#include "Player.h"

void Player::update()
{
	Vector2 dXdY(cos(DEG_TO_RAD(m_iAngle)), sin(DEG_TO_RAD(m_iAngle)));
	if (m_iMovingDirection < 0)
	{
		dXdY *= -1.0;
	}
	if (m_iMovingDirection)
	{
		m_vecCoord += dXdY;
		if (arField[static_cast<int>(m_vecCoord.y / 40)][static_cast<int>(m_vecCoord.x / 40)] ||
			m_vecCoord.x < 0 || m_vecCoord.x > 799 || m_vecCoord.y < 0 || m_vecCoord.y > 799)
		{
			m_vecCoord = m_vecCoord - dXdY;
		}
	}
	m_iAngle += m_iRotationDirection;
	if (m_iAngle == -1)
	{
		m_iAngle += 359;
	}
	else if (m_iAngle == 360)
	{
		m_iAngle = 0;
	}
}

int Player::getAngle() const
{
	return m_iAngle;
}

Vector2 Player::getCoord() const
{
	return m_vecCoord;
}

void Player::setMovingDirection(int iDirection)
{
	m_iMovingDirection = iDirection;
}

void Player::setRotationDirection(int iRotationDirection)
{
	m_iRotationDirection = iRotationDirection;
}
