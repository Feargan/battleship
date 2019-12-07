#include "localplayer.h"



CLocalPlayer::CLocalPlayer(IGameController* Controller)
	: IPlayer(Controller)
{
}


CLocalPlayer::~CLocalPlayer()
{
}

void CLocalPlayer::play() // fix, this is temp
{
	/*auto Players = getController()->getPlayers();
	const IPlayer* Victim = nullptr;
	for (auto &p : Players)
	{
		if (p != this)
		{
			Victim = p;
			break;
		}
	}
	if (!Victim)
	{
		std::cout << "am i the only one playing this game?" << std::endl;
		return;
	}
	std::cout << m_Name << ", attack: ";
	unsigned int x, y;
	std::cin >> x >> y;

	getController()->attack(this, Victim, x, y);*/
}

const CLocalPlayer::CContext* CLocalPlayer::getEnemyField(const IPlayer * Player) const // this is temp
{
	if (m_Enemies.find(Player) != m_Enemies.end())
		return &m_Enemies.at(Player);
	return nullptr;
}

void CLocalPlayer::onEvent(const CGameEvent& Event)
{
	using CType = CGameEvent::CType;
	switch (Event.m_Type)
	{
	case CType::GAME_STARTED:
		onGameStarted();
		break;
	case CType::PLAYER_ATTACKED:
		onPlayerAttacked(Event.m_PlayerAttackedEvent.m_Victim, Event.m_PlayerAttackedEvent.m_X, Event.m_PlayerAttackedEvent.m_Y, Event.m_PlayerAttackedEvent.m_State);
		break;
	case CType::SHIP_DESTROYED:
		onShipDestroyed(Event.m_ShipDestroyedEvent.m_Victim, Event.m_ShipDestroyedEvent.m_Ship);
		break;
	}
	
}

void CLocalPlayer::onPlayerAttacked(const IPlayer * Victim, int x, int y, CTile::CState State)
{
	if (Victim == this)
		return;
	
	switch (State)
	{
	case CTile::CState::MISS:
		m_Enemies[Victim].m_Field.at(x, y) = CTile(CTile::CState::MISS);
		break;
	case CTile::CState::HIT:
	case CTile::CState::DESTROYED:
		m_Enemies[Victim].m_Field.at(x, y) = CTile(CTile::CState::HIT);
		break;
	}
}

void CLocalPlayer::onGameStarted()
{
	m_Enemies.clear();
	auto Players = getController()->getPlayers();
	for (const IPlayer* p : Players)
		prepareNewBoard(p);
}

void CLocalPlayer::onShipDestroyed(const IPlayer * Victim, const CShip * Ship)
{
	m_Enemies[Victim].m_DestroyedShips.push_back(Ship);
}

void CLocalPlayer::prepareNewBoard(const IPlayer * Enemy)
{
	if (m_Enemies.find(Enemy) == m_Enemies.end()) // remove..?
	{
		auto Size = getController()->getPreset()->getBoardSize();
		m_Enemies[Enemy].m_Field = CField(Size.first, Size.second);
		m_Enemies[Enemy].m_Field.fill(CTile(CTile::CState::EMPTY));
	}
}
