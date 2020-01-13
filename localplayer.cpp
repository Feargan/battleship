#include "localplayer.h"



CLocalPlayer::CLocalPlayer(CGameController* Controller)
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

