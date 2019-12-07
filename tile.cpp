#include "tile.h"
#include "ship.h"

CTile::CTile(CState State)
{
	m_Owner = nullptr;
	m_State = State;
}


CTile::CTile(const std::shared_ptr<CShip>& Owner)
{
	m_Owner = Owner;
	m_Owner->increaseHealth();
	m_State = TAKEN;
}

CTile::~CTile()
{
	/*if(m_Owner)
		m_Owner->decreaseHealth();*/
}

CTile::CState CTile::getState() const
{
	if (m_Owner && m_Owner->isDestroyed())
		return DESTROYED;
	return m_State;
}

void CTile::setState(CState State)
{
	m_State = State;
}

void CTile::setOwner(const std::shared_ptr<CShip>& Owner)
{
	m_Owner = Owner;
}

const std::shared_ptr<CShip>& CTile::getOwner() const
{
	return m_Owner;
}

CTile::CState CTile::attack()
{
	if (m_State == HIT || m_State == MISS)
		return CState::NO_ATTACK;
	if (m_Owner && m_State == TAKEN)
	{
		m_Owner->decreaseHealth();
		m_State = HIT;
		return m_Owner->isDestroyed() ? CState::DESTROYED : CState::HIT;
	}

	switch (m_State)
	{
	case EMPTY:
		m_State = MISS;
		break;
	case TAKEN:
		m_State = HIT;
		break;
	}
	return m_State;
}
