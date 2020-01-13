#include "aiplayer.h"
#include "tileset.h"


CAiPlayer::CAiPlayer(CGameController* Controller) : CIntelligentPlayer(Controller)
{
}


CAiPlayer::~CAiPlayer()
{
}

void CAiPlayer::play()
{
	using CState = CGraphTile::CState;
	const IPlayer* Victim = getController()->getSuggestedVictim();
	auto& Context = m_Contexts[Victim];
	auto& Targets = Context.m_Targets;
	auto BoardSize = getController()->getPreset()->getBoardSize();
	if (!Targets.size())
	{
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
	auto Target = Targets.begin()->second;
	CTileSet Attackable;
	auto& Field = Context.m_Field;
	auto Add = [&Field, &Attackable](int x, int y) -> void
	{
		if (Field.checkPoint(x, y) && Field[{x,y}].getState() == CState::EMPTY)
			Attackable.insert({ x,y });
	};
	for (auto t : Target.m_Tiles)
	{
		int x = t->getPos().first;
		int y = t->getPos().second;
		Add(x - 1, y);
		Add(x + 1, y);
		Add(x, y - 1);
		Add(x, y + 1);
	}
	int Index = rand() % Attackable.size();
	auto It = Attackable.begin();
	for (; Index > 0; Index--)
		It++;
	getController()->attack(this, Victim, It->first, It->second);
}

