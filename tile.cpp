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
	m_State = CState::TAKEN;
}

CTile::CTile(const CTile & r) : m_Owner(r.m_Owner), m_State(r.m_State)
{

}

CTile::CTile(CTile&& r) : CTile()
{
	swap(*this, r);
}

CTile& CTile::operator=(CTile r)
{
	swap(*this, r);
	return *this;
}

CTile::~CTile()
{
	if(m_Owner)
		m_Owner->decreaseHealth();
}

CTile::CState CTile::getState() const
{
	if (m_Owner && m_Owner->isDestroyed())
		return CState::DESTROYED;
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
	if (m_State == CState::HIT || m_State == CState::MISS)
		return CState::NO_ATTACK;
	if (m_Owner && m_State == CState::TAKEN)
	{
		m_Owner->decreaseHealth();
		m_State = CState::HIT;
		return m_Owner->isDestroyed() ? CState::DESTROYED : CState::HIT;
	}

	switch (m_State)
	{
	case CState::EMPTY:
		m_State = CState::MISS;
		break;
	case CState::TAKEN:
		m_State = CState::HIT;
		break;
	}
	return m_State;
}

void swap(CTile & l, CTile & r)
{
	using std::swap;
	swap(l.m_Owner, r.m_Owner);
	swap(l.m_State, r.m_State);
}
