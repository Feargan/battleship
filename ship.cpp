#include "ship.h"

#include <iostream>

CShip::CShip()
	: m_Health(0)
{
}

void CShip::increaseHealth()
{
	m_Health++;
}

void CShip::decreaseHealth()
{
	if(m_Health > 0)
		m_Health--;
}

int CShip::getHealth() const
{
	return m_Health;
}

bool CShip::isDestroyed() const
{
	return m_Health <= 0 ? true : false;
}