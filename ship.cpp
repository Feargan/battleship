#include "ship.h"

#include <iostream>

CShip::CShip()
	: m_Meta({ {0, 0}, CRotation::CValue::NONE, 0 }), m_Health(0)
{
}

CShip::CShip(const CMeta& Meta)
	: m_Meta(Meta), m_Health(0)
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

const CShip::CMeta & CShip::getMeta() const
{
	return m_Meta;
}

void CShip::setMeta(const CMeta & Meta)
{
	m_Meta = Meta;
}