#pragma once

#include "gameboard.h"
#include "gamecontroller.h"

class IPlayer : public CGameController::IObserver
{
	CGameController* m_Controller;
	std::string m_Name;
protected:
	CGameController* getController();
public:
	IPlayer(CGameController* Controller);
	virtual ~IPlayer();

	virtual void play() = 0;

	void setName(const std::string& Name);
	const std::string& getName() const;
	const CGameController* getController() const;
/*protected:
	virtual void onEvent(const CGameEvent& Event) override {};*/
};

