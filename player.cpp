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

IGameController * IPlayer::getController()
{
	return m_Controller;
}
