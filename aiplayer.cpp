#include "aiplayer.h"

#include <optional>

CAiPlayer::CAiPlayer(IGameController* Controller) : IPlayer(Controller)
{
}


CAiPlayer::~CAiPlayer()
{
}

/*CGameBoard CAiPlayer::buildBoard()
{
	CGameBoard Board(getController()->getPreset());
	//CGameBoard::CField HelperField(Board.getField().getWidth(), Board.getField().getHeight());
	//int Empty;

	for (unsigned int i = 0; i < getController()->getPreset()->numTemplates(); i++)
	{
		CShipTemplate Template = *getController()->getPreset()->getTemplate(i);
		while (Board.remaining(i))
		{
			int x = rand() % Board.getField().getWidth();
			int y = rand() % Board.getField().getHeight();
			int r = rand() % 4;
			CRotation Rotation;
			switch (r)
			{
			using CValue = CRotation::CValue;
			case 0:
				Rotation.set(CValue::NONE);
				break;
			case 1:
				Rotation.set(CValue::QUARTER);
				break;
			case 2:
				Rotation.set(CValue::HALF);
				break;
			case 3:
				Rotation.set(CValue::THREE_QUARTER);
				break;
			}
			Template.setRotation(Rotation);
			Board.place(x, y, Template);
		}
	}
	return Board;
}*/

#include <iostream>
#include <cstdlib>

void CAiPlayer::play()
{
	using CState = CGraphTile::CState;
	const IPlayer* Victim = getController()->getSuggestedVictim();
	auto& Context = m_Contexts[Victim];
	auto& Targets = Context.m_Targets;
	auto BoardSize = getController()->getPreset()->getBoardSize();
	//if (!Targets.size())
	{
		std::cout << Context.m_Empty;
		int Index = rand() % Context.m_Empty + 1;
		int i;
		for (i = 0; Index > 0; i++)
		{
			if (Context.m_Field.at(i%BoardSize.first, i / BoardSize.first).getState() == CState::EMPTY)
				Index--;
		}
		i--;

		getController()->attack(this, Victim, i%BoardSize.first, i / BoardSize.first);
		return;
	}
	/*

	if (!Targets.size())
	{
		std::cout << Context.m_Empty;
		int Index = rand() % Context.m_Empty+1;
		int i;
		for (i = 0; Index > 0; i++)
		{
			if (Context.m_Field.at(i%BoardSize.first, i / BoardSize.first).getState() == CState::EMPTY)
				Index--;
		}
		i--;
		
		getController()->attack(this, Victim, i%BoardSize.first, i/BoardSize.first);
		return;
	}

	auto& Target = (*Targets.begin()).second;
	const CShipTemplate* Template;
	while ((Template = getController()->getPreset()->getTemplate(Target.m_LastMatch)) && Target.m_LastMatch < getController()->getPreset()->numTemplates())
	{
		CShipLayout Layout = Template->getLayout();
		Layout.setRotation(Target.m_MatchingRotation);

		bool Match = false;
		auto TargetSize = std::make_pair(Target.m_BottomRight.first - Target.m_TopLeft.first+1, Target.m_BottomRight.second - Target.m_BottomRight.second+1);
		do
		{
			
			for (int i = 0; i < Layout.getWidth()-TargetSize.first+1 ; i++)
			{
				int LayoutX = Target.m_TopLeft.first - i;
				if (LayoutX < 0 || LayoutX + Layout.getWidth() > BoardSize.first)
					continue;
				for (int j = 0; j < Layout.getHeight()-TargetSize.second+1; j++)
				{
					int LayoutY = Target.m_TopLeft.second - j;
					if (LayoutY < 0 || LayoutY + Layout.getHeight() > BoardSize.second)
						continue;
					
					std::optional<CPos> NextAttack;
					for (auto& t : Target.m_Tiles)
					{
						Match = true;
						int x = t->getPos().first;
						int y = t->getPos().second;
						if (x < LayoutX || y < LayoutY || x >= LayoutX + Layout.getWidth() || y >= LayoutY + Layout.getHeight()
							|| Layout.at(x - Target.m_TopLeft.first + i, y - Target.m_TopLeft.second + j) != 1) // if the checked tile is outside the boundaries (uh - no need to check actly), aand not 1
						{
							Match = false;
							std::cout << "didn't match because hit" << std::endl;
							break;
						}
						if (!NextAttack)
						{
							for (int i = -1; i <= 1; i++)
							{
								for (int j = -1; j <= 1; j++)
								{
									int PosX = x - i, PosY = y - j;
									if (PosX < BoardSize.first && PosY < BoardSize.second &&
										Layout.checkPoint(PosX - LayoutX, PosY - LayoutY) && Layout[{PosX - LayoutX, PosY - LayoutY}] == 1
										&& std::find_if(Target.m_Tiles.begin(), Target.m_Tiles.end(),
											[PosX, PosY](CGraphTile* Tile) -> bool { return Tile->getPos() == std::make_pair(PosX, PosY); }) == Target.m_Tiles.end())
									{
										NextAttack = { PosX, PosY };
										std::cout << "next attack = " << PosX << " " << PosY << std::endl;
										break;
									}

								}
								if (NextAttack)
									break;
							}
						}
					}
					if (!Match)
						continue;

					unsigned int Ones = 0;
					for (int k = 0; k < Layout.getWidth(); k++)
					{
						for (int l = 0; l < Layout.getHeight(); l++)
						{
							if (Layout[{k, l}] == 1)
							{
								Ones++;
								if (Context.m_Field[{LayoutX + k, LayoutY + l}].getState() == CState::MISS)
								{
									Match = false;
									break;
								}
								
							}
						}
						if (!Match)
							break;
					}
					if (!Match || Ones <= Target.m_Tiles.size())
					{
						//std::cout << "layout mismatch type 2" << std::endl;
						continue;

					}

					Target.m_MatchingRotation = Layout.getRotation();
					Target.m_LayoutPos = std::make_pair(LayoutX, LayoutY);
					std::cout << "match found: " << Target.m_LastMatch << ", rotation: " << Target.m_MatchingRotation << ", lpos: " << LayoutX << " " << LayoutY << std::endl;
					getController()->attack(this, Victim, NextAttack->first, NextAttack->second);
					return;
				}

			}
			Layout.rotate(CRotation::CDir::CLOCKWISE);
		} while (Layout.getRotation() != CRotation::CValue::NONE);

		Target.m_MatchingRotation = CRotation::CValue::NONE;
		Target.m_LastMatch++;
	}*/
}

