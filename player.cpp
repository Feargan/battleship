#include "player.h"


IPlayer::IPlayer(IGameController* Controller) : IObserver()
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

const IGameController * IPlayer::getController() const
{
	return m_Controller;
}

IGameController * IPlayer::getController()
{
	return m_Controller;
}
