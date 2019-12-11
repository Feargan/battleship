#pragma once

#include "matrix.h"
#include "ship.h"
#include "shiptemplate.h"
#include "gamepreset.h"

#include <list>
#include <memory>
#include <vector>

class CGameBoard
{
public:
	using CField = CMatrix<CTile>;
private:
	using CState = CTile::CState;

	CField m_Field;
	//std::list<CShip> m_Ships;
	std::vector<std::shared_ptr<CShip>> m_Ships;
	std::vector<int> m_ShipCounters;

	const CGamePreset* m_Preset;
public:
	CGameBoard(const CGamePreset* Preset = nullptr);
	CGameBoard(int Width, int Height);
	
	CGameBoard(const CGameBoard& r);
	CGameBoard& operator=(const CGameBoard& r);
	CGameBoard(CGameBoard&& r) = default;
	CGameBoard& operator=(CGameBoard&& r) = default;

	~CGameBoard();

	bool canPlace(int x, int y, const CShipTemplate& Template) const;
	bool place(int x, int y, const CShipTemplate& Template);
	void remove(int x, int y);
	bool isReady() const;
	bool destroyed() const;

	CTile::CState attack(int x, int y);
	bool canAttack(int x, int y);

	const std::vector<std::shared_ptr<CShip>>& getShips() const; // ----v
	const CField& getField() const; //---> this will be used for UI design
	const CGamePreset* getPreset() const;
};

