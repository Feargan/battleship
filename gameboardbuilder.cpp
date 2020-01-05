#include "gameboardbuilder.h"

#include <algorithm>

CGameBoardBuilder::CGameBoardBuilder(const CGamePreset* Preset) : m_Preset(Preset), m_Field(m_Board.m_Field), m_Ships(m_Board.m_Ships)
{
	if (!m_Preset)
		return;
	rebuild();
}


CGameBoardBuilder::~CGameBoardBuilder()
{
}

void CGameBoardBuilder::rebuild()
{
	auto Size = m_Preset->getBoardSize();
	if (Size.first > 0 && Size.second > 0)
		m_Board = CGameBoard(Size.first, Size.second);
	clear();
}

bool CGameBoardBuilder::canPlace(int x, int y) const
{
	// check collisions
	if (!m_Field.checkPoint(x, y))
		return false;
	auto TileState = m_Field[{x, y}].getState();
	int NumTiles = static_cast<int>(m_NewShip.size());
	if (NumTiles >= m_Preset->getMaxShipSize())
		return false;
	if (!NumTiles && TileState != CTile::CState::EMPTY)
		return false;
	if (NumTiles && TileState != CTile::CState::RESERVED)
		return false;
	return true;
}

bool CGameBoardBuilder::place(int x, int y)
{
	if (!canPlace(x, y))
		return false;
	set(x, y, CTile::CState::TAKEN);
	m_NewShip.insert({ x, y });
	surround(m_NewShip, true, CState::RESERVED, false);
	return true;
}

void CGameBoardBuilder::remove(int x, int y)
{
	if (!m_Field.checkPoint(x, y))
		return;
	const CTile* RemovedTile = &m_Field[{x, y}];
	auto It = std::find_if(m_NewShip.begin(), m_NewShip.end(),
		[this, RemovedTile](const std::pair<int, int>& p)
	{
		if (&m_Field[p] == RemovedTile)
			return true;
		return false;
	});
	if (It == m_NewShip.end())
	{
		removeShip(x, y);
		surround(m_NewShip, true, CState::RESERVED, false);
		return;
	}
	CTileSet Next, Visited;
	getNeighbours(x, y, Next, true, [](const CTile& Tile)->bool {return Tile.getState() == CState::TAKEN;});
	set(x, y, m_NewShip.size() > 1 ? CState::RESERVED : CState::EMPTY); // temporarily unlink tree
	if (Next.size())
	{
		auto p = *Next.begin();
		Next.clear();
		Next.insert(p);
	}
	while (Next.size())
	{
		auto PairIt = Next.begin();
		Visited.insert(*PairIt);
		CTileSet New;
		getNeighbours(PairIt->first, PairIt->second, New, true, [](const CTile& Tile)->bool {return Tile.getState() == CState::TAKEN;});
		for (auto& p : New)
		{
			if (Visited.find(p) == Visited.end())
				Next.insert(p);
		}
		Next.erase(PairIt);
	}
	if (Visited.size() != m_NewShip.size() - 1)
	{
		set(x, y, CState::TAKEN);
		return;
	}
	m_NewShip.erase(It);
	CTileSet Unreserve;
	Unreserve.insert({ x,y });
	surround(Unreserve, true, CState::EMPTY, true);
}

void CGameBoardBuilder::removeShip(int x, int y)
{
	if (!m_Field.checkPoint(x, y))
		return;
	auto Owner = m_Field[{x, y}].getOwner();
	if (!Owner)
		return;
	auto It = std::find_if(m_Ships.begin(), m_Ships.end(),
		[Owner](const std::shared_ptr<CShip>& s) -> bool
	{
		if (Owner == s)
			return true;
		return false;
	});
	CTileSet CheckErase;
	for (int i = 0; i < m_Field.getWidth(); i++)
	{
		for (int j = 0; j < m_Field.getHeight(); j++)
		{
			if (m_Field[{i, j}].getOwner() == Owner)
			{
				//m_Field[{i, j}] = CState::EMPTY;
				set(i, j, CState::EMPTY);
				CheckErase.insert({ i,j });
			}
		}
	}
	surround(CheckErase, false, CState::EMPTY, true, [](const CTile &t)->bool { return t.getState() == CState::MISS; }, [](const CTile &t)->bool { return t.getState() == CState::TAKEN && t.getOwner(); });
	if (It != m_Ships.end())
	{
		m_ShipCounters[Owner->getHealth() - 1]--;
		m_Ships.erase(It);
	}
}

