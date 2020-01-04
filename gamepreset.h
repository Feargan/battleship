#pragma once

#include "shiptemplate.h"

#include <vector>

class CGamePreset
{
public:
	using CSize = std::pair<int, int>;
private:
	std::vector<int> m_Sizes;
	CSize m_BoardSize;
public:
	CGamePreset();
	~CGamePreset();
	
	void setBoardSize(CSize Size);
	CSize getBoardSize() const;

	void setMaxShipSize(int Max);
	int getMaxShipSize() const;
	void setShipAmount(int Size, int Amount);
	int getShipAmount(int Size) const;

	float indicator() const;
	float minSuccessChance() const;
	float maxSuccessChance() const;
};

