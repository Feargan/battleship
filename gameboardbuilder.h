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
	CGameBoardBuilder(const CGamePreset* Preset);
	~CGameBoardBuilder();

	/*
		use when either the board size or the maximum ship size has changed 
	*/
	void rebuild();
	/*
		validity check for place(), read below
	*/
	bool canPlace(int x, int y) const;
	/*
		places a single tile of a new ship
		if it's the first tile - can be placed in any EMPTY slot
		otherwise can be placed only on RESERVED tiles
	*/
	bool place(int x, int y);
	/*
		removes a single tile of a new ship (if possible)
		or removes a previously placed ship
	*/
	void remove(int x, int y);
	/*
		removes a previously placed ship
	*/
	void removeShip(int x, int y);
	/*
		commits newly placed ship
		returns true if the placement is successful (meets the size counter limits)
		returns false otherwise and reverts changes
	*/
	bool commit();
	/*
		automatically fills the board
		might fail if the preset minSuccessChance() is below 100%
		check validity with isReady()
	*/
	void autoFill();
	/*
		returns true if the board is ready to be used in the gameplay
	*/
	bool isReady() const;
	/*
		returns remaining number of ships of specified size required to consider the board as ready
	*/
	int remaining(int Size) const;
	/*
		sets all tiles to EMPTY
	*/
	void clear();
	/*
		returns true if all tiles are in EMPTY state
	*/
	bool isEmpty() const;
	/*
		returns number of EMPTY tiles
	*/
	int getEmpty() const;

	/*
		returns a cleaned-up copy of edited board consisting of TAKEN tiles and emplaced ships
	*/
	CGameBoard getBoard() const;
	const CGamePreset* getPreset() const;
	const CField& getField() const;

	//static void getNeighbours(CField& Field, int x, int y, CTileSet& Set, bool Strict = true, std::function<bool(const CTile&)> Pred = [](const CTile& Tile)-> bool { return true;});
private:
	/*
		update a tile state and count empty tiles
	*/
	void set(int x, int y, CState State);
	/*
	   surround selected tiles in Strict (or not) neighbourhood with selected State
	   use SelPred to filter out a tile, leaving it with an unchanged state
	   use CheckAssociation with AscPred to filter out tiles that are associated with some other tile 
	 */
	void surround(const CTileSet& Set, bool Strict, CState State, bool CheckAssociation, std::function<bool(const CTile&)> SelPred = [](const CTile &t)->bool { return t.getState() == CTile::CState::EMPTY || t.getState() == CTile::CState::RESERVED; },
		std::function<bool(const CTile&)> AscPred = [](const CTile &t)->bool { return t.getState() == CTile::CState::TAKEN; });
	/*
		returns Strict (or not) neighbours of a tile by a reference to a set
		filter out unwanted neighbours by using a predicate - return false to filter
	*/
	void getNeighbours(int x, int y, CTileSet& Set, bool Strict = true, std::function<bool(const CTile&)> Pred = [](const CTile& Tile)-> bool { return true;});
};

