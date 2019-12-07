#pragma once

#include "player.h"

#include <unordered_map>

class CLocalPlayer : public IPlayer
{
public:
	using CState = CTile::CState;
	using CField = CGameBoard::CField;
	struct CContext // make a mutual class CMysloncyGracz
	{
		CField m_Field;
		std::vector<const CShip*> m_DestroyedShips; // fix to shared_ptr
	};
private:
	std::unordered_map<const IPlayer*, CContext> m_Enemies;
public:
	CLocalPlayer(IGameController* Controller); // CGameController*
	virtual ~CLocalPlayer();

	virtual void play() override;

	const CContext* getEnemyField(const IPlayer* Player) const; // delete
protected:
	virtual void onEvent(const CGameEvent& Event) override;
private:
	void onPlayerAttacked(const IPlayer* Victim, int x, int y, CTile::CState State);
	void onGameStarted();
	void onShipDestroyed(const IPlayer* Victim, const CShip* Ship);
	void prepareNewBoard(const IPlayer* Enemy);
	//const std::vector<CGameBoard>& getEnemyBoards() const;
	//void putBoard(CGameBoard Board);
};

