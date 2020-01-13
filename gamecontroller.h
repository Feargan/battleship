#pragma once

#include "gameboardbuilder.h"
#include "gamepreset.h"
#include "gameevents.h"
#include "tile.h"

#include <unordered_map>
#include <unordered_set>
#include <queue>

class IPlayer;

class CGameController
{
public:
	class IObserver
	{
	public:
		virtual ~IObserver() {}
		virtual void onEvent(const CGameEvent& Event) = 0; // onEvent(CEvent) // after function completion, references to events are invalidated!!
	};
protected:
	std::unordered_map<IPlayer*, CGameBoard> m_Players;
	std::queue<IPlayer*> m_Queue;
	std::vector<IPlayer*> m_Victims;
private:
	std::unordered_set<IObserver*> m_Observers;
	const CGamePreset* m_Preset;
public:
	CGameController(const CGamePreset* Preset = nullptr);
	virtual ~CGameController();

	const IPlayer* whoseTurn() const;
	virtual const CGameBoard* seat(IPlayer* Player, const CGameBoardBuilder& Builder);
	virtual bool attack(IPlayer* Attacker, const IPlayer* Victim, int x, int y);
	virtual void start();
	virtual void run();
	bool isInProgress() const;

	void addObserver(IObserver* Observer);
	void removeObserver(IObserver* Observer);

	std::vector<const IPlayer*> getPlayers() const;
	const IPlayer* getSuggestedVictim() const;
	const CGamePreset* getPreset() const;
protected:
	void notifyObservers(const CGameEvent& Event);
private:
	void prepareVictims();
};

// pass server in constructor
// server.peekGamePacket()
// process packets
// server.sendGamePacket()