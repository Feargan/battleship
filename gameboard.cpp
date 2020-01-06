#include "gameboard.h"

#include <algorithm>
#include <unordered_map>
#include <iostream>

CGameBoard::CGameBoard()
{
	
}


CGameBoard::CGameBoard(int Width, int Height)
{
	m_Field = CField(Width, Height);
	m_Field.fill(CTile(CState::EMPTY));
}

CGameBoard::CGameBoard(const CGameBoard & r)
	: m_Field(r.m_Field)
{
	std::unordered_map <std::shared_ptr<const CShip>, std::shared_ptr<CShip>> Remap;
	// copy values and make new pointers
	for (auto& s : r.m_Ships)
	{
		m_Ships.emplace_back(std::make_shared<CShip>(*s));
		Remap[s] = m_Ships.back(); // tiles with the same old address will obtain this new pointer
	}
	// remap all tiles that have old instances
	for (int i = 0; i < m_Field.getWidth(); i++)
	{
		for (int j = 0; j < m_Field.getHeight(); j++)
		{
			const auto& Owner = m_Field.at(i, j).getOwner();
			if (Owner)
				m_Field.at(i, j).setOwner(Remap[Owner]);
		}
	}
}

CGameBoard& CGameBoard::operator=(const CGameBoard& r)
{
	CGameBoard s(r);
	std::swap(*this, s);
	return *this;
}

CGameBoard::~CGameBoard()
{

}

CTile::CState CGameBoard::attack(int x, int y)
{
	if (!m_Field.checkPoint(x, y))
		return CState::NO_ATTACK;
	return m_Field.at(x, y).attack();
}

bool CGameBoard::canAttack(int x, int y)
{
	if (!m_Field.checkPoint(x, y))
		return false;
	auto State = m_Field.at(x, y).getState();
	return State == CState::EMPTY || State == CState::TAKEN;
}

bool CGameBoard::destroyed() const
{
	// all ships must be destroyed
	for (const auto &s : m_Ships)
		if (!s->isDestroyed())
			return false;
	return true;
}

const std::vector<std::shared_ptr<CShip>>& CGameBoard::getShips() const
{
	return m_Ships;
}

const CGameBoard::CField & CGameBoard::getField() const
{
	return m_Field;
}