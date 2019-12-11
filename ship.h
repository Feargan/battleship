#pragma once

#include "matrix.h"
#include "tile.h"

#include <vector>

class CShip // : Drawable
{
public:
	using CPos = std::pair<int, int>;
	struct CMeta
	{
		CPos m_Pos;
		CRotation m_Rotation;
		unsigned int m_TemplateId;
	};
private:
	int m_Health;
	CMeta m_Meta;
public:
	CShip();
	CShip(const CMeta& Meta);

	void increaseHealth();
	void decreaseHealth();
	int getHealth() const;
	bool isDestroyed() const;

	const CMeta& getMeta() const;
	void setMeta(const CMeta& Meta);
};
