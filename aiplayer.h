#pragma once
#include "intelligentplayer.h"
class CAiPlayer :
	public CIntelligentPlayer
{
public:
	CAiPlayer(IGameController* Controller);
	virtual ~CAiPlayer();

	virtual void play() override;
};