bool CGameBoardBuilder::commit()
{
	if (!m_NewShip.size())
		return false;
	m_Ships.emplace_back(std::make_shared<CShip>()); //CShip::CMeta{ *m_NewShip.begin(), CRotation::CValue::NONE, m_NewShip.size() }
	auto& Ship = m_Ships.back();
	for (auto p : m_NewShip)
		m_Field[p] = CTile(Ship);
	surround(m_NewShip, false, CState::MISS, false);
	m_ShipCounters[m_NewShip.size() - 1]++;
	if (static_cast<int>(m_NewShip.size()) > m_Preset->getMaxShipSize() || m_ShipCounters[m_NewShip.size() - 1] > m_Preset->getShipAmount(m_NewShip.size()))
	{
		auto p = *m_NewShip.begin();
		removeShip(p.first, p.second);
		m_NewShip.clear();
		return false;
	}
	m_NewShip.clear();
	return true;
}
void CGameBoardBuilder::autoFill()
{
	int BoardWidth = m_Field.getWidth();
	commit();

	CTileSet Vitrified;

	for (int n = 1; n <= static_cast<int>(m_ShipCounters.size()); n++)
	{
		while (remaining(n) > 0)
		{
			if (!m_Empty)
				break;

			int RemainingTiles = n - 1;
			int Index = rand() % m_Empty + 1;
			int i;
			for (i = 0; Index > 0; i++)
			{
				if (m_Field.at(i%BoardWidth, i / BoardWidth).getState() == CState::EMPTY)
					Index--;
			}
			i--;
			int x = i % BoardWidth, y = i / BoardWidth;
			place(x, y);
			CTileSet Reserved;
			while (RemainingTiles)
			{
				getNeighbours(x, y, Reserved, true, [](const CTile &t)->bool { return t.getState() == CTile::CState::RESERVED;});
				if (!Reserved.size())
					break;
				RemainingTiles--;
				int Next = rand() % Reserved.size();
				auto It = Reserved.begin();
				for (; Next > 0; Next--)
					++It;
				x = It->first;
				y = It->second;
				place(x, y);
				Reserved.erase({ x,y });
			}
			CTileSet NextVitrify = m_NewShip;
			if (!commit())
			{
				for (auto p : NextVitrify)
				{
					set(p.first, p.second, CState::VITRIFIED);
					Vitrified.insert(p);
				}
			}
		}
		if (!m_Empty)
			break;
	}
	for (auto p : Vitrified)
	{
		set(p.first, p.second, CState::EMPTY);
	}
}

bool CGameBoardBuilder::isReady() const
{
	if (m_Preset->getMaxShipSize() != m_ShipCounters.size())
		return false;
	for (unsigned int i = 0; i < m_ShipCounters.size(); i++)
	{
		if (m_ShipCounters[i] != m_Preset->getShipAmount(i + 1))
			return false;
	}
	return true;
}

int CGameBoardBuilder::remaining(int Size) const
{
	return m_Preset->getShipAmount(Size) - m_ShipCounters[Size - 1];
}

/*void CGameBoardBuilder::cleanup()
{
	
}*/

void CGameBoardBuilder::clear()
{
	m_Field.fill(CState::EMPTY);
	m_Empty = m_Field.getWidth()*m_Field.getHeight();
	m_ShipCounters.clear();
	m_ShipCounters.resize(m_Preset->getMaxShipSize(), 0);
	m_NewShip.clear();
	m_Ships.clear();
}

bool CGameBoardBuilder::isEmpty() const
{
	return m_Empty == m_Field.getHeight()*m_Field.getWidth();
}

int CGameBoardBuilder::getEmpty() const
{
	return m_Empty;
}

CGameBoard CGameBoardBuilder::getBoard() const
{
	CGameBoard Board(m_Board);
	Board.m_Field.fill(CState::EMPTY, [](const CTile &t) -> bool { return t.getState() == CState::MISS; });
	return Board;
}

const CGamePreset * CGameBoardBuilder::getPreset() const
{
	return m_Preset;
}

const CGameBoard::CField & CGameBoardBuilder::getField() const
{
	return m_Field;
}

void CGameBoardBuilder::set(int x, int y, CState State)
{
	auto CurState = m_Field[{x, y}].getState();
	if (CurState == State)
		return;
	if ((CurState != CState::TAKEN && CurState != CState::MISS && CurState != CState::VITRIFIED) && (State == CState::TAKEN || State == CState::MISS || State == CState::VITRIFIED))
		m_Empty--;
	else if ((CurState != CState::EMPTY && CurState != CState::RESERVED) && (State == CState::EMPTY || State == CState::RESERVED))
		m_Empty++;
	m_Field[{x, y}] = State;
}

void CGameBoardBuilder::surround(CTileSet & Set, bool Strict, CState State, bool CheckAssociation, std::function<bool(const CTile&)> Pred, std::function<bool(const CTile&)> AscPred)
{
	CTileSet Reserve;
	for (auto p : Set)
		getNeighbours(p.first, p.second, Reserve, Strict, Pred);
	for (auto p : Reserve)
	{
		if (CheckAssociation)
		{
			CTileSet Associated;
			getNeighbours(p.first, p.second, Associated, Strict, AscPred);
			if (!Associated.size())
				set(p.first, p.second, State);
			continue;
		}
		set(p.first, p.second, State);
	}

}

void CGameBoardBuilder::getNeighbours(int x, int y, CTileSet & Set, bool Strict, std::function<bool(const CTile&)> Pred)
{
	auto Add = [this, &Set, Pred](int x, int y) -> void
	{
		if (m_Field.checkPoint(x, y) && Pred(m_Field[{ x, y }]))
			Set.insert({ x,y });
	};
	Add(x - 1, y);
	Add(x + 1, y);
	Add(x, y - 1);
	Add(x, y + 1);
	if (Strict)
		return;
	Add(x - 1, y - 1);
	Add(x + 1, y - 1);
	Add(x - 1, y + 1);
	Add(x + 1, y + 1);
}

