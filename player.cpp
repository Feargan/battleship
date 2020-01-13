#include "player.h"


IPlayer::IPlayer(CGameController* Controller) : IObserver()
{
	m_Controller = Controller;
}

IPlayer::~IPlayer()
{
}

void IPlayer::setName(const std::string & Name)
{
	m_Name = Name;
}

const std::string & IPlayer::getName() const
{
	return m_Name;
}

const CGameController * IPlayer::getController() const
{
	return m_Controller;
}

CGameController * IPlayer::getController()
{
	return m_Controller;
}
