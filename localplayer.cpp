#include "localplayer.h"



CLocalPlayer::CLocalPlayer(IGameController* Controller)
	: CIntelligentPlayer(Controller)
{
}


CLocalPlayer::~CLocalPlayer()
{
}

void CLocalPlayer::play()
{
}

void CLocalPlayer::onEvent(const CGameEvent& Event)
{
	CIntelligentPlayer::onEvent(Event);
	//
}

