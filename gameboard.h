#pragma once

#include "matrix.h"
#include "ship.h"
#include "shiptemplate.h"
#include "gamepreset.h"

#include <list>
#include <memory>
#include <vector>
#include <unordered_set>

class CGameBoard // split into: CGameBoard (attackable), CGameBoardBuilder (no attack)
{
	friend class CGameBoardBuilder;
public:
	using CField = CMatrix<CTile>;
private:
	using CState = CTile::CState;
	CField m_Field;
	std::vector<std::shared_ptr<CShip>> m_Ships;
public:
	CGameBoard();
	CGameBoard(int Width, int Height);
	
	CGameBoard(const CGameBoard& r);
	CGameBoard& operator=(const CGameBoard& r);
	CGameBoard(CGameBoard&& r) = default;
	CGameBoard& operator=(CGameBoard&& r) = default;

	~CGameBoard();

	CTile::CState attack(int x, int y);
	bool canAttack(int x, int y);
	bool destroyed() const;

	const std::vector<std::shared_ptr<CShip>>& getShips() const;
	const CField& getField() const;
};

