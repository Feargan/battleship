#include "intelligentplayer.h"

#include <optional>

CIntelligentPlayer::CIntelligentPlayer(IGameController* Controller) : IPlayer(Controller)
{
}


CIntelligentPlayer::~CIntelligentPlayer()
{
}

const CIntelligentPlayer::CField* CIntelligentPlayer::getEnemyField(const IPlayer * Player) const
{
	auto It = m_Contexts.find(Player);
	if (It != m_Contexts.end())
		return &It->second.m_Field;
	return nullptr;
}

void CIntelligentPlayer::onEvent(const CGameEvent& Event)
{
	using CType = CGameEvent::CType;
	switch (Event.m_Type)
	{
	case CType::GAME_STARTED:
		onGameStarted();
		break;
	case CType::PLAYER_ATTACKED:
		onPlayerAttacked(Event.m_PlayerAttackedEvent.m_Victim, Event.m_PlayerAttackedEvent.m_X, Event.m_PlayerAttackedEvent.m_Y, Event.m_PlayerAttackedEvent.m_State);
		break;
	}

}

void CIntelligentPlayer::onPlayerAttacked(const IPlayer * Victim, int x, int y, CTile::CState State)
{
	using CState = CTile::CState;
	bool Destroyed = false;
	if (Victim == this) // we don't track ourselves
		return;
	if (m_Contexts.find(Victim) == m_Contexts.end()) // if somehow there is no enemy board, prepare it
		prepareNewBoard(Victim);
	auto& Context = m_Contexts[Victim];
	auto& Tile = Context.m_Field.at(x, y);
	if (State == CState::DESTROYED)
	{
		// first let it link the tile to its belonging target, the target is removed afterwards
		State = CState::HIT;
		Destroyed = true;
	}
	Tile = CGraphTile(State, {x, y});
	Context.m_Empty--;

	if (State != CState::HIT)
		return;

	CGraphTile* Root = nullptr;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
				continue;
			int nx = x + i;
			int ny = y + j;
			if (!Context.m_Field.checkPoint(nx, ny))
				continue;

			CGraphTile* Child = &Context.m_Field.at(nx, ny);
			CGraphTile* ChildRoot = Child->getRoot();
			if (Child->getState() != CState::HIT)
				continue;
			if (!Root)
			{
				Root = ChildRoot;
				Tile.setRoot(Root);
				auto& MainTarget = Context.m_Targets[Root];
				MainTarget.m_Tiles.insert(&Tile);
				MainTarget.m_TopLeft = { std::min(MainTarget.m_TopLeft.first, x), std::min(MainTarget.m_TopLeft.second, y) };
				MainTarget.m_BottomRight = { std::max(MainTarget.m_BottomRight.first, x), std::max(MainTarget.m_BottomRight.second, y) };
			}
			else if(Root && ChildRoot != Root)
			{
				auto& ChildTarget = Context.m_Targets[ChildRoot];
				auto& MainTarget = Context.m_Targets[Root];
				auto& Branch = ChildTarget.m_Tiles;
				while (Branch.size())
				{
					(*Branch.begin())->setRoot(Root);
					MainTarget.m_Tiles.insert(*Branch.begin());
					Branch.erase(Branch.begin());
				}
				MainTarget.m_TopLeft = { std::min(MainTarget.m_TopLeft.first, ChildTarget.m_TopLeft.first), std::min(MainTarget.m_TopLeft.second, ChildTarget.m_TopLeft.second) };
				MainTarget.m_BottomRight = { std::max(MainTarget.m_BottomRight.first, ChildTarget.m_BottomRight.first), std::max(MainTarget.m_BottomRight.second, ChildTarget.m_BottomRight.second) };
				MainTarget.m_LastMatch = std::max(MainTarget.m_LastMatch, ChildTarget.m_LastMatch);
				MainTarget.m_MatchingRotation = CRotation::CValue::NONE;
				Context.m_Targets.erase(ChildRoot);
			}
		}
	}
	if (!Root)
	{
		Root = &Tile;
		Tile.setRoot(&Tile);
		auto& Target = Context.m_Targets[Root];
		Target.m_Tiles.insert(&Tile);
		Target.m_TopLeft = Target.m_BottomRight = { x, y };
	}

	if (Destroyed)
	{
		for (auto t : Context.m_Targets[Root].m_Tiles)
		{
			if (!(t->getState() == CState::HIT || t->getState() == CState::DESTROYED))
				continue;
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					int xch = t->getPos().first + i;
					int ych = t->getPos().second + j;
					if (Context.m_Field.checkPoint(xch, ych) && Context.m_Field[{xch, ych}].getState() == CState::EMPTY)
					{
						Context.m_Field[{xch, ych}] = CGraphTile(CState::MISS, { xch, ych });
						Context.m_Empty--;
					}
				}
			}
			t->setState(CState::DESTROYED);
		}
		Context.m_Targets.erase(Root);
	}
}

void CIntelligentPlayer::prepareNewBoard(const IPlayer * Enemy)
{
	auto Size = getController()->getPreset()->getBoardSize();
	auto& Context = m_Contexts[Enemy];
	Context.m_Field = CField(Size.first, Size.second);
	Context.m_Field.fill(CGraphTile(CTile::CState::EMPTY));
	Context.m_Empty = Size.first*Size.second;
}

void CIntelligentPlayer::onGameStarted()
{
	m_Contexts.clear();
	auto Players = getController()->getPlayers();
	for (const IPlayer* p : Players)
		if(p != this)
			prepareNewBoard(p);
}
