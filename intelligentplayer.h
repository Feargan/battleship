#pragma once

#include "player.h"
#include "matrix.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

class CGraphTile : public CTile
{
public:
	using CPos = std::pair<int, int>;
private:
	CGraphTile* m_Root;
	CPos m_Pos;
public:
	CGraphTile(CState State = CState::EMPTY, CPos Pos = { 0, 0 }, CGraphTile* Root = nullptr) : CTile(State), m_Root(Root), m_Pos(Pos) {}

	void setRoot(CGraphTile* Root) { m_Root = Root; }
	CGraphTile* getRoot() { return m_Root; }

	void setPos(CPos Pos) { m_Pos = Pos; }
	CPos getPos() const { return m_Pos; }
};

class CIntelligentPlayer : public IPlayer
{
public:
	using CPos = CGraphTile::CPos;
	using CField = CMatrix<CGraphTile>;
protected:
	struct CTarget
	{
		CTarget() : m_LastMatch(0), m_MatchingRotation(CRotation::CValue::NONE) {}
		std::unordered_set<CGraphTile*> m_Tiles;
		CPos m_TopLeft;
		CPos m_BottomRight;
		CPos m_LayoutPos;
		unsigned int m_LastMatch;
		CRotation m_MatchingRotation;
	};
	struct CContext
	{
		CField m_Field;
		std::unordered_map<CGraphTile*, CTarget> m_Targets;
		int m_Empty;
	};
	std::unordered_map<const IPlayer*, CContext> m_Contexts;
public:
	CIntelligentPlayer(IGameController* Controller);
	virtual ~CIntelligentPlayer();

	//virtual void play() override;

	//void surroundDestroyed(bool Enable);

	const CField* getEnemyField(const IPlayer* Player) const; //
protected:
	virtual void onEvent(const CGameEvent& Event) override;
private:
	void onPlayerAttacked(const IPlayer * Victim, int x, int y, CTile::CState State);
	void prepareNewBoard(const IPlayer* Enemy);
	void onGameStarted();
};

