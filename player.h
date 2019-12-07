#pragma once

#include "gameboard.h"
#include "gamecontroller.h"

class IPlayer : public IGameController::IObserver
{
	IGameController* m_Controller;
	std::string m_Name;
public:
	IPlayer(IGameController* Controller);
	virtual ~IPlayer();

	virtual void play() {};

	void setName(const std::string& Name);
	const std::string& getName() const;

	IGameController* getController();
protected:
	virtual void onEvent(const CGameEvent& Event) override {};
};