const CAiPlayer::CField* CAiPlayer::getEnemyField(const IPlayer * Player) const
{
	auto It = m_Contexts.find(Player);
	if (It != m_Contexts.end())
		return &It->second.m_Field;
	return nullptr;
}

void CAiPlayer::onEvent(const CGameEvent& Event)
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
	case CType::SHIP_DESTROYED:
		//onShipDestroyed(Event.m_ShipDestroyedEvent.m_Ship)
		break;
	}

}

void CAiPlayer::onPlayerAttacked(const IPlayer * Victim, int x, int y, CTile::CState State)
{
	using CState = CTile::CState;
	bool Destroyed = false;
	if (Victim == this)
		return;
	if (m_Contexts.find(Victim) == m_Contexts.end()) // remove..?{
		prepareNewBoard(Victim);
	auto& Context = m_Contexts[Victim];
	auto& Tile = Context.m_Field.at(x, y);
	if (State == CState::DESTROYED)
	{
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

	std::cout << "top left: " << Context.m_Targets[Root].m_TopLeft.first << " " << Context.m_Targets[Root].m_TopLeft.second << std::endl;
	std::cout << "bottom right: " << Context.m_Targets[Root].m_BottomRight.first << " " << Context.m_Targets[Root].m_BottomRight.second << std::endl;
	for (auto t : Context.m_Targets[Root].m_Tiles)
	{
		std::cout << t->getPos().first << " " << t->getPos().second << ", state: ";
		switch (t->getState())
		{
		case CGraphTile::CState::HIT:
			std::cout << "hit ";
			break;
		default:
			std::cout << "this should not print";
		}
		if (t == Root)
			std::cout << "<--- root";
		std::cout << std::endl;
	}
	if (Destroyed)
	{
		std::cout << "destroyed" << std::endl;
		for (auto t : Context.m_Targets[Root].m_Tiles)
		{
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					int xch = t->getPos().first + i;
					int ych = t->getPos().second + j;
					if ((t->getState() == CGraphTile::CState::HIT || t->getState() == CGraphTile::CState::DESTROYED) &&
						Context.m_Field.checkPoint(xch, ych) && Context.m_Field[{xch, ych}].getState() == CGraphTile::CState::EMPTY)
					{
						Context.m_Field[{xch, ych}] = CGraphTile(CGraphTile::CState::MISS, { xch, ych });
						Context.m_Empty--;
					}
				}
			}
		}
		Context.m_Targets.erase(Root);
	}
	std::cout << "num targets: " << Context.m_Targets.size() << std::endl;
	std::cout << std::endl;
}

void CAiPlayer::prepareNewBoard(const IPlayer * Enemy)
{
	auto Size = getController()->getPreset()->getBoardSize();
	auto& Context = m_Contexts[Enemy];
	Context.m_Field = CField(Size.first, Size.second);
	Context.m_Field.fill(CGraphTile(CTile::CState::EMPTY));
	Context.m_Empty = Size.first*Size.second;
}

void CAiPlayer::onGameStarted()
{
	m_Contexts.clear();
	auto Players = getController()->getPlayers();
	for (const IPlayer* p : Players)
		prepareNewBoard(p);
}
