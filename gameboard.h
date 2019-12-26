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
public:
	using CField = CMatrix<CTile>;
private:
	using CState = CTile::CState;
	struct CPairIntHash{
		inline std::size_t operator()(const std::pair<int, int> & v) const {
			return v.first * 31 + v.second;
		}
	};
	using CTileSet = std::unordered_set<std::pair<int, int>, CPairIntHash>;

	CField m_Field;
	int m_Empty;
	std::vector<std::shared_ptr<CShip>> m_Ships;
	std::vector<int> m_ShipCounters;
	CTileSet m_NewShip;

	const CGamePreset* m_Preset;
public:
	CGameBoard(const CGamePreset* Preset = nullptr);
	CGameBoard(int Width, int Height);
	
	CGameBoard(const CGameBoard& r);
	CGameBoard& operator=(const CGameBoard& r);
	CGameBoard(CGameBoard&& r) = default;
	CGameBoard& operator=(CGameBoard&& r) = default;

	~CGameBoard();

	bool canPlace(int x, int y) const;
	bool place(int x, int y);
	void remove(int x, int y);
	void removeShip(int x, int y);
	bool commit();
	void autoFill();
	bool isReady() const;
	bool destroyed() const;
	int remaining(int Size) const;
	void cleanup(); // tmp
	void clear();
	int getEmpty() const;

	CTile::CState attack(int x, int y);
	bool canAttack(int x, int y);

	const std::vector<std::shared_ptr<CShip>>& getShips() const;
	const CField& getField() const;
	const CGamePreset* getPreset() const;
private:
	void set(int x, int y, CState State);
	void surround(CTileSet& Set, bool Strict, CState State, bool CheckAssociation, std::function<bool(const CTile&)> SelPred = [](const CTile &t)->bool { return t.getState() == CTile::CState::EMPTY || t.getState() == CTile::CState::RESERVED; },
																				   std::function<bool(const CTile&)> AscPred = [](const CTile &t)->bool { return t.getState() == CTile::CState::TAKEN; });
	void getNeighbours(int x, int y, CTileSet& Set, bool Strict = true, std::function<bool(const CTile&)> Pred = [](const CTile& Tile)-> bool { return true;});
};

