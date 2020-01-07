#pragma once

#include "gameboard.h"
#include "gamecontroller.h"

class IPlayer : public IGameController::IObserver
{
	IGameController* m_Controller;
	std::string m_Name;
protected:
	IGameController* getController();
public:
	IPlayer(IGameController* Controller);
	virtual ~IPlayer();

	virtual void play() = 0;

	void setName(const std::string& Name);
	const std::string& getName() const;
	const IGameController* getController() const;
/*protected:
	virtual void onEvent(const CGameEvent& Event) override {};*/
};

