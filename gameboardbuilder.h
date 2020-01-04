#pragma once
#include "gameboard.h"
#include "tileset.h"

class CGameBoardBuilder
{
	using CState = CTile::CState;
	using CField = CGameBoard::CField;

	CGameBoard m_Board;
	CField& m_Field;
	std::vector<std::shared_ptr<CShip>>& m_Ships;

	int m_Empty;

	std::vector<int> m_ShipCounters;
	CTileSet m_NewShip;

	const CGamePreset* m_Preset;
public:
	CGameBoardBuilder(const CGamePreset* Preset=nullptr);
	~CGameBoardBuilder();

	bool canPlace(int x, int y) const;
	bool place(int x, int y);
	void remove(int x, int y);
	void removeShip(int x, int y);
	bool commit();
	void autoFill();
	bool isReady() const;
	int remaining(int Size) const;
	void clear();
	bool isEmpty() const;
	int getEmpty() const;

	CGameBoard getBoard() const;
	const CGamePreset* getPreset() const;
	const CField& getField() const;

	//static void getNeighbours(CField& Field, int x, int y, CTileSet& Set, bool Strict = true, std::function<bool(const CTile&)> Pred = [](const CTile& Tile)-> bool { return true;});
private:
	void set(int x, int y, CState State);
	void surround(CTileSet& Set, bool Strict, CState State, bool CheckAssociation, std::function<bool(const CTile&)> SelPred = [](const CTile &t)->bool { return t.getState() == CTile::CState::EMPTY || t.getState() == CTile::CState::RESERVED; },
		std::function<bool(const CTile&)> AscPred = [](const CTile &t)->bool { return t.getState() == CTile::CState::TAKEN; });
	void getNeighbours(int x, int y, CTileSet& Set, bool Strict = true, std::function<bool(const CTile&)> Pred = [](const CTile& Tile)-> bool { return true;});
};

