#pragma once

#include "matrix.h"
#include "tile.h"

class CShip
{
	int m_Health;
public:
	CShip();

	void increaseHealth();
	void decreaseHealth();
	int getHealth() const;
	/*
		a destroyed ship is an equivalent of ship with a health less or equal 0
	*/
	bool isDestroyed() const;
};
