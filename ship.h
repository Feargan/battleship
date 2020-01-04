#pragma once

#include "matrix.h"
#include "tile.h"

#include <vector>

class CShip
{
	int m_Health;
public:
	CShip();

	void increaseHealth();
	void decreaseHealth();
	int getHealth() const;
	bool isDestroyed() const;
};
