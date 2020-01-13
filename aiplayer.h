#pragma once
#include "intelligentplayer.h"
class CAiPlayer :
	public CIntelligentPlayer
{
public:
	CAiPlayer(CGameController* Controller);
	virtual ~CAiPlayer();

	virtual void play() override;
};

