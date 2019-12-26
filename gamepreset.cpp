#include "gamepreset.h"
#include "gameboard.h"

#include <cmath>

CGamePreset::CGamePreset()
{
}


CGamePreset::~CGamePreset()
{
}

void CGamePreset::setBoardSize(CSize Size)
{
	m_BoardSize = Size;
}

CGamePreset::CSize CGamePreset::getBoardSize() const
{
	return m_BoardSize;
}

void CGamePreset::setMaxShipSize(int Max)
{
	if(Max >= 0)
		m_Sizes.resize(Max, 1);
}

int CGamePreset::getMaxShipSize() const
{
	return m_Sizes.size();
}

void CGamePreset::setShipAmount(int Size, int Amount)
{
	int i = Size - 1;
	if (i >= 0 && i < static_cast<int>(m_Sizes.size()))
		m_Sizes[i] = Amount;
}

int CGamePreset::getShipAmount(int Size) const
{
	int i = Size - 1;
	if (i >= 0 && i < static_cast<int>(m_Sizes.size()))
		return m_Sizes[i];
	return 0;
}

float CGamePreset::indicator() const
{
	int Loose = 0, Strict = 0;
	for (int i = 1; i<=static_cast<int>(m_Sizes.size()); i++)
	{
		Loose += ((i + 2) * 3)*m_Sizes[i-1];
		Strict += ((i + 1) * 2)*m_Sizes[i-1];
	}
	int BoardArea = m_BoardSize.first*m_BoardSize.second;
	if (Loose < BoardArea)
		return std::numeric_limits<float>::infinity();
	return static_cast<float>(BoardArea - Strict) / BoardArea;//Strict > BoardArea ? 0.f : static_cast<float>(BoardArea-Strict) / BoardArea;
}

float CGamePreset::successChance() const
{
	static constexpr float Mean = 0.088101f;
	static constexpr float StdDev = 0.067701f;
	return 0.5f*(1+std::erf((indicator()-Mean)/(StdDev*std::sqrt(2))));
}

