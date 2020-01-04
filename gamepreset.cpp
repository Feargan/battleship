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
	int Loose = 0 ;
	float Strict = 0.f;
	for (int i = 1; i<=static_cast<int>(m_Sizes.size()); i++)
	{
		/*Loose += ((i + 2) * 3)*m_Sizes[i-1];
		Strict += ((i + 1) * 2)*m_Sizes[i-1];*/
		Loose += ((i + 2) * 3)*m_Sizes[i - 1];
		Strict += ((i + 1) * 2)*m_Sizes[i - 1];// * std::pow(0.9625f, i);
	}
	int BoardArea = m_BoardSize.first*m_BoardSize.second;
	if (Loose < BoardArea)
		return std::numeric_limits<float>::infinity();
	return static_cast<float>(BoardArea - Strict) / BoardArea;//Strict > BoardArea ? 0.f : static_cast<float>(BoardArea-Strict) / BoardArea;
}

static inline float cdf(float x, float Mean, float StdDev)
{
	return 0.5f*(1 + std::erf((x - Mean) / (StdDev*std::sqrt(2.f))));
}

float CGamePreset::minSuccessChance() const
{
	if (indicator() == std::numeric_limits<float>::infinity())
		return 1.f;
	static const float Mean = 0.136410f;
	static const float StdDev = 0.077328f;
	return cdf(indicator(), Mean, StdDev);
}

float CGamePreset::maxSuccessChance() const
{
	if (indicator() == std::numeric_limits<float>::infinity())
		return 1.f;
	static const float Mean = 0.062398f;
	static const float StdDev = 0.071962f;
	return cdf(indicator(), Mean, StdDev);
}

